#include <M5StickC.h>
#include <ArduinoJson.h>
#include <Ambient.h>
#include <WiFi.h>

WiFiClient client;
const char *ssid = "SSID";
const char *password = "PASSWORD";

Ambient ambient;
unsigned int channelId = 10000; // CHANNEL ID
const char *writeKey = "WRITE_KEY";

int lastSendTime = 0;

DynamicJsonDocument doc(1024);

void setup()
{
  M5.begin();

  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 33, 32);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {}
}

void loop()
{
  if (Serial2.available())
  {
    String receiveString = Serial2.readStringUntil('\n');

    // JSONをデシリアライズ
    deserializeJson(doc, receiveString);

    // こんな感じで中身を取り出せる
    double x = doc["face"][0]["x"];
    Serial.println(x);

    // 一定間隔でAmbientに送信
    if (millis() - lastSendTime > 60000)
    {
      ambient.begin(channelId, writeKey, &client);
      ambient.set(1, x);
      ambient.send();

      lastSendTime = millis();
    }
  }
}
