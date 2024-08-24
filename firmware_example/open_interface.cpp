#include <Arduino.h>
#include "open_interface.h"

// Constructor function
open_interface::open_interface() {
  
}

// Assign Serial Port Objects
void open_interface::setSerial(Stream *serobj) {
  _serobj = serobj;
}

// Assign Serial Port Objects
void open_interface::setSerial1(Stream *serobj) {
  _serobj1 = serobj;
}


void open_interface::startVac() {
  _serobj->write("starting roomba");
  _serobj1->write(128);  //Start
  delay(100);
  _serobj1->write(135);
  delay(100);
}

void open_interface::backwards() {
  _serobj->println("going backwards");
  _serobj1->write(128);
  _serobj1->write(131);
  _serobj1->write(145);
  _serobj1->write(255);
  _serobj1->write(56);
  _serobj1->write(255);
  _serobj1->write(56);
}

void open_interface::forwards() {
  _serobj->println("going forwards");
  _serobj1->write(128);
  _serobj1->write(131);
  _serobj1->write(145);
  _serobj1->write(1);
  _serobj1->write(244);
  _serobj1->write(1);
  _serobj1->write(244);
}

void open_interface::right() {
  _serobj->println("going right");
  _serobj1->write(128);
  _serobj1->write(131);
  _serobj1->write(145);
  _serobj1->write(255);
  _serobj1->write(56);
  _serobj1->write((byte) 0x00);
  _serobj1->write(200);
}
void open_interface::left() {
  _serobj->println("going left");
  _serobj1->write(128);
  _serobj1->write(131);
  _serobj1->write(145);
  _serobj1->write((byte) 0x00);
  _serobj1->write(200);
  _serobj1->write(255);
  _serobj1->write(56);
}

void open_interface::stop() {
  _serobj->println("stopping motion");
  _serobj1->write(128);
  _serobj1->write(131);
  _serobj1->write(145);
  _serobj1->write((byte) 0x00);
  _serobj1->write((byte) 0x00);
  _serobj1->write((byte) 0x00);
  _serobj1->write((byte) 0x00);
}

void open_interface::mainVac() {
  _serobj->println("vacuum on");
  _serobj1->write(128);
  _serobj1->write(131);
  _serobj1->write(138);
  _serobj1->write(12);
}
void open_interface::brush() {
  _serobj->println("brush on");
  _serobj1->write(128);
  _serobj1->write(131);
  _serobj1->write(138);
  _serobj1->write(3);
}

void open_interface::motorsOff() {
  _serobj->println("motor off");
  _serobj1->write(128);
  _serobj1->write(131);
  _serobj1->write(138);
  _serobj1->write(138);
}

void open_interface::fullVac() {
  _serobj->println("full vac");
  _serobj1->write(128);
  _serobj1->write(131);
  _serobj1->write(138);
  _serobj1->write(0b00000111);
}
void open_interface::fullVacOff() {
  _serobj->println("full vac");
  _serobj1->write(128);
  _serobj1->write(131);
  _serobj1->write(138);
  _serobj1->write((byte) 0x00);
}

void open_interface::triggerControl(int brake, int throttle,bool reverse){
  _serobj->println("Tank Steer");

  int16_t _brake = map(brake,0,1023,0,500);
  if(reverse){
    _brake = _brake*(-1);
  }
  uint8_t brake_low = (_brake >> 0) & 0xFF;
  uint8_t brake_high = (_brake >> 8) & 0xFF;
  // _serobj->print("Brake--> ");_serobj->print(_brake);_serobj->print("High Byte:");_serobj->print(brake_high);_serobj->print(" Low Byte:");_serobj->println(brake_low);

  int16_t _throttle = map(throttle,0,1023,0,500);
  if(reverse){
    _throttle = _throttle*(-1);
  }
  uint8_t throttle_low = (_throttle >> 0) & 0xFF;
  uint8_t throttle_high = (_throttle >> 8) & 0xFF;
  // _serobj->print("Throttle--> ");_serobj->print(_throttle);_serobj->print("High Byte:");_serobj->print(throttle_high);_serobj->print(" Low Byte:");_serobj->println(throttle_low);
  _serobj1->write(128);
  _serobj1->write(131);
  _serobj1->write(145);
  // Right Side Velocity
  _serobj1->write(throttle_high);
  _serobj1->write(throttle_low);
  // Left Side Velocity
  _serobj1->write(brake_high);
  _serobj1->write(brake_low);
}

