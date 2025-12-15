import serial
import psutil
import time

# --- CONFIGURACIÓN ---
# ¡CAMBIA ESTO POR TU PUERTO BLUETOOTH! (Ver en config de Windows)
PUERTO_COM = 'COM4'  
BAUD_RATE = 115200

try:
    # Conectar al ESP32
    ser = serial.Serial(PUERTO_COM, BAUD_RATE, timeout=1)
    print(f"Conectado a {PUERTO_COM}")
    time.sleep(2) # Esperar a que estabilice

    while True:
        # 1. Obtener datos de la PC
        cpu = int(psutil.cpu_percent())
        ram = int(psutil.virtual_memory().percent)
        
        # Nota: Leer temperatura en Windows es difícil sin librerías complejas.
        # Por ahora simularemos temperatura o enviaremos un valor fijo/calculado.
        temp = 45 + (cpu / 5) # Simulación basada en carga de CPU
        
        # 2. Crear el mensaje formato: "CPU|RAM|TEMP\n"
        mensaje = f"{cpu}|{ram}|{int(temp)}\n"
        
        # 3. Enviar por Bluetooth
        ser.write(mensaje.encode('utf-8'))
        print(f"Enviado: {mensaje.strip()}")
        
        time.sleep(1) # Actualizar cada segundo

except Exception as e:
    print(f"Error: {e}")
    print("Asegúrate de que el puerto COM es correcto y el ESP32 está emparejado.")
finally:
    if 'ser' in locals() and ser.is_open:
        ser.close()
