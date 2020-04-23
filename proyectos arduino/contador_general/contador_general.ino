// General Counter
// Proyecto Original de Becky Stern 2017
// Modificado Por SrFerrete. 
// Modificado Por @vivirenremoto. 


//////////// modifica esta parte
#define WIFI_SSID "xxxxxxxx"
#define WIFI_PASSWORD "xxxxxxxx"
#define API_HOST "dominio.com"
#define API_URL "/analytics23411/ver.php?id=xxxx"
#define API_PORT 443
#define API_TIMEOUT 300000
//////////// modifica esta parte

#include <ESP8266WiFi.h>
#include <GeneralApi.h>
#include <MAX7219.h>
#include <SPI.h>

#define CLIENT_TIMEOUT 1500
#define PIN_CS_MAX7219 0 // Pin de Arduino al que se ha conectado el pin CS/SS del MAX7219
#define DIGITOS_DISPLAY_LED 8 // El display LED que se ha usado tiene 8 dígitos
MAX7219 pantalla(PIN_CS_MAX7219, DIGITOS_DISPLAY_LED);

long api_lasttime; // last time api request has been done

BearSSL::WiFiClientSecure client;
GeneralApi api(client, API_HOST, API_URL, API_PORT, CLIENT_TIMEOUT);

void setup() {
  Serial.begin(115200);
  
  client.setTimeout(CLIENT_TIMEOUT);
  client.setInsecure();
  
  SPI.begin(); // Para no interferir con otro código que utilice SPI, la librería MAX7219 no inicializa SPI y hay que hacerlo desde el programa
  pantalla.inicializar(); // SPI debe estar inicializado antes de inicializar el MAX7219 (que usa SPI)
  pantalla.apagar();
  pantalla.desactivar_modo_prueba();
  pantalla.brillo(15); // Brillo medio (va desde cero hasta 15)
  pantalla.borrar();
  pantalla.encender();

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  
  Serial.print("Connecting Wifi: ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
}

void loop() {
  if ( !api_lasttime || ( millis() > ( api_lasttime + API_TIMEOUT ) ) )
  {
    if( api.get())
    {
      pantalla.mostrar_cifra( api.info.data );
    }
    api_lasttime = millis();
  }
}
