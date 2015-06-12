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

//const int delayTime = 10;
const int UPDATE_TIME = 30000;
int lastUpdate = millis() + UPDATE_TIME + 1;

HardwareSerial *mercuryPort = &Serial1;
HardwareSerial *modbusPort = &Serial;
HardwareSerial *debugLogger = &Serial2;
bool debugMode = true;

const int authLevel = 1;
String password = "111111";

const word modbusBaud = 9600;
const byte modbusDeviceId = 1;

const word mercuryBaud = 9600;
const int mercuryDeviceIds[] = {84};

const int devicesCount = 1;
Mercury230** devices;

MercuryModbusIntegrator integrator;

Mercury230* buildDevice(word id, MercuryServerSerial* server) {
    Mercury230Impl* result = new Mercury230Impl(id);
    result->setServer(server);
    return result;
}

void buildDevices(MercuryServerSerial* server) {
  devices = new Mercury230*[devicesCount];
  for(int i = 0; i < devicesCount; ++i) {
    devices[i] = buildDevice(mercuryDeviceIds[i], server);
  }
}

void setup() {
  MercuryServerSerial* server = new MercuryServerSerial();
  buildDevices(server);
  integrator.setDevices(devicesCount, devices);
  integrator.setAuthLevel(authLevel);
  integrator.setPassword(password);
  integrator.init(modbusBaud, modbusDeviceId);
  integrator.setModbusPort(modbusPort);
  integrator.debugLogger = debugLogger;
  
  server->debugMode = debugMode;
  server->setPort(mercuryPort);
  server->logger = debugLogger;
  mercuryPort->begin(mercuryBaud);
  debugLogger->begin(modbusBaud);
  
  integrator.updateData();
}

//TODO make updating data run each N seconds.

void loop() {
  if(modbusPort->available() > 0) {
    integrator.run();
//    integrator.updateData();
if(millis() - lastUpdate > UPDATE_TIME) {
integrator.updateData();
lastUpdate = millis();

}
  }
  
//  if(timeSinceLastUpdate >= UPDATE_TIME) {
//    integrator.updateData();
//    timeSinceLastUpdate = 0;    
//  } else {
//    timeSinceLastUpdate += delayTime;
//  }
  
//  delay(delayTime);
  
}
