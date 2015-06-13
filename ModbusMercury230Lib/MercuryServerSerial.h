#ifndef MERCURYSERVERSERIAL_H
#define	MERCURYSERVERSERIAL_H

#include <MercuryServer.h>

class MercuryServerSerial : public MercuryServer {
public:
    MercuryServerSerial(HardwareSerial *port, unsigned long baud);
    virtual ~MercuryServerSerial();

    virtual int* process(size_t length, byte* data, size_t &resLength);

    void setPort(HardwareSerial *value, unsigned long baud) {
        this->port = value;
        initPort(baud);
    }

    bool debugMode;
    HardwareSerial *logger;

private:
    HardwareSerial *port;
    unsigned long baud;

    void initPort(unsigned long baud);
    template <class T>
    void logData(T *data, size_t length, bool isRequest);

    unsigned long responseTimeout();
    unsigned long silenceTimeout();
    int* readResponse(size_t &length);

};

#endif	/* MERCURYSERVERSERIAL_H */

