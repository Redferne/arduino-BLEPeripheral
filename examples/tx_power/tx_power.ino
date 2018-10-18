// Copyright (c) Sandeep Mistry. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
// This work file is based on the arduino-ble-led example, adapted for PCA10059 and using setTxPower

#include <Arduino.h>

// LED pin
#define LED_PIN   LED_BUILTIN

#include <BLEPeripheral.h>
BLEPeripheral blePeripheral = BLEPeripheral();
BLEService ledService = BLEService("19b10000e8f2537e4f6cd104768a1214");
BLECharCharacteristic switchCharacteristic = BLECharCharacteristic("19b10001e8f2537e4f6cd104768a1214", BLERead | BLEWrite);

void setup() {
  // set LED pins to output open drain mode
  pinMode(LED_PIN, OUTPUT_OPEN_DRAIN);
  digitalWrite(LED_PIN, LOW);

  // set advertised local name and service UUID
  blePeripheral.setLocalName("PCA10059");
  blePeripheral.setDeviceName("Green LED");
  blePeripheral.setAdvertisedServiceUuid(ledService.uuid());
  // add service and characteristic
  blePeripheral.addAttribute(ledService);
  blePeripheral.addAttribute(switchCharacteristic);
  //set tx power for both advertising and connected mode
  blePeripheral.setTxPower(8);
  // begin initialization
  blePeripheral.begin();
  
  digitalWrite(LED_PIN, HIGH);
}

void loop() {
  BLECentral central = blePeripheral.central();

  if (central) {
    while (central.connected()) {
      // central still connected to peripheral
      if (switchCharacteristic.written()) {
        // central wrote new value to characteristic, update LED
        if (switchCharacteristic.value()) {
          digitalWrite(LED_PIN, LOW);
        } else {
          digitalWrite(LED_PIN, HIGH);
        }
      }
    }
  }
}