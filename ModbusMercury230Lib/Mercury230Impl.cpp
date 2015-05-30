#include <Mercury230Impl.h>

#include "MercuryServer.h"
#include "Mercury230.h"
#include "MercuryException.h"

Mercury230Impl::Mercury230Impl(const byte id) : Mercury230(id) {
}

Mercury230Impl::~Mercury230Impl() {
}

void Mercury230Impl::setServer(MercuryServer* server) {
    this->server = server;
}

int Mercury230Impl::echo() {
    int *response = sendEcho();
    int result = response[1];
    delete[] response;
    return result;
}

int Mercury230Impl::auth(byte authLevel, String password) {
    int *response = sendAuthRequest(authLevel, password);
    int result = response[1];
    delete[] response;
    return result;
}

EnergyLevel Mercury230Impl::getEnergyFromReset() {
    MercuryException *cause = 0;
    return buildEnergyLevel(sendEnergyRequest(0x0, cause), cause);
}

EnergyLevel Mercury230Impl::getEnergyForYear() {
    MercuryException *cause = 0;
    return buildEnergyLevel(sendEnergyRequest(0x1, cause), cause);
}

EnergyLevel Mercury230Impl::getEnergyForPrevYear() {
    MercuryException *cause = 0;
    return buildEnergyLevel(sendEnergyRequest(0x2, cause), cause);
}

EnergyLevel Mercury230Impl::getEnergyForMonth(byte month) {
    MercuryException *cause = 0;
    return buildEnergyLevel(sendEnergyMonthRequest(0x3, month, cause), cause);
}

EnergyLevel Mercury230Impl::getEnergyForDay() {
    MercuryException *cause = 0;
    return buildEnergyLevel(sendEnergyRequest(0x4, cause), cause);
}

EnergyLevel Mercury230Impl::getEnergyForPrevDay() {
    MercuryException *cause = 0;
    return buildEnergyLevel(sendEnergyRequest(0x5, cause), cause);
}

EnergyLevel Mercury230Impl::getEnergyForYearBegin() {
    MercuryException *cause = 0;
    return buildEnergyLevel(sendEnergyRequest(0x9, cause), cause);
}

EnergyLevel Mercury230Impl::getEnergyForPrevYearBegin() {
    MercuryException *cause = 0;
    return buildEnergyLevel(sendEnergyRequest(0xA, cause), cause);
}

EnergyLevel Mercury230Impl::getEnergyForMonthBegin(byte month) {
    MercuryException *cause = 0;
    return buildEnergyLevel(sendEnergyMonthRequest(0xB, month, cause), cause);
}

EnergyLevel Mercury230Impl::getEnergyForDayBegin() {
    MercuryException *cause = 0;
    return buildEnergyLevel(sendEnergyRequest(0xC, cause), cause);
}

EnergyLevel Mercury230Impl::getEnergyForPrevDayBegin() {
    MercuryException *cause = 0;
    return buildEnergyLevel(sendEnergyRequest(0xD, cause), cause);
}

EnergyLevelPhase Mercury230Impl::getPhaseActiveEnergyLevel() {
    MercuryException *cause = 0;
    return buildEnergyPhase(sendEnergyPhaseRequest(0x6, cause), cause);
}

int* Mercury230Impl::sendEcho() {
    uint8_t *request = new uint8_t[REQ_LENGTH_ECHO];

    //add device id to request and fill all data with zero bytes.
    preProcessRequest(request, REQ_LENGTH_ECHO);

    //fill request
    request[1] = 0x00; //operation code

    //calc CRC
    fillCRC(request, REQ_LENGTH_ECHO);

    //send
    int resLength = 0;
    int *response = server->process(REQ_LENGTH_ECHO, request, resLength);
    MercuryException* cause = checkResult(response, resLength, RESP_LENGTH_ECHO);
    
    if(cause != 0) {
        //original response is deleted now, create new
        response = new int[2]; 
        response[1] = 1; //write error code
    }

    delete[] request;
    return response;
}

