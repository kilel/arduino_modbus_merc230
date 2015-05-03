#ifndef MERCURYSERVER_H
#define	MERCURYSERVER_H

#include <Arduino.h>

class MercuryServer {
public:
    MercuryServer();
    virtual ~MercuryServer();

    byte* process(int length, byte* request);

    //HardwareSerial
private:

};

#endif	/* MERCURYSERVER_H */

