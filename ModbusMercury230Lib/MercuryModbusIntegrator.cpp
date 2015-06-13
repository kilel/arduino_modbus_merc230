#include <MercuryModbusIntegrator.h>

#include "MercuryException.h"

MercuryModbusIntegrator::MercuryModbusIntegrator() :
deviceCount(0),
devices(0),
authLevel(1),
password("INVALID_PASSWORD"),
slave(modbusSlave()) {
}

MercuryModbusIntegrator::~MercuryModbusIntegrator() {
}

void MercuryModbusIntegrator::initDevices(int count, Mercury230 **target) {
    logString("Setting devices: " + String(count));
    this->deviceCount = count;
    this->devices = target;
}

void MercuryModbusIntegrator::init(HardwareSerial *modbusPort, word modbusBaud, byte modbusDeviceId) {
    logString("Setting modbus baud: " + String(modbusBaud));
    slave.setPort(modbusPort);
    slave.setBaud(modbusBaud);
    createRegBank(modbusDeviceId);
    slave._device = &regBank;
}

void MercuryModbusIntegrator::initAuth(int level, String password) {
    this->authLevel = level;
    this->password = password;
}

void MercuryModbusIntegrator::initLogging(HardwareSerial *logPort, bool mode) {
    this->debugLogger = logPort;
    this->debugMode = mode;
}

void MercuryModbusIntegrator::createRegBank(byte deviceId) {
    logString("Registering modbus devices: " + String(deviceId));

    regBank = modbusDevice();
    regBank.setId(deviceId);

    for (int deviceIndex = 0; deviceIndex < deviceCount; ++deviceIndex) {
        registerEnergyLevel(deviceIndex, KEY_ENERGY_RESET);

        registerEnergyLevel(deviceIndex, KEY_ENERGY_YEAR);
        registerEnergyLevel(deviceIndex, KEY_ENERGY_PREV_YEAR);
        registerEnergyLevel(deviceIndex, KEY_ENERGY_DAY);
        registerEnergyLevel(deviceIndex, KEY_ENERGY_PREV_DAY);

        registerEnergyLevel(deviceIndex, KEY_ENERGY_YEAR_BEGIN);
        registerEnergyLevel(deviceIndex, KEY_ENERGY_PREV_YEAR_BEGIN);
        registerEnergyLevel(deviceIndex, KEY_ENERGY_DAY_BEGIN);
        registerEnergyLevel(deviceIndex, KEY_ENERGY_PREV_DAY_BEGIN);

        registerEnergyPhase(deviceIndex, KEY_ENERGY_PHASE);

        for (int month = 0; month < MONTHS_COUNT; ++month) {
            registerEnergyLevel(deviceIndex, KEY_ENERGY_MONTH + month * ENERGY_LEVEL_FIELDS);
            registerEnergyLevel(deviceIndex, KEY_ENERGY_MONTH_BEGIN + month * ENERGY_LEVEL_FIELDS);
        }
    }
}

void MercuryModbusIntegrator::run() {
    tryAnswerModbus();
    updateData();
    tryAnswerModbus();
}

void MercuryModbusIntegrator::tryAnswerModbus() {
    while (slave.getPort()->available() > 0) {
        logString("Answering on Modbus request");
        slave.run();
    }
}

void MercuryModbusIntegrator::updateData() {
    logString("Starting updating new batch of data");
    for (int deviceIndex = 0; deviceIndex < deviceCount; ++deviceIndex) {
        Mercury230* device = devices[deviceIndex];
        byte deviceId = device->id;

        logString("Updating data for device " + String(deviceId));

        int authResult = device->auth(authLevel, password);

        if (authResult != 0) {
            logString("Authentication failed for device " + String(deviceId) + ". Code: " + String(authResult));
        } else {
            updateEnergyLevel(deviceIndex, KEY_ENERGY_RESET, device->getEnergyFromReset());

            updateEnergyLevel(deviceIndex, KEY_ENERGY_YEAR, device->getEnergyForYear());
            updateEnergyLevel(deviceIndex, KEY_ENERGY_PREV_YEAR, device->getEnergyForPrevYear());
            updateEnergyLevel(deviceIndex, KEY_ENERGY_DAY, device->getEnergyForDay());
            updateEnergyLevel(deviceIndex, KEY_ENERGY_PREV_DAY, device->getEnergyForPrevDay());

            updateEnergyLevel(deviceIndex, KEY_ENERGY_YEAR_BEGIN, device->getEnergyForYearBegin());
            updateEnergyLevel(deviceIndex, KEY_ENERGY_PREV_YEAR_BEGIN, device->getEnergyForPrevYearBegin());
            updateEnergyLevel(deviceIndex, KEY_ENERGY_DAY_BEGIN, device->getEnergyForDayBegin());
            updateEnergyLevel(deviceIndex, KEY_ENERGY_PREV_DAY_BEGIN, device->getEnergyForPrevDayBegin());

            updateEnergyPhase(deviceIndex, KEY_ENERGY_PHASE, device->getPhaseActiveEnergyLevel());

            for (int month = 0; month < MONTHS_COUNT; ++month) {
                int monthShift = month * ENERGY_LEVEL_FIELDS;
                updateEnergyLevel(deviceIndex, KEY_ENERGY_MONTH + monthShift, device->getEnergyForMonth(month + 1));
                updateEnergyLevel(deviceIndex, KEY_ENERGY_MONTH_BEGIN + monthShift, device->getEnergyForMonthBegin(month + 1));
            }
        }
    }
}

