# Laboratorio 5 - Coap Server
Para la pregunta 3 se implementó el Coap Server como un servidor web para manejar peticiones desde distintos puntos.

## Dependencias

1. Para Arduino
- Dispositivo ESP32
- Sensor DHT11
- [COAP Simple Library](https://github.com/hirotakaster/CoAP-simple-library)
- DHT Sensor Library by Adafruit
    
## Instrucciones
Tanto el Coap Server como el Coap Client requieren Python 3 para su ejecución, para el Coap Client ESP32 se utiliza Arduino. 

### Coap Server
Dentro de la carpeta server ejecutar para instalar las dependencias:
>> pip3 install -r requirements.txt

Para ejecutar 
>>python server.py

### Coap Client
Dentro de la carpeta server ejecutar para instalar las dependencias:
>> pip3 install -r requirements.txt

Para ejecutar 
>>python client_put.py

### Coap Client - ESP32
Dentro de la carpeta ESP32 se encuentran los archivos para realizar el deploy en el dispositivo ESP32.

Nota: La carpeta ESP38 fue un intento fallido de implementarlo en el dispositivo ESP82.   