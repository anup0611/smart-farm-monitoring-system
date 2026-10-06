#include <DHT.h>

#define DHT_PIN D4
#define DHT_TYPE DHT11

#define SOIL_PIN A0
#define RELAY_PIN D1

#define RELAY_ON LOW
#define RELAY_OFF HIGH

DHT dht(DHT_PIN, DHT_TYPE);

const float TEMP_LIMIT = 30.0;

void setup() {
  Serial.begin(115200);

  dht.begin();

  pinMode(RELAY_PIN, OUTPUT);

  digitalWrite(RELAY_PIN, RELAY_OFF);

  Serial.println("Smart Farm Monitoring System Started");
}

void loop() {

  static unsigned long lastReadTime = 0;

  // Read sensors every 2 seconds
  if (millis() - lastReadTime < 2000) {
    return;
  }

  lastReadTime = millis();

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  int soilMoisture = analogRead(SOIL_PIN);

  // Convert raw soil moisture value to approximate percentage
  int soilMoisturePercent = map(soilMoisture, 1023, 0, 0, 100);

  // Keep percentage within 0-100 range
  soilMoisturePercent = constrain(soilMoisturePercent, 0, 100);

  // Check whether DHT11 readings are valid
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

  Serial.print("Soil Moisture Raw: ");
  Serial.println(soilMoisture);

  Serial.print("Soil Moisture: ");
  Serial.print(soilMoisturePercent);
  Serial.println(" %");

  // Automatic ventilation control
  if (temperature > TEMP_LIMIT) {

    digitalWrite(RELAY_PIN, RELAY_ON);

    Serial.println("High temperature detected");
    Serial.println("Ventilation Fan: ON");

  } else {

    digitalWrite(RELAY_PIN, RELAY_OFF);

    Serial.println("Temperature normal");
    Serial.println("Ventilation Fan: OFF");
  }
}
