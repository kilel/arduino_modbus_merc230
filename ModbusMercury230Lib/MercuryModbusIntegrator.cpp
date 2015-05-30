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

void MercuryModbusIntegrator::setDevices(int count, Mercury230 **target) {
    logString("Setting devices: " + String(count));
    this->deviceCount = count;
    this->devices = target;
}

void MercuryModbusIntegrator::init(word modbusBaud, byte modbusDeviceId) {
    logString("Setting modbus baud: " + String(modbusBaud));
    slave.setBaud(modbusBaud);
    createRegBank(modbusDeviceId);
    slave._device = &regBank;
}

void MercuryModbusIntegrator::run() {
    logString("Running slave");
    slave.run();
}

void MercuryModbusIntegrator::createRegBank(byte deviceId) {
    logString("Creating modbus device: " + String(deviceId));

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

void MercuryModbusIntegrator::updateData() {
    for (int deviceIndex = 0; deviceIndex < deviceCount; ++deviceIndex) {
        int authResult = devices[deviceIndex]->auth(authLevel, password);

        if (authResult == 0) {
            updateEnergyLevel(deviceIndex, KEY_ENERGY_RESET, devices[deviceIndex]->getEnergyFromReset());

            updateEnergyLevel(deviceIndex, KEY_ENERGY_YEAR, devices[deviceIndex]->getEnergyForYear());
            updateEnergyLevel(deviceIndex, KEY_ENERGY_PREV_YEAR, devices[deviceIndex]->getEnergyForPrevYear());
            updateEnergyLevel(deviceIndex, KEY_ENERGY_DAY, devices[deviceIndex]->getEnergyForDay());
            updateEnergyLevel(deviceIndex, KEY_ENERGY_PREV_DAY, devices[deviceIndex]->getEnergyForPrevDay());

            updateEnergyLevel(deviceIndex, KEY_ENERGY_YEAR_BEGIN, devices[deviceIndex]->getEnergyForYearBegin());
            updateEnergyLevel(deviceIndex, KEY_ENERGY_PREV_YEAR_BEGIN, devices[deviceIndex]->getEnergyForPrevYearBegin());
            updateEnergyLevel(deviceIndex, KEY_ENERGY_DAY_BEGIN, devices[deviceIndex]->getEnergyForDayBegin());
            updateEnergyLevel(deviceIndex, KEY_ENERGY_PREV_DAY_BEGIN, devices[deviceIndex]->getEnergyForPrevDayBegin());

            updateEnergyPhase(deviceIndex, KEY_ENERGY_PHASE, devices[deviceIndex]->getPhaseActiveEnergyLevel());

            for (int month = 0; month < MONTHS_COUNT; ++month) {
                updateEnergyLevel(
                        deviceIndex,
                        KEY_ENERGY_MONTH + month * ENERGY_LEVEL_FIELDS,
                        devices[deviceIndex]->getEnergyForMonth(month)
                        );
                updateEnergyLevel(
                        deviceIndex,
                        KEY_ENERGY_MONTH_BEGIN + month * ENERGY_LEVEL_FIELDS,
                        devices[deviceIndex]->getEnergyForMonthBegin(month)
                        );
            }
        }
    }
}

void MercuryModbusIntegrator::registerEnergyLevel(int deviceIndex, word startIndex) {
    logString("Creating energy level from start index: " + String(getDeviceSpan(deviceIndex) + startIndex)
            + " device index: " + String(deviceIndex));
    for (int i = 0; i < 4; ++i) {
        regBank.add(getDeviceSpan(deviceIndex) + startIndex + i);
    }
}

void MercuryModbusIntegrator::registerEnergyPhase(int deviceIndex, word startIndex) {
    logString("Creating energy phase from start index: " + String(getDeviceSpan(deviceIndex) + startIndex)
            + " device index: " + String(deviceIndex));
    for (int i = 0; i < 3; ++i) {
        regBank.add(getDeviceSpan(deviceIndex) + startIndex + i);
    }
}

void MercuryModbusIntegrator::updateEnergyLevel(int deviceIndex, word startIndex, EnergyLevel level) {
    if (level.cause != 0) {
        logString("Error updating energy level from start index: " + String(getDeviceSpan(deviceIndex) + startIndex)
                + " device index: " + String(deviceIndex) + ". Cause: " + level.cause->getMessage());
        return;
    }

    logString("Updating energy level from start index: " + String(getDeviceSpan(deviceIndex) + startIndex)
            + " device index: " + String(deviceIndex));
    doWriteData(getDeviceSpan(deviceIndex) + startIndex, level.active);
    doWriteData(getDeviceSpan(deviceIndex) + startIndex + 1, level.activeRevers);
    doWriteData(getDeviceSpan(deviceIndex) + startIndex + 2, level.passive);
    doWriteData(getDeviceSpan(deviceIndex) + startIndex + 3, level.passiveRevers);
}

void MercuryModbusIntegrator::updateEnergyPhase(int deviceIndex, word startIndex, EnergyLevelPhase level) {
    if (level.cause != 0) {
        logString("Updating energy phase from start index: " + String(getDeviceSpan(deviceIndex) + startIndex)
                + " device index: " + String(deviceIndex) + ". Cause: " + level.cause->getMessage());
        return;
    }

    logString("Updating energy phase from start index: " + String(getDeviceSpan(deviceIndex) + startIndex)
            + " device index: " + String(deviceIndex));
    for (int i = 0; i < 3; ++i) {
        doWriteData(getDeviceSpan(deviceIndex) + startIndex + i, level.phase[i]);
    }
}

void MercuryModbusIntegrator::doWriteData(word cell, word value) {
    logString("Updating data in cell: " + String(cell) + " with value: " + String(value));
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

