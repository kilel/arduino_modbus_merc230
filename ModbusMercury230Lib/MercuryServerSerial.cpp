#include "MercuryServerSerial.h"
#include <Arduino.h>

MercuryServerSerial::MercuryServerSerial() : debugMode(false) {
}

MercuryServerSerial::~MercuryServerSerial() {
}

int* MercuryServerSerial::process(int length, byte* data, int &resLength) {
    logData(data, length, true);
    port->write(data, length);
    port->flush();

    int *result = readResponse(resLength);

    logData(result, resLength, false);
    return result;
}

template<class T>
void MercuryServerSerial::logData(T* data, int length, bool isRequest) {
    if (!debugMode) {
        return;
    }
    
    //TODO: write in HEX with leading zeros

    if (isRequest) {
        Serial.write("#Request: \n");
    } else {
        Serial.write("#Response: \n");
    }

    for (int i = 0; i < length; ++i) {
        Serial.print((byte) data[i], HEX);
        //Serial.write(convertToHex(data[i])); //write String convertToHex(byte) function
    }

    Serial.write("\n\n");
}

int MercuryServerSerial::calcDelay() {
    //TODO: implement correct response read
    if(baud < 5000) return 3000;
    else return 200;
}

int* MercuryServerSerial::readResponse(int& length) {
    length = 0;
    delay(calcDelay());
    while (port->available() > length) {
        length = port->available();
        delay(calcDelay());
    }
    
    int* response = new int[length];
    for (int i = 0; i < length; ++i) {
        response[i] = port->read();
    }
    
    return response;
}

