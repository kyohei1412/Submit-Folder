#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

const char* SSID = "ESP32_SERVO";
const char* PASS = "12345678";

WebServer server(80);

Servo servo;

const int SERVO_PIN = 14;     // MG90Sの信号線をGPIO14へ

int currentAngle = 0;

String makePage() {
  String html;
  html += "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<title>Smart Servo</title>";
  html += "<style>";
  html += "body{font-family:sans-serif;text-align:center;margin-top:40px;}";
  html += "button{font-size:28px;padding:15px 40px;margin:20px;}";
  html += "</style></head><body>";
  html += "<h1>ESP32 Servo Control</h1>";
  html += "<p>現在角度: " + String(currentAngle) + "°</p>";
  html += "<a href='/on'><button>ON (90°)</button></a><br>";
  html += "<a href='/off'><button>OFF (0°)</button></a>";
  html += "</body></html>";
  return html;
}

void handleRoot() {
  server.send(200, "text/html", makePage());
}

void handleOn() {
  currentAngle = 90;
  servo.write(currentAngle);
  server.sendHeader("Location", "/");
  server.send(303); // 再読み込みで角度表示が更新
}

void handleOff() {
  currentAngle = 0;
  servo.write(currentAngle);
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);

  servo.setPeriodHertz(50);                // 50Hz
  servo.attach(SERVO_PIN, 500, 2400);      // pin, min_us, max_us
  servo.write(0);
  delay(300);

  // WiFi アクセスポイント設定
  WiFi.mode(WIFI_AP);
  WiFi.softAP(SSID, PASS);

  Serial.println("WiFi AP モード起動");
  Serial.print("SSID: ");
  Serial.println(SSID);
  Serial.print("接続パス: ");
  Serial.println(PASS);
  Serial.print("アクセス先URL: http://");
  Serial.println(WiFi.softAPIP()); // 192.168.4.1

  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);
  server.begin();
  Serial.println("WebServer started");
}

void loop() {
  server.handleClient();
}

