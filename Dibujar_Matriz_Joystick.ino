//Código ElectroCR
//Consultas al Whatsapp +506 6175-5602, info@electrocr.tech
//
#include <LedControl.h>

// Pines para MAX7219
#define DIN_PIN 11
#define CS_PIN 10
#define CLK_PIN 13

// Joystick
#define VRX_PIN A0
#define VRY_PIN A1

// Botones
#define DRAW_BUTTON_PIN 2   // Botón para dibujar
#define CLEAR_BUTTON_PIN 8  // Botón del joystick para borrar

// Configuración de la matriz
LedControl lc = LedControl(DIN_PIN, CLK_PIN, CS_PIN, 1);

// Variables de posición del puntero
int posX = 3;
int posY = 3;

// Estado del joystick
bool moved = false;

// Variables para parpadeo
unsigned long lastBlink = 0;
bool blinkState = false;

// Matriz de dibujo
bool pixels[8][8] = {false};

// Zona muerta del joystick
const int deadZoneMin = 450;
const int deadZoneMax = 570;

void setup() {
  lc.shutdown(0, false);   // Activar matriz
  lc.setIntensity(0, 8);   // Brillo medio
  lc.clearDisplay(0);      // Limpiar matriz

  pinMode(DRAW_BUTTON_PIN, INPUT);         // Botón externo con pull-up externo
  pinMode(CLEAR_BUTTON_PIN, INPUT_PULLUP); // Botón joystick usa pull-up interno

  Serial.begin(9600);
}

void loop() {
  // Leer joystick
  int xValue = analogRead(VRX_PIN);
  int yValue = analogRead(VRY_PIN);

  // Leer botones
  bool drawPressed = (digitalRead(DRAW_BUTTON_PIN) == LOW); // LOW = presionado
  bool clearPressed = !digitalRead(CLEAR_BUTTON_PIN);       // LOW = presionado

  // Si se presiona el botón del joystick → borrar todo
  if (clearPressed) {
    clearMatrix();
    delay(200); // Anti-rebote
  }

  // Manejar movimiento del puntero
  if (!moved) {
    if (xValue < 400 && posX > 0) {
      posX--;
      moved = true;
    }
    else if (xValue > 600 && posX < 7) {
      posX++;
      moved = true;
    }
    else if (yValue < 400 && posY > 0) {
      posY--;  // Eje Y invertido
      moved = true;
    }
    else if (yValue > 600 && posY < 7) {
      posY++;  // Eje Y invertido
      moved = true;
    }
  }

  // Resetear movimiento cuando joystick vuelve al centro
  if (xValue > deadZoneMin && xValue < deadZoneMax && yValue > deadZoneMin && yValue < deadZoneMax) {
    moved = false;
  }

  // Si el botón de dibujar está presionado, encender píxel actual
  if (drawPressed) {
    pixels[posX][posY] = true;
  }

  // Dibujar todos los píxeles guardados
  drawMatrix();

  // Manejar parpadeo del puntero
  if (millis() - lastBlink > 300) {
    blinkState = !blinkState;
    lastBlink = millis();
  }

  // El puntero SIEMPRE parpadea, aunque haya LED dibujado debajo
  lc.setLed(0, posY, posX, blinkState);
}

// Función para actualizar la matriz con los píxeles dibujados
void drawMatrix() {
  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 8; x++) {
      lc.setLed(0, y, x, pixels[x][y]);
    }
  }
}

// Función para borrar todo
void clearMatrix() {
  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 8; x++) {
      pixels[x][y] = false;
    }
  }
  lc.clearDisplay(0);
}
