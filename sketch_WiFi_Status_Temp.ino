#include <SPI.h>
#include <WiFiNINA.h>
#include "Arduino_SensorKit.h"

char ssid[] = "SSID Here";
char pass[] = "Password Here";
int keyIndex = 0;

int status = WL_IDLE_STATUS;
WiFiServer server(80);

void setup() {
  Serial.begin(9600);

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }
  server.begin();
  
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  Environment.begin();
}

void loop() {

  float temperatureF = (Environment.readTemperature() * 9.0 / 5.0) + 32.0;
  
  WiFiClient client = server.available();   
  if (client) {                             
    Serial.println("new client");           
    String currentLine = "";                
    while (client.connected()) {            
      if (client.available()) {             
        char c = client.read();             
        Serial.write(c);                    
        if (c == '\n') {                    

          if (currentLine.length() == 0) {

            client.print("Temperature = ");
            client.print(Environment.readTemperature()); //print temperature
            client.println(" C");
            client.print("Temperature = ");
            client.print(temperatureF); //print temperature
            client.println(" F");
            client.print("Humidity = ");
            client.print(Environment.readHumidity()); //print humidity
            client.println(" %");
            delay(2000);

            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }

    client.stop();
    Serial.println("client disonnected");
  }
}
