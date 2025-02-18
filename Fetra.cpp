#include "Fetra.h"
#include <Arduino.h>
#include <EEPROM.h>

// Constructor implementation
Fetra::Fetra() :
    _sensorCount(0),
    _analogPins(nullptr),
    _analogReads(nullptr),
    _calibrationMin(nullptr),
    _calibrationMax(nullptr),
    _dpr(3) ,
    _lineThickness(20),
    _lastLinePos(0),
    _distBetweenSens(10){}

// Destructor implementation
Fetra::~Fetra() {
    delete[] _analogPins;
    delete[] _analogReads;
    delete[] _calibrationMin;
    delete[] _calibrationMax;
}

void Fetra::sensorPins(int pins[], int sensorCount) {
    // Free existing arrays if any
    delete[] _analogPins;
    delete[] _analogReads;
    delete[] _calibrationMin;
    delete[] _calibrationMax;

    // Allocate new arrays
    _sensorCount = sensorCount;
    _analogPins = new uint8_t[_sensorCount];
    _analogReads = new uint16_t[_sensorCount];
    _calibrationMin = new uint16_t[_sensorCount];
    _calibrationMax = new uint16_t[_sensorCount];

    lineBlackColor = true;

    // Initialize arrays
    for(int i = 0; i < _sensorCount; i++) {
        _analogPins[i] = pins[i];
        _analogReads[i] = 0; // Initialize to 0
        _calibrationMin[i] = 1023;
        _calibrationMax[i] = 0;
        pinMode(_analogPins[i], INPUT);
    }
}

void Fetra::setLineColorBlack(){
    lineBlackColor = true;
}

void Fetra::setLineColorWhite(){
    lineBlackColor = false;
}


void Fetra::dataPerRead(int dpr) {
    _dpr = dpr;
}

void Fetra::setLineThick(int lt) {
    _lineThickness = lt;
}

void Fetra::setSensBtwDist(int sbd) {
    _distBetweenSens = sbd;
}

void Fetra::readAnalogs() {
    for(int i = 0; i < _sensorCount; i++) {
        _analogReads[i] = analogRead(_analogPins[i]);
    }
}

int* Fetra::getAnalogReads() {
    readAnalogs();
    return _analogReads; // Return the pointer to the analog reads array
}

void Fetra::calibrate(int repeat) {
    for(int i = 0; i < repeat; i++) {
        readAnalogs();
        for(int s = 0; s < _sensorCount; s++) {
            if(_analogReads[s] < _calibrationMin[s]) {
                _calibrationMin[s] = _analogReads[s];
            }
            if(_analogReads[s] > _calibrationMax[s]) {
                _calibrationMax[s] = _analogReads[s];
            }
        }
     delay(30);   
    }
}

void Fetra::calibrateOnce() {
        readAnalogs();
        for(int s = 0; s < _sensorCount; s++) {
            if(_analogReads[s] < _calibrationMin[s]) {
                _calibrationMin[s] = _analogReads[s];
            }
            if(_analogReads[s] > _calibrationMax[s]) {
                _calibrationMax[s] = _analogReads[s];
            }
        }
    
}

void Fetra::calibrateForAll(int maxV,int minV) {
    //readAnalogs();
    for(int s = 0; s < _sensorCount; s++) {
            _calibrationMin[s] = minV;
       
            _calibrationMax[s] = maxV;
        
    }

}

void Fetra::calibratedAnalog() {
    readAnalogs();
    if(lineBlackColor == false){invert_analog();}
    for(int i = 0; i < _sensorCount; i++) {
        _analogReads[i] = constrain(
            map(_analogReads[i], _calibrationMin[i], _calibrationMax[i], 0, 1000),
            0, 
            1000
        );
        if(_analogReads[i] < 200){
            _analogReads[i] = 0;
        }
    }
}

uint16_t Fetra::dashedLinePosition() {
    uint32_t total = 0;
    uint32_t lineSum = 0;

    uint8_t densL = lineDensAdvanced();

    
    for(uint8_t s = 0; s < _dpr; s++) {
        calibratedAnalog();
        uint32_t iterationSum = 0;
        uint32_t iterationTotal = 0;
        
        for(uint8_t i = 0; i < _sensorCount; i++) {
            iterationSum += (uint32_t)_analogReads[i] * i * 1000;
            iterationTotal += _analogReads[i];
        }
        
        lineSum += iterationSum;
        total += iterationTotal;
    }
    
    
    if(densL > 15){
        _lastLinePos = lineSum / (total);
        return _lastLinePos;
    }
    else{
        
            if(_lastLinePos > ((_sensorCount+1)*500)){
                return (_sensorCount-1)*1000;
            }
            else if(_lastLinePos < ((_sensorCount-3)*500)){return 0;}
            else{return 3500;}
    }
    

    
}

uint16_t Fetra::linePosition() {
    uint32_t total = 0;
    uint32_t lineSum = 0;

    
    for(uint8_t s = 0; s < _dpr; s++) {
        calibratedAnalog();
        uint32_t iterationSum = 0;
        uint32_t iterationTotal = 0;
        
        for(uint8_t i = 0; i < _sensorCount; i++) {
            iterationSum += (uint32_t)_analogReads[i] * i * 1000;
            iterationTotal += _analogReads[i];
        }
        
        lineSum += iterationSum;
        total += iterationTotal;
    }
    
    if(total == 0){
        return _lastLinePos;
    }
    _lastLinePos = lineSum / (total);
        return _lastLinePos;
    

    
}



void Fetra::invert_analog() {
    for(int i = 0; i < _sensorCount; i++) {
        _analogReads[i] = 1000 - _analogReads[i];
    }
}

uint16_t Fetra::lineDensity(){
    //calibratedAnalog();
    int sum = 0;
    for(int i = 0;i < _sensorCount;i++){
        sum += _analogReads[i];
    }
    return (sum/((_sensorCount-1)*1000))*100;
}

uint16_t Fetra::lineDensAdvanced(){
    //calibratedAnalog();
    //int mainDistance = (_sensorCount+2)*_distBetweenSens;
    int lineSum = 0;
    for(int i = 0;i < _sensorCount;i++){
        lineSum += (_analogReads[i]*_distBetweenSens)/1000;
    }
    return lineSum;
}

int8_t Fetra::lineAngle(){
    //calibratedAnalog();
    int midLine = (_sensorCount-1)*500;
    bool passMod = false;
    if(_sensorCount%2 == 1){
        passMod = true;
    }
    
    for(int i = 0;i<_sensorCount;i++){
        if(passMod && i == _sensorCount/2){continue;}
        if(i<_sensorCount/2){midLine -= _analogReads[i];}
        else{midLine += _analogReads[i];}
    }
    midLine = map(midLine,500,6500,-90,90);
    //    midLine = map(midLine,0,7000,-90,90);

    return midLine;
}

int8_t Fetra::lineAnglePure(){
    //calibratedAnalog();
    readAnalogs();
    int midLine = (_sensorCount-1)*500;
    bool passMod = false;
    if(_sensorCount%2 == 1){
        passMod = true;
    }
    
    for(int i = 0;i<_sensorCount;i++){
        if(passMod && i == _sensorCount/2){continue;}
        if(i<_sensorCount/2){midLine -= _analogReads[i];}
        else{midLine += _analogReads[i];}
    }
    midLine = map(midLine,500,6500,-90,90);
    return midLine;
}
