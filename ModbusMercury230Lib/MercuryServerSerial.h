#ifndef MERCURYSERVERSERIAL_H
#define	MERCURYSERVERSERIAL_H

#include <MercuryServer.h>

class MercuryServerSerial : public MercuryServer {
public:
    MercuryServerSerial();
    virtual ~MercuryServerSerial();
    

    virtual int* process(int length, byte* data, int resLength);
    
    void setPort(HardwareSerial *value) {
        this->port = value;
    }

private:
    HardwareSerial *port;

};

#endif	/* MERCURYSERVERSERIAL_H */

