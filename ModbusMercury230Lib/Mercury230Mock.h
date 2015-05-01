#ifndef MERCURY230MOCK_H
#define	MERCURY230MOCK_H

#include <Mercury230.h>

class Mercury230Mock : public Mercury230 {
public:
    Mercury230Mock(const byte id);
    virtual ~Mercury230Mock();

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

private:
    EnergyLevel buildRandomEnergyLevel();
    EnergyLevelPhase buildRandomEnergyPhase();


};

#endif	/* MERCURY230MOCK_H */

