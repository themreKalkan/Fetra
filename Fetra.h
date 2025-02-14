#ifndef Fetra_H
#define Fetra_H

#include <stdint.h>
#include <Arduino.h>

class Fetra {
public:
    Fetra();
    ~Fetra();
    void sensorPins(int pins[], int sensorCount);
    void dataPerRead(int dpr);
    void setLineThick(int lt);
    void setSensBtwDist(int sbd);
    void readAnalogs();
    void calibrate(int repeat);
    uint16_t linePosition();  
    int* getAnalogReads(); 
    void calibratedAnalog(); 
    uint16_t lineDensity();
    uint16_t lineDensAdvanced();
    int8_t lineAngle();
    bool lineBlackColor;
    void setLineColorBlack();
    void setLineColorWhite();


private:
    int _sensorCount;    
    int* _analogPins;
    int* _analogReads;
    int* _calibrationMin;
    int* _calibrationMax;
    int _dpr;
    int _lineThickness;
    int _distBetweenSens;
    
    void invert_analog();
     // Changed return type to void
};

#endif