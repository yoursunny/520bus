#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Streaming.h>
#include "credentials.hpp"

LiquidCrystal_I2C lcd(0x3F, 16, 2);

WiFiClient mqttSocket;
PubSubClient mqtt(IBMIOT_HOST, IBMIOT_PORT, mqttSocket);

void
connect()
{
  if (WiFi.status() == WL_CONNECTED && mqtt.connected()) {
    return;
  }

  Serial << "Connecting to WiFi ";
  lcd.clear();
  lcd << "WiFi " << WIFI_SSID;
  lcd.setCursor(14, 0);
  lcd << "  ";
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  for (int i = 0; i < 60 && WiFi.status() != WL_CONNECTED; ++i) {
    if (WiFi.status() == WL_CONNECT_FAILED) {
      WiFi.begin(WIFI_SSID, WIFI_PASS);
    }
    delay(1000);
    Serial << WiFi.status();
    lcd.setCursor(15, 0);
    lcd.print(WiFi.status());
  }
  if (WiFi.status() != WL_CONNECTED) {
    Serial << endl << "Cannot connect to WiFi" << endl;
    lcd.setCursor(5, 0);
    lcd << "FAIL";
    delay(5000);
    ESP.restart();
    return;
  }
  Serial << endl << "Connected to WiFi " << WiFi.localIP() << endl;
  lcd.clear();
  lcd << WiFi.localIP();

  Serial << "Connecting to IBM IoT ";
  lcd.setCursor(0, 1);
  lcd << "IBM IoT ";
  mqtt.connect(IBMIOT_DEVID, "use-token-auth", IBMIOT_TOKEN);
  for (int i = 0; i < 60 && !mqtt.connected(); ++i) {
    delay(1000);
    Serial << '.';
  }
  if (!mqtt.connected()) {
    Serial << endl << "Cannot connect to IBM IoT" << endl;
    lcd << "FAIL";
    delay(5000);
    ESP.restart();
    return;
  }
  mqtt.subscribe("iot-2/cmd/+/fmt/json");
  Serial << endl << "Connected to IBM IoT" << endl;
  lcd << "OK";
}

void
handleRefreshCommand(JsonObject& payload)
{
  lcd.clear();
  lcd << payload["L1"].asString();
  lcd.setCursor(0, 1);
  lcd << payload["L2"].asString();
}

void
handleCommand(char* topic, byte* payload, unsigned int length)
{
  Serial << "Received command " << topic << ' ';

  StaticJsonBuffer<1024> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(reinterpret_cast<char*>(payload));

  if (!root.success()) {
    Serial << "Payload parsing failed" << endl;
    return;
  }

  root.printTo(Serial);
  Serial << endl;

  if (strcmp(topic, "iot-2/cmd/refresh/fmt/json") == 0) {
    handleRefreshCommand(root);
  }
  else {
    Serial << "Unknown command" << endl;
  }
}

void
setup()
{
  Wire.pins(12, 14); // D5-SCL, D6-SDA

  Serial.begin(115200);
  Serial.println();
  Serial.println();

  WiFi.disconnect();
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.setSleepMode(WIFI_MODEM_SLEEP);

  mqtt.setCallback(handleCommand);

  lcd.begin(16, 2);
  lcd.init();
  lcd.backlight();
}

void
loop()
{
  connect();
  mqtt.loop();
  delay(100);
}
