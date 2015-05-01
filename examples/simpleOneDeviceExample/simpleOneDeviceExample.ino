#include <modbusDevice.h>
#include <modbusSlave.h>
#include <modbusRegBank.h>
#include <modbus.h>

#include <MercuryModbusIntegrator.h>
#include <Mercury230.h>
#include <Mercury230Impl.h>
#include <Mercury230Mock.h>

Mercury230* buildDevice(word id) {
    return new Mercury230Mock(id);
}

MercuryModbusIntegrator* integrator;

void setup() {
  integrator = new MercuryModbusIntegrator(1, 9600, buildDevice(123));
  integrator->setAuthLevel(1);
  integrator->setPassword("111111");
}

void loop() {
  integrator->updateData();
  integrator->run();
}
