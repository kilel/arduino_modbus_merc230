#include <Mercury230Impl.h>

Mercury230Impl::Mercury230Impl(const byte id) : Mercury230(id) {
}

Mercury230Impl::~Mercury230Impl() {
}

void Mercury230Impl::echo() {
}

int Mercury230Impl::auth(int authLevel, String password) {
    return 0;
}

EnergyLevel Mercury230Impl::getEnergyFromReset() {
    return EnergyLevel(0, 0, 0, 0);
}

EnergyLevel Mercury230Impl::getEnergyForYear() {
    return EnergyLevel(0, 0, 0, 0);
}

EnergyLevel Mercury230Impl::getEnergyForPrevYear() {
    return EnergyLevel(0, 0, 0, 0);
}

EnergyLevel Mercury230Impl::getEnergyForMonth(byte month) {
    return EnergyLevel(0, 0, 0, 0);
}

EnergyLevel Mercury230Impl::getEnergyForDay() {
    return EnergyLevel(0, 0, 0, 0);
}

EnergyLevel Mercury230Impl::getEnergyForPrevDay() {
    return EnergyLevel(0, 0, 0, 0);
}

EnergyLevel Mercury230Impl::getEnergyForYearBegin() {
    return EnergyLevel(0, 0, 0, 0);
}

EnergyLevel Mercury230Impl::getEnergyForPrevYearBegin() {
    return EnergyLevel(0, 0, 0, 0);
}

EnergyLevel Mercury230Impl::getEnergyForMonthBegin(byte month) {
    return EnergyLevel(0, 0, 0, 0);
}

EnergyLevel Mercury230Impl::getEnergyForDayBegin() {
    return EnergyLevel(0, 0, 0, 0);
}

EnergyLevel Mercury230Impl::getEnergyForPrevDayBegin() {
    return EnergyLevel(0, 0, 0, 0);
}

EnergyLevelPhase Mercury230Impl::getPhaseActiveEnergyLevel() {
    return EnergyLevelPhase(0, 0, 0);
}
