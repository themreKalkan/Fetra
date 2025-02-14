# Fetra Library

The **Fetra Library** is an Arduino library designed to simplify the process of reading and interpreting data from analog line-following sensors. It provides functions for calibrating sensors, reading analog values, and calculating the position, density, and angle of a line relative to the sensors. This library is particularly useful for robotics projects, especially line-following robots.

## Features

- **Sensor Calibration**: Automatically calibrates sensors to determine minimum and maximum values for accurate readings.
- **Line Position**: Calculates the position of the line relative to the sensors.
- **Line Density**: Provides a measure of how dense the line is under the sensors.
- **Advanced Line Density**: Takes into account the distance between sensors for more accurate density calculations.
- **Line Angle**: Calculates the angle of the line relative to the sensors.
- **Configurable**: Allows you to set the number of sensors, line thickness, distance between sensors, and line color (black or white).

## Installation

1. Download the `Fetra` library files (`Fetra.h` and `Fetra.cpp`).
2. Place the files in a folder named `Fetra` inside your Arduino libraries directory (usually `Documents/Arduino/libraries/`).
3. Restart the Arduino IDE to recognize the new library.

## Usage

### Basic Example

Below is a basic example of how to use the `Fetra` library in your Arduino sketch.

```cpp
#include <Fetra.h>

Fetra fetra;

const int sensorCount = 5;
int sensorPins[sensorCount] = {A0, A1, A2, A3, A4};

void setup() {
    Serial.begin(9600);
    fetra.sensorPins(sensorPins, sensorCount);
    fetra.setLineColorBlack();
    fetra.dataPerRead(3);
    fetra.setLineThick(20);
    fetra.setSensBtwDist(10);
    Serial.println("Calibrating sensors...");
    fetra.calibrate(100);
    Serial.println("Calibration complete!");
}

void loop() {
    fetra.readAnalogs();
    fetra.calibratedAnalog();

    uint16_t position = fetra.linePosition();
    Serial.print("Line Position: ");
    Serial.println(position);

    uint16_t density = fetra.lineDensity();
    Serial.print("Line Density: ");
    Serial.println(density);

    uint16_t advancedDensity = fetra.lineDensAdvanced();
    Serial.print("Advanced Line Density: ");
    Serial.println(advancedDensity);

    int8_t angle = fetra.lineAngle();
    Serial.print("Line Angle: ");
    Serial.println(angle);

    delay(100);
}
