#include <SPI.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>

// 2. DEFINICIÓN DE PINES (Basado en tu código anterior)
#define T_CS_PIN  13  // Pin Chip Select del Touch

// 3. INSTANCIAR OBJETOS
TFT_eSPI tft = TFT_eSPI(); 
XPT2046_Touchscreen touchscreen(T_CS_PIN);

// Variables para coordenadas
TS_Point p;
int x, y;

// ================================================================
// SETUP
// ================================================================
void setup() {
  Serial.begin(115200);
  
  // --- INICIAR TOUCH ---
  touchscreen.begin();
  touchscreen.setRotation(0); // Rotación 1 = Horizontal (Igual que la TFT)

  // --- INICIAR PANTALLA ---
  tft.init();
  tft.setRotation(2); // Rotación 1 = Horizontal (Landscape)
  
  // Limpiar pantalla en negro
  tft.fillScreen(TFT_BLACK);

  // --- DIBUJAR EL TEXTO "PROFE GALO" ---
  mostrarMensajePrincipal();
}

// ================================================================
// LOOP (Se repite indefinidamente)
// ================================================================
void loop() {
  
  // Si se detecta un toque en la pantalla...
  if (touchscreen.touched()) {
    
    // Obtener coordenadas del hardware
    p = touchscreen.getPoint();

    // Mapear las coordenadas del touch (0-4095) a los pixeles de la pantalla (320x240)
    // NOTA: Estos valores (200, 3700) son aproximados. Si el toque no coincide
    // exactamente donde pones el dedo, se ajustan aquí.
    x = map(p.y, 200, 3700, 0, 320); // Intercambiamos p.y a x por la rotación
    y = map(p.x, 100, 1850, 0, 240); // Intercambiamos p.x a y
    
    // Dibujar un pequeño círculo rojo donde se toca (para probar el touch)
    tft.fillCircle(x, 240 - y, 2, TFT_RED); 
    
    // Imprimir en monitor serie para depuración
    Serial.print("X: "); Serial.print(x);
    Serial.print(" Y: "); Serial.println(y);
  }
}

// ================================================================
// FUNCIÓN PARA DIBUJAR EL MENSAJE
// ================================================================
void mostrarMensajePrincipal() {
  // Configurar alineación de texto al centro
  tft.setTextDatum(MC_DATUM); // Middle Center (Centro Medio)

  // -- Escribir "ALUMNO" --
  tft.setTextColor(TFT_BLUE, TFT_BLACK); // Texto Cyan, Fondo Negro
  tft.setTextSize(3);                    // Tamaño de fuente (1-7)
  // (Texto, Posición X, Posición Y) -> width/2 es el centro horizontal
  tft.drawString("ALUMNO", (tft.width() / 2) - 50, (tft.height() / 2) - 80);

  // -- Escribir "FARFAN" --
  tft.setTextColor(TFT_RED, TFT_BLACK); // Texto Amarillo
  tft.setTextSize(5);                      // Un poco más grande
  tft.drawString("AUGUSTO", (tft.width() / 2) - 50, (tft.height() / 2) - 40);

  // -- Decoración (Marco) --
  tft.drawRect(10, 10, 300, 220, TFT_WHITE);
  tft.drawRect(12, 12, 296, 216, TFT_RED);
  
  // -- Pie de página --
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("telecomunicaciones", tft.width() / 2, 150);
