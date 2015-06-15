#ifndef MERCURY230_H
#define	MERCURY230_H

#include <Arduino.h>

#include "MercuryException.h"

struct EnergyLevel {
    MercuryException* cause;

    EnergyLevel(unsigned long a, unsigned long aR, unsigned long p, unsigned long pR, MercuryException *cause = 0) :
    active(a),
    activeRevers(aR),
    passive(p),
    passiveRevers(pR),
    cause(cause) {
    }

    word getActiveHi() {
        return (active >> 8) & 0xFF;
    }

    word getActiveLow() {
        return active & 0xFF;
    }

    word getActiveReversHi() {
        return (activeRevers >> 8) & 0xFF;
    }

    word getActiveReversLow() {
        return activeRevers & 0xFF;
    }

    word getPassiveHi() {
        return (passive >> 8) & 0xFF;
    }

    word getPassiveLow() {
        return passive & 0xFF;
    }

    word getPassiveReversHi() {
        return (passiveRevers >> 8) & 0xFF;
    }

    word getPassiveReversLow() {
        return passiveRevers & 0xFF;
    }

private:
    const unsigned long active;
    const unsigned long activeRevers;
    const unsigned long passive;
    const unsigned long passiveRevers;
};

struct EnergyLevelPhase {
    MercuryException *cause;

    EnergyLevelPhase(unsigned long a, unsigned long b, unsigned long c, MercuryException *cause = 0) :
    phase({a, b, c}), cause(cause) {
    }

    byte getHi(int idx) {
        return (phase[idx] >> 8) & 0xFF;
    }

    byte getLow(int idx) {
        return phase[idx] & 0xFF;
    }

private:
    const unsigned long phase[3];
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

