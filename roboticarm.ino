//Electro CR //+506 6036-1412 //cha.gerencia@electrocr.store

//Se debe descargar e instalar la libería servo para que este código funcione
#include <Servo.h>

//Acá se define la velocidad inicial, minima y maxima 
int Speed =7;
int vel_max=15;
int vel_min=3;

//Pulsando los joystick como botones se puede aumentar o disminuir la velocidad,
// se definen dichos pines como entradas
const int pinIncrementar = 4;  // Pin para incrementar (botón o interruptor)
const int pinDecrementar = 2;  // Pin para restar (botón o interruptor)

//Se definen los objetos Servos
Servo servoX1;  // Servo for X-axis control from Joystick 1
Servo servoY1;  // Servo for Y-axis control from Joystick 1
Servo servoX2;  // Servo for X-axis control from Joystick 2
Servo servoY2;  // Servo for Y-axis control from Joystick 2

//Variables para almacenar la posicion actual de los joysticks/servos
int joyX1;  // Variable to store the X-axis value from Joystick 1
int joyY1;  // Variable to store the Y-axis value from Joystick 1
int joyX2;  // Variable to store the X-axis value from Joystick 2
int joyY2;  // Variable to store the Y-axis value from Joystick 2


// IMPORTANTE!! Las siguientes variables deben ajustarse a los rangos minimos y maximos de movimiento de los motores
//Esta calibracion debe hacerse para evitar danos en los servos o en las piezas acrilicas

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
int servoY1Min=110;  // Variable to store the servo Y1 position
int servoX2Min=10;  // Variable to store the servo X2 position
int servoY2Min=90;  // Variable to store the servo Y2 position


void setup() {
  Serial.begin(9600);  // Initialize serial communication at 9600 bps
  servoX1.attach(9);   // Attach the servo for X-axis from Joystick 1 to pin 9
  servoY1.attach(7);  // Attach the servo for Y-axis from Joystick 1 to pin 10
  servoX2.attach(6);  // Attach the servo for X-axis from Joystick 2 to pin 11
  servoY2.attach(5);  // Attach the servo for Y-axis from Joystick 2 to pin 12
  pinMode(pinIncrementar, INPUT_PULLUP);  // Configura el pin de incrementar como entrada con pull-up
  pinMode(pinDecrementar, INPUT_PULLUP);  // Configura el pin de restar como entrada con pull-up
  
}

void loop() {
  control_vel();
  control_serv();
  prints();
  delay(2);
}





void prints(){
  // Print the servo angles and joystick values to the serial monitor
  Serial.print("Servo X1 Angle: ");
  Serial.print(servoX1Position);
  Serial.print("\tServo Y1 Angle: ");
  Serial.print(servoY1Position);
  Serial.print("\tServo X2 Angle: ");
  Serial.print(servoX2Position);
  Serial.print("\tServo Y2 Angle: ");
  Serial.println(servoY2Position);
  Serial.print("Speed: ");
  Serial.println(Speed);
}
void control_serv(){
  // Read the X and Y axis values from Joystick 1 (analog inputs)
  //Se leen los ejes X y Y de ambos joysticks
  joyX1 = analogRead(A0);
  joyY1 = analogRead(A3);

  // Read the X and Y axis values from Joystick 2 (analog inputs)
  joyX2 = analogRead(A2);
  joyY2 = analogRead(A1);

  //Se cambia la posicion del joystick correspondiente incrementando o decrementando la variable speed, limitado al rango de movimiento
  if(joyX1<100&&servoX1Position>servoX1Min){servoX1Position=servoX1Position-Speed;}
  if(joyX2<100&&servoX2Position>servoX2Min){servoX2Position=servoX2Position-Speed;}
  if(joyY1<100&&servoY1Position>servoY1Min){servoY1Position=servoY1Position-Speed;}
  if(joyY2<100&&servoY2Position>servoY2Min){servoY2Position=servoY2Position-Speed;}

  if(joyX1>900&&servoX1Position<servoX1Max){servoX1Position=servoX1Position+Speed;}
  if(joyY1>900&&servoY1Position<servoY1Max){servoY1Position=servoY1Position+Speed;}
  if(joyX2>900&&servoX2Position<servoX2Max){servoX2Position=servoX2Position+Speed;}
  if(joyY2>900&&servoY2Position<servoY2Max){servoY2Position=servoY2Position+Speed;}

  // Move the servos to the calculated positions
  servoX1.write(servoX1Position);
  servoY1.write(servoY1Position);
  servoX2.write(servoX2Position);
  servoY2.write(servoY2Position);

}
void control_vel(){
  // Lee el estado de los pines
  int estadoIncrementar = digitalRead(pinIncrementar);
  int estadoDecrementar = digitalRead(pinDecrementar);

  // Incrementa en 1 si el pin 4 está en bajo y Speed no ha alcanzado el límite superior
  if (estadoIncrementar == LOW && Speed < vel_max) {
    Speed++;
    delay(200);  // Añade un pequeño retardo para evitar rebotes
  }

  // Resta 1 si el pin 2 está en bajo y Speed no ha alcanzado el límite inferior
  if (estadoDecrementar == LOW && Speed > vel_min) {
    Speed--;
    delay(200);  // Añade un pequeño retardo para evitar rebotes
  }


}
