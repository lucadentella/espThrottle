#define DEVICE_NAME_X   5
#define DEVICE_NAME_Y   14
#define TRACK_POWER_X   178
#define TRACK_POWER_Y   14
#define BATTERY_X       215
#define BATTERY_Y       5
#define LOCO_ADDRESS_POS_X  20
#define LOCO_ADDRESS_POS_Y  65
#define LOCO_NAME_POS_Y     55
#define FUNCTIONS_POS_X     20
#define FUNCTIONS_POS_Y     115
#define SPEED_POS_X         20
#define SPEED_POS_Y         85
#define SPEED_TEXT_POS_X    165
#define SPEED_TEXT_POS_Y    100
#define DIR_ARROW_POS_X     205
#define DIR_ARROW_POS_Y     88

Screen_Loco::Screen_Loco() {}

void Screen_Loco::init(Adafruit_ST7789 *tft, WiThrottle *wiThrottle) {

  _tft = tft;
  _wiThrottle = wiThrottle;
}

void Screen_Loco::show() {

  locoFullAddress = locoAddressLength + String(locoAddress);

  // clear screen area
  _tft->fillRect(0, 0, 240, 135, ST77XX_BLACK);

  // set myself as a delegate for WiThrottle
  _wiThrottle->setDelegate(this);
  _wiThrottle->releaseLocomotive();
  _wiThrottle->addLocomotive(locoFullAddress);

  // write device name
  _tft->setCursor(DEVICE_NAME_X, DEVICE_NAME_Y);
  _tft->setFont(&FreeMono9pt7b);
  _tft->print(DEVICE_NAME);

  // write loco name (if available) and address
  _tft->setFont(&FreeMonoBold18pt7b);
  _tft->setTextColor(ST77XX_GREEN);  
  if(locoName == "") {
    _tft->setCursor(LOCO_ADDRESS_POS_X, LOCO_ADDRESS_POS_Y);
    _tft->print(locoFullAddress);
  } else {
    _tft->setCursor(LOCO_ADDRESS_POS_X, LOCO_NAME_POS_Y);
    _tft->print(locoName);
    _tft->setCursor(LOCO_ADDRESS_POS_X, LOCO_ADDRESS_POS_Y + 10);
    _tft->setFont(&FreeMono9pt7b);
    _tft->print(locoFullAddress);  
  }
  
  // draw empty squares for functions (F0-F7)
  for(int i = 0; i < 8; i++)
    _tft->drawRect(FUNCTIONS_POS_X + (20 * i), FUNCTIONS_POS_Y, 12, 12, ST77XX_WHITE); 
  
  // draw empty rectangle for speed
  _tft->drawRect(SPEED_POS_X, SPEED_POS_Y, 138, 20, ST77XX_WHITE);

  // draw initial values
  updateTrackPower();
  updateSpeed();
  updateDirection();
  for(int i = 0; i < 8; i++) updateFunction(i);
  
  // init battery check
  previousBatteryCheck = 0;
  batteryLevel = 0;
}

void Screen_Loco::run() {

  // check battery status
  if(millis() > previousBatteryCheck + BATTERY_CHECK_INTERVAL) {
    checkBattery();
    previousBatteryCheck = millis();
  }  
}

void Screen_Loco::encoderTick(bool forward) { 

  if(forward && locoSpeed < 126) locoSpeed++;
  else if(!forward && locoSpeed > 0) locoSpeed--;
  _wiThrottle->setSpeed(locoSpeed);
  updateSpeed();  
}

void Screen_Loco::encoderClick() { 

  locoDirection = (locoDirection == Forward) ? Reverse : Forward;
  _wiThrottle->setDirection(locoDirection);
  updateDirection(); 

  // if configured, set speed to 0
  if(CHANGE_DIR_SPEED_ZERO) {
    locoSpeed = 0;
    _wiThrottle->setSpeed(locoSpeed);
    updateSpeed();
  }
}

void Screen_Loco::fBtnChange(int fNum, bool pressed, bool withMenu) { 

  if(withMenu) fNum = fNum + 4;
  _wiThrottle->setFunction(fNum, pressed);
}

void Screen_Loco::menuClick() { 

  if(!FIXED_ADDRESS_MODE) {
    currentScreen = &screenMenu;
    redrawScreen = true;
  }
}

