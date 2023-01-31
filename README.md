# BellGuard_Insert_Accelerometer
A Bellguard Insert for an epee that can measure acceleration and transmit wirelessly to a receiver. The general layout can be seen in the schematic [Schematic Epee Bellguard Insert and Speaker.pdf](https://github.com/BenKohn2004/BellGuard_Insert_Accelerometer/blob/main/Schematic%20Epee%20Bellguard%20Insert%20and%20Speaker.pdf)

The Bellguard Insert is modeled using [OnShape](https://cad.onshape.com/documents/51ca8c06bc5b5404b12fe69e/w/3a4a6ec2c7af2a419aeb4d50/e/05886a4a9ec08934e00e5f42?renderMode=0&uiState=63d94f4438040d08cedf6ea3), an example can be found at [3D Model of the Insert.png](https://raw.githubusercontent.com/BenKohn2004/BellGuard_Insert_Accelerometer/main/3D%20Model%20of%20the%20Insert.png). The assembled and wired insert would look like [this](https://raw.githubusercontent.com/BenKohn2004/BellGuard_Insert_Accelerometer/main/Inside%20of%20the%20Insert.jpg).

A [Wemos](https://www.amazon.com/Organizer-ESP8266-Internet-Development-Compatible/dp/B081PX9YFV/ref=sr_1_1?keywords=wemos+d1+mini&qid=1675193531&sprefix=wemos+%2Caps%2C158&sr=8-1) D1 Mini is used to control an accelerometer, [MPU6050](https://www.amazon.com/HiLetgo-MPU-6050-Accelerometer-Gyroscope-Converter/dp/B00LP25V1A/ref=sr_1_2?keywords=MPU-6050&qid=1675186277&sr=8-2&th=1), and is charged using an [MH-CD42 Module](https://www.amazon.com/Discharge-Integrated-Charging-Protection-Converter/dp/B0836J8LR4/ref=sr_1_1?crid=8CMVFWO3CG4X&keywords=MH-CD42+module&qid=1675186433&sprefix=mh-cd42+module%2Caps%2C127&sr=8-1) along with a [Lithium Polymer Drone Battery](https://www.amazon.com/URGENEX-380mAh-Battery-Charger-Controller/dp/B08DD5MTKM/ref=sr_1_5?crid=IUKUTK3ZQSIM&keywords=Lithium+Polymer+Drone+Battery+380+mAh&qid=1675186473&sprefix=lithium+polymer+drone+battery+380+mah%2Caps%2C153&sr=8-5) which then connects wirelessly to an [ESP32](https://www.amazon.com/ESP-WROOM-32-Development-Microcontroller-Integrated-Compatible/dp/B08D5ZD528/ref=sr_1_1?crid=2RWGOKEGREG5K&keywords=ESP32&qid=1675186567&sprefix=esp32%2Caps%2C160&sr=8-1&th=1) which drives a [MAX98357A](https://www.amazon.com/Teyleten-Robot-Amplifier-Interface-Filterless/dp/B0B4GK5R1R/ref=sr_1_6?crid=YBPRLZ9H5BZV&keywords=MAX98357A&qid=1675186611&sprefix=max98357a%2Caps%2C162&sr=8-6) amplifier module which is connected to a speaker. The mp3 sounds that are played are stored on an SD Card connected to the ESP32 and are randomly chosen each time the acceleration threshold is met on the Epee Bellguard Insert.

An example of the insert working can be seen on [Youtube](https://youtu.be/urIJzAw4_HY).

The goal of the project was to create a small, wireless, battery powered attachment that could be used with a fencing weapon to measure acceleration. A benefit of the accelerometer is that it can also measure orientation such as Roll, Pitch and Yaw.

One idea is that an SD card could be added to the bellguard insert so that the data could be easily recorded from a fencing match. This would allow a higher sample rate than transmitting the data after each sample and then the data could be saved and transmitter after each touch. Combining and coordinating the insert with a scoring machine, such as a Favero Full Arm 05, the data could automatically be sent following a touch. This would allow one to analyze the acceleration and orientation of the fencer's bellguard during the encounter and potentially reconstruct the action. This could be done using bellguards of both fencers so that interactions between the fencers could be mapped.
