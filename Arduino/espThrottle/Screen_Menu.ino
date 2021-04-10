#define MENU_ITEMS          4
#define MENU_ITEM_SPACE     22

Screen_Menu::Screen_Menu() {}

void Screen_Menu::init(Adafruit_ST7789 *tft, WiThrottle *wiThrottle) {

  _tft = tft;
  _wiThrottle = wiThrottle;
  menuBoxX = (TFT_WIDTH - MENU_BOX_WIDTH) / 2;
  menuBoxY = (TFT_HEIGHT - MENU_BOX_HEIGHT) / 2;
}

void Screen_Menu::show() {

  // display menu box
  _tft->fillRect(menuBoxX, menuBoxY, MENU_BOX_WIDTH, MENU_BOX_HEIGHT, ST77XX_BLACK);
  _tft->drawRect(menuBoxX, menuBoxY, MENU_BOX_WIDTH, MENU_BOX_HEIGHT, ST77XX_WHITE);

  displayMenuItems();
}

void Screen_Menu::encoderTick(bool forward) {

  // loop until an enabled menu item is found
  bool found = false;
  do {
    
    if(forward) selectedMenuItem++;
    else selectedMenuItem--;

    if(selectedMenuItem == MENU_ITEMS) selectedMenuItem = 0;
    else if(selectedMenuItem == -1) selectedMenuItem = MENU_ITEMS - 1;    

    if(selectedMenuItem == 0 && ENTER_ADDRESS_ENABLED) found = true;
    if(selectedMenuItem == 1 && FAVOURITE_LIST_ENABLED) found = true;
    if(selectedMenuItem == 2 && ROSTER_LIST_ENABLED) found = true;
    if(selectedMenuItem == 3 && POWER_CONTROL_ENABLED) found = true;    
  } while(!found);
  
  displayMenuItems();
}

void Screen_Menu::encoderClick() { 

  if(selectedMenuItem == 0) currentScreen = &screenChangeAddr;
  
  else if(selectedMenuItem == 1) {
    screenList.setSource(SOURCE_FAVOURITE);
    currentScreen = &screenList;
  }
  
  else if(selectedMenuItem == 2) {
    screenList.setSource(SOURCE_ROSTER);
    currentScreen = &screenList;
  }
  
  else if(selectedMenuItem == 3) {
    TrackPower newTrackPower = (trackPower == PowerOn) ? PowerOff : PowerOn;
    _wiThrottle->setTrackPower(newTrackPower);
    trackPower = newTrackPower;
    if(locoAddress != -1) currentScreen = &screenLoco;
  }

  redrawScreen = true;
}

void Screen_Menu::menuClick() {
 
  if(locoAddress != -1) {
    currentScreen = &screenLoco;
    redrawScreen = true;  
  }
}

void Screen_Menu::displayMenuItems() {

  _tft->setFont(&FreeMono12pt7b);

  for(int i = 0; i < MENU_ITEMS; i++) {
    _tft->setCursor(menuBoxX + 10, menuBoxY + MENU_ITEM_SPACE * (i + 1));
    if(i == selectedMenuItem) _tft->setTextColor(ST77XX_YELLOW);
    else _tft->setTextColor(ST77XX_WHITE);

    // print in gray color if disabled
    if(i == 0 && !ENTER_ADDRESS_ENABLED) _tft->setTextColor(ST77XX_GRAY);
    if(i == 1 && !FAVOURITE_LIST_ENABLED) _tft->setTextColor(ST77XX_GRAY);
    if(i == 2 && !ROSTER_LIST_ENABLED) _tft->setTextColor(ST77XX_GRAY);
    if(i == 3 && !POWER_CONTROL_ENABLED) _tft->setTextColor(ST77XX_GRAY);
    _tft->print(menuItems[i]);
  }
}
