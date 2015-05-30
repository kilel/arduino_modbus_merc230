#include <Mercury230.h>
#include <Mercury230Impl.h>
#include <Mercury230Mock.h>
#include <MercuryException.h>
#include <MercuryModbusIntegrator.h>
#include <MercuryServer.h>
#include <MercuryServerSerial.h>

#include <modbusDevice.h>
#include <modbusSlave.h>
#include <modbusRegBank.h>
#include <modbus.h>

HardwareSerial *mercuryPort = &Serial1;
HardwareSerial *modbusPort = &Serial;
HardwareSerial *debugLogger = &Serial1;
bool debugMode = false;

const int authLevel = 1;
String password = "111111";

const word modbusBaud = 9600;
const byte modbusDeviceId = 1;

const word mercuryBaud = 9600;
const int mercuryDeviceIds[] = {5, 13};

const int devicesCount = 2;
Mercury230** devices;

MercuryModbusIntegrator integrator;

Mercury230* buildDevice(word id) {
    Mercury230* result = new Mercury230Mock(id);
    //INIT
    return result;
}

void buildDevices() {
  devices = new Mercury230*[devicesCount];
  for(int i = 0; i < devicesCount; ++i) {
    devices[i] = buildDevice(mercuryDeviceIds[i]);
  }
}

void setup() {
  buildDevices();
  integrator.setDevices(devicesCount, devices);
  integrator.setAuthLevel(authLevel);
  integrator.setPassword(password);
  integrator.init(modbusBaud, modbusDeviceId);
  integrator.setModbusPort(modbusPort);
  integrator.debugLogger = debugLogger;
}

//TODO make updating data run each N seconds.

void loop() {
  if(Serial.available() > 0) {
    integrator.updateData();
    integrator.run();
  }
}
