#include <PS2X_lib.h>
#include <Servo.h>

// ===================== Pin Definition =====================
// PS2 Game Controller Communication Pins
#define PS2_DAT 13    // PS2 data signal pin
#define PS2_CMD 12    // PS2 command signal pin
#define PS2_SEL 11    // PS2 select signal pin
#define PS2_CLK 10    // PS2 clock signal pin

// Servo signal pins for 6-axis robotic arm
#define SERVO1_PIN 2
#define SERVO2_PIN 3
#define SERVO3_PIN 4
#define SERVO4_PIN 5
#define SERVO5_PIN 6
#define SERVO6_PIN 7

#define BUZZER_PIN A0  // Active buzzer control pin

// ===================== Independent Angle Limit for Each Servo =====================
// Set min/max angle separately to avoid mechanical overtravel
#define SERVO1_MIN 0
#define SERVO1_MAX 180
#define SERVO2_MIN 0
#define SERVO2_MAX 180
#define SERVO3_MIN 0
#define SERVO3_MAX 180
#define SERVO4_MIN 0
#define SERVO4_MAX 180
#define SERVO5_MIN 0
#define SERVO5_MAX 180
#define SERVO6_MIN 85
#define SERVO6_MAX 180

// ===================== Motion Control Parameters =====================
#define STEP 2           // Normal moving step of servos
#define SLOW_STEP 1      // Step for slow reset / homing movement
#define DELAY_SLOW 20    // Delay per step during slow movement (larger = slower)
#define DEAD_ZONE 55     // Joystick dead zone, filter analog drift signal

// ===================== Servo6 Anti-Stall Protection Parameters =====================
#define HOLD_TIME 1500   // Max holding time(ms) at limit before anti-stall trigger
#define RELEASE_ANGLE 170// Safety release angle after protection activated

// ===================== Object & Variable Declaration =====================
PS2X ps2x;                    // Create PS2 controller object
int error = 0;                // PS2 controller initialization error code

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
Servo servo6;

// Current angle of each servo, default to center position 90°
int angle1 = 90;
int angle2 = 90;
int angle3 = 90;
int angle4 = 90;
int angle5 = 90;
int angle6 = 90;

unsigned long limitStartTime = 0;  // Record timestamp when servo6 reaches limit
bool atLimit = false;              // Flag: servo6 is at maximum limit position

const unsigned long SAMPLE_INTERVAL = 50;  // Controller data sampling period (ms)
unsigned long sampleTimer = 0;             // Timer for periodic sampling

