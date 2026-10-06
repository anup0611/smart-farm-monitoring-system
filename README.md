# Smart Farm Environment Monitoring and Automatic Ventilation System

## Project Overview

The Smart Farm Environment Monitoring System is an ESP8266-based embedded system designed to monitor important environmental conditions in a farm.

The system collects temperature, humidity, soil moisture, and air-quality data using different sensors. Based on predefined conditions, it can automatically control a ventilation fan.

## Problem Statement

Manual monitoring of farm environmental conditions can be time-consuming and may not provide immediate alerts when conditions become unsuitable.

This project aims to provide continuous monitoring and automatic ventilation control using an ESP8266 NodeMCU.

## Objectives

- Monitor temperature and humidity.
- Monitor soil moisture.
- Monitor air quality.
- Automatically control a ventilation fan.
- Provide a foundation for IoT-based farm monitoring.
- Maintain proper documentation and testing using GitHub.

## Hardware Components

| Component | Quantity |
|---|---:|
| ESP8266 NodeMCU | 1 |
| DHT11 Sensor | 1 |
| Soil Moisture Sensor | 1 |
| MQ-135 Gas Sensor | 1 |
| Relay Module | 1 |
| DC Fan | 1 |
| Breadboard | 1 |
| Jumper Wires | 1 |
| Power Supply | 1 |

## System Working

The sensors collect environmental data and send it to the ESP8266 NodeMCU.

The ESP8266 processes the sensor readings and checks them against predefined thresholds.

If the temperature becomes too high, the ESP8266 activates the relay and turns ON the ventilation fan.

### Basic Flow

Sensors
↓
ESP8266 NodeMCU
↓
Data Processing
↓
Threshold Checking
↓
Relay Control
↓
Ventilation Fan

## Software Used

- Arduino IDE
- C/C++
- ESP8266
- Git
- GitHub

## Quality Assurance

GitHub Issues are used to document problems found during development and testing.

The project will track:

- Sensor reading problems
- Fan control problems
- Soil moisture calibration
- Wi-Fi connection problems
- Air-quality threshold problems

## Project Status

Currently under development.

## Learning Objective

This project demonstrates how GitHub can be used for version control, QA documentation, issue tracking, problem solving, and project collaboration in an embedded system.
