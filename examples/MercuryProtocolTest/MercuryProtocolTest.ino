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

bool debugMode = true;
HardwareSerial *mercuryPort = &Serial1;
HardwareSerial *debugLogger = &Serial;

const int authLevel = 1;
String password = "111111";

const word mercuryBaud = 9600;
const int mercuryDeviceId = 84;

Mercury230Impl* device;

void setup() {
  device = new Mercury230Impl(mercuryDeviceId);
  MercuryServerSerial* server = new MercuryServerSerial();
  server->debugMode = debugMode;
  server->setPort(mercuryPort);
  device->setServer(server);
  server->logger = debugLogger;
  mercuryPort->begin(mercuryBaud);
  debugLogger->begin(9600);
}

void loop() {
  device->auth(authLevel, password);
  delay(5000);
}
