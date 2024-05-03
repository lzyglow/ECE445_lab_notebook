**Zhuoyuan Worklog**


______________________________________________________________________________________________________________________________________________________________________________________________________
**3/14/2024 Workload distribution**


In order to alert the bike user swiftly and provide important information about the thief, the proposed solution is a cable bike lock that detects when the cable is cut. A current will be passed through the cable and an open circuit will be detected by the microcontroller if the cable is cut. When the cable is cut, our cameras positioned on the cable and bike will record images that may potentially identify the criminal. The microcontroller will also send out a signal to trigger an alarm, as well as relay all this information to the user via bluetooth connection. The entire system serves to provide multiple layers of safeguards for the user’s bike. First, it deters theft attempts. Second, it alerts the public to a crime. Third, it captures evidence that can enable bike recovery. Here's the visual aid from proposal with the block diagrams:
![block_diagram](block_diagram.png)
![visual_aid](visual_aid.png)


I have interest and experience in coding, so I will focus on the software part of our porject. The bluetooth communication between two microcontroller and the bluetooth communication from esp32-CAM to the user phone. Nicki is focusing on dealing with the communication to user phone, so I will start to research on bluetooth communication between two microcontroller. We will do our own parts and I will integrate our code together. Jonathan will focus on pcb design.
______________________________________________________________________________________________________________________________________________________________________________________________________
**3/26/2024 Bluetooth communication**


It seems like nicki changed her mind to use WIFI module to communicate from esp32-CAM to user phone, but I can still use Bluetooth BLE for two chip communication. Bluetooth BLE should save more energy for communication comparing to WIFI communication between two phones.
I finish the coding of the server
