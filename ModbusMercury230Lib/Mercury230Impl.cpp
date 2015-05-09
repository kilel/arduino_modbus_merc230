#include <Mercury230Impl.h>

#include "MercuryServer.h"
#include "Mercury230.h"

Mercury230Impl::Mercury230Impl(const byte id) : Mercury230(id) {
}

Mercury230Impl::~Mercury230Impl() {
}

void Mercury230Impl::echo() {
}

int Mercury230Impl::auth(byte authLevel, String password) {
    int *response = sendAuthRequest(authLevel, password);
    int result = response[1];
    delete[] response;
    return result;
}

EnergyLevel Mercury230Impl::getEnergyFromReset() {
    return buildEnergyLevel(sendEnergyRequest(0x0));
}

EnergyLevel Mercury230Impl::getEnergyForYear() {
    return buildEnergyLevel(sendEnergyRequest(0x1));
}

EnergyLevel Mercury230Impl::getEnergyForPrevYear() {
    return buildEnergyLevel(sendEnergyRequest(0x2));
}

EnergyLevel Mercury230Impl::getEnergyForMonth(byte month) {
    return buildEnergyLevel(sendEnergyMonthRequest(0x3, month));
}

EnergyLevel Mercury230Impl::getEnergyForDay() {
    return buildEnergyLevel(sendEnergyRequest(0x4));
}

EnergyLevel Mercury230Impl::getEnergyForPrevDay() {
    return buildEnergyLevel(sendEnergyRequest(0x5));
}

EnergyLevel Mercury230Impl::getEnergyForYearBegin() {
    return buildEnergyLevel(sendEnergyRequest(0x9));
}

EnergyLevel Mercury230Impl::getEnergyForPrevYearBegin() {
    return buildEnergyLevel(sendEnergyRequest(0xA));
}

EnergyLevel Mercury230Impl::getEnergyForMonthBegin(byte month) {
    return buildEnergyLevel(sendEnergyMonthRequest(0xB, month));
}

EnergyLevel Mercury230Impl::getEnergyForDayBegin() {
    return buildEnergyLevel(sendEnergyRequest(0xC));
}

EnergyLevel Mercury230Impl::getEnergyForPrevDayBegin() {
    return buildEnergyLevel(sendEnergyRequest(0xD));
}

EnergyLevelPhase Mercury230Impl::getPhaseActiveEnergyLevel() {
    return buildEnergyPhase(sendEnergyPhaseRequest(0x6));
}

int* Mercury230Impl::sendAuthRequest(byte authLevel, String password) {
    uint8_t *request = new uint8_t[REQ_LENGTH_AUTH];

    //add device id to request and fill all data with zero bytes.
    preProcessRequest(request, REQ_LENGTH_AUTH);

    //fill request
    request[1] = 0x1; // operation code
    request[2] = authLevel;

    int pwdLength = password.length();
    for (int i = 0; i < 6; ++i) {
        //read data from password, if it has it.
        //if password is too short, fill with 0.
        byte data = i >= pwdLength ? 0 : password.charAt(i);
        request[3 + i] = data;
    }

    //calc CRC
    fillCRC(request, REQ_LENGTH_AUTH);

    //send
    int *response = server->process(REQ_LENGTH_AUTH, request, RESP_LENGTH_AUTH);
    delete[] request;
    return response;
}

int* Mercury230Impl::sendEnergyRequest(byte opCode) {
    return sendEnergyMonthRequest(opCode, 0);
}

int* Mercury230Impl::sendEnergyMonthRequest(byte opCode, byte month) {
    uint8_t *request = new uint8_t[REQ_LENGTH_ENERGY_LEVEL];

    //add device id to request and fill all data with zero bytes.
    preProcessRequest(request, REQ_LENGTH_ENERGY_LEVEL);

    //fill request
    request[1] = 0x1; // operation code
    request[2] = ENERGY_REQ_CODE;
    request[3] = opCode << 4 | (month & 0xF); //in one byte: (opCode | month)
    request[4] = ENERGY_REQ_TARIFF;

    //calc CRC
    fillCRC(request, REQ_LENGTH_ENERGY_LEVEL);

    //send
    int *response = server->process(REQ_LENGTH_ENERGY_LEVEL, request, RESP_LENGTH_ENERGY_LEVEL);
    delete[] request;
    return response;
}

int* Mercury230Impl::sendEnergyPhaseRequest(byte opCode) {
    uint8_t *request = new uint8_t[REQ_LENGTH_ENERGY_LEVEL];

    //add device id to request and fill all data with zero bytes.
    preProcessRequest(request, REQ_LENGTH_ENERGY_LEVEL);

    //fill request
    request[1] = 0x1; // operation code
    request[2] = ENERGY_REQ_CODE;
    request[3] = opCode << 4; //in one byte: (opCode | 0)
    request[4] = ENERGY_REQ_TARIFF;

    //calc CRC
    fillCRC(request, REQ_LENGTH_ENERGY_LEVEL);

    //send
    int *response = server->process(REQ_LENGTH_ENERGY_LEVEL, request, RESP_LENGTH_ENERGY_PHASE);
    delete[] request;
    return response;
}

void Mercury230Impl::fillCRC(byte* data, int length) {
    word crc = calcCRC(data, length - 2);
    data[length - 2] = (crc >> 8) & 0xFF;
    data[length - 1] = (crc) & 0xFF;
}

word Mercury230Impl::calcCRC(byte* data, int length) {
    byte crcHi = 0xFF;
    byte crcLo = 0xFF;
    byte nextIndex;

    for (int i = 0; i < length; ++i) {
        nextIndex = crcHi ^ data[i];
        crcHi = crcLo ^ CRCHiTable[nextIndex];
        crcLo = CRCLoTable[nextIndex];
    }
    return (((word) crcHi) << 8) | crcLo;
}

void Mercury230Impl::preProcessRequest(byte* data, int length) {
    data[0] = id;
    for (int i = 1; i < length; ++i) {
        data[i] = 0;
    }
}

EnergyLevel Mercury230Impl::buildEnergyLevel(int* response) {
    word *data = parseEnergyValue(response, 4);
    EnergyLevel result = EnergyLevel(data[0], data[1], data[2], data[3]);
    delete[] data;
    return result;
}

EnergyLevelPhase Mercury230Impl::buildEnergyPhase(int* response) {
    word *data = parseEnergyValue(response, 3);
    EnergyLevelPhase result = EnergyLevelPhase(data[0], data[1], data[2]);
    delete[] data;
    return result;
}

word* Mercury230Impl::parseEnergyValue(int* response, int count) {
    word *data = new word[count];
    for (int i = 0; i < count; ++i) {
        word value = 0;
        // addr | 4 bytes of value | again | ...
        int* startPtr = response + 1 + 4 * i;

        //bytes order: 2 -> 1 -> 4 -> 3
        value = startPtr[1] & 0xFF;
        value = (value << 8) | (startPtr[0] & 0xFF);
        value = (value << 8) | (startPtr[3] & 0xFF);
        value = (value << 8) | (startPtr[2] & 0xFF);

        data[i] = value;
    }

    return data;
}







