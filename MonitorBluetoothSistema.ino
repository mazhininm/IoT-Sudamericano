#include <SPI.h>
#include <TFT_eSPI.h>
#include "BluetoothSerial.h"

// --- CONFIGURACIÓN ---
TFT_eSPI tft = TFT_eSPI(); 
BluetoothSerial SerialBT;

// Colores estilo "Cyberpunk"
#define COLOR_FONDO   TFT_BLACK
#define COLOR_TITULO  0x04B6 // Cían oscuro
#define COLOR_TEXTO   TFT_WHITE
#define COLOR_BARRA   TFT_DARKGREY
#define COLOR_CPU     TFT_RED
#define COLOR_RAM     TFT_GREEN
#define COLOR_TEMP    TFT_YELLOW

// Variables para guardar los datos recibidos
String cpuVal = "0";
String ramVal = "0";
String tempVal = "0";

// Sprite para evitar parpadeos en los números
TFT_eSprite spr = TFT_eSprite(&tft);

void setup() {
  Serial.begin(115200);
  
  // 1. Iniciar Bluetooth
  // El nombre "Monitor_ESP32" aparecerá en tu PC/Celular
  if(!SerialBT.begin("Monitor_ESP32")) {
    Serial.println("Error al iniciar Bluetooth");
  } else {
    Serial.println("Bluetooth iniciado! Esperando conexión...");
  }

  // 2. Iniciar Pantalla
  tft.init();
  tft.setRotation(0); // Horizontal (320x240)
  tft.fillScreen(COLOR_FONDO);
  
  // 3. Dibujar la Interfaz Fija (Lo que no cambia)
  dibujarInterfazBase();
  
  // 4. Configurar Sprite para los números
  spr.setColorDepth(8);
  spr.createSprite(100, 100); // Un área para actualizar datos
  spr.fillSprite(COLOR_FONDO);
}

void loop() {
  // Verificar si hay datos por Bluetooth
  if (SerialBT.available()) {
    String data = SerialBT.readStringUntil('\n'); // Leer hasta fin de línea
    // Formato esperado: "CPU|RAM|TEMP" (Ej: "45|60|55")
    
    if (data.length() > 0) {
      parsearDatos(data);
      actualizarPantalla();
    }
  }
  delay(20);
}

// --- FUNCIONES AUXILIARES ---

void dibujarInterfazBase() {
  // Título Superior
  tft.fillRect(0, 0, 320, 30, COLOR_TITULO);
  tft.setTextColor(TFT_WHITE, COLOR_TITULO);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(2);
  tft.drawString("SYSTEM MONITOR", 160, 15);

  // Etiquetas
  tft.setTextDatum(TL_DATUM); // Top Left
  tft.setTextColor(TFT_SILVER, COLOR_FONDO);
  tft.setTextSize(2);
  
  tft.drawString("CPU USAGE:", 20, 60);
  tft.drawString("RAM USAGE:", 20, 110);
  tft.drawString("GPU/TEMP:",  20, 160);
  
  // Líneas decorativas
  tft.drawFastHLine(0, 220, 320, COLOR_TITULO);
  tft.setTextSize(1);
  tft.setTextColor(TFT_DARKGREY, COLOR_FONDO);
  tft.drawRightString("Conectado: Bluetooth", 310, 225, 2);
}

void parsearDatos(String data) {
  // Separa el texto recibido por el simbolo "|"
  // Ejemplo: "25|40|50" -> CPU=25, RAM=40, TEMP=50
  
  int primerSeparador = data.indexOf('|');
  int segundoSeparador = data.indexOf('|', primerSeparador + 1);
  
  if (primerSeparador > 0) {
    cpuVal = data.substring(0, primerSeparador);
    if (segundoSeparador > 0) {
      ramVal = data.substring(primerSeparador + 1, segundoSeparador);
      tempVal = data.substring(segundoSeparador + 1);
    } else {
      ramVal = data.substring(primerSeparador + 1);
    }
  }
}

void actualizarPantalla() {
  // Dibujar barras de progreso
  int cpu = cpuVal.toInt();
  int ram = ramVal.toInt();
  
  // Barra CPU
  int anchoBarra = 150;
  int xBarra = 150;
  
  // Borrar valores numéricos viejos (pintando negro encima)
  tft.fillRect(xBarra, 60, 60, 20, COLOR_FONDO);
  tft.fillRect(xBarra, 110, 60, 20, COLOR_FONDO);
  tft.fillRect(xBarra, 160, 60, 20, COLOR_FONDO);

  // Escribir nuevos números
  tft.setTextSize(2);
  tft.setTextColor(COLOR_CPU, COLOR_FONDO);
  tft.drawString(cpuVal + "%", xBarra, 60);
  
  tft.setTextColor(COLOR_RAM, COLOR_FONDO);
  tft.drawString(ramVal + "%", xBarra, 110);
  
  tft.setTextColor(COLOR_TEMP, COLOR_FONDO);
  tft.drawString(tempVal + " C", xBarra, 160);

  // Dibujar barras gráficas (debajo de los textos)
  dibujarBarra(20, 85, cpu, COLOR_CPU);
  dibujarBarra(20, 135, ram, COLOR_RAM);
}

void dibujarBarra(int x, int y, int porcentaje, uint16_t color) {
  // Marco de la barra
  tft.drawRect(x, y, 280, 12, TFT_WHITE);
  // Relleno (mapeado de 0-100% a 0-276 pixeles)
  int longitud = map(porcentaje, 0, 100, 0, 276);
  
  // Limitar para no salirse
  if(longitud > 276) longitud = 276;
  if(longitud < 0) longitud = 0;

  // Dibujar relleno
  tft.fillRect(x+2, y+2, longitud, 8, color);
  // Dibujar el resto en negro (para borrar si baja el porcentaje)
  tft.fillRect(x+2+longitud, y+2, 276-longitud, 8, TFT_BLACK); // Fondo negro de barra vacía
}
