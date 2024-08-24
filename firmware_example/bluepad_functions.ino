void onConnectedController(ControllerPtr ctl) {
    pixels.setPixelColor(0, pixels.Color(0, 255, 0));
    pixels.show();
    bool foundEmptySlot = false;
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == nullptr) {
            Serial.printf("CALLBACK: Controller is connected, index=%d\n", i);
            
            ControllerProperties properties = ctl->getProperties();
            Serial.printf("Controller model: %s, VID=0x%04x, PID=0x%04x\n", ctl->getModelName().c_str(), properties.vendor_id,
                           properties.product_id);
            myControllers[i] = ctl;
            foundEmptySlot = true;
            break;
        }
    }
    if (!foundEmptySlot) {
        Serial.println("CALLBACK: Controller connected, but could not found empty slot");
    }
}

void onDisconnectedController(ControllerPtr ctl) {
    bool foundController = false;
    pixels.setPixelColor(0, pixels.Color(255, 0, 0));
    pixels.show();

    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == ctl) {
            Serial.printf("CALLBACK: Controller disconnected from index=%d\n", i);
            myControllers[i] = nullptr;
            foundController = true;
            break;
        }
    }

    if (!foundController) {
        Serial.println("CALLBACK: Controller disconnected, but not found in myControllers");
    }
}

void dumpGamepad(ControllerPtr ctl) {
    Serial.printf(
        "idx=%d, dpad: 0x%02x, buttons: 0x%04x, axis L: %4d, %4d, axis R: %4d, %4d, brake: %4d, throttle: %4d, "
        "misc: 0x%02x\n",
        ctl->index(),        // Controller Index
        ctl->dpad(),         // D-pad
        ctl->buttons(),      // bitmask of pressed buttons
        ctl->axisX(),        // (-511 - 512) left X Axis
        ctl->axisY(),        // (-511 - 512) left Y axis
        ctl->axisRX(),       // (-511 - 512) right X axis
        ctl->axisRY(),       // (-511 - 512) right Y axis
        ctl->brake(),        // (0 - 1023): brake button
        ctl->throttle(),     // (0 - 1023): throttle (AKA gas) button
        ctl->miscButtons()  // bitmask of pressed "misc" buttons
    );
}






void processGamepad(ControllerPtr ctl) {
    bool reverse;
    // Serial.print("Specific Value:");Serial.println(ctl->throttle());
    if (ctl->dpad() == 1) { gamepad.forwards(); }
    if (ctl->dpad() == 2) { gamepad.backwards(); }
    if (ctl->dpad() == 8) { gamepad.left(); }
    if (ctl->dpad() == 4) { gamepad.right(); }

    if (ctl->miscButtons() == 4) { gamepad.fullVac(); }
    if (ctl->miscButtons() == 2) { gamepad.fullVacOff(); }

    if(ctl->throttle() >= 50) {
      bool _reverse = false;
      gamepad.racingControl(ctl->axisX(),ctl->throttle(),_reverse);
    }
    else if(ctl->brake() >= 50) {
      bool _reverse = true;
      gamepad.racingControl(ctl->axisX(),ctl->brake(),_reverse);
    }
    else { gamepad.stop(); }

    if(ctl->brake() == 0 && ctl->throttle() == 0 && (ctl->axisX() >150 || ctl->axisX() < -150)){ 
      Serial.println("Zero Throttle");
      gamepad.tankSteer(ctl->axisX());
    }
}




void processControllers() {
    for (auto myController : myControllers) {
        if (myController && myController->isConnected() && myController->hasData()) {
            if (myController->isGamepad()) {
                processGamepad(myController);
            } else {
                Serial.println("Unsupported controller");
            }
        }
    }
}