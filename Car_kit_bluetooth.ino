//Código creado por Electro CR, instagram electro__cr, correo: cristopher533@gmail.com
char t;
const int LR = 7; //IN3
const int LF = 6; ////IN4
const int RF = 5; //IN1
const int RR = 4; //IN2

void setup() {
  pinMode(LF, OUTPUT);  //left motors  forward
  pinMode(LR, OUTPUT);  //left motors reverse
  pinMode(RF, OUTPUT);  //right  motors forward
  pinMode(RF, OUTPUT);  //right motors reverse
  //pinMode(9,OUTPUT);   //Led
  Serial.begin(9600);

  Serial.flush();

  pinMode(8, OUTPUT);  //right  motors forward
  pinMode(6, OUTPUT);
  //digitalWrite(8, HIGH);
  //digitalWrite(6, HIGH);
}

void loop() {
  if (Serial.available()) {
    t = Serial.read();
    Serial.println(t);
    
    if (t == 'F') {          //move  forward(all motors rotate in forward direction)
      digitalWrite(LF, HIGH);
      digitalWrite(RF, HIGH);
      Serial.println("1");
    }

    else if (t == 'G') {    //move reverse (all  motors rotate in reverse direction)
      digitalWrite(LR, HIGH);
      digitalWrite(RR, HIGH);
      Serial.println("2");

    }

  

    else if (t == 'R') {    //turn right (left side motors rotate in forward direction,  right side motors doesn't rotate)
      digitalWrite(LF, HIGH);
      digitalWrite(RR, HIGH);
      Serial.println("3");

    }
  
    else  if (t == 'L') {    //turn left (right side motors rotate in forward direction, left  side motors doesn't rotate)
      digitalWrite(LR, HIGH);
      digitalWrite(RF, HIGH);
      Serial.println("4");

    }
  
    else if (t == 'S') {    //STOP (all motors stop)
      digitalWrite(RF, LOW);
      digitalWrite(LF, LOW);
      digitalWrite(RR, LOW);
      digitalWrite(LR, LOW);
      Serial.println("5");

    }
    delay(10);
  }
}
