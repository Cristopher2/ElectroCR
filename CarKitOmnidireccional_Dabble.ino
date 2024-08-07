#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <Dabble.h>
//Set the following parameter to true if you want avoiding obstacles mode
//Cambie el siguiente parametro por true si quieres que esquive obstaculos al moverte hacia adelante
bool AVOID_OBST=false;


//Configure THE PWM control pin
const int PWM2A = 11;      //M1 motor
const int PWM2B = 3;       //M2 motor
const int PWM0A = 6;       //M3 motor
const int PWM0B = 5;       //M4 motor

const int DIR_CLK = 4;     // Data input clock line
const int DIR_EN = 7;      //Equip the L293D enabling pins
const int DATA = 8;        // USB cable
const int DIR_LATCH = 12;  // Output memory latch clock
//Define the pin of ultrasonic obstacle avoidance sensor
const int Trig = A2;       //A2 is defined as the pin Trig connected to the ultrasonic sensor
const int Echo = A3;       //A3 is defined as the pin Echo connected to the ultrasonic sensor
//Define motion state
const int Forward = 216;    //216 save to Forward
const int Back = 39;      //39 save to Back
const int Left = 116;      //116 save to Left
const int Right = 139;     //139 save to right
const int Stop = 0;        //Parking variable
const int L_turn = 198;    //turn left
const int R_turn = 57;    //turn right

// const int mytest = 1;     //test
//Set the default speed between 1 and 255
int Speed1 = 180;
int Speed2 = 180;
int Speed3 = 180;
int Speed4 = 180;

int distance = 0;           //Variables for storing ultrasonic sensor measurements
char serialData;            //put the data received by the serial port in serialData
char cmd;                   //Store bluetooth receive commands
char Previous = 'S';
void setup() 
{
    Serial.begin(250000);      // make sure your Serial Monitor is also set at this baud rate.
    Dabble.begin(9600,9,10);      //Enter baudrate of your bluetooth.Connect bluetooth on Bluetooth port present on evive.
    Serial.flush();

    //Configure as output mode
    pinMode(DIR_CLK,OUTPUT);
    pinMode(DATA,OUTPUT);
    pinMode(DIR_EN,OUTPUT);
    pinMode(DIR_LATCH,OUTPUT);
    pinMode(PWM0B,OUTPUT);
    pinMode(PWM0A,OUTPUT);
    pinMode(PWM2A,OUTPUT);
    pinMode(PWM2B,OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);

    pinMode(Trig,OUTPUT);//The Trig pin connected to the ultrasound is set to output mode
    pinMode(Echo,INPUT);//The Echo pin connected to the ultrasound is set to input mode
}

void loop()
{
    //Serial.println("Speed: ");    
    //Serial.println(Speed1);
    if(AVOID_OBST==true){
      distance = SR04(Trig,Echo);       //Acquisition of ultrasonic distance
      Serial.print("D");    //Character Distance displayed in serial port monitor window:
      Serial.println(distance);   
    }
    control_func(); //Call the Bluetooth car control function
}

/* Function name: Motor();
* Function: Change the movement direction and speed of the car through the entrance parameters
* Entry parameter 1: Dri car movement direction
* Entry parameters 2~3: Speed1~Speed4 motor speed, value range 0~255
* Dri value description (forward :216; Back: 39; Left translation: 116; Right translation: 139; Stop: 0;
* Right rotation: 198; Left rotation: 57)
* Return value: None
 */
void Motor(int Dir,int Speed1,int Speed2,int Speed3,int Speed4)
{
    analogWrite(PWM2A,Speed1); //Motor PWM speed regulation
    analogWrite(PWM2B,Speed2); //Motor PWM speed regulation
    analogWrite(PWM0A,Speed3); //Motor PWM speed regulation
    analogWrite(PWM0B,Speed4); //Motor PWM speed regulation
    
    digitalWrite(DIR_LATCH,LOW); //DIR_LATCH sets the low level and writes the direction of motion in preparation
    shiftOut(DATA,DIR_CLK,MSBFIRST,Dir);//Write Dir motion direction value
    digitalWrite(DIR_LATCH,HIGH);//DIR_LATCH sets the high level and outputs the direction of motion
}