void Screen_Loco::updateSpeed() {

  // draw speed bar
  _tft->fillRect(SPEED_POS_X + 6, SPEED_POS_Y + 5, 126, 10, ST77XX_BLACK);
  _tft->fillRect(SPEED_POS_X + 6, SPEED_POS_Y + 5, locoSpeed, 10, ST77XX_YELLOW);

  // write speed
  _tft->fillRect(SPEED_TEXT_POS_X, SPEED_TEXT_POS_Y - 15, 39, 20, ST77XX_BLACK);
  _tft->setCursor(SPEED_TEXT_POS_X, SPEED_TEXT_POS_Y);
  _tft->setFont(&FreeMono9pt7b);
  _tft->setTextColor(ST77XX_YELLOW);
  _tft->print(locoSpeed);
}

void Screen_Loco::updateDirection() {

  uint8_t* arrow = (locoDirection == Forward) ? arrowright : arrowleft;
  _tft->fillRect(DIR_ARROW_POS_X, DIR_ARROW_POS_Y, 18, 15, ST77XX_BLACK);
  _tft->drawBitmap(DIR_ARROW_POS_X, DIR_ARROW_POS_Y, arrow, 18, 15, ST77XX_YELLOW);
}

void Screen_Loco::updateFunction(int funcNumber) {

  bool funcStatus = locoFunctions[funcNumber];
  uint16_t color = (funcStatus) ?  ST77XX_YELLOW : ST77XX_BLACK;
  _tft->fillRect(FUNCTIONS_POS_X + 2 + (20 * funcNumber), FUNCTIONS_POS_Y + 2, 8, 8, color);
}

void Screen_Loco::updateTrackPower() {

  // write track power status
  _tft->fillRect(TRACK_POWER_X, TRACK_POWER_Y - 15, 214 - TRACK_POWER_X, 20, ST77XX_BLACK);
  _tft->setCursor(TRACK_POWER_X, TRACK_POWER_Y);
  _tft->setFont(&FreeMono9pt7b);
  if(trackPower == PowerOff) {
    _tft->setTextColor(ST77XX_RED);
    _tft->print("OFF");
  }
  else if(trackPower == PowerOn) {
    _tft->setTextColor(ST77XX_GREEN);
    _tft->print("ON");
  }
  else {
    _tft->setTextColor(ST77XX_YELLOW);
    _tft->print("?");
  }
}

void Screen_Loco::checkBattery() {

  // read the new voltage
  uint16_t v = analogRead(BATTERY_PIN);
  float battery_voltage = ((float)v / 4095.0) * 2.0 * 3.3 * (vref / 1000.0);

  // calculate the battry level
  int newBatteryLevel;
  if(battery_voltage > 4.5) newBatteryLevel = 4;
  else if(battery_voltage > 4.1) newBatteryLevel = 3;
  else if(battery_voltage > 3.5) newBatteryLevel = 2;
  else newBatteryLevel = 1;

  if(newBatteryLevel == batteryLevel) return;

  batteryLevel = newBatteryLevel;

  // clear the previous icon
  _tft->fillRect(BATTERY_X, BATTERY_Y, 21, 11, ST77XX_BLACK);
  
  // if battery voltage > 4.5 the usb cable is connected, charging
  if(batteryLevel == 4) _tft->drawBitmap(BATTERY_X, BATTERY_Y, batterycharging, 21, 11, ST77XX_WHITE);
  
  // else draw the battery level
  else {
    _tft->drawBitmap(BATTERY_X, BATTERY_Y, battery, 21, 11, ST77XX_WHITE);
    if(batteryLevel == 3) for(int i = 0; i < 3; i++) _tft->fillRect(BATTERY_X + 3 + (5 * i), BATTERY_Y + 2, 3, 7, ST77XX_GREEN); 
    else if(batteryLevel == 2) for(int i = 0; i < 2; i++) _tft->fillRect(BATTERY_X + 3 + (5 * i), BATTERY_Y + 2, 3, 7, ST77XX_YELLOW);
    else _tft->fillRect(BATTERY_X + 3, BATTERY_Y + 2, 3, 7, ST77XX_RED);
  }   
}


// ----- delegate functions for WiThrottle callbacks -----

void Screen_Loco::receivedTrackPower(TrackPower state) {

  trackPower = state;
  updateTrackPower();
}

void Screen_Loco::receivedSpeed(int speed) {

  locoSpeed = speed;
  updateSpeed();
}

void Screen_Loco::receivedDirection(Direction dir) {

  locoDirection = dir;
  updateDirection();
}

void Screen_Loco::receivedFunctionState(uint8_t func, bool state) {

  locoFunctions[func] = state;
  updateFunction(func);
}
