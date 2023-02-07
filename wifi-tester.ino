#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFiNINA.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define SECRET_SSID "MON_SSID"
#define SECRET_PASS "MON_PASS"

void setup() {
  Serial.begin(9600);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  writeText("hello");
  connectToWifi();
}

void loop() {
  while(true) {
    delay(1000);
    long strength = WiFi.RSSI();
    writeText(String(strength));
  }
}

void connectToWifi() {
  char ssid[] = SECRET_SSID;
  char pass[] = SECRET_PASS;
  int status = WL_IDLE_STATUS; // the WiFi radio's status

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    writeText("Communication with WiFi module failed!");
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    writeText("Please upgrade the firmware");
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    writeText(String() + "Attempting to connect to " + ssid);
    status = WiFi.begin(ssid, pass);
    delay(5000);
  }

  IPAddress ip = WiFi.localIP();
  String ipStringified = toString(ip);
  writeText(ipStringified);
}

void writeText(String text) {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE); 
  display.setCursor(0, 0);
  display.cp437(true);    // Use full 256 char 'Code

  display.clearDisplay();
  display.write(text.c_str());
  display.display();
}

String toString(IPAddress address){
  return String() + address[0] + "." + address[1] + "." + address[2] + "." + address[3];
}

