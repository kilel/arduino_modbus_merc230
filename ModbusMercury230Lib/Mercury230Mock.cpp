#include <Mercury230Mock.h>

Mercury230Mock::Mercury230Mock(const byte id) : Mercury230(id) {
}

Mercury230Mock::~Mercury230Mock() {
}

void Mercury230Mock::echo() {

}

int Mercury230Mock::auth(int authLevel, String password) {
    if (authLevel == 1 && String("111111").equals(password)) {
        return 0;
    } else if (authLevel == 2 && String("222222").equals(password)) {
        return 0;
    }
    return 0;
}

EnergyLevel Mercury230Mock::getEnergyFromReset() {
    return buildRandomEnergyLevel();
}

EnergyLevel Mercury230Mock::getEnergyForYear() {
    return buildRandomEnergyLevel();
}

EnergyLevel Mercury230Mock::getEnergyForPrevYear() {
    return buildRandomEnergyLevel();
}

EnergyLevel Mercury230Mock::getEnergyForMonth(byte month) {
    return buildRandomEnergyLevel();
}

EnergyLevel Mercury230Mock::getEnergyForDay() {
    return buildRandomEnergyLevel();
}

EnergyLevel Mercury230Mock::getEnergyForPrevDay() {
    return buildRandomEnergyLevel();
}

EnergyLevel Mercury230Mock::getEnergyForYearBegin() {
    return buildRandomEnergyLevel();
}

EnergyLevel Mercury230Mock::getEnergyForPrevYearBegin() {
    return buildRandomEnergyLevel();
}

EnergyLevel Mercury230Mock::getEnergyForMonthBegin(byte month) {
    return buildRandomEnergyLevel();
}

EnergyLevel Mercury230Mock::getEnergyForDayBegin() {
    return buildRandomEnergyLevel();
}

EnergyLevel Mercury230Mock::getEnergyForPrevDayBegin() {
    return buildRandomEnergyLevel();
}

EnergyLevelPhase Mercury230Mock::getPhaseActiveEnergyLevel() {
    return buildRandomEnergyPhase();
}

EnergyLevel Mercury230Mock::buildRandomEnergyLevel() {
    return EnergyLevel(
            (word) random(0, 32767),
            (word) random(0, 32767),
            (word) random(0, 32767),
            (word) random(0, 32767)
            );
}

EnergyLevelPhase Mercury230Mock::buildRandomEnergyPhase() {
    return EnergyLevelPhase(
            (word) random(0, 32767),
            (word) random(0, 32767),
            (word) random(0, 32767)
            );
}


