#include <dummy.h>

/*
    Video: https://www.youtube.com/watch?v=oCMOYS71NIU
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updated by chegewara

   Create a BLE server that, once we receive a connection, will send periodic notifications.
   The service advertises itself as: 4fafc201-1fb5-459e-8fcc-c5c9c331914b
   And has a characteristic of: beb5483e-36e1-4688-b7f5-ea07361b26a8

   The design of creating the BLE server is:
   1. Create a BLE Server
   2. Create a BLE Service
   3. Create a BLE Characteristic on the Service
   4. Create a BLE Descriptor on the characteristic
   5. Start the service.
   6. Start advertising.

   A connect hander associated with the server starts a background task that performs notification
   every couple of seconds.
*/
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Arduino.h>

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;
const int GPIO5 = 5;  // the number of the input pin
bool trigger = true;
 

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"


class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};


//niki
struct Chain {
    const uint8_t PIN;
    bool cut;
};

struct Buzzer {
    const uint8_t PIN;
    bool active;
};

Chain chain1 = {4, false}; //GPIO1 39 //change to 4
Buzzer buzzer1 = {5, false}; //GPIO2 38 //change to 5

void ARDUINO_ISR_ATTR isr() {
    chain1.cut = true;
}
//niki


void setup() {
  Serial.begin(115200);

  //buzzer setup
  pinMode(chain1.PIN, INPUT);
  chain1.cut = false;
  attachInterrupt(chain1.PIN, isr, FALLING);
  pinMode(buzzer1.PIN, OUTPUT);
  delay(1000);
  Serial.printf("setup.");

  // Create the BLE Device
  BLEDevice::init("ESP32");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );

  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
  // Create a BLE Descriptor
  pCharacteristic->addDescriptor(new BLE2902());

  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
  Serial.println("Waiting a client connection to notify...");
}

void loop() {
    if (deviceConnected) {
      //Serial.printf("connected");
      if (chain1.cut) {
        delay(1000);
        //Serial.printf("Chain 1 has been cut.");
        digitalWrite(buzzer1.PIN, HIGH);
        value = 1;
        pCharacteristic->setValue(value);
        pCharacteristic->notify();
        delay(10000);
        chain1.cut = false;
      }else {
        value = 0;
        digitalWrite(buzzer1.PIN, LOW);
        pCharacteristic->setValue(value);
        pCharacteristic->notify();
        delay(1000);
      }
    }
    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        value = 0;
        chain1.cut = false;
        digitalWrite(buzzer1.PIN, LOW);
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
        value = 0;
        chain1.cut = false;
        digitalWrite(buzzer1.PIN, LOW);
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
}