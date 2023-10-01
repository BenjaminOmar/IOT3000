#include <Stepper.h>

#define Y_PIN   A1
#define SW_PIN  2
#define SW_INITIAL_STATE  HIGH
#define Y_AT_REST  500

#define IN1 11
#define IN2 10
#define IN3 9
#define IN4 8
#define DEFAULT_STEPS         0
#define STEPS_PER_REVOLUTION  2038
#define DEFAULT_SPEED         5
#define MAX_SPEED             15
#define DEFAULT_AUTOPILOT     true
#define AUTOPILOT_REVOLUTIONS 1

bool autopilot = false;
int revolutions_counter = 0;
int steps, motor_speed, clockwise;

bool last_released_state = (SW_INITIAL_STATE == HIGH) ? true : false;
Stepper stepper = Stepper(STEPS_PER_REVOLUTION, IN1, IN3, IN2, IN4);

void setup() {
  Serial.begin(115200);

  // Joystick
  pinMode(SW_PIN, INPUT);
  digitalWrite(SW_PIN, HIGH);
  Serial.println("Joystick ready for use.");

  // Stepper
  set_motor_values(autopilot);
  Serial.println("Stepper motor ready for use.");

  delay(3500);
}

void loop() {
  // Handle SWITCH button click
  bool current_released_state = digitalRead(SW_PIN);
  if (last_released_state != current_released_state) {
    String state = current_released_state ? "RELEASED" : "PRESSED"; 
    Serial.println("!\t" + state + "\t!");
    if (state == "PRESSED") {
      switch_mode();
    }
  }
  last_released_state = current_released_state;

  if (!autopilot) {
    // Read Y value from joystick
    int y = analogRead(Y_PIN);
    
    if (y != Y_AT_REST) {
      // Adjust speed, direction and steps to non-null Y-axis value
      clockwise = (y > Y_AT_REST);
      motor_speed = map(y, 0, 1023, 0, MAX_SPEED);
      steps = STEPS_PER_REVOLUTION / MAX_SPEED;
      if (!clockwise) steps *= -1;
    } else {
      steps = 0;
    }
  }
  
  Serial.println("Mode\t-\t" + String(autopilot ? "AUTOPILOT" : "MANUAL"));
  Serial.println("Speed\t-\t" + String(motor_speed));
  Serial.println("Steps\t-\t" + String(steps));
  if (autopilot) Serial.println("Revolution #\t-\t" + String(revolutions_counter) + " / " + String(AUTOPILOT_REVOLUTIONS));
  Serial.println("==================================");
  
  if (!autopilot || revolutions_counter < AUTOPILOT_REVOLUTIONS) {
    if (motor_speed > 0) stepper.setSpeed(motor_speed);
    if (steps != 0) stepper.step(steps);
    if (autopilot) revolutions_counter++;
  }

  delay(250);
}

void switch_mode() {
  autopilot = !autopilot;
  set_motor_values(autopilot);
}

void set_motor_values(bool autopilot) {
  if (autopilot) {
    clockwise = true;
    motor_speed = MAX_SPEED;
    steps = STEPS_PER_REVOLUTION;
    revolutions_counter = 0;
  } else {
    motor_speed = DEFAULT_SPEED;
    steps = STEPS_PER_REVOLUTION / MAX_SPEED;
  }
}