void MercuryModbusIntegrator::registerEnergyLevel(int deviceIndex, word startIndex) {
    byte deviceId = devices[deviceIndex]->id;
    int shift = getDeviceSpan(deviceIndex) + startIndex;
    logString("Registering energy level for device " + String(deviceId)
            + " with shift: " + String(startIndex) + " (real shift = " + shift + ")");
    for (int i = 0; i < ENERGY_LEVEL_FIELDS; ++i) {
        regBank.add(getDeviceSpan(deviceIndex) + startIndex + i);
        regBank.add(shift + i);
    }
}

void MercuryModbusIntegrator::registerEnergyPhase(int deviceIndex, word startIndex) {
    byte deviceId = devices[deviceIndex]->id;
    int shift = getDeviceSpan(deviceIndex) + startIndex;
    logString("Registering energy phase for device " + String(deviceId)
            + " with shift: " + String(startIndex) + " (real shift = " + shift + ")");
    for (int i = 0; i < ENERGY_PHASE_FIELDS; ++i) {
        regBank.add(shift + i);
    }
}

void MercuryModbusIntegrator::updateEnergyLevel(int deviceIndex, word startIndex, EnergyLevel level) {
    byte deviceId = devices[deviceIndex]->id;
    int shift = getDeviceSpan(deviceIndex) + startIndex;
    if (level.cause != 0) {
        logString("Error updating energy level for device " + String(deviceId)
                + " with shift: " + String(startIndex) + " (real shift = " + shift + ")");
        logString("Cause: " + level.cause->getMessage());
        delete level.cause;
        return;
    }

    logString("Updating energy level for device " + String(deviceId)
            + " with shift: " + String(startIndex) + " (real shift = " + shift + ")");

    doWriteData(shift, level.getActiveHi());
    doWriteData(shift + 1, level.getActiveLow());

    doWriteData(shift + 2, level.getActiveReversHi());
    doWriteData(shift + 3, level.getActiveReversHi());

    doWriteData(shift + 4, level.getPassiveHi());
    doWriteData(shift + 5, level.getPassiveLow());

    doWriteData(shift + 6, level.getPassiveReversHi());
    doWriteData(shift + 7, level.getPassiveReversLow());

    //Try to answer for modbus requests after updating a value
    tryAnswerModbus();
}

void MercuryModbusIntegrator::updateEnergyPhase(int deviceIndex, word startIndex, EnergyLevelPhase level) {
    byte deviceId = devices[deviceIndex]->id;
    int shift = getDeviceSpan(deviceIndex) + startIndex;
    if (level.cause != 0) {
        logString("Error updating energy phase for device " + String(deviceId)
                + " with shift: " + String(startIndex) + " (real shift = " + shift + ")");
        logString("Cause: " + level.cause->getMessage());
        delete level.cause;
        return;
    }

    logString("Updating energy level for device " + String(deviceId)
            + " with shift: " + String(startIndex) + " (real shift = " + shift + ")");

    for (int i = 0; i < ENERGY_PHASE_FIELDS; i += 2) {
        doWriteData(shift + i, level.getHi(i));
        doWriteData(shift + i + 1, level.getLow(i));
    }

    //Try to answer for modbus requests after updating a value
    tryAnswerModbus();
}

void MercuryModbusIntegrator::doWriteData(word cell, word value) {
    //logString("Updating data in cell: " + String(cell) + " with value: " + String(value));
    slave._device->set(cell, value);
}

word MercuryModbusIntegrator::getDeviceSpan(int deviceIndex) {
    return deviceIndex * KEY_SPAN_BETWEEN_DEVICES;
}

void MercuryModbusIntegrator::logString(String data) {
    if (debugMode && debugLogger != 0) {
        debugLogger->println(data);
    }
}

