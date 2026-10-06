# Smart Farm Monitoring System - Testing

## 1. Testing Objective

The objective of testing is to verify that the Smart Farm Monitoring System correctly reads environmental sensor data and controls the ventilation fan according to the temperature threshold.

The testing process focuses on:

- DHT11 temperature and humidity monitoring
- Soil moisture monitoring
- Soil moisture percentage conversion
- Sensor reading interval
- Automatic ventilation control
- Temperature hysteresis
- DHT11 error handling
- Relay ON/OFF control

## 2. Test Environment

| Item | Details |
|---|---|
| Microcontroller | ESP8266 |
| Temperature/Humidity Sensor | DHT11 |
| Soil Moisture Sensor | Analog sensor |
| DHT11 Data Pin | D4 |
| Soil Moisture Pin | A0 |
| Relay Control Pin | D1 |
| Programming Language | C++ |
| Temperature ON Threshold | 30°C |
| Temperature OFF Threshold | 28°C |
| Sensor Sampling Interval | 2 seconds |

## 3. Test Cases

| Test ID | Test Condition | Input/Condition | Expected Result |
|---|---|---|---|
| TC01 | Normal temperature | Temperature below 28°C | Fan remains OFF |
| TC02 | High temperature | Temperature above 30°C | Fan turns ON |
| TC03 | Hysteresis range | Temperature between 28°C and 30°C | Fan maintains previous state |
| TC04 | Sensor sampling | Continuous monitoring | Sensor readings occur at approximately 2-second intervals |
| TC05 | Invalid DHT11 reading | DHT11 unavailable/disconnected | Error message is displayed |
| TC06 | Soil moisture monitoring | Soil sensor connected | Raw soil moisture value is displayed |
| TC07 | Soil moisture percentage | Soil sensor connected | Approximate moisture percentage between 0% and 100% is displayed |
| TC08 | Temperature returns to normal | Temperature below 28°C | Fan turns OFF |
| TC09 | High temperature after normal state | Temperature above 30°C | Fan turns ON |

## 4. Expected System Behaviour

### 4.1 Temperature Monitoring

The DHT11 sensor provides temperature and humidity readings. The system uses separate temperature thresholds for fan control.

```text
Temperature > 30°C
        ↓
      Fan ON

Temperature < 28°C
        ↓
      Fan OFF
```

When the temperature is between 28°C and 30°C, the system maintains the previous fan state.

### 4.2 Automatic Ventilation

When the temperature rises above 30°C, the ESP8266 activates the relay and the ventilation fan should turn ON. When the temperature falls below 28°C, the fan should turn OFF.

The relay states are explicitly defined in the program:

```cpp
#define RELAY_ON LOW
#define RELAY_OFF HIGH
```

### 4.3 Temperature Hysteresis

Temperature hysteresis was implemented to prevent frequent switching of the ventilation fan.

The system uses:

- Above 30°C → Fan ON
- Below 28°C → Fan OFF
- Between 28°C and 30°C → Maintain previous fan state

This reduces unnecessary relay switching when the temperature fluctuates around the threshold.

### 4.4 Sensor Sampling

The DHT11 sensor readings are controlled using a 2-second sampling interval. The system uses `millis()` to control the reading interval:

```cpp
static unsigned long lastReadTime = 0;

if (millis() - lastReadTime < 2000) {
    return;
}

lastReadTime = millis();
```

This prevents the system from continuously requesting DHT11 readings.

### 4.5 DHT11 Error Handling

If the DHT11 sensor does not provide a valid temperature or humidity reading, the system checks the reading using `isnan()`. If the reading is invalid, the system displays:

```text
DHT11 sensor reading failed
```

The system then waits before attempting another reading.

### 4.6 Soil Moisture Monitoring

The soil moisture sensor is connected to the analog input of the ESP8266. The system reads the sensor using:

```cpp
int soilMoisture = analogRead(SOIL_PIN);
```

The raw ADC value is displayed in the Serial Monitor.

### 4.7 Soil Moisture Percentage

The raw soil moisture ADC value is converted into an approximate percentage to make the reading easier to interpret. The conversion is performed using:

```cpp
int soilMoisturePercent = map(soilMoisture, 1023, 0, 0, 100);

soilMoisturePercent = constrain(soilMoisturePercent, 0, 100);
```

The Serial Monitor displays both the raw value and the approximate percentage. Example:

```text
Soil Moisture Raw: 650
Soil Moisture: 36 %
```

## 5. QA Issues Resolved

### Issue #1 - DHT11 Temperature Readings Are Unstable

**Problem:** DHT11 temperature readings required a controlled sampling interval.

**Root Cause:** The initial implementation did not provide a controlled sensor sampling mechanism.

**Solution:** A 2-second sampling interval was implemented using `millis()`.

**GitHub Resolution:** The fix was implemented in the `fix/temperature-reading` branch and merged into `main` through Pull Request #2.

**Status:** Resolved

### Issue #3 - Fan Does Not Turn ON Above Temperature Threshold

