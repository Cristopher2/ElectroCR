#include <Servo.h>

Servo servoX1;  // Servo for X-axis control from Joystick 1
Servo servoY1;  // Servo for Y-axis control from Joystick 1
Servo servoX2;  // Servo for X-axis control from Joystick 2
Servo servoY2;  // Servo for Y-axis control from Joystick 2

int Speed = 5;
int joyX1=1;  // Variable to store the X-axis value from Joystick 1
int joyY1=1;  // Variable to store the Y-axis value from Joystick 1
int joyX2=1;  // Variable to store the X-axis value from Joystick 2
int joyY2=1;  // Variable to store the Y-axis value from Joystick 2

bool a=false;
bool b=true;
bool c=false;
bool d=false;

int servoX1Position=45;  // Variable to store the servo X1 position
int servoY1Position=135;  // Variable to store the servo Y1 position
int servoX2Position=95;  // Variable to store the servo X2 position
int servoY2Position=135;  // Variable to store the servo Y2 position
void setup() {
  Serial.begin(9600);  // Initialize serial communication at 9600 bps
  servoX1.attach(9);   // Attach the servo for X-axis from Joystick 1 to pin 9
  servoY1.attach(7);  // Attach the servo for Y-axis from Joystick 1 to pin 10
  servoX2.attach(6);  // Attach the servo for X-axis from Joystick 2 to pin 11
  servoY2.attach(5);  // Attach the servo for Y-axis from Joystick 2 to pin 12
  // Move the servos to the calculated positions
}

void loop() {


  if(joyX1&&a){
    servoX1Position=servoX1Position+Speed;
    if(servoX1Position==90){
      joyX1=0;
    }
  }else if(a){
    servoX1Position=servoX1Position-Speed;
    if(servoX1Position==30){
      joyX1=1;
      a=false;
      b=true;
    }
  }
  if(joyY1&&b){
    servoY1Position=servoY1Position+Speed;
    if(servoY1Position==150){
      joyY1=0;
    }
  }else if(b){
    servoY1Position=servoY1Position-Speed;
    if(servoY1Position==120){
      joyY1=1;
      b=false;
      c=true;
    }
  }
  if(joyX2&&c){
    servoX2Position=servoX2Position+Speed;
    if(servoX2Position==180){
      joyX2=0;
    }
  }else if(c){
    servoX2Position=servoX2Position-Speed;
    if(servoX2Position==10){
      joyX2=1;
      c=false;
      d=true;
    }
  }
  if(joyY2&&d){
    servoY2Position=servoY2Position+Speed;
    if(servoY2Position==180){
      joyY2=0;
    }
  }else if(d){
    servoY2Position=servoY2Position-Speed;
    if(servoY2Position==90){
      joyY2=1;
      d=false;
      a =true;
    }
  }

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
  delay(20);
}
