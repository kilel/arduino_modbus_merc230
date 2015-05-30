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

//TODO add variables for mercury port and debug port

bool debugMode = true;

const int authLevel = 1;
String password = "111111";

const word mercuryBaud = 9600;
const int mercuryDeviceId = 84;

Mercury230Impl* device;

void setup() {
  device = new Mercury230Impl(mercuryDeviceId);
  MercuryServerSerial* server = new MercuryServerSerial();
  server->debugMode = debugMode;
  server->setPort(&Serial);
  device->setServer(server);
  Serial.begin(mercuryBaud);
  Serial1.begin(mercuryBaud);
}

void loop() {
  device->auth(authLevel, password);
  delay(5000);
}