**Problem:** The ventilation fan could remain OFF when the temperature exceeded the defined 30°C threshold.

**Root Cause:** The relay control state was not explicitly defined in the original implementation.

**Solution:** Explicit relay ON and OFF states were defined:

```cpp
#define RELAY_ON LOW
#define RELAY_OFF HIGH
```

The automatic ventilation logic was updated to use these states.

**GitHub Resolution:** The fix was implemented in the `fix/fan-control` branch and merged through Pull Request #4.

**Status:** Resolved

### Issue #5 - Soil Moisture Readings Are Difficult to Interpret

**Problem:** The soil moisture sensor was displaying only the raw ADC value, which was difficult for users to interpret.

**Root Cause:** The raw analog value from the soil moisture sensor was not converted into a meaningful moisture percentage.

**Solution:** The raw soil moisture value was converted into an approximate percentage using the `map()` function. The system now displays:

- Raw soil moisture ADC value
- Approximate soil moisture percentage

The percentage is constrained between 0% and 100%.

**GitHub Resolution:** The fix was implemented in the `fix/soil-moisture-reading` branch and merged through Pull Request #6.

**Status:** Resolved

### Issue #7 - Fan Switches ON and OFF Too Frequently Near Temperature Threshold

**Problem:** The ventilation fan could switch ON and OFF repeatedly when the temperature fluctuated around the 30°C threshold.

**Root Cause:** The original implementation used the same temperature threshold for turning the fan ON and OFF.

**Solution:** Temperature hysteresis was implemented using separate ON and OFF thresholds:

- Temperature above 30°C → Fan ON
- Temperature below 28°C → Fan OFF
- Temperature between 28°C and 30°C → Maintain previous fan state

This reduces unnecessary relay switching.

**GitHub Resolution:** The fix was implemented in the `fix/fan-hysteresis` branch and merged through Pull Request #8.

**Status:** Resolved

## 6. Test Execution Status

The following test cases should be verified on the physical hardware.

| Test ID | Expected Observation | Result |
|---|---|---|
| TC01 | Fan remains OFF below 28°C | To be tested |
| TC02 | Fan turns ON above 30°C | To be tested |
| TC03 | Fan maintains its previous state between 28°C and 30°C | To be tested |
| TC04 | Sensor readings occur at approximately 2-second intervals | To be tested |
| TC05 | DHT11 error message appears for invalid reading | To be tested |
| TC06 | Raw soil moisture value appears in Serial Monitor | To be tested |
| TC07 | Soil moisture percentage between 0% and 100% is displayed | To be tested |
| TC08 | Fan turns OFF below 28°C | To be tested |
| TC09 | Fan turns ON above 30°C after being OFF | To be tested |

> **Note:** Results are marked "To be tested" because physical hardware testing should be performed before recording a test as passed.

## 7. QA Workflow

The project used GitHub to track and resolve software issues.

```text
Problem Identification
        ↓
GitHub Issue
        ↓
Root Cause Analysis
        ↓
Create Fix Branch
        ↓
Implement Solution
        ↓
Commit Changes
        ↓
Create Pull Request
        ↓
Review Changes
        ↓
Merge into main
        ↓
Close Issue
```

## 8. GitHub QA Evidence

### Issue #1 - DHT11 temperature readings are unstable

- Issue created and documented
- `fix/temperature-reading` branch created
- DHT11 sampling interval implemented
- Pull Request #2 created
- Pull Request #2 merged
- Issue #1 closed

**Status:** Resolved

### Issue #3 - Fan does not turn ON above temperature threshold

- Issue created and documented
- `fix/fan-control` branch created
- Relay control logic updated
- Pull Request #4 created
- Pull Request #4 merged
- Issue #3 linked using `Fixes #3`

**Status:** Resolved

### Issue #5 - Soil moisture readings are difficult to interpret

- Issue created and documented
- `fix/soil-moisture-reading` branch created
- Soil moisture percentage conversion implemented
- Pull Request #6 created
- Pull Request #6 merged
- Issue #5 linked using `Fixes #5`

**Status:** Resolved

### Issue #7 - Fan switches ON and OFF too frequently near temperature threshold

- Issue created and documented
- `fix/fan-hysteresis` branch created
- Temperature hysteresis implemented
- Pull Request #8 created
- Pull Request #8 merged
- Issue #7 linked using `Fixes #7`

**Status:** Resolved

## 9. Conclusion

The Smart Farm Monitoring System was reviewed using a structured GitHub-based QA workflow.

Four important issues were identified and resolved:

1. Unstable DHT11 temperature readings
2. Fan control above the temperature threshold
3. Soil moisture readings that were difficult to interpret
4. Frequent fan switching near the temperature threshold

The fixes were implemented on separate branches, committed, submitted through Pull Requests, and merged into the main branch.

The testing plan covers sensor monitoring, soil moisture percentage conversion, temperature threshold control, temperature hysteresis, relay operation, sensor error handling, and sensor sampling.

Final hardware test results should be recorded after executing the test cases on the physical ESP8266-based system.
