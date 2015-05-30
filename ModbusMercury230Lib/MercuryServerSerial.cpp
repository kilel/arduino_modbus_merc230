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
    if (!debugMode || logger == 0) {
        return;
    }
    
    //TODO: write in HEX with leading zeros

    if (isRequest) {
        logger->write("#Request: \n");
    } else {
        logger->write("#Response: \n");
    }

    for (int i = 0; i < length; ++i) {
        logger->print((byte) data[i], HEX);
        logger->write(" ");
        //Serial.write(convertToHex(data[i]));
		//write String convertToHex(byte) function
    }

    logger->write("\n\n");
}

int MercuryServerSerial::calcDelay() {
    //TODO: implement correct response read
    return 1000;
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

