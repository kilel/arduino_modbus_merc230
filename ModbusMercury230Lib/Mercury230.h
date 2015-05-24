#ifndef MERCURY230_H
#define	MERCURY230_H

#include <Arduino.h>

#include "MercuryException.h"

struct EnergyLevel {
    const word active;
    const word activeRevers;
    const word passive;
    const word passiveRevers;

    MercuryException* cause;

    EnergyLevel(word a, word aR, word p, word pR, MercuryException *cause = 0) :
    active(a),
    activeRevers(aR),
    passive(p),
    passiveRevers(pR),
    cause(cause) {
    }
    
    ~EnergyLevel() {
        if(cause != 0) {
            delete cause;
        }
    }
};

struct EnergyLevelPhase {
    const word phase[3];
    MercuryException *cause;

    EnergyLevelPhase(word a, word b, word c, MercuryException *cause = 0) : phase({a, b, c}), cause(cause) {
    }
    
    ~EnergyLevelPhase() {
        if(cause != 0) {
            delete cause;
        }
    }
};

class Mercury230 {
public:
    const byte id;

    Mercury230(const byte id) : id(id) {
    }

    virtual ~Mercury230() {
    }

    virtual int echo() = 0;

    virtual int auth(byte authLevel, String password) = 0;

    virtual EnergyLevel getEnergyFromReset() = 0;

    virtual EnergyLevel getEnergyForYear() = 0;
    virtual EnergyLevel getEnergyForPrevYear() = 0;
    virtual EnergyLevel getEnergyForMonth(byte month) = 0;
    virtual EnergyLevel getEnergyForDay() = 0;
    virtual EnergyLevel getEnergyForPrevDay() = 0;

    virtual EnergyLevel getEnergyForYearBegin() = 0;
    virtual EnergyLevel getEnergyForPrevYearBegin() = 0;
    virtual EnergyLevel getEnergyForMonthBegin(byte month) = 0;
    virtual EnergyLevel getEnergyForDayBegin() = 0;
    virtual EnergyLevel getEnergyForPrevDayBegin() = 0;

    virtual EnergyLevelPhase getPhaseActiveEnergyLevel() = 0;


private:

};


#endif	/* MERCURY230_H */

