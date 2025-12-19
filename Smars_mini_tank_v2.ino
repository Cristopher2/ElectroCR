//#Arduino Code for Dabble
//#Conect TX -> PIN 10, RX -> PIN 9

#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <Dabble.h>
#ifndef SoftwareSerial_h
  #include <SoftwareSerial.h>
#endif
#include <AFMotor_R4.h>  // Librería del shield L293D de Adafruit

// --- Bluetooth ---
SoftwareSerial BT(9, 10); // RX, TX  -> conecta TX del Bluetooth al pin 9 y RX al pin 10

// --- Motores usando librería Adafruit ---
AF_DCMotor motorLeft(2);   // M2 = motor izquierdo
AF_DCMotor motorRight(1);  // M1 = motor derecho

// --- Ultrasónico (HC-SR04) ---
const uint8_t PIN_TRIG = A5;   // Trigger en A5
const uint8_t PIN_ECHO = A4;   // Echo en A4

// --- Configuración ---
const int VELOCIDAD = 255;     // PWM máx.
const uint8_t N_SAMPLES = 3;   // lecturas para promedio
const unsigned long PULSE_TIMEOUT = 30000UL; // 30 ms

// Umbrales "follow me"
const int NEAR_CM = 7;   // <10 cm -> retrocede
const int FAR_CM  = 8;   // >20 cm -> avanza

// Estado
bool followMode = false;

// Anti-rebote por valor anterior (estilo referencia)
char Previous = 'S';

void setup() {
  Serial.begin(250000);
  BT.begin(9600);
  Dabble.begin(BT);
  Serial.flush();

  motorLeft.setSpeed(VELOCIDAD);
  motorRight.setSpeed(VELOCIDAD);

  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  digitalWrite(PIN_TRIG, LOW);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);  // LED indica Follow-Me (apagado al inicio)
}

void loop() {
  Dabble.processInput();  // refresca los datos de la app Dabble

  // --- Toggle con SELECT usando supresión de rebotes por valor anterior ---
  if (GamePad.isSelectPressed() && Previous != 'M') {
    followMode = !followMode;
    digitalWrite(LED_BUILTIN, followMode ? HIGH : LOW);  // LED = estado de Follow-Me
    Serial.print("FollowMode: ");
    Serial.println(followMode ? "ON" : "OFF");
    Previous = 'M';  // fijamos 'M' para no re-togglear mientras se mantenga presionado
  } else if (!GamePad.isSelectPressed()) {
    Previous = 'S';  // al soltar, vuelve a 'S' (listo para el próximo flanco)
  }

  if (followMode) {
    followMeLoop();
  } else {
    manualLoop();
  }

  delay(10);
}

// ---------------- Funciones ----------------

void manualLoop() {
  Serial.print("Manual Key: ");
  if (GamePad.isUpPressed()) {            // adelante
    motorLeft.run(FORWARD);
    motorRight.run(FORWARD);
    Serial.println("UP -> FWD");
  }
  else if (GamePad.isDownPressed()) {     // atrás
    motorLeft.run(BACKWARD);
    motorRight.run(BACKWARD);
    Serial.println("DOWN -> BACK");
  }
  else if (GamePad.isRightPressed()) {    // girar a la derecha (tank turn)
    motorLeft.run(FORWARD);
    motorRight.run(BACKWARD);
    Serial.println("RIGHT -> TURN R");
  }
  else if (GamePad.isLeftPressed()) {     // girar a la izquierda (tank turn)
    motorLeft.run(BACKWARD);
    motorRight.run(FORWARD);
    Serial.println("LEFT -> TURN L");
  }
  else {                                  // stop
    stopMotors();
    Serial.println("STOP");
  }
}

void followMeLoop() {
  int d = distanceCm();
  Serial.print("Follow d[cm]: ");
  Serial.println(d);

  if (d <= 0) {            // lectura inválida -> seguridad
    stopMotors();
    return;
  }

  if (d < NEAR_CM) {
    // Mano/objeto muy cerca -> retrocede
    motorLeft.run(BACKWARD);
    motorRight.run(BACKWARD);
  } else if (d > FAR_CM) {
    // Mano/objeto lejos -> avanza
    motorLeft.run(FORWARD);
    motorRight.run(FORWARD);
  } else {
    // Zona muerta (10–20 cm) -> quieto para estabilidad
    stopMotors();
  }
}

void stopMotors() {
  motorLeft.run(RELEASE);
  motorRight.run(RELEASE);
}

int distanceCm() {
  long sum = 0;
  uint8_t valid = 0;

  for (uint8_t i = 0; i < N_SAMPLES; i++) {
    // Pulso de trigger de 10 us
    digitalWrite(PIN_TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(PIN_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_TRIG, LOW);

    unsigned long duration = pulseIn(PIN_ECHO, HIGH, PULSE_TIMEOUT); // us
    if (duration > 0) {
      // ~58 us por cm (ida y vuelta ya contemplada)
      int cm = (int)(duration / 58UL);
      sum += cm;
      valid++;
    }
    delay(5);
  }

  if (valid == 0) return -1;
  return (int)(sum / valid);
}
