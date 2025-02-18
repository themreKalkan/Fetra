#include <Fetra.h>

// Create an instance of the Fetra class
Fetra fetra;

// Define the number of sensors and their corresponding analog pins
const int sensorCount = 5;
int sensorPins[sensorCount] = {A0, A1, A2, A3, A4}; // Example pins for 5 sensors

void setup() {
    // Initialize serial communication for debugging
    Serial.begin(9600);

    // Set the sensor pins and initialize the Fetra library
    fetra.sensorPins(sensorPins, sensorCount);

    // Set the line color to black (default is black)
    fetra.setLineColorBlack();

    // Set the number of data points to read per sensor reading
    fetra.dataPerRead(3);

    // Set the line thickness (optional, default is 20)
    fetra.setLineThick(20);

    // Set the distance between sensors (optional, default is 10)
    fetra.setSensBtwDist(10);

    // Calibrate the sensors
    Serial.println("Calibrating sensors...");
    fetra.calibrate(100); // Calibrate 100 times for better accuracy
    Serial.println("Calibration complete!");
}

void loop() {
    // Read the analog values from the sensors
    int* analogs = fetra.getAnalogReads();

    //All sensors calibrated with just one data from analogs. You can create your own calibration loop.
    fetra.calibrateOnce();

    //Now all sensors calibrated between 200-800 analog reads. 
    fetra.calibrateForAll(200,800);
    
    // Get the calibrated analog values
    fetra.calibratedAnalog();

    // Get the line position
    uint16_t position = fetra.linePosition();
    Serial.print("Line Position: ");
    Serial.println(position);

    //You can use this code for dashed lines in your track. If the last position of line is straight and there is no line position output set on middle value for go straight
    uint16_t position = fetra.linePosition();
    Serial.print("Line Position: ");
    Serial.println(position);

    // Get the line density
    uint16_t density = fetra.lineDensity();
    Serial.print("Line Density: ");
    Serial.println(density);

    // Get the advanced line density
    uint16_t advancedDensity = fetra.lineDensAdvanced();
    Serial.print("Advanced Line Density: ");
    Serial.println(advancedDensity);

    // Get the line angle
    int8_t angle = fetra.lineAngle();
    Serial.print("Line Angle: ");
    Serial.println(angle);

    // Delay for a short period before the next reading
    delay(100);
}
