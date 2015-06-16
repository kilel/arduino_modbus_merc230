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
// Mercury-Modbus integration test.
// Used to test integration with mecrury 230 device over Modbus protocol.
// Can run in mock mode without real Mercury 230 device.
//

//
// Debug parameters
//
// Logging port
HardwareSerial *debugLogger = &Serial2;
// Logging port speed (baud)
const word loggerBaud = 9600;
// Debug mode activation flag
bool debugMode = false;
// Mock mode -- mercury devices are replaced with fake one.
bool mockMode = false;

//
// Modbus parameters
//
// Connection port
HardwareSerial *modbusPort = &Serial;
// Connection speed
const word modbusBaud = 9600;
// Modbus device ID
const byte modbusDeviceId = 1;

//
// Mercury device settings
//
// Mercury device connection port
HardwareSerial *mercuryPort = &Serial1;
// Connection speed (baud)
const word mercuryBaud = 9600;
// Mercury device identifiers
const int mercuryDeviceIds[] = {84};
const int devicesCount = 1;
// Authentication level
const int authLevel = 1;
// Authentication password
String password = "111111";

//
//Internal parameters
//
MercuryModbusIntegrator integrator;

Mercury230* buildDevice(word id, MercuryServerSerial* server) {
    Mercury230Impl* result = new Mercury230Impl(id);
    result->setServer(server);
    return result;
}

Mercury230* buildMockDevice(word id) {
    Mercury230* result = new Mercury230Mock(id);
    return result;
}

Mercury230** buildDevices(MercuryServerSerial* server) {
    Mercury230** devices = new Mercury230*[devicesCount];

    for (int i = 0; i < devicesCount; ++i) {
        if (!mockMode) {
            // Regular integration, device is connected to server.
            devices[i] = buildDevice(mercuryDeviceIds[i], server);
        } else {
            // In mock integration we should build mock device.
            devices[i] = buildMockDevice(mercuryDeviceIds[i]);
        }
    }

    return devices;
}

void setup() {
    // Setup ports
    if (debugMode) {
        debugLogger->begin(loggerBaud);
    }

    MercuryServerSerial* server = new MercuryServerSerial(mercuryPort, mercuryBaud);
    server->debugMode = debugMode;
    server->logger = debugLogger;

    integrator.initAuth(authLevel, password);
    integrator.initLogging(debugLogger, debugMode);
    integrator.initDevices(devicesCount, buildDevices(server));
    integrator.init(modbusPort, modbusBaud, modbusDeviceId);
}

void loop() {
    integrator.run();
}
