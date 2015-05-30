#ifndef MERCURYSERVERSERIAL_H
#define	MERCURYSERVERSERIAL_H

#include <MercuryServer.h>

class MercuryServerSerial : public MercuryServer {
public:
    MercuryServerSerial();
    virtual ~MercuryServerSerial();


    virtual int* process(int length, byte* data, int &resLength);

    void setPort(HardwareSerial *value) {
        this->port = value;
    }

    bool debugMode;
	HardwareSerial *logger;

private:
    HardwareSerial *port;
    
    template <class T>
    void logData(T *data, int length, bool isRequest);
    
    int calcDelay();
    int* readResponse(int &length);

};

#endif	/* MERCURYSERVERSERIAL_H */

