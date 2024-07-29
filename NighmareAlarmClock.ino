#include <Servo.h>

//variable declarations

Servo servo1;
Servo servo2;

int startDelay = 20;

const int servoPin1 = 9;   // TOP SERVO
const int servoPin2 = 10;  // BOTTOM SERVO

const int minPos1 = 125;
const int maxPos1 = 180;

const int minPos2 = 55;
const int maxPos2 = 0;

const int micPin = A0;
int micRead = 0;
int prevMicRead = 0;

int delta;
const int deltaThreshold = 100;

void setup() {
  //Attach and declare all the pins
  servo1.attach(servoPin1);
  servo2.attach(servoPin2);
  pinMode(micPin, INPUT);
  //Start the serial communication and implement a short delay.
  Serial.begin(9600);
  delay(100);
}

//Calculate delta will be called every loop, calculating the delta based on the mic's current value minus the mic's value from the previous loop
int calculateDelta() {
  //Set the prevMicRead before we get a new value from the mic
  prevMicRead = micRead;
  micRead = analogRead(micPin);
  delta = micRead - prevMicRead;
  return delta;
}

enum LoopState {
  IDLE,
  STATE_ONE,
  STATE_TWO
};

LoopState loopState = IDLE;

void loop() {
  //Get the delta
  delta = calculateDelta();
  //Print delta for convenience
  Serial.println(delta);

  //Implement a start delay to avoid false positives on startup
  if (startDelay > 0) {
    delay(200);
    startDelay--;
    return;
  }

  //Check if the absolute value of the delta exceeds the threshold. The delta can sometimes be negative, so we check the absolute value.
  if (abs(delta) >= deltaThreshold && loopState == IDLE) {
    Serial.println(loopState);
    //Set the loopState to STATE_ONE to start moving the servos
    loopState = STATE_ONE;
    delay(200);
  }

  if (loopState == STATE_ONE) {
    servo1.write(minPos1);
    servo2.write(maxPos2);
    loopState = STATE_TWO;
  } else if (loopState == STATE_TWO) {
    servo1.write(maxPos1);
    servo2.write(minPos2);
    loopState = STATE_ONE;
  }
  delay(200);
}