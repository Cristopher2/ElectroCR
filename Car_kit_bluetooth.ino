char t;
const int LR = 8;
const int LF = 5;
const int RF = 6;
const int RR = 7;

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

    else if (t == 'B') {    //move reverse (all  motors rotate in reverse direction)
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
