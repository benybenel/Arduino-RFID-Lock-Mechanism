#include <BLEDevice.h>
#ifndef LED_BUILTIN
#define LED_BUILTIN (2) // LED Pin on DevKit V1 boards
#endif
#include <BLE2902.h>
int seconds = 0;
// See https://www.uuidgenerator.net/ for generating UUIDs:
BLEUUID serviceID("XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX");
BLEUUID readonlyCharID("XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX");
BLEUUID writableCharID("6a7a6f24-ae44-11ed-afa1-0242ac120002");
//A class definition for handling callbacks
class MyCallbacks: public BLECharacteristicCallbacks {
//this method will be call to perform writes to characteristic
  void onWrite(BLECharacteristic *pCharacteristic) {
   if (writableCharID.equals(pCharacteristic->getUUID())) { //is it our characteristic ?
    uint8_t *value = pCharacteristic->getData(); //get the data associated with it.
     digitalWrite (LED_BUILTIN, value[0]?LOW:HIGH); //turn off if 0; on otherwise.
  }
 }
};

//A global store for the data in our writable characteristic
uint8_t writable_store[1];

//Create a callback handler
MyCallbacks cb;

BLECharacteristic *readCharacteristic;

void setup() {
  BLEDevice::init("Ben_Bluetooth");
  
  //Set up the BLE Device in Server mode
  BLEServer *pServer = BLEDevice::createServer();
  
  //Each BLE Server can have multiple services
  BLEService *pService = pServer->createService(serviceID);

  //An example read-only characteristic
  readCharacteristic = pService->createCharacteristic(
     readonlyCharID,
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
  readCharacteristic ->setValue(seconds);

  readCharacteristic->addDescriptor(new BLE2902());

 //An example writable characteristic
BLECharacteristic *writeCharacteristic = pService->createCharacteristic(
writableCharID,
BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);

//bit-wise OR is used to enable both properties.
writeCharacteristic ->setValue(writable_store, 1);
writeCharacteristic ->setCallbacks(&cb);

  //Start the service
  pService->start();
 
  //Advertising config
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(serviceID);
  pAdvertising->setScanResponse(true);

  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);

  //Start advertising the device
  BLEDevice::startAdvertising();
  
  //Enable the LED
pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
 delay(2000);
 seconds ++;

 readCharacteristic->setValue(seconds);
 readCharacteristic->notify();
}
