#ifndef MERCURYMODBUSINTEGRATOR_H
#define	MERCURYMODBUSINTEGRATOR_H

#include <modbusDevice.h>
#include <modbusSlave.h>
#include <Mercury230.h>

class MercuryModbusIntegrator {
public:
    MercuryModbusIntegrator(byte deviceId, word baud, Mercury230* device);
    virtual ~MercuryModbusIntegrator();

    void run();
    void updateData();
    
    void setAuthLevel(int value) {
        this->authLevel = value;
    }

    void setPassword(String value) {
        this->password = value;
    }

private:
    Mercury230* device;
    modbusSlave* slave;

    modbusDevice* createRegBank(byte deviceId);
    void registerEnergyLevel(modbusDevice* regBank, word startIndex);
    void registerEnergyPhase(modbusDevice* regBank, word startIndex);
    
    void updateEnergyLevel(word startIndex, EnergyLevel level);
    void updateEnergyPhase(word startIndex, EnergyLevelPhase level);

    const word KEY_ENERGY_RESET = 30001;
    const word KEY_ENERGY_YEAR = 30005;
    const word KEY_ENERGY_PREV_YEAR = 30009;
    const word KEY_ENERGY_MONTH = 30013;
    const word KEY_ENERGY_DAY = 30017;
    const word KEY_ENERGY_PREV_DAY = 30021;
    const word KEY_ENERGY_YEAR_BEGIN = 30025;
    const word KEY_ENERGY_PREV_YEAR_BEGIN = 30029;
    const word KEY_ENERGY_MONTH_BEGIN = 30033;
    const word KEY_ENERGY_DAY_BEGIN = 30037;
    const word KEY_ENERGY_PREV_DAY_BEGIN = 30041;
    const word KEY_ENERGY_PHASE = 30045;

    int authLevel;
    String password;
};

#endif	/* MERCURYMODBUSINTEGRATOR_H */

