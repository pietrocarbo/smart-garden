
#include <SPI.h>
#include <WiFi.h>
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  30    /* Time ESP32 will go to sleep (in seconds) */

char ssid[] = "SmartGarden"; //  your network SSID (name)
char pass[] = "longpassword";    // your network password (use for WPA, or use as key for WEP)

int status = WL_IDLE_STATUS;

char server[] = "192.168.4.1";

WiFiClient client;

int sensorPin = 34;


void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  pinMode(sensorPin, INPUT);

  int j = 0;
  int sensorValue = 0;
  while(j < 10) {
    sensorValue = sensorValue + analogRead(sensorPin);
    j = j + 1;
    delay(300);
  }
  sensorValue = sensorValue / 10;

  // attempt to connect to Wifi network:
  int i = 0;
  while (status != WL_CONNECTED) {
    i = i + 1;
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    WiFi.mode(WIFI_STA);
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
    if (i == 10) {
      esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP/4 * uS_TO_S_FACTOR);
      esp_deep_sleep_start();
    }
  }
  Serial.println("Connected to wifi");
  printWifiStatus();

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server, 1880)) {
    Serial.println("connected to server");
    // Make a HTTP request:    
    char body[50];
    sprintf(body, "{\"station\":\"Station-1\",\"luminosity\":%d}", sensorValue);
    
    client.print(String("PUT ") + "/sensor HTTP/1.1\r\n" +
                 "Host: 192.168.4.1:1880\r\n" +
                 "Connection: close\r\n" +
                 "Content-Type: application/json\r\n" + 
                 "Content-length: " + strlen(body) + "\r\n\r\n" +
                 body);
  }
}

void loop() {
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();

    // do nothing forevermore:
    Serial.println("Going to sleep");
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
    esp_deep_sleep_start();
  }
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}