/*
Function name: SR04()
Function: Obtain ultrasonic ranging data
Entry parameters: Trig, Echo
Function return value: cm
*/
int SR04(int Trig,int Echo)
{
    digitalWrite(Trig,LOW);     //Trig is set to low level
    delayMicroseconds(2);       //Wait 2 microseconds
    digitalWrite(Trig,HIGH);    //Trig is set to high level
    delayMicroseconds(10);      //Wait 15 microseconds
    digitalWrite(Trig,LOW);     //Trig is set to low

    float cm = pulseIn(Echo,HIGH)/58; //Convert the ranging time to CM
    return cm;      //Returns cm value ranging data
}
/*
* Function name: control_func()
* Function: Receive Bluetooth data, control the car movement direction
* Entry parameters: None
* Return value: None
*/
void control_func()
{
    Dabble.processInput();             //this function is used to refresh data obtained from smartphone.Hence calling this function is mandatory in order to get data properly from your mobile.
      if(GamePad.isLeftPressed()&&GamePad.isUpPressed())  cmd = 'Z';
      else if(GamePad.isDownPressed()&&GamePad.isRightPressed() )  cmd = 'E';
      else  if (GamePad.isUpPressed())  cmd = 'F';     //If the data received by the serial port is character F, save F to CMD
      else if (GamePad.isDownPressed())  cmd = 'B';     //If the data received by the serial port is character B, save F to CMD
      else if(GamePad.isLeftPressed())  cmd = 'L';     //If the serial port receives data as the character L, save F to CMD
      else if (GamePad.isRightPressed())  cmd = 'R';     //If the serial port receives data as the character Y, save F to CMD
      else if(GamePad.isSelectPressed())  cmd = 'M';     //If the data received by the serial port is character F, save F to CMD
      else if( GamePad.isCirclePressed() && Speed1 < 245)//If you receive a string plus, the speed increases 
      {
          Speed1 += 10;   //We're going to increase the velocity by 10 at a time
          Speed2 = Speed1;
          Speed3 = Speed1;
          Speed4 = Speed1;


      }
      else if(GamePad.isSquarePressed() && Speed1 > 30 )//When I receive a string -- the speed decreases
      {
          Speed1 -= 10;   //I'm going to subtract 10 at a time
          Speed2 = Speed1;
          Speed3 = Speed1;
          Speed4 = Speed1;    
          Previous='Square';


      }
      else {cmd = 'S';}     //If the serial port receives data as character S, save F to CMD
      //Serial.println(serialData);
  
    if('F' == cmd)   //If Bluetooth receives the string F, the dolly moves forward and enables obstacle avoidance
    {      
       if(AVOID_OBST==true){
          AvoidingObstacles();//
       }else{
          Motor(Forward,Speed1,Speed2,Speed3,Speed4);
        
       }
    }
    else if('B' == cmd)     //Bluetooth receives string G, car backs up
    {   
        Motor(Back,Speed1,Speed2,Speed3,Speed4);
    }
    else if('L' == cmd)    //Bluetooth received the string L, car left translation
    {              
        Motor(Left,Speed1,Speed2,Speed3,Speed4); 
    }
    else if('R' == cmd)     //Bluetooth received the string R
    {
        Motor(Right,Speed1,Speed2,Speed3,Speed4);      //right translation   
    }
    else if('S' == cmd)      //When the string S is received, the cart stops moving
    {
        Motor(Stop,0,0,0,0);
        Previous='S';//We define the previous as S to avoid rebounds 
    }
    else if('Z' == cmd)     //Bluetooth received the string Z
    {
        Motor(L_turn,Speed1,Speed2,Speed3,Speed4);      //turn left   
    }
    else if('E' == cmd)     //Bluetooth received the string E
    {
        Motor(R_turn,Speed1,Speed2,Speed3,Speed4);      //turn right   
    }
    else if('M' == cmd&&Previous!='M')     //Bluetooth received the string M on this key is important to suppress rebounds, thats why we check that the previous cmd is not M
    {
      AVOID_OBST = !AVOID_OBST;   
      Previous='M'; //We set the previous cmd as M to avoid rebounds
      digitalWrite(LED_BUILTIN, digitalRead(LED_BUILTIN) ^ 1);
      cmd = 'S';

    }
    delay(10);//Delay of 100 ms

}

void AvoidingObstacles()
{
    if((distance > 20 ) || cmd == 'F')//If the distance is greater than 20cm or bluetooth receives a command equal to F
    {
        delay(100);//Delay of 100 ms
        if(distance > 20)//Again, determine if the distance is really greater than 20cm
        {
            Motor(Forward,Speed1,Speed2,Speed3,Speed4); //Call forward function
        }
        else //Otherwise the distance is less than 20
        {
            Motor(Back,Speed1,Speed2,Speed3,Speed4);//retreat
            delay(500);
            Motor(Left,Speed1,Speed2,Speed3,Speed4);//Turn left to change the direction of the car
            delay(500);
        }
    }
}
