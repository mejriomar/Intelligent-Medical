#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_MLX90614.h>
#include "MAX30105.h"
#include "spo2_algorithm.h"
#include "heartRate.h"
#include <WiFi.h>
#include <PubSubClient.h>

/* ====== PINS ====== */
#define BUZZER_PIN 17
#define I2C1_SDA   25
#define I2C1_SCL   26

/* ===== WIFI ===== */
const char* ssid = "omar";
const char* password = "123456788.";

/* ===== MQTT ===== */
const char* mqtt_server = "192.168.43.89";
const int mqtt_port = 1883;
const char* topic = "esp32/health";

WiFiClient espClient;
PubSubClient client(espClient);

/* ====== OBJECTS ====== */
LiquidCrystal_I2C lcd(0x27, 20, 4);
Adafruit_MLX90614 mlx;
MAX30105 particleSensor;

/* ====== MAX30102 VARIABLES ====== */
uint32_t irBuffer[50];
uint32_t redBuffer[50];
int32_t spo2_value, heartRate_value,heartRate_final;
int8_t validSPO2, validHeartRate;
int32_t spo2_final = 0, CFV = 0;

long lastBeat = 0;
float beatsPerMinute = 0;
bool beatStarted = false;
int proximityThreshold = 3000;

/* ====== TEMP VARIABLES ====== */
float temperature = 0;

/* ====== TIMING ====== */
unsigned long lastMeasurement = 0;
const unsigned long MEASUREMENT_INTERVAL = 500;

/* ======================================================= */
/* ==================== INITIALIZATION =================== */
/* ======================================================= */
/* ===== Fonctions ===== */
void setup_wifi() {
  delay(100);
  Serial.println("Connexion WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connecté");
}
void reconnect() {
  while (!client.connected()) {
    Serial.print("Connexion MQTT...");
    if (client.connect("ESP32_Health_Simulator")) {
      Serial.println("connecté");
    } else {
      Serial.print("échec, rc=");
      Serial.print(client.state());
      delay(2000);
    }
  }
}
void initLCD() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(3, 0);
  lcd.print("Health Care");

  lcd.setCursor(0, 1);
  lcd.print("Temp: -- C");

  lcd.setCursor(0, 2);
  lcd.print("HR:-- SPO2:--%");

  lcd.setCursor(0, 3);
  lcd.print("BPM:--");
}

void initMLX90614() {
  if (!mlx.begin()) {
    Serial.println("MLX90614 not found!");
    while (1);
  }
}

void initMAX30102() {
  Wire1.begin(I2C1_SDA, I2C1_SCL, 100000);

  if (!particleSensor.begin(Wire1, I2C_SPEED_FAST)) {
    Serial.println("MAX30102 not found!");
    while (1);
  }

  particleSensor.setup(
    50,   // LED brightness
    1,    // Sample average
    2,    // LED mode
    100,  // Sample rate
    69,   // Pulse width
    4096  // ADC range
  );

  particleSensor.enablePROXINT();
  particleSensor.setPROXINTTHRESH(proximityThreshold / 16);
}

/* ======================================================= */
/* ====================== FUNCTIONS ====================== */
/* ======================================================= */

void readTemperature() {
  temperature = mlx.readObjectTempC();

  if (temperature < 0 || temperature > 100 || isnan(temperature)) return;

  digitalWrite(BUZZER_PIN, temperature > 38);

  lcd.setCursor(6, 1);
  lcd.print(temperature);
  lcd.print(" C ");
}

void collectPulseSamples() {
  for (byte i = 0; i < 50; i++) {
    while (!particleSensor.available())
      particleSensor.check();

    redBuffer[i] = particleSensor.getRed();
    irBuffer[i]  = particleSensor.getIR();
    particleSensor.nextSample();
  }
}

void calculateBPM() {
  for (byte i = 0; i < 50; i++) {
    if (checkForBeat(irBuffer[i])) {
      long delta = millis() - lastBeat;
      lastBeat = millis();

      if (delta > 300) {
        beatsPerMinute = 60000.0 / delta;
        beatStarted = true;
      }
    }
  }
}

void readPulseOximeter() {
  collectPulseSamples();

  maxim_heart_rate_and_oxygen_saturation(
    irBuffer, 50,
    redBuffer,
    &spo2_value, &validSPO2,
    &heartRate_value, &validHeartRate
  );

  if (validSPO2) spo2_final = spo2_value;
  if (validHeartRate) heartRate_final = heartRate_value;
  
  if(heartRate_final > 120) heartRate_final = 120;
  calculateBPM();
}

void updateLCD() {
  lcd.setCursor(0, 2);
  lcd.print("HR:");
  lcd.print(heartRate_final);
  lcd.print(" SPO2:");
  lcd.print(spo2_final);
  lcd.print("% ");

  lcd.setCursor(0, 3);
  lcd.print("BPM:");
  lcd.print((int)beatsPerMinute);
  lcd.print("   ");
}

/* ======================================================= */
/* ======================== SETUP ======================== */
/* ======================================================= */

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);

  initLCD();
  initMLX90614();
  initMAX30102();
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  Serial.println("System Ready");
}

/* ======================================================= */
/* ========================= LOOP ======================== */
/* ======================================================= */

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  uint32_t irValue = particleSensor.getIR();

  if (millis() - lastMeasurement < MEASUREMENT_INTERVAL) return;
  lastMeasurement = millis();

  readTemperature();

  if (irValue > proximityThreshold) {
    readPulseOximeter();
    updateLCD();

    Serial.printf(
      "{\"temp\":%.1f,\"bpm\":%d,\"spo2\":%d}\n",
      temperature, heartRate_final, spo2_final
    );
  } else {
    spo2_final = 0;
    heartRate_final = 0;
    beatsPerMinute = 0;

    lcd.setCursor(0, 2);
    lcd.print("HR:-- SPO2:--%");

    lcd.setCursor(0, 3);
    lcd.print("BPM:--");
  }

    char payload[128];
    snprintf(payload, sizeof(payload),"{\"temperature\": %.1f, \"bpm\": %d, \"spo2\": %d}",temperature, heartRate_final, spo2_final);
    Serial.println(payload);
    client.publish(topic, payload);
}
