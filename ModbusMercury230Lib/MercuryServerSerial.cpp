#include "MercuryServerSerial.h"
#include <Arduino.h>

MercuryServerSerial::MercuryServerSerial(HardwareSerial *port, unsigned long baud) :
port(port), debugMode(false) {
    //initialize connection port
    initPort(baud);
}

MercuryServerSerial::~MercuryServerSerial() {
}

void MercuryServerSerial::initPort(unsigned long baud) {
    port->begin(baud);
    this->baud = baud;
}

int* MercuryServerSerial::process(size_t length, byte* data, size_t &resLength) {
    logData(data, length, true);
    port->write(data, length);
    port->flush();

    int *result = readResponse(resLength);

    logData(result, resLength, false);
    return result;
}

template<class T>
void MercuryServerSerial::logData(T* data, size_t length, bool isRequest) {
    if (!debugMode || logger == 0) {
        return;
    }

    if (isRequest) {
        logger->write("#Request: \n");
    } else {
        logger->write("#Response: \n");
    }

    for (int i = 0; i < length; ++i) {
        logger->print((byte) data[i], HEX);
        logger->write(" ");
    }

    logger->write("\n\n");
}

unsigned long MercuryServerSerial::responseTimeout() {
    unsigned long result = 3000;

    if (baud >= 9600) {
        result = 150;
    } else if (baud >= 4800) {
        result = 180;
    } else if (baud >= 2400) {
        result = 250;
    } else if (baud >= 1200) {
        result = 400;
    } else if (baud >= 600) {
        result = 800;
    } else if (baud >= 300) {
        result = 1600;
    }

    //Add 1 ms to ensure timeout exceeded.
    return result + 1;
}

unsigned long MercuryServerSerial::silenceTimeout() {
    unsigned long result = 300;

    if (baud >= 38400) {
        result = 2;
    } else if (baud >= 19200) {
        result = 3;
    } else if (baud >= 9600) {
        result = 5;
    } else if (baud >= 4800) {
        result = 10;
    } else if (baud >= 2400) {
        result = 20;
    } else if (baud >= 1200) {
        result = 40;
    } else if (baud >= 600) {
        result = 80;
    } else if (baud >= 300) {
        result = 160;
    }

    //Add 1 ms to ensure timeout exceeded.
    return result + 1;
}

int* MercuryServerSerial::readResponse(size_t& length) {
    length = 0;

    delay(responseTimeout());
    while (port->available() > length) {
        length = port->available();
        delay(silenceTimeout());
    }

    int* response = new int[length];
    for (int i = 0; i < length; ++i) {
        response[i] = port->read();
    }

    return response;
}

