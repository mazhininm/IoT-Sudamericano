import network
import socket
import time

# Configuración del Access Point
SSID = "HOLA"
PASSWORD = "12345678"  # Mínimo 8 caracteres

ap = network.WLAN(network.AP_IF)
ap.config(essid=SSID, password=PASSWORD, authmode=network.AUTH_WPA_WPA2_PSK)
ap.active(True)

# IP fija para el AP
ap.ifconfig(('192.168.4.1', '255.255.255.0', '192.168.4.1', '8.8.8.8'))
print("📶 AP activo")
print("SSID:", SSID)
print("Contraseña:", PASSWORD)
print("IP del AP:", ap.ifconfig()[0])
print("Conecta tu teléfono a esta red para usar Fing.\n")

# Servidor TCP para recibir datos del sensor
def start_server():
    addr = socket.getaddrinfo('0.0.0.0', 8888)[0][-1]
    s = socket.socket()
    s.bind(addr)
    s.listen(1)
    print("📡 Servidor esperando conexión del ESP32 secundario...\n")

    while True:
        try:
            cl, addr = s.accept()
            print(f"✅ Conexión desde: {addr[0]}")
            data = cl.recv(128)
            if data:
                msg = data.decode().strip()
                print(f"🌡️  Datos del sensor: {msg}")
            cl.close()
        except Exception as e:
            print("❌ Error en servidor:", e)

# Iniciar servidor
start_server()
