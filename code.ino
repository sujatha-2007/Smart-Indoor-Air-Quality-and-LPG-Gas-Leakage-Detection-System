#define BLYNK_TEMPLATE_ID "TMPL3-n-cLd3R"
#define BLYNK_TEMPLATE_NAME "airMonitor1"
#define BLYNK_AUTH_TOKEN "Qt5Joadw_D4H8s0F3qgRhy3c9bPoU9N9"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <DHT.h>

// -------- WIFI --------
char ssid[] = "iQOO Neo 10R";
char pass[] = "ammuchinnu";

// -------- OLED --------
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

// -------- DHT --------
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// -------- MQ --------
#define MQ2_PIN 35
#define MQ135_PIN 34

int mq135_base = 0;
int mq2_base = 0;

// -------- ALERT --------
#define RED_LED_PIN 18
#define GREEN_LED_PIN 5
#define BUZZER_PIN 19

BlynkTimer timer;

// -------- READ AVERAGE --------
int readMQAverage(int pin) {
  long sum = 0;
  for (int i = 0; i < 10; i++) {
    sum += analogRead(pin);
    delay(10);
  }
  return sum / 10;
}

// -------- MAIN FUNCTION --------
void sendData() {

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  if (isnan(temp) || isnan(hum)) return;

  int mq2 = readMQAverage(MQ2_PIN);
  int mq135 = readMQAverage(MQ135_PIN);

  // -------- AQI (SCALED TO 100s) --------
  int aqi = (mq135 - mq135_base)*0.3+110;
  aqi = constrain(aqi, 0, 200);

  // -------- GAS DETECTION --------
  bool gasLeak = mq2 > (mq2_base + 800);

  // -------- STATUS --------
  String status;
  if (gasLeak) status = "GAS LEAK!";
  else if (aqi <= 50) status = "GOOD";
  else if (aqi <= 100) status = "MOD";
  else if (aqi <= 150) status = "WARN";
  else status = "DANGER";

  // -------- BLYNK --------
  Blynk.virtualWrite(V0, temp);
  Blynk.virtualWrite(V1, hum);
  Blynk.virtualWrite(V2, mq2);
  Blynk.virtualWrite(V3, status);
  Blynk.virtualWrite(V4, mq135);

  // -------- ALERT SYSTEM --------
  static bool blinkState = false;
  blinkState = !blinkState;

  if (gasLeak) {
    digitalWrite(RED_LED_PIN, HIGH);
    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, HIGH);

  } else if (aqi <= 50) {
    digitalWrite(GREEN_LED_PIN, HIGH);
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);

  } else if (aqi <= 100) {
    digitalWrite(GREEN_LED_PIN, HIGH);
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);

  } else if (aqi <= 150) {
    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, blinkState);
    digitalWrite(BUZZER_PIN, LOW);

  } else {
    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
  }

  // -------- OLED --------
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x12_tr);

  u8g2.drawStr(10, 10, "AIR QUALITY");

  u8g2.setCursor(0, 25);
  u8g2.print("Temp: ");
  u8g2.print(temp);

  u8g2.setCursor(0, 38);
  u8g2.print("Hum : ");
  u8g2.print(hum);

  u8g2.setCursor(0, 51);
  u8g2.print("AQI : ");
  u8g2.print(aqi);

  u8g2.setCursor(0, 64);
  u8g2.print("MQ2 : ");
  u8g2.print(mq2);

  u8g2.sendBuffer();

  // -------- DEBUG --------
  Serial.print("MQ2: "); Serial.print(mq2);
  Serial.print(" MQ135: "); Serial.print(mq135);
  Serial.print(" AQI: "); Serial.println(aqi);
}

// -------- SETUP --------
void setup() {
  Serial.begin(9600);
  Wire.begin(21, 22);

  dht.begin();
  u8g2.begin();

  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  analogSetPinAttenuation(MQ2_PIN, ADC_11db);
  analogSetPinAttenuation(MQ135_PIN, ADC_11db);

  // -------- AUTO CALIBRATION --------
  for (int i = 0; i < 50; i++) {
    mq135_base += analogRead(MQ135_PIN);
    mq2_base += analogRead(MQ2_PIN);
    delay(20);
  }
  mq135_base /= 50;
  mq2_base /= 50;

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(2000L, sendData);

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x12_tr);
  u8g2.drawStr(10, 30, "Calibrating...");
  u8g2.sendBuffer();
  delay(2000);
}

// -------- LOOP --------
void loop() {
  Blynk.run();
  timer.run();
}
