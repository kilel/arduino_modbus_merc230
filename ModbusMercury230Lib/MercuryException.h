#ifndef MERCURYEXCEPTION_H
#define	MERCURYEXCEPTION_H

#include <Arduino.h>

class MercuryException {
public:
    MercuryException(String message): message(message) {}
    virtual ~MercuryException() {}
    
    String getMessage() {
        return message;
    }
private:
    String message;

};

#endif	/* MERCURYEXCEPTION_H */

