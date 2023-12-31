#include "CPOledDisplay.h"
#include "cp_ps2_joystick.h"


#include <BluetoothSerial.h>

#define USE_NAME 1         // Comment this to use MAC address instead of a slaveName
const char *pin = "1211";  // Change this to reflect the pin expected by the real slave BT device

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;

#ifdef USE_NAME
String slaveName = "CP_IOT_CAR";  // Change this to reflect the real name of your slave BT device
#else
String MACadd = "AA:BB:CC:11:22:33";                          // This only for printing
uint8_t address[6] = { 0xAA, 0xBB, 0xCC, 0x11, 0x22, 0x33 };  // Change this to reflect real MAC address of your slave BT device
#endif

String myName = "CP_IOT_CAR_Joystick";

bool connected;



CPPS2Joystick joystick;
CPOledDisplay display;

int car_state = MOVE_NONE;

void setup() {
  delay(1000);
  Serial.begin(115200);

  Serial.println("Serial inited");
  delay(2000);

  display.init();
  Serial.println("display inited");
  delay(200);


  joystick.init();
  Serial.println("joystick inited");
  delay(200);
  display.setStatus("joystick inited");

  SerialBT.begin(myName, true);
  Serial.printf("The device \"%s\" started in master mode, make sure slave BT device is on!\n", myName.c_str());

#ifndef USE_NAME
  SerialBT.setPin(pin);
  Serial.println("Using PIN");
#endif

// connect(address) is fast (up to 10 secs max), connect(slaveName) is slow (up to 30 secs max) as it needs
// to resolve slaveName to address first, but it allows to connect to different devices with the same name.
// Set CoreDebugLevel to Info to view devices Bluetooth address and device names
#ifdef USE_NAME
  connected = SerialBT.connect(slaveName);
  Serial.printf("Connecting to slave BT device named \"%s\"\n", slaveName.c_str());
#else
  connected = SerialBT.connect(address);
  Serial.print("Connecting to slave BT device with MAC ");
  Serial.println(MACadd);
#endif

  if (connected) {
    Serial.println("Connected Successfully!");
  } else {
    while (!SerialBT.connected(10000)) {
      Serial.println("Failed to connect. Make sure remote device is available and in range, then restart app.");
    }
  }
  // Disconnect() may take up to 10 secs max
  if (SerialBT.disconnect()) {
    Serial.println("Disconnected Successfully!");
  }
  // This would reconnect to the slaveName(will use address, if resolved) or address used with connect(slaveName/address).
  SerialBT.connect();
  if (connected) {
    Serial.println("Reconnected Successfully!");
  } else {
    while (!SerialBT.connected(10000)) {
      Serial.println("Failed to reconnect. Make sure remote device is available and in range, then restart app.");
    }
  }


  display.setStatus("bluetooth connected");
}

void loop() {
#if 1
  int event = joystick.readMove();
  if (event != car_state) {
    if (event == MOVE_DOWN) {
      SerialBT.write('b');
      display.setStatus("backward");
    } else if (event == MOVE_UP) {
      SerialBT.write('f');
      display.setStatus("forward");
    } else if (event == MOVE_LEFT) {
      SerialBT.write('l');
      display.setStatus("left");
    } else if (event == MOVE_RIGHT) {
      SerialBT.write('r');
      display.setStatus("right");
    } else {
      SerialBT.write('s');
      display.setStatus("stop");
    }
    car_state = event;
  }

#else
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
  delay(20);
#endif
}
