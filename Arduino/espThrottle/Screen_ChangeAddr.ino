#define ITEM_X            20
#define FIRST_ITEM_Y      35
#define LIST_ITEM_SPACE   25
#define FORM_SPACE        100
#define BOX_WIDTH         MENU_BOX_WIDTH - ITEM_X - FORM_SPACE
#define BOX_START         16
#define BOX_HEIGHT        18

Screen_ChangeAddr::Screen_ChangeAddr() {}

void Screen_ChangeAddr::init(Adafruit_ST7789 *tft, WiThrottle *wiThrottle) {

  _tft = tft;
  _wiThrottle = wiThrottle;
  menuBoxX = (TFT_WIDTH - MENU_BOX_WIDTH) / 2;
  menuBoxY = (TFT_HEIGHT - MENU_BOX_HEIGHT) / 2;
}

void Screen_ChangeAddr::show() {

  // clear dialog area
  _tft->fillRect(menuBoxX + 1, menuBoxY + 1, MENU_BOX_WIDTH - 2, MENU_BOX_HEIGHT - 2, ST77XX_BLACK);

  // print screen title
  _tft->setCursor(menuBoxX + 10, menuBoxY + 20);
  _tft->setFont(&FreeMonoBold12pt7b);
  _tft->setTextColor(ST77XX_GREEN);
  _tft->print("Change address");

  _tft->setFont(&FreeMonoBold12pt7b);
  _tft->setTextColor(ST77XX_WHITE);
  _tft->setCursor(ITEM_X, FIRST_ITEM_Y + LIST_ITEM_SPACE);
  _tft->print("Addr:");
  _tft->setCursor(ITEM_X, FIRST_ITEM_Y + LIST_ITEM_SPACE * 2);
  _tft->print("Length:");

  // init cursor position
  cursorPosition = 0;

  // display elements
  printAddress(false);
  printAddressLength(true);
  printOk(true);
}

void Screen_ChangeAddr::encoderTick(bool forward) {

  if(!inEditMode) {

    int oldCursorPosition = cursorPosition;
    
    if(forward) cursorPosition++;
    else cursorPosition--;
    
    if(cursorPosition == 3) cursorPosition = 0;
    if(cursorPosition < 0) cursorPosition = 2;

    redrawCursor(oldCursorPosition, true);
    redrawCursor(cursorPosition, false);
  } 
  
  else {

    if(cursorPosition == 0) {
      
      if(forward) locoAddress++;
      else locoAddress--;

      if(locoAddress == 0) locoAddress = 1;
      printAddress(false);
    }

    else if(cursorPosition == 1) {

      if(locoAddressLength == 'S') locoAddressLength = 'L';
      else locoAddressLength = 'S';
      printAddressLength(false);
    }
  }
}

void Screen_ChangeAddr::encoderClick() {

  if(!inEditMode) {

    // click on OK
    if(cursorPosition == 2) {

      currentScreen = &screenLoco;
      redrawScreen = true;
    }
    else {
      inEditMode = true;
      redrawCursor(cursorPosition, false);
    }
    
  }

  else {

    inEditMode = false;
    redrawCursor(cursorPosition, false);    
  }
}

void Screen_ChangeAddr::fBtnChange(int fNum, bool pressed, bool withMenu) { 

  if(inEditMode && cursorPosition == 0) {

    int oldLocoAddress = locoAddress;
    if(fNum == 0 && pressed) locoAddress = locoAddress + 10;
    if(fNum == 1 && pressed) locoAddress = locoAddress + 100;
    if(fNum == 2 && pressed) locoAddress = locoAddress - 10;
    if(fNum == 3 && pressed) locoAddress = locoAddress - 100;
    
    if(locoAddress != oldLocoAddress) {
      if(locoAddress < 1) locoAddress = 1;
      printAddress(false);
    }    
  }
}

void Screen_ChangeAddr::printAddress(bool old) {

  if(old) _tft->setTextColor(ST77XX_WHITE);
  else _tft->setTextColor(ST77XX_YELLOW);
  if(inEditMode) _tft->setTextColor(ST77XX_RED);

  _tft->setFont(&FreeMono12pt7b);
  _tft->fillRect(ITEM_X + FORM_SPACE, FIRST_ITEM_Y + LIST_ITEM_SPACE - BOX_START, BOX_WIDTH, BOX_HEIGHT, ST77XX_BLACK);
  _tft->setCursor(ITEM_X + FORM_SPACE, FIRST_ITEM_Y + LIST_ITEM_SPACE);
  _tft->print(locoAddress);
}

void Screen_ChangeAddr::printAddressLength(bool old) {

  if(old) _tft->setTextColor(ST77XX_WHITE);
  else _tft->setTextColor(ST77XX_YELLOW);
  if(inEditMode) _tft->setTextColor(ST77XX_RED);

  _tft->setFont(&FreeMono12pt7b);
  _tft->fillRect(ITEM_X + FORM_SPACE, FIRST_ITEM_Y + LIST_ITEM_SPACE * 2 - BOX_START, BOX_WIDTH, BOX_HEIGHT, ST77XX_BLACK);
  _tft->setCursor(ITEM_X + FORM_SPACE, FIRST_ITEM_Y + LIST_ITEM_SPACE * 2);
  _tft->print(locoAddressLength);
}

void Screen_ChangeAddr::printOk(bool old) {

  if(old) _tft->setTextColor(ST77XX_WHITE);
  else _tft->setTextColor(ST77XX_YELLOW);

  _tft->setFont(&FreeMonoBold12pt7b);
  _tft->setCursor(180, FIRST_ITEM_Y + LIST_ITEM_SPACE * 3);
  _tft->print("OK");
}

void Screen_ChangeAddr::redrawCursor(int cursorPosition, bool old) {

  if(cursorPosition == 0) printAddress(old);
  else if(cursorPosition == 1) printAddressLength(old);
  else if(cursorPosition == 2) printOk(old);
}
