#include <Bluepad32.h>
#include <Adafruit_NeoPixel.h>
#include "open_interface.h"

ControllerPtr myControllers[BP32_MAX_GAMEPADS];
Adafruit_NeoPixel pixels(1, PIN_NEOPIXEL);
open_interface gamepad;

void setup() {
    Serial.begin(115200);
    Serial1.begin(115200,SERIAL_8N1,7,32);
    gamepad.setSerial(&Serial);
    gamepad.setSerial1(&Serial1);
    pixels.setPixelColor(0, pixels.Color(255, 0, 0));
    pixels.show();
    Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
    const uint8_t* addr = BP32.localBdAddress();
    Serial.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

    BP32.setup(&onConnectedController, &onDisconnectedController);
    BP32.forgetBluetoothKeys();
    BP32.enableVirtualDevice(false);
}

void loop() {

    bool dataUpdated = BP32.update();
    if (dataUpdated)
        processControllers();

    delay(150);
}
