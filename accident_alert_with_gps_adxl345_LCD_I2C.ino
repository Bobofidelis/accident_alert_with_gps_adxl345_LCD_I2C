#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <TinyGPS++.h>
#include <LiquidCrystal_I2C.h>

// Replace with your network credentials
const char* ssid = "myjoy";
const char* password = "iloveyou";

//// Initialize Telegram BOT
//#define BOTtoken "6831581865:AAH4wqYjtw7TCmQtDiPo-f8dEF6viuI1TC8"  // your Bot Token (Get from Botfather)
//
//// Use @myidbot to find out the chat ID of an individual or a group
//// Also note that you need to click "start" on a bot before it can
//// message you
//#define CHAT_ID "2014246209"

#define BOTtoken "7095957866:AAFG2WtKlUCA4FIbKGYZ77x9FQKJ9k4EkW8"  // your Bot Token (Get from Botfather)

// Use @myidbot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
#define CHAT_ID "1136386551"

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Initialize ADXL345
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

// Initialize GPS
TinyGPSPlus gps;

// Initialize LCD
LiquidCrystal_I2C lcd(0x3F, 16, 2);  // Address 0x27, 16 column and 2 rows


void setup() {
  Serial.begin(115200);
  Serial2.begin(9600);
   // Initialize LCD
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Connecting Wifi:");
  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  lcd.clear();
   lcd.setCursor(0, 0);
  lcd.print("WiFi connected");
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Initialize ADXL345
  if (!accel.begin()) {
    Serial.println("Could not find a valid ADXL345 sensor, check wiring!");
    while (1);
  }

  // Set accelerometer range
  accel.setRange(ADXL345_RANGE_16_G);

  // Print the header
  Serial.println("Accident Alert System");
  lcd.clear();
   lcd.setCursor(0, 0);
  lcd.print(" MADE BY");
  lcd.setCursor(0, 1);
  lcd.print("  PRAISE");
  delay(2000);
  bot.sendMessage(CHAT_ID, "Bot started up", "");
}

void loop() {
  // Read accelerometer data
  sensors_event_t event;
  accel.getEvent(&event);
  // Check for an accident (e.g., sudden acceleration)
  if (abs(event.acceleration.x) < 5 || abs(event.acceleration.y) > 5 || abs(event.acceleration.z) > 2) {
    lcd.clear();
    lcd.setCursor(0, 0);
  lcd.print("Accident detected");
    Serial.println("Accident detected!");
    displayInfo();
    
    bot.sendMessage(CHAT_ID, "Accident detected!!", "");
    
    bot.sendMessage(CHAT_ID, "Accident detected!\nLocation: https://maps.google.com/?q=" + String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6), "");

  displayInfo();
//    // Get GPS location
//    while (gps.location.isValid() == 0) {
//      // Read GPS data
//      while (Serial.available() > 0) {
//        gps.encode(Serial.read());
//      }
//    }
    bot.sendMessage(CHAT_ID, "Accident detected!!", "");
    // Send location to Telegram
    
    bot.sendMessage(CHAT_ID, "Accident detected!\nLocation: https://maps.google.com/?q=" + String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6), "");

    // Wait for a while to avoid sending multiple alerts for the same accident
    delay(10000);
  }
  else{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("NO Accident");
    Serial.println("NO Accident detected!");
  }

  delay(1000);
}
void displayInfo()


{
   while (Serial2.available() > 0)


   if (gps.encode(Serial2.read()))

  Serial.print(F("Location: "));


  if (gps.location.isValid()){


    Serial.print("Lat: ");


    Serial.print(gps.location.lat(), 6);


    Serial.print(F(","));


    Serial.print("Lng: ");


    Serial.print(gps.location.lng(), 6);


    Serial.println();


  }  


  else


  {


    Serial.print(F("INVALID"));


  }

}