// ===================== Setup Function (Execute once after power on) =====================
void setup() {
  Serial.begin(9600);                  // Initialize serial port baud rate
  pinMode(BUZZER_PIN, OUTPUT);         // Set buzzer pin as output mode

  // Power-on prompt beep
  digitalWrite(BUZZER_PIN, HIGH);
  delay(200);
  digitalWrite(BUZZER_PIN, LOW);

  // Attach all servos to corresponding pins
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo3.attach(SERVO3_PIN);
  servo4.attach(SERVO4_PIN);
  servo5.attach(SERVO5_PIN);
  servo6.attach(SERVO6_PIN);

  // Initialize PS2 game controller
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT);
  
  // Print controller connection status via serial port
  if(error == 0){
    Serial.print("Found Controller, configured successful ");
  }
  else if(error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
   
  else if(error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

  else if(error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
  
  ps2x.enableRumble();        // Enable controller vibration function
  ps2x.enablePressures();     // Enable button pressure detection function
  delay(300);

  // All axes slowly return to center position after startup
  slowMoveTo(90, 90, 90, 90, 90, 90);
}

// ===================== Main Loop (Run repeatedly) =====================
void loop() {
  ps2x.read_gamepad(false, false);  // Read current controller status
  delay(10);

  // If controller connection failed, lock all servos to current position
  if (error != 0)
  {
    servo1.write(angle1);
    servo2.write(angle2);
    servo3.write(angle3);
    servo4.write(angle4);
    servo5.write(angle5);
    servo6.write(angle6);
    return;
  }

  // Process controller command only at fixed sampling interval
  if (millis() - sampleTimer >= SAMPLE_INTERVAL)
  {
    sampleTimer = millis();

    // ---------- Axis 1: Controlled by Left Joystick X ----------
    int leftX = ps2x.Analog(PSS_LX);
    if (abs(leftX - 128) > DEAD_ZONE)
    {
      leftX > 128 ? angle1 += STEP : angle1 -= STEP;
      angle1 = constrain(angle1, SERVO1_MIN, SERVO1_MAX);
      delay(10);
    }

    // ---------- Axis 2: Controlled by Left Joystick Y ----------
    int leftY = ps2x.Analog(PSS_LY);
    if (abs(leftY - 128) > DEAD_ZONE)
    {
      leftY > 128 ? angle2 += STEP : angle2 -= STEP;
      angle2 = constrain(angle2, SERVO2_MIN, SERVO2_MAX);
      delay(20);
    }

    // ---------- Axis 3: Controlled by Right Joystick X ----------
    int rightX = ps2x.Analog(PSS_RX);
    if (abs(rightX - 128) > DEAD_ZONE)
    {
      rightX > 128 ? angle3 += STEP : angle3 -= STEP;
      angle3 = constrain(angle3, SERVO3_MIN, SERVO3_MAX);
      delay(15);
    }

    // ---------- Axis 4: Controlled by Right Joystick Y ----------
    int rightY = ps2x.Analog(PSS_RY);
    if (abs(rightY - 128) > DEAD_ZONE)
    {
      rightY > 128 ? angle4 += STEP : angle4 -= STEP;
      angle4 = constrain(angle4, SERVO4_MIN, SERVO4_MAX);
      delay(10);
    }

    // ---------- Axis 5: Controlled by D-Pad Up / Down ----------
    bool upKey = ps2x.Button(PSB_PAD_UP);
    bool downKey = ps2x.Button(PSB_PAD_DOWN);
    if (upKey != downKey)
    {
      if (upKey) angle5 += STEP;
      if (downKey) angle5 -= STEP;
      angle5 = constrain(angle5, SERVO5_MIN, SERVO5_MAX);
      delay(10);
    }

    // ---------- Axis 6: Controlled by D-Pad Left / Right ----------
    bool leftPad = ps2x.Button(PSB_PAD_LEFT);
    bool rightPad = ps2x.Button(PSB_PAD_RIGHT);
    if (rightPad) angle6 += STEP;  // D-Pad Right = Close gripper
    if (leftPad)  angle6 -= STEP;  // D-Pad Left = Open gripper
    angle6 = constrain(angle6, SERVO6_MIN, SERVO6_MAX);
    delay(10);
  }

  // ===================== Servo6 Limit & Anti-Stall Protection Logic =====================
  if (angle6 >= SERVO6_MAX) {
    angle6 = SERVO6_MAX;  // Lock servo at maximum limit angle

    if (!atLimit) {
      atLimit = true;
      limitStartTime = millis();  // Start timing when reaching limit
      Serial.println("Axis 6 has reached the limit");
    }

    // Buzzer alarm when holding close button at limit position
    digitalWrite(BUZZER_PIN, ps2x.Button(PSB_PAD_RIGHT) ? HIGH : LOW);

    // Judge if keep pressing close button over holding time
    if (millis() - limitStartTime >= HOLD_TIME) {
      if (ps2x.Button(PSB_PAD_RIGHT)) {
        angle6 = RELEASE_ANGLE;   // Release servo to prevent stall
        atLimit = false;
        limitStartTime = 0;
        digitalWrite(BUZZER_PIN, LOW);
        Serial.println("Hold continuously → Release anti-blocking rotation");
      }
    }
  }
  else {
    // Reset flag and timer when leaving limit position
    atLimit = false;
    limitStartTime = 0;
    digitalWrite(BUZZER_PIN, LOW);
  }

  // ---------- Triangle Button: Slow reset all axes to center position ----------
  if (ps2x.Button(PSB_TRIANGLE)) {
    Serial.println("Manual reset: Slowly return to the neutral position");
    slowMoveTo(90, 90, 90, 90, 90, 90);
  }

  // Output current angle to all servos
  servo1.write(angle1);
  servo2.write(angle2);
  servo3.write(angle3);
  servo4.write(angle4);
  servo5.write(angle5);
  servo6.write(angle6);
}

// ===================== Slow Movement Function =====================
// Function: Move all servos synchronously to target angle with slow speed
void slowMoveTo(int t1, int t2, int t3, int t4, int t5, int t6) {
  while (true) {
    bool done = true;  // Flag: check if all servos reach target angle

    // Move servo 1 to target
    if (angle1 < t1) { angle1 += SLOW_STEP; done = false; }
    if (angle1 > t1) { angle1 -= SLOW_STEP; done = false; }
    angle1 = constrain(angle1, SERVO1_MIN, SERVO1_MAX);
    delay(10);

    // Move servo 2 to target
    if (angle2 < t2) { angle2 += SLOW_STEP; done = false; }
    if (angle2 > t2) { angle2 -= SLOW_STEP; done = false; }
    angle2 = constrain(angle2, SERVO2_MIN, SERVO2_MAX);
    delay(10);

    // Move servo 3 to target
    if (angle3 < t3) { angle3 += SLOW_STEP; done = false; }
    if (angle3 > t3) { angle3 -= SLOW_STEP; done = false; }
    angle3 = constrain(angle3, SERVO3_MIN, SERVO3_MAX);
    delay(10);

    // Move servo 4 to target
    if (angle4 < t4) { angle4 += SLOW_STEP; done = false; }
    if (angle4 > t4) { angle4 -= SLOW_STEP; done = false; }
    angle4 = constrain(angle4, SERVO4_MIN, SERVO4_MAX);
    delay(10);

    // Move servo 5 to target
    if (angle5 < t5) { angle5 += SLOW_STEP; done = false; }
    if (angle5 > t5) { angle5 -= SLOW_STEP; done = false; }
    angle5 = constrain(angle5, SERVO5_MIN, SERVO5_MAX);
    delay(10);

    // Move servo 6 to target
    if (angle6 < t6) { angle6 += SLOW_STEP; done = false; }
    if (angle6 > t6) { angle6 -= SLOW_STEP; done = false; }
    angle6 = constrain(angle6, SERVO6_MIN, SERVO6_MAX);
    delay(10);

    // Update actual servo position
    servo1.write(angle1);
    servo2.write(angle2);
    servo3.write(angle3);
    servo4.write(angle4);
    servo5.write(angle5);
    servo6.write(angle6);

    delay(DELAY_SLOW);
    if (done) break;  // Exit loop when all servos arrive at target
  }
}