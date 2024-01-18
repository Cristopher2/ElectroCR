#include <Servo.h>

Servo servoX1;  // Servo for X-axis control from Joystick 1
Servo servoY1;  // Servo for Y-axis control from Joystick 1
Servo servoX2;  // Servo for X-axis control from Joystick 2
Servo servoY2;  // Servo for Y-axis control from Joystick 2

int joyX1;  // Variable to store the X-axis value from Joystick 1
int joyY1;  // Variable to store the Y-axis value from Joystick 1
int joyX2;  // Variable to store the X-axis value from Joystick 2
int joyY2;  // Variable to store the Y-axis value from Joystick 2


//THIS PART SHOULD BE CALIBRATAD PER THE SERVO MOTOR MOVEMENT RANGE:

//STARTING POSITION OF THE SERVOS:
int servoX1Position=45;  // Variable to store the servo X1 position
int servoY1Position=135;  // Variable to store the servo Y1 position
int servoX2Position=95;  // Variable to store the servo X2 position
int servoY2Position=135;  // Variable to store the servo Y2 position

//LIMITS FOR THE RANGE OF THE MOVEMENT
int servoX1Max=90;  // Variable to store the servo X1 position
int servoY1Max=150;  // Variable to store the servo Y1 position
int servoX2Max=180;  // Variable to store the servo X2 position
int servoY2Max=180;  // Variable to store the servo Y2 position

int servoX1Min=30;  // Variable to store the servo X1 position
int servoY1Min=120;  // Variable to store the servo Y1 position
int servoX2Min=10;  // Variable to store the servo X2 position
int servoY2Min=190;  // Variable to store the servo Y2 position


void setup() {
  Serial.begin(9600);  // Initialize serial communication at 9600 bps
  servoX1.attach(9);   // Attach the servo for X-axis from Joystick 1 to pin 9
  servoY1.attach(7);  // Attach the servo for Y-axis from Joystick 1 to pin 10
  servoX2.attach(6);  // Attach the servo for X-axis from Joystick 2 to pin 11
  servoY2.attach(5);  // Attach the servo for Y-axis from Joystick 2 to pin 12
  // Move the servos to the calculated positions
}

void loop() {
  // Read the X and Y axis values from Joystick 1 (analog inputs)
  joyX1 = analogRead(A0);
  joyY1 = analogRead(A3);

  // Read the X and Y axis values from Joystick 2 (analog inputs)
  joyX2 = analogRead(A2);
  joyY2 = analogRead(A1);

  if(joyX1<100&&servoX1Position>servoX1Min){servoX1Position--;}
  if(joyX2<100&&servoX2Position>servoX2Min){servoX2Position--;}
  if(joyY1<100&&servoY1Position>servoY1Min){servoY1Position--;}
  if(joyY2<100&&servoY2Position>servoY2Min){servoY2Position--;}

  if(joyX1>900&&servoX1Position<servoX1Max){servoX1Position++;}
  if(joyY1>900&&servoY1Position<servoY1Max){servoY1Position++;}
  if(joyX2>900&&servoX2Position<servoX2Max){servoX2Position++;}
  if(joyY2>900&&servoY2Position<servoY2Max){servoY2Position++;}

  // Move the servos to the calculated positions
  servoX1.write(servoX1Position);
  servoY1.write(servoY1Position);
  servoX2.write(servoX2Position);
  servoY2.write(servoY2Position);

  // Print the servo angles and joystick values to the serial monitor
  Serial.print("Servo X1 Angle: ");
  Serial.print(servoX1Position);
  Serial.print("\tServo Y1 Angle: ");
  Serial.print(servoY1Position);
  Serial.print("\tServo X2 Angle: ");
  Serial.print(servoX2Position);
  Serial.print("\tServo Y2 Angle: ");
  Serial.println(servoY2Position);

  // Add a short delay for stability
  delay(2);
}
