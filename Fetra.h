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
    void calibrate(int repeat);
    void calibrateForAll(int maxV,int minV);
    void calibrateOnce();
    uint16_t linePosition();  
    uint16_t dashedLinePosition();  
    int* getAnalogReads(); 
    void calibratedAnalog(); 
    uint16_t lineDensity();
    uint16_t lineDensAdvanced();
    int8_t lineAngle();
    int8_t lineAnglePure();
    bool lineBlackColor;
    void setLineColorBlack();
    void setLineColorWhite();
    


private:
    uint8_t _sensorCount;    
    uint8_t* _analogPins;
    uint16_t* _analogReads;
    uint16_t* _calibrationMin;
    uint16_t* _calibrationMax;
    uint8_t _dpr;
    uint8_t _lineThickness;
    uint8_t _distBetweenSens;
    uint16_t _lastLinePos;
    void readAnalogs();
    
    void invert_analog();
     // Changed return type to void
};

#endif