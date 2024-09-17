//#Arduino Code for Dabble
//#Conect TX -> PIN 2, RX -> PIN 3

#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <Dabble.h>


//Código creado por Electro CR, instagram electro__cr, correo: cristopher533@gmail.com
char t;
const int LR = 7;
const int LF = 6;
const int RF = 5;
const int RR = 4;

void setup() {
  pinMode(LF, OUTPUT);  //left motors  forward
  pinMode(LR, OUTPUT);  //left motors reverse
  pinMode(RF, OUTPUT);  //right  motors forward
  pinMode(RF, OUTPUT);  //right motors reverse
  //pinMode(9,OUTPUT);   //Led
  Serial.begin(250000);      // make sure your Serial Monitor is also set at this baud rate.
  Dabble.begin(9600);      //Enter baudrate of your bluetooth.Connect bluetooth on Bluetooth port present on evive.

  Serial.flush();


}

void loop() {
  Dabble.processInput();             //this function is used to refresh data obtained from smartphone.Hence calling this function is mandatory in order to get data properly from your mobile.
  Serial.print("KeyPressed: ");
    
  if (GamePad.isUpPressed()) {          //move  forward(all motors rotate in forward direction)
    digitalWrite(LF, HIGH);
    digitalWrite(RF, HIGH);
    Serial.println("1");
  }

  else if (GamePad.isDownPressed()) {    //move reverse (all  motors rotate in reverse direction)
    digitalWrite(LR, HIGH);
    digitalWrite(RR, HIGH);
    Serial.println("2");

  }

  else if (GamePad.isRightPressed()) {    //turn right (left side motors rotate in forward direction,  right side motors doesn't rotate)
    digitalWrite(LF, HIGH);
    digitalWrite(RR, HIGH);
    Serial.println("3");

  }

  else  if (GamePad.isLeftPressed()) {    //turn left (right side motors rotate in forward direction, left  side motors doesn't rotate)
    digitalWrite(LR, HIGH);
    digitalWrite(RF, HIGH);
    Serial.println("4");

  }

  else {    //STOP (all motors stop)
    digitalWrite(RF, LOW);
    digitalWrite(LF, LOW);
    digitalWrite(RR, LOW);
    digitalWrite(LR, LOW);
    Serial.println("5");

  }
  delay(10);
}
