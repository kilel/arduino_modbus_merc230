#include <MercuryModbusIntegrator.h>

MercuryModbusIntegrator::MercuryModbusIntegrator(byte deviceId, word baud, Mercury230* device) :
device(device), authLevel(1), password("INVALID_PASSWORD") {
    slave = new modbusSlave();
    slave->setBaud(baud);
    slave->_device = createRegBank(deviceId);
}

MercuryModbusIntegrator::~MercuryModbusIntegrator() {
    delete slave->_device;
    delete slave;
}

void MercuryModbusIntegrator::run() {
    slave->run();
}

modbusDevice* MercuryModbusIntegrator::createRegBank(byte deviceId) {
    modbusDevice * regBank = new modbusDevice();
    regBank->setId(deviceId);

    registerEnergyLevel(regBank, KEY_ENERGY_RESET);
    
    registerEnergyLevel(regBank, KEY_ENERGY_YEAR);
    registerEnergyLevel(regBank, KEY_ENERGY_PREV_YEAR);
    registerEnergyLevel(regBank, KEY_ENERGY_MONTH);
    registerEnergyLevel(regBank, KEY_ENERGY_DAY);
    registerEnergyLevel(regBank, KEY_ENERGY_PREV_DAY);
    
    registerEnergyLevel(regBank, KEY_ENERGY_YEAR_BEGIN);
    registerEnergyLevel(regBank, KEY_ENERGY_PREV_YEAR_BEGIN);
    registerEnergyLevel(regBank, KEY_ENERGY_MONTH_BEGIN);
    registerEnergyLevel(regBank, KEY_ENERGY_DAY_BEGIN);
    registerEnergyLevel(regBank, KEY_ENERGY_PREV_DAY_BEGIN);
    
    registerEnergyPhase(regBank, KEY_ENERGY_PHASE);

    return regBank;
}

void MercuryModbusIntegrator::registerEnergyLevel(modbusDevice* regBank, word startIndex) {
    for (int i = 0; i < 4; ++i) {
        regBank->add(startIndex + i);
    }
}

void MercuryModbusIntegrator::registerEnergyPhase(modbusDevice* regBank, word startIndex) {
    for (int i = 0; i < 3; ++i) {
        regBank->add(startIndex + i);
    }
}

void MercuryModbusIntegrator::updateData() {
    int  authResult = device->auth(authLevel, password);
    
    if(authResult == 0) {
        updateEnergyLevel(KEY_ENERGY_RESET, device->getEnergyFromReset());

        updateEnergyLevel(KEY_ENERGY_YEAR, device->getEnergyForYear());
        updateEnergyLevel(KEY_ENERGY_PREV_YEAR, device->getEnergyForPrevYear());
        updateEnergyLevel(KEY_ENERGY_MONTH, device->getEnergyForMonth(1)); //TODO
        updateEnergyLevel(KEY_ENERGY_DAY, device->getEnergyForDay());
        updateEnergyLevel(KEY_ENERGY_PREV_DAY, device->getEnergyForPrevDay());
        
        updateEnergyLevel(KEY_ENERGY_YEAR_BEGIN, device->getEnergyForYearBegin());
        updateEnergyLevel(KEY_ENERGY_PREV_YEAR_BEGIN, device->getEnergyForPrevYearBegin());
        updateEnergyLevel(KEY_ENERGY_MONTH_BEGIN, device->getEnergyForMonthBegin(1)); // TODO
        updateEnergyLevel(KEY_ENERGY_DAY_BEGIN, device->getEnergyForDayBegin());
        updateEnergyLevel(KEY_ENERGY_PREV_DAY_BEGIN, device->getEnergyForPrevDayBegin());
        
        updateEnergyPhase(KEY_ENERGY_PHASE, device->getPhaseActiveEnergyLevel());
    }
}

void MercuryModbusIntegrator::updateEnergyLevel(word startIndex, EnergyLevel level) {
    slave->_device->set(startIndex, level.active);
    slave->_device->set(startIndex + 1, level.activeRevers);
    slave->_device->set(startIndex + 2, level.passive);
    slave->_device->set(startIndex + 3, level.passiveRevers);
}

void MercuryModbusIntegrator::updateEnergyPhase(word startIndex, EnergyLevelPhase level) {
    for (int i = 0; i < 3; ++i) {
        slave->_device->set(startIndex + i, level.phase[i]);
    }
}


