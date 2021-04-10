#define SERVER_OCTET_SPACE    50
#define MAX_CURSOR_POSITION   6

Screen_Server::Screen_Server() {}

void Screen_Server::init(Adafruit_ST7789 *tft, WiThrottle *wiThrottle) {

  _tft = tft;
  _wiThrottle = wiThrottle;

  // check server IP stored in EEPROM and init 
  bool serverIPvalid = false;
  for(int i = 0; i < 4; i++)
    if(EEPROM.read(i) != 255) serverIPvalid = true;

  if(!serverIPvalid) {
    EEPROM.write(0, 192);
    EEPROM.write(1, 168);
    EEPROM.write(2, 1);
    EEPROM.write(3, 1);
    EEPROM.commit();  
  }
}

void Screen_Server::show() {

  // clear screen area
  _tft->fillRect(0, 0, 240, 135, ST77XX_BLACK);

  // title
  _tft->setCursor(5, 20);
  _tft->setFont(&FreeMonoBold12pt7b);
  _tft->setTextColor(ST77XX_GREEN);
  _tft->print("Choose Server");

  // auto-discovered
  _tft->setTextColor(ST77XX_WHITE);
  _tft->setCursor(5, 50);
  _tft->print("mDNS:");

  // manual
  _tft->setCursor(5, 100);
  _tft->print("Manual:");

  // dots
  _tft->setFont(&FreeMono12pt7b);
  for(int i = 1; i < 4; i++) {
    _tft->setCursor(SERVER_OCTET_SPACE * i - 5, 125);
    _tft->print(".");
  }

  // Query mDNS service to find a wiThrottle server
  if(mdns_init() == ESP_OK)
    if(MDNS.queryService("withrottle", "tcp") > 0) mDNSfound = true;
  
  _tft->setCursor(5, 75);
  if(mDNSfound) _tft->print(MDNS.IP(0));
  else {
    _tft->setTextColor(ST77XX_RED);
    _tft->print("not found");
    _tft->setTextColor(ST77XX_WHITE); 
  }

  // Manual server, read from EEPROM
  for(int i = 0; i < 4; i++) {
    manualIP[i] = EEPROM.read(i);
    _tft->setCursor(5 + (SERVER_OCTET_SPACE * i), 125);
    if(manualIP[i] < 100) _tft->print("0");
    if(manualIP[i] < 10) _tft->print("0");
    _tft->print(manualIP[i]);
  }

  // init cursor
  if(mDNSfound) cursorPosition = 0;
  else cursorPosition = 1;
  redrawCursor(cursorPosition, false);
}

void Screen_Server::encoderTick(bool forward) {

  if(!inEditMode) {

    int oldCursorPosition = cursorPosition;
    
    if(forward) cursorPosition++;
    else cursorPosition--;
    if(cursorPosition == MAX_CURSOR_POSITION) cursorPosition = 0;
    if(cursorPosition < 0) cursorPosition = MAX_CURSOR_POSITION - 1;
    if(!mDNSfound && cursorPosition == 0) {
      if(oldCursorPosition == 1) cursorPosition = MAX_CURSOR_POSITION - 1;
      else cursorPosition = 1;
    }
    
    redrawCursor(oldCursorPosition, true);
    redrawCursor(cursorPosition, false);
  } 
  
  else {

    if(forward) manualIP[inEditOctet]++;
    else manualIP[inEditOctet]--;
    
    if(manualIP[inEditOctet] > 255) manualIP[inEditOctet] = 0;
    if(manualIP[inEditOctet] < 0) manualIP[inEditOctet] = 255;

    _tft->fillRect(5 + (SERVER_OCTET_SPACE * inEditOctet), 110, SERVER_OCTET_SPACE - 5, 20, ST77XX_BLACK);
    _tft->setCursor(5 + (SERVER_OCTET_SPACE * inEditOctet), 125);
    if(manualIP[inEditOctet] < 100) _tft->print("0");
    if(manualIP[inEditOctet] < 10) _tft->print("0");
    _tft->print(manualIP[inEditOctet]);    
  }
}

void Screen_Server::encoderClick() {

  if(!inEditMode) {

    if(cursorPosition < 2) {

  
      bool connectionOk;
      if(cursorPosition == 0) connectionOk = connectToServer(MDNS.IP(0));
      else connectionOk = connectToServer(IPAddress(manualIP[0], manualIP[1], manualIP[2], manualIP[3]));
      
      if (connectionOk) {

        if(FIXED_ADDRESS_MODE) {
          locoAddress = favLocos[0].locoAddress;
          locoAddressLength = favLocos[0].locoAddressLength;
          locoName = favLocos[0].locoName;          
          currentScreen = &screenLoco;
          redrawScreen = true;            
        
        } else {
          _tft->fillRect(0, 0, 240, 135, ST77XX_BLACK);
          currentScreen = &screenMenu;
          redrawScreen = true;         
        }
        
      } else {
          _tft->fillRect(30, 45, 180, 45, ST77XX_BLACK);
          _tft->drawRect(30, 45, 180, 45, ST77XX_WHITE);
          _tft->setFont(&FreeMonoBold12pt7b);
          _tft->setTextColor(ST77XX_RED);
          _tft->setCursor(85, 70);
          _tft->print("ERROR");
          delay(2000);
          redrawScreen = true;    
      }
    }
    else {
      inEditOctet = cursorPosition - 2;
      inEditMode = true;
      redrawCursor(cursorPosition, false);
    }
  }
  else {
    if(cursorPosition > 1) {
      EEPROM.write(inEditOctet, manualIP[inEditOctet]);
      EEPROM.commit();
    }
    inEditMode = false;
    redrawCursor(cursorPosition, false);
  }
}

void Screen_Server::redrawCursor(int cursorPosition, bool old) {

  if(old) _tft->setTextColor(ST77XX_WHITE);
  else _tft->setTextColor(ST77XX_YELLOW);
  
  if(inEditMode) _tft->setTextColor(ST77XX_RED);
  
  _tft->setFont(&FreeMonoBold12pt7b);
  
  if(cursorPosition == 0) {
    _tft->setCursor(5, 50);
    _tft->print("mDNS:");    
  } 
  
  else if(cursorPosition == 1) {
    _tft->setCursor(5, 100);
    _tft->print("Manual:");    
  } 
  
  else {
    _tft->setFont(&FreeMono12pt7b);
    _tft->setCursor(5 + (SERVER_OCTET_SPACE * (cursorPosition - 2)), 125);
    if(manualIP[cursorPosition - 2] < 100) _tft->print("0");
    if(manualIP[cursorPosition - 2] < 10) _tft->print("0");
    _tft->print(manualIP[cursorPosition - 2]);    
  }
}

bool Screen_Server::connectToServer(IPAddress ip) {
 
  if (client.connect(ip, SERVER_PORT, 5000)) {
    //_wiThrottle->setLogStream(&Serial);
    _wiThrottle->setDelegate(&rosterDelegate);
    _wiThrottle->connect(&client);
    _wiThrottle->setDeviceName(DEVICE_NAME);
    return true;
  }

  return false;
}
