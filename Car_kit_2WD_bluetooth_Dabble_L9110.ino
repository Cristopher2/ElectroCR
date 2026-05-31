/*
  ============================================================================
  Proyecto: Robot Bluetooth controlado mediante Dabble GamePad
  Autor: Cristopher Hernandez
  Empresa: ElectroCR
  Contacto: info@electrocr.tech

  Descripción:
  Este programa controla un robot mediante la aplicación Dabble usando
  comandos del módulo GamePad.

  Incluye un sistema de protección contra desconexiones Bluetooth para evitar
  movimientos inesperados cuando el teléfono pierde conexión o cuando la
  aplicación vuelve a conectarse manteniendo una tecla virtual presionada.

  ============================================================================
*/

#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <Dabble.h>

const int LR = 7;
const int LF = 6;
const int RF = 5;
const int RR = 4;

const int BT_STATE = 8;

/*
  btHighStartTime:
  Guarda el instante en que la señal BT_STATE pasa a HIGH.

  Se utiliza para verificar que la conexión Bluetooth permanezca estable
  durante un tiempo determinado antes de aceptar comandos.
*/
unsigned long btHighStartTime = 0;

/*
  Tiempo mínimo que BT_STATE debe mantenerse en HIGH antes de permitir
  el control del robot.

  Esto evita movimientos accidentales durante la reconexión del módulo
  Bluetooth.
*/
const unsigned long BT_STABLE_TIME = 1500; // ms

/*
  esperarNuevaTecla:
  Se activa cuando ocurre una desconexión Bluetooth.

  Obliga al sistema a ignorar cualquier tecla que pudiera quedar reportada
  como presionada después de la reconexión.
*/
bool esperarNuevaTecla = false;

/*
  huboReposo:
  Indica que después de una reconexión ya se detectó un estado sin teclas
  presionadas.

  Sólo después de detectar este reposo se permitirá nuevamente aceptar
  comandos de movimiento.
*/
bool huboReposo = false;

/*
  stopMotors()

  Detiene completamente el robot.

  En este diseño los motores se encuentran desactivados cuando todas las
  salidas están en HIGH.
*/
void stopMotors() {
  digitalWrite(RF, HIGH);
  digitalWrite(LF, HIGH);
  digitalWrite(RR, HIGH);
  digitalWrite(LR, HIGH);
}

void setup() {
  pinMode(LF, OUTPUT);
  pinMode(LR, OUTPUT);
  pinMode(RF, OUTPUT);
  pinMode(RR, OUTPUT);

  pinMode(BT_STATE, INPUT);

  Serial.begin(250000);
  Dabble.begin(9600);

  stopMotors();
}

void loop() {

  int btState = digitalRead(BT_STATE);

  Serial.print("BT_STATE: ");
  if (btState == HIGH) Serial.println("HIGH");
  else Serial.println("LOW");

  /*
    =======================================================================
    PROTECCIÓN CONTRA DESCONEXIONES BLUETOOTH
    =======================================================================

    Cuando BT_STATE pasa a LOW significa que el módulo Bluetooth perdió la
    conexión con el teléfono.

    En ese momento:

    1. Se detienen inmediatamente los motores.
    2. Se activa el modo "esperarNuevaTecla".
    3. Se reinicia el temporizador de conexión estable.
    4. Se obliga al usuario a soltar todas las teclas antes de aceptar
       nuevos comandos.

    Esto evita que el robot continúe moviéndose por datos residuales o
    estados inconsistentes después de una desconexión.
  */
  if (btState == LOW) {
    stopMotors();

    esperarNuevaTecla = true;
    huboReposo = false;
    btHighStartTime = 0;

    return;
  }

  /*
    Cuando BT_STATE pasa a HIGH por primera vez se almacena el instante
    exacto de reconexión.

    No se aceptan comandos todavía.
  */
  if (btHighStartTime == 0) {
    btHighStartTime = millis();
    stopMotors();
    return;
  }

  /*
    Espera de estabilización.

    Aunque el módulo indique conexión, se exige que la señal permanezca
    estable durante BT_STABLE_TIME milisegundos.

    Durante este período todos los motores permanecen detenidos.

    Esto evita movimientos inesperados mientras la aplicación Dabble y el
    enlace Bluetooth terminan de sincronizarse.
  */
  if (millis() - btHighStartTime < BT_STABLE_TIME) {
    stopMotors();
    return;
  }

  stopMotors();

  Dabble.processInput();

  Serial.print("KeyPressed: ");

  bool algunaPresionada =
    GamePad.isUpPressed() ||
    GamePad.isDownPressed() ||
    GamePad.isRightPressed() ||
    GamePad.isLeftPressed();

  /*
    =======================================================================
    BLOQUEO ANTI "TECLA PEGADA"
    =======================================================================

    Problema:

    Algunos teléfonos o aplicaciones pueden reconectarse reportando una
    tecla virtual como todavía presionada aunque el usuario ya no la esté
    tocando.

    Si el robot aceptara inmediatamente ese dato, podría comenzar a moverse
    solo, después de una reconexión.

    Solución implementada:

    1. Tras una desconexión se activa esperarNuevaTecla.
    2. El sistema ignora TODOS los comandos.
    3. Espera detectar un estado completamente libre de teclas.
    4. Una vez detectado ese reposo (huboReposo = true), espera una nueva
       pulsación real del usuario.
    5. Sólo entonces vuelve a habilitar el control.

    Esto garantiza que cada movimiento posterior a una reconexión provenga
    de una acción nueva e intencional del operador.
  */
  if (esperarNuevaTecla) {

    stopMotors();

    /*
      Detecta que el usuario ya soltó todas las teclas.
    */
    if (!algunaPresionada) {
      huboReposo = true;
    }

    /*
      Una vez detectado reposo, se espera una nueva pulsación real
      para abandonar el modo de bloqueo.
    */
    if (huboReposo && algunaPresionada) {
      esperarNuevaTecla = false;
      huboReposo = false;
    }

    Serial.println("Esperando nueva tecla...");

    return;
  }

  /*
    =======================================================================
    CONTROL NORMAL DE MOVIMIENTO
    =======================================================================
  */

  if (GamePad.isUpPressed()) {
    digitalWrite(LF, LOW);
    digitalWrite(RF, LOW);
    Serial.println("1");
  }

  else if (GamePad.isDownPressed()) {
    digitalWrite(LR, LOW);
    digitalWrite(RR, LOW);
    Serial.println("2");
  }
  else if (GamePad.isLeftPressed()) {

    digitalWrite(LF, LOW);
    digitalWrite(RR, LOW);
    Serial.println("3");
  }
  else if (GamePad.isRightPressed()) {
    digitalWrite(LR, LOW);
    digitalWrite(RF, LOW);
    Serial.println("4");
  }

  else {
    Serial.println("None");
  }

  delay(10);
}
