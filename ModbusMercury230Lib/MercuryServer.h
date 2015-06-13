#ifndef MERCURYSERVER_H
#define	MERCURYSERVER_H

#include <Arduino.h>

class MercuryServer {
public:
    MercuryServer(): baud(1) {}
    virtual ~MercuryServer() {}

    virtual int* process(size_t length, byte* data, size_t &resLength) = 0;
    
    void setBaud (int value) {
        baud = value;
    }
private:
protected:
    int baud;
};

#endif	/* MERCURYSERVER_H */

