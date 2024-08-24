#ifndef open_interface_h
#define open_interface_h
#include "Arduino.h"

class open_interface{
  public:
    open_interface(); // Constructor
    void setSerial(Stream *serobj);
    void setSerial1(Stream *serobj);
    void startVac();
    void backwards();
    void forwards();
    void right();
    void left();
    void stop();
    void mainVac();
    void brush();
    void motorsOff();
    void fullVac();
    void fullVacOff();
    void triggerControl(int brake, int throttle,bool reverse);
    void racingControl(int steering, int throttle,bool reverse);
    void tankSteer(int steering);
    
  private:
    Stream *_serobj;
    Stream *_serobj1;
    int brake;
    int throttle;
    bool reverse;
    int steering;
    
};


#endif