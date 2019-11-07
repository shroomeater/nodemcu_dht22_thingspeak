// Open Lanuza Thingspeak Ver 1.3
//
//MEDIDOR DE TEMPERATURA Y HUMEDAD
//Publica datos en Thingspeak
//www.openlanuza.com

#include "DHT.h" // Librería sensores DHT
#include "ThingSpeak.h" // Include ThingSpeak Library
#include <ESP8266WiFi.h> // Include ESP8266wifi Library

#define DHTPIN D6 // pin al que hemos conectado el DHT 22

char ssid[] = "xxxxx"; // Nombre del WiFi (nombre del router)
char pass[] = "xxxxx"; // WiFi router password
unsigned long myChannelNumber = 000000; // Thingspeak número de canal
const char * myWriteAPIKey = "xxxxxx"; // ThingSpeak write API Key

/* creamos el objeto DHT para utilizar sus funciones, especificando que nuestro
modelo es el DHT22. Existen otros como el DHT11
*/

DHT dht(DHTPIN, DHT22);

int status = WL_IDLE_STATUS;

WiFiClient client;

void setup() {

WiFi.begin(ssid, pass); // Inicia WiFi
ThingSpeak.begin(client); // Inicia ThingSpeak
dht.begin(); // Inicia el sensor

Serial.begin(115200); // Velocidad puerto serie

Serial.println();
Serial.print("Conectando a ");
Serial.print(ssid);

while (WiFi.status() != WL_CONNECTED)
{
delay(500);
Serial.print(".");
}

Serial.println();
Serial.println("Conectado a WiFi");
Serial.print("Dirección IP: ");
Serial.println(WiFi.localIP());

}

void loop() {

// Hacemos pausa de dos segundos antes de cada nueva medición
//al sensor le cuesta 250ms leer estos datos
delay(2000);

//leemos humedad
float h = dht.readHumidity();

// usamos variables tipo float para guardar valores decimales
// Leemos temperatura
float t = dht.readTemperature();

// Comprobamos si las lecturas pueden dar algún fallo mediante la función isnan()
// Esta función devuelve un 1 en caso de que el valor no sea numérico
// Los caracteres || son como un OR. Si se cumple una de las dos condiciones mostramos error
if (isnan(h) || isnan(t)) {
Serial.println("Fallo en la lectura del sensor DHT!");
return;
}

Serial.print("Humedad: ");
Serial.print(h);
Serial.print(" %\t");
Serial.print("Temperatura: ");
Serial.print(t);
Serial.println(" *C\t ");

// Carga los valores a enviar
ThingSpeak.setField(1, t);
ThingSpeak.setField(2, h);

// Escribe todos los campos a la vez.
ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

Serial.println("¡Datos enviados a ThingSpeak!");

// Añadimos un retraso para limtitar el número de escrituras en Thinhspeak

int duracionDelay = 300; //En segundos
for (int i = 0; i < duracionDelay; i ++) { //Esto es debido a que el máximo que el Arduino puede procesar con precisión es 5000ms o 5 segundos
delay (1000);
}

}
