#ifndef MERCURYSERVER_H
#define	MERCURYSERVER_H

#include <Arduino.h>

class MercuryServer {
public:
    MercuryServer();
    virtual ~MercuryServer();

    virtual int* process(int length, int* data, int resLength) = 0;
private:

};

#endif	/* MERCURYSERVER_H */