int* Mercury230Impl::sendAuthRequest(byte authLevel, String password) {
    uint8_t *request = new uint8_t[REQ_LENGTH_AUTH];

    //add device id to request and fill all data with zero bytes.
    preProcessRequest(request, REQ_LENGTH_AUTH);

    //fill request
    request[1] = 0x01; // operation code
    request[2] = authLevel;

    int pwdLength = password.length();
    for (int i = 0; i < 6; ++i) {
        //read data from password, if it has it.
        //if password is too short, fill with 0.
        byte data = i >= pwdLength ? 0 : (password.charAt(i) - '0');
        request[3 + i] = data;
    }

    //calc CRC
    fillCRC(request, REQ_LENGTH_AUTH);

    //send
    int resLength = 0;
    int *response = server->process(REQ_LENGTH_AUTH, request, resLength);
    MercuryException* cause = checkResult(response, resLength, RESP_LENGTH_AUTH);
    
    if(cause != 0) {
        //original response is deleted now, create new
        response = new int[2]; 
        response[1] = 1; //write error code
    }

    delete[] request;
    return response;
}

int* Mercury230Impl::sendEnergyRequest(byte opCode, MercuryException *&cause) {
    return sendEnergyMonthRequest(opCode, 0, cause);
}

int* Mercury230Impl::sendEnergyMonthRequest(byte opCode, byte month, MercuryException *&cause) {
    uint8_t *request = new uint8_t[REQ_LENGTH_ENERGY_LEVEL];

    //add device id to request and fill all data with zero bytes.
    preProcessRequest(request, REQ_LENGTH_ENERGY_LEVEL);

    //fill request
    request[1] = ENERGY_REQ_CODE;
    //request[2] = 0x01; // operation code
    request[2] = opCode << 4 | (month & 0xF); //in one byte: (opCode | month)
    request[3] = ENERGY_REQ_TARIFF;

    //calc CRC
    fillCRC(request, REQ_LENGTH_ENERGY_LEVEL);

    //send
    int resLength = 0;
    int *response = server->process(REQ_LENGTH_ENERGY_LEVEL, request, resLength);
    cause = checkResult(response, resLength, RESP_LENGTH_ENERGY_LEVEL);

    delete[] request;
    return response; //return builded data
}

int* Mercury230Impl::sendEnergyPhaseRequest(byte opCode, MercuryException *&cause) {
    uint8_t *request = new uint8_t[REQ_LENGTH_ENERGY_LEVEL];

    //add device id to request and fill all data with zero bytes.
    preProcessRequest(request, REQ_LENGTH_ENERGY_LEVEL);

    //fill request
    request[1] = ENERGY_REQ_CODE;
    //request[2] = 0x6; // operation code
    request[2] = opCode << 4; //in one byte: (opCode | 0)
    request[3] = ENERGY_REQ_TARIFF;

    //calc CRC
    fillCRC(request, REQ_LENGTH_ENERGY_LEVEL);

    //send
    int resLength = 0;
    int *response = server->process(REQ_LENGTH_ENERGY_LEVEL, request, resLength);
    cause = checkResult(response, resLength, RESP_LENGTH_ENERGY_PHASE);

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

EnergyLevel Mercury230Impl::buildEnergyLevel(int* response, MercuryException *cause) {
    word *data = parseEnergyValue(response, 4);
    EnergyLevel result = EnergyLevel(data[0], data[1], data[2], data[3], cause);
    delete[] data;
    return result;
}

EnergyLevelPhase Mercury230Impl::buildEnergyPhase(int* response, MercuryException *cause) {
    word *data = parseEnergyValue(response, 3);
    EnergyLevelPhase result = EnergyLevelPhase(data[0], data[1], data[2], cause);
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

MercuryException* Mercury230Impl::checkResult(int *response, int length, int expectedLength) {
    if (length != expectedLength) {
        delete[] response;
        return new MercuryException(String("Response length is expected to be ") + String(expectedLength) + String("but was ") + String(length));
    }

    byte *tempData = new byte[length];
    for (int i = 0; i < length; ++i) {
        tempData[i] = (byte) response[i];
    }

    int crc = (tempData[length - 2] << 8) & tempData[length - 1];
    fillCRC(tempData, length);
    int correctCrc = (tempData[length - 2] << 8) & tempData[length - 1];

    if (crc != correctCrc) {
        delete[] response;
        return new MercuryException(String("Response CRC is incorrect"));
    }

    if (response[0] != id) {
        delete[] response;
        return new MercuryException(String("Response came from device ") + String(response[0]) + String("but expected from ") + String(id));
    }
    
    //Success
    return 0;
}







