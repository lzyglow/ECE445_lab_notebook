**Zhuoyuan Worklog**


___________________________________________________________________________________________________________________________________________________________________________________________________
**3/14/2024 Workload distribution**


In order to alert the bike user swiftly and provide important information about the thief, the proposed solution is a cable bike lock that detects when the cable is cut. A current will be passed through the cable and an open circuit will be detected by the microcontroller if the cable is cut. When the cable is cut, our cameras positioned on the cable and bike will record images that may potentially identify the criminal. The microcontroller will also send out a signal to trigger an alarm, as well as relay all this information to the user via bluetooth connection. The entire system serves to provide multiple layers of safeguards for the user’s bike. First, it deters theft attempts. Second, it alerts the public to a crime. Third, it captures evidence that can enable bike recovery. Here's the visual aid from proposal with the block diagrams:
![block_diagram](block_diagram.png)
![visual_aid](visual_aid.png)


I have interest and experience in coding, so I will focus on the software part of our porject. The bluetooth communication between two microcontroller and the bluetooth communication from esp32-CAM to the user phone. Nicki is focusing on dealing with the communication to user phone, so I will start to research on bluetooth communication between two microcontroller. We will do our own parts and I will integrate our code together. Jonathan will focus on pcb design.
__________________________________________________________________________________________________________________________________________________________________________________________________
**3/26/2024 Bluetooth communication**


It seems like nicki changed her mind to use WIFI module to communicate from esp32-CAM to user phone, but I can still use Bluetooth BLE for two chip communication. Bluetooth BLE should save more energy for communication comparing to WIFI communication between two phones.
I finish the coding of the server by adjusting the example from the ESP32C3 Dev Module's ESP32 BLE adruino library. I researched some online tutorial and only adjust the main loop of the library besides using a UUID from a UUID generator.
void loop() {
    if (deviceConnected) {
      //trigger = digitalRead(GPIO5);
      //if (trigger == false) {
        //pCharacteristic->setValue((uint8_t*)&value, 1);
        //pCharacteristic->notify();
        //delay(1000); 
      //}
      pCharacteristic->setValue((uint8_t*)&value, 1);
      pCharacteristic->notify();
      value++;
      delay(1000);
    }
    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
}
I used a app called nRF Connect to that can connect to esp32 bluetooth to receive the value of characteristic it sents once connected. I tested that the server can successfully send values now, after programming the code to esp32S3.

![rnf](rnf.jpg)
__________________________________________________________________________________________________________________________________________________________________________________________________
**4/2/2024 Bluetooth communication client**

I finished the coding of the client, or the esp32-CAM. I mainly adjust its callback function so that the value received can be shown on the seial monitor.

static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
    Serial.print("Notify callback for characteristic ");
    Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
    Serial.print(" of data length ");
    Serial.println(length);
    uint32_t value = (pData[3] << 24) | (pData[2] << 16) | (pData[1] << 8) | (pData[0]);
    //Serial.print(pData);
    Serial.print("data: ");
    Serial.println(value);
}
The value got was originally shown as a char, and I transfer it into an int, easier to verify.

The problem now was I faced some bug that the value got wasn't as expected: we expected value 1 sent by server if chain is cut, and 0 if not, but now the value kind of piles up.
Also, I am thinking of passing value's value to a global variable to be used in the main loop, so that we can use that variable as an indicator of whether the chain is cut in client.

__________________________________________________________________________________________________________________________________________________________________________________________________
**4/4/2024 Bluetooth communication client**



