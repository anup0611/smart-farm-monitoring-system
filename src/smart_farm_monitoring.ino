#include <DHT.h>

#define DHT_PIN D4
#define DHT_TYPE DHT11

#define SOIL_PIN A0
#define RELAY_PIN D1

DHT dht(DHT_PIN, DHT_TYPE);

const float TEMP_LIMIT = 30.0;

void setup() {
  Serial.begin(115200);

  dht.begin();

  pinMode(RELAY_PIN, OUTPUT);

  digitalWrite(RELAY_PIN, HIGH);

  Serial.println("Smart Farm Monitoring System Started");
}

void loop() {

  static unsigned long lastReadTime = 0;

  if (millis() - lastReadTime < 2000) {
    return;
  }

  lastReadTime = millis();

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  int soilMoisture = analogRead(SOIL_PIN);

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("DHT11 sensor reading failed");
    delay(2000);
    return;
  }

  Serial.println("----------------------------");

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("Soil Moisture: ");
  Serial.println(soilMoisture);

  if (temperature > TEMP_LIMIT) {

    digitalWrite(RELAY_PIN, LOW);

    Serial.println("High temperature detected");
    Serial.println("Ventilation Fan: ON");

  } else {

    digitalWrite(RELAY_PIN, HIGH);

    Serial.println("Temperature normal");
    Serial.println("Ventilation Fan: OFF");
  }

  delay(2000);
}
