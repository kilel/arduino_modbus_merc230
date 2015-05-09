#include "MercuryServerSerial.h"

MercuryServerSerial::MercuryServerSerial() {
}

MercuryServerSerial::~MercuryServerSerial() {
}

int* MercuryServerSerial::process(int length, byte* data, int resLength) {
    port->write(data, length);
    port->flush();
    
    int *result = new int[resLength];
    
    for(int i = 0; i < resLength; ++i) {
        result[i] = port->read();
    }
    
    return result;
}


