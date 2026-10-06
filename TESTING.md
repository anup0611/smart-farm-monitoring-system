# Smart Farm Monitoring System - Testing

## 1. Testing Objective

The objective of testing is to verify that the Smart Farm Monitoring System correctly reads environmental sensor data and controls the ventilation fan according to the temperature threshold.

The testing process focuses on:

- DHT11 temperature and humidity monitoring
- Soil moisture monitoring
- Sensor reading interval
- Automatic ventilation control
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
| Temperature Threshold | 30°C |
| Sensor Sampling Interval | 2 seconds |

## 3. Test Cases

| Test ID | Test Condition | Input/Condition | Expected Result |
|---|---|---|---|
| TC01 | Normal temperature | Temperature below 30°C | Fan remains OFF |
| TC02 | Threshold temperature | Temperature = 30°C | Fan remains OFF |
| TC03 | High temperature | Temperature above 30°C | Fan turns ON |
| TC04 | Sensor sampling | Continuous monitoring | Sensor readings occur at approximately 2-second intervals |
| TC05 | Invalid DHT11 reading | DHT11 unavailable/disconnected | Error message is displayed |
| TC06 | Soil moisture monitoring | Soil sensor connected | Soil moisture value is displayed |
| TC07 | Temperature returns to normal | Temperature falls to 30°C or below | Fan turns OFF |

## 4. Expected System Behaviour

### 4.1 Temperature Monitoring

The DHT11 sensor provides temperature and humidity readings. The system checks the temperature against the defined threshold:

```text
Temperature <= 30°C
        ↓
     Fan OFF

Temperature > 30°C
        ↓
      Fan ON
```

### 4.2 Automatic Ventilation

When the temperature rises above 30°C, the ESP8266 activates the relay and the ventilation fan should turn ON. When the temperature is 30°C or below, the fan should remain OFF.

The relay states are explicitly defined in the program:

```cpp
#define RELAY_ON LOW
#define RELAY_OFF HIGH
```

The automatic ventilation logic uses these relay states to control the fan.

### 4.3 Sensor Sampling

The DHT11 sensor readings are controlled using a 2-second sampling interval. The system uses `millis()` to control the reading interval:

```cpp
static unsigned long lastReadTime = 0;

if (millis() - lastReadTime < 2000) {
    return;
}

lastReadTime = millis();
```

This prevents the system from continuously requesting DHT11 readings.

### 4.4 DHT11 Error Handling

If the DHT11 sensor does not provide a valid temperature or humidity reading, the system checks the reading using `isnan()`. If the reading is invalid, the system displays:

```text
DHT11 sensor reading failed
```

The system then waits before attempting another reading.

### 4.5 Soil Moisture Monitoring

The soil moisture sensor is connected to the analog input of the ESP8266. The system reads the sensor using:

```cpp
int soilMoisture = analogRead(SOIL_PIN);
```

The measured soil moisture value is displayed through the Serial Monitor.

## 5. QA Issues Resolved

### Issue #1 - DHT11 Temperature Readings Are Unstable

**Problem:** DHT11 temperature readings required a controlled sampling interval.

**Root Cause:** The initial implementation did not provide a controlled sensor sampling mechanism.

**Solution:** A 2-second sampling interval was implemented using `millis()`.

**GitHub Resolution:** The fix was implemented in the `fix/temperature-reading` branch and merged into `main`.

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

## 6. Test Execution Status

The following test cases should be verified on the physical hardware.

| Test ID | Expected Observation | Result |
|---|---|---|
| TC01 | Fan remains OFF below 30°C | To be tested |
| TC02 | Fan remains OFF at 30°C | To be tested |
| TC03 | Fan turns ON above 30°C | To be tested |
| TC04 | Sensor readings occur at approximately 2-second intervals | To be tested |
| TC05 | DHT11 error message appears for invalid reading | To be tested |
| TC06 | Soil moisture value appears in Serial Monitor | To be tested |
| TC07 | Fan turns OFF when temperature returns to 30°C or below | To be tested |

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

## 9. Conclusion

The Smart Farm Monitoring System was reviewed using a structured GitHub-based QA workflow.

Two important issues were identified and resolved:

1. Unstable DHT11 temperature readings
2. Fan control above the temperature threshold

The fixes were implemented on separate branches, committed, submitted through Pull Requests, and merged into the main branch.

The testing plan covers sensor monitoring, temperature threshold control, relay operation, sensor error handling, and soil moisture monitoring.

Final hardware test results should be recorded after executing the test cases on the physical ESP8266-based system.
