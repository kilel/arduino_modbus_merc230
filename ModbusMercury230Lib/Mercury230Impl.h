#ifndef MERCURY230IMPL_H
#define	MERCURY230IMPL_H

#include <Mercury230.h>

#include "MercuryServer.h"

class Mercury230Impl : public Mercury230 {
public:
    Mercury230Impl(const byte id);
    virtual ~Mercury230Impl();

    virtual void echo();

    virtual int auth(int authLevel, String password);

    virtual EnergyLevel getEnergyFromReset();

    virtual EnergyLevel getEnergyForYear();
    virtual EnergyLevel getEnergyForPrevYear();
    virtual EnergyLevel getEnergyForMonth(byte month);
    virtual EnergyLevel getEnergyForDay();
    virtual EnergyLevel getEnergyForPrevDay();

    virtual EnergyLevel getEnergyForYearBegin();
    virtual EnergyLevel getEnergyForPrevYearBegin();
    virtual EnergyLevel getEnergyForMonthBegin(byte month);
    virtual EnergyLevel getEnergyForDayBegin();
    virtual EnergyLevel getEnergyForPrevDayBegin();

    virtual EnergyLevelPhase getPhaseActiveEnergyLevel();
    
    void setServer(MercuryServer *server) {
        this->server = server;
    }

private:
    MercuryServer *server;

};

#endif	/* MERCURY230IMPL_H */

