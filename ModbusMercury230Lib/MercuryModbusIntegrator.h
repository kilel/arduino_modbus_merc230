#ifndef MERCURYMODBUSINTEGRATOR_H
#define	MERCURYMODBUSINTEGRATOR_H

#include <modbusDevice.h>
#include <modbusSlave.h>
#include <Mercury230.h>

class MercuryModbusIntegrator {
public:
    MercuryModbusIntegrator();
    virtual ~MercuryModbusIntegrator();

    void run();
    void updateData();

    void setDevices(int count, Mercury230 **target);

    void init(word modbusBaud, byte modbusDeviceId);

    void setAuthLevel(int value) {
        this->authLevel = value;
    }

    void setPassword(String value) {
        this->password = value;
    }

private:
    int deviceCount;
    Mercury230** devices;
    modbusSlave slave;
    modbusDevice regBank;

    int authLevel;
    String password;

    void createRegBank(byte deviceId);
    void registerEnergyLevel(int deviceIndex, word startIndex);
    void registerEnergyPhase(int deviceIndex, word startIndex);

    void updateEnergyLevel(int deviceIndex, word startIndex, EnergyLevel level);
    void updateEnergyPhase(int deviceIndex, word startIndex, EnergyLevelPhase level);
    void doWriteData(word cell, word value);

    word getDeviceSpan(int deviceIndex);

    static const word ENERGY_LEVEL_FIELDS = 4;
    static const word ENERGY_PHASE_FIELDS = 3;
    static const word MONTHS_COUNT = 12;
    
    static const word KEY_SPAN_BETWEEN_DEVICES = 200;

    /** Start index for energy level (4 fields). */
    static const word KEY_ENERGY_RESET = 30001;
    
    static const word KEY_ENERGY_YEAR =         KEY_ENERGY_RESET + ENERGY_LEVEL_FIELDS;
    static const word KEY_ENERGY_PREV_YEAR =    KEY_ENERGY_YEAR + ENERGY_LEVEL_FIELDS;
    static const word KEY_ENERGY_DAY =          KEY_ENERGY_PREV_YEAR + ENERGY_LEVEL_FIELDS;
    static const word KEY_ENERGY_PREV_DAY =     KEY_ENERGY_DAY + ENERGY_LEVEL_FIELDS;
    
    static const word KEY_ENERGY_YEAR_BEGIN =   KEY_ENERGY_PREV_DAY + ENERGY_LEVEL_FIELDS;
    static const word KEY_ENERGY_PREV_YEAR_BEGIN = KEY_ENERGY_YEAR_BEGIN + ENERGY_LEVEL_FIELDS;
    static const word KEY_ENERGY_DAY_BEGIN =    KEY_ENERGY_PREV_YEAR_BEGIN + ENERGY_LEVEL_FIELDS;
    static const word KEY_ENERGY_PREV_DAY_BEGIN = KEY_ENERGY_DAY_BEGIN + ENERGY_LEVEL_FIELDS;
    
    static const word KEY_ENERGY_PHASE =        KEY_ENERGY_PREV_DAY_BEGIN + ENERGY_LEVEL_FIELDS;

    /** Energy level for months (12 months with 4 fields = 48 fields totally)*/
    static const word KEY_ENERGY_MONTH =        KEY_ENERGY_PHASE + ENERGY_PHASE_FIELDS;
    static const word KEY_ENERGY_MONTH_BEGIN =  KEY_ENERGY_MONTH + ENERGY_LEVEL_FIELDS * MONTHS_COUNT;

    

};

#endif	/* MERCURYMODBUSINTEGRATOR_H */

