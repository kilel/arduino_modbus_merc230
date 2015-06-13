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

//
// Mercury 230 protocol test.
// Used to test connection with mecrury 230 device.
//

//
// Debug parameters
//
// Logging port
HardwareSerial *debugLogger = &Seria2;
// Debug mode activation flag
bool debugMode = true;


//
// Mercury device settings
//
// Mercury device connection port
HardwareSerial *mercuryPort = &Serial;
// Connection speed (baud)
const word mercuryBaud = 9600;
// Device ID
const int mercuryDeviceId = 84;
// Authentication level
const int authLevel = 1;
// Authentication password
String password = "111111";


// Internal parameters
Mercury230* device;

void setup() {
    // Setup ports
    if (debugMode) {
        debugLogger->begin(9600);
    }

    // Setup mercury connection server module
    MercuryServerSerial* server = new MercuryServerSerial(mercuryPort, mercuryBaud);
    server->debugMode = debugMode;
    server->logger = debugLogger;

    // Create target device
    Mercury230Impl* targetDevice = new Mercury230Impl(mercuryDeviceId);
    targetDevice->setServer(server);
    device = targetDevice;
}

void loop() {
    device->echo();
    delay(5000);
    device->auth(authLevel, password);
    delay(5000);
    device->getEnergyFromReset();
    delay(5000);
}