void open_interface::racingControl(int steering, int throttle,bool reverse){
  int16_t right_speed;
  int16_t left_speed;
  float steering_speed;
  float steering_gain;
  float speed_bias;
  float bias;
  float speed_bias_ratio = .25;

  int16_t _steering = map(steering,-512,512,-500,500);
  int16_t _throttle = map(throttle,0,1023,0,500);

  if(reverse){
    _throttle = _throttle*(-1);
  }
  // Steer Right
  if (steering >= 150) {
    steering_gain = (_steering)/500.0;
    speed_bias = (throttle/1023.0)*speed_bias_ratio;
    bias = (1.0-(steering_gain*(1.0-speed_bias)));
    steering_speed = (_throttle*bias);
    right_speed = steering_speed;
    left_speed = _throttle;
  }
  // Steer Left
  else if (steering <= -150) {
    steering_gain = ((-1.0)*_steering)/500.0;
    speed_bias = (throttle/1023.0)*speed_bias_ratio;
    bias = (1.0-(steering_gain*(1.0-speed_bias)));
    steering_speed = (_throttle*bias);
    right_speed = _throttle;
    left_speed = steering_speed;
  }
  else {
    right_speed = _throttle;
    left_speed = _throttle;
  }
  
  // _serobj.print("Steering Gain:");_serobj.print(steering_gain);_serobj.print(" Speed Bias:");_serobj.print(speed_bias);_serobj.print(" Bias:");_serobj.println(bias);
  // _serobj.print(" Right:");_serobj.print(right_speed);_serobj.print(" Left:");_serobj.println(left_speed);
  uint8_t right_high = (right_speed >> 8) & 0xFF;
  uint8_t right_low = (right_speed >> 0) & 0xFF;

  uint8_t left_high = (left_speed >> 8) & 0xFF;
  uint8_t left_low = (left_speed >> 0) & 0xFF;

  _serobj1->write(128);
  _serobj1->write(131);
  _serobj1->write(145);
  // Right Side Velocity
  _serobj1->write(right_high);
  _serobj1->write(right_low);
  // Left Side Velocity
  _serobj1->write(left_high);
  _serobj1->write(left_low);
}


void open_interface::tankSteer(int steering){
  int16_t right_speed;
  int16_t left_speed;
  int16_t _steering = map(steering,-512,512,-150,150);

  // Steer Right
  if (_steering >= 0) {
    right_speed = _steering*(-1);
    left_speed = _steering;
  }
  // Steer Left
  else if (_steering <= 0) {
    right_speed = _steering*(-1);
    left_speed = _steering;
  }
  _serobj->print("Speed--> Right:");_serobj->print(right_speed);_serobj->print(" Left:");_serobj->println(left_speed);
  uint8_t right_high = (right_speed >> 8) & 0xFF;
  uint8_t right_low = (right_speed >> 0) & 0xFF;

  uint8_t left_high = (left_speed >> 8) & 0xFF;
  uint8_t left_low = (left_speed >> 0) & 0xFF;
  _serobj->print("Right--> High Byte:");_serobj->print(right_high);_serobj->print(" Low Byte:");_serobj->println(right_low);
  _serobj->print("Left--> High Byte:");_serobj->print(left_high);_serobj->print(" Low Byte:");_serobj->println(left_low);
  _serobj1->write(128);
  _serobj1->write(131);
  _serobj1->write(145);
  // Right Side Velocity
  _serobj1->write(right_high);
  _serobj1->write(right_low);
  // Left Side Velocity
  _serobj1->write(left_high);
  _serobj1->write(left_low);
}