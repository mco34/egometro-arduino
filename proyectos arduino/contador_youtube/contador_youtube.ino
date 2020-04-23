// YouTube Subscriber Counter
// Proyecto Original de Becky Stern 2017
// Modificado Por SrFerrete. 
// Modificado Por @vivirenremoto. 


//////////// modifica esta parte
char wifi_ssid[] = "xxxxxxxx";
char wifi_password[] = "xxxxxxxx";
#define GOOGLE_API_KEY "xxxxxxxx"
#define YOUTUBE_CHANNEL_ID "xxxxxxxx"
//////////// modifica esta parte

#include <ESP8266WiFi.h>
#include <YoutubeApi.h>
#include <MAX7219.h>
#include <SPI.h>

#define PIN_CS_MAX7219 0 // Pin de Arduino al que se ha conectado el pin CS/SS del MAX7219
#define DIGITOS_DISPLAY_LED 8 // El display LED que se ha usado tiene 8 dígitos
MAX7219 pantalla(PIN_CS_MAX7219, DIGITOS_DISPLAY_LED);

int api_mtbs = 1000; // time between api requests
long api_lasttime; // last time api request has been done

BearSSL::WiFiClientSecure client;
YoutubeApi api(GOOGLE_API_KEY, client);

void setup() {
  Serial.begin(115200);
  
  client.setTimeout(api_mtbs);
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
  Serial.println(wifi_ssid);
  WiFi.begin(wifi_ssid, wifi_password);
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
  if ( millis() > ( api_lasttime + api_mtbs ) )
  {
    if( api.getChannelStatistics( YOUTUBE_CHANNEL_ID ))
    {
      pantalla.mostrar_cifra( api.channelStats.subscriberCount );
    }
    api_lasttime = millis();
  }
}
