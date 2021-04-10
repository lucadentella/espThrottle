class Screen : public WiThrottleDelegate {

  public:
    virtual void init(Adafruit_ST7789 *tft, WiThrottle *wiThrottle);
    virtual void show();
    virtual void run();
    virtual void encoderTick(bool forward);
    virtual void encoderClick();
    virtual void fBtnChange(int fNum, bool pressed, bool withMenu);
    virtual void menuClick();

  protected:
    Adafruit_ST7789 *_tft;
    WiThrottle *_wiThrottle;
};

class Screen_Loco : public Screen {

  public:
    Screen_Loco();
    void init(Adafruit_ST7789 *tft, WiThrottle *wiThrottle);  
    void show();
    void run();
    void encoderTick(bool forward);
    void encoderClick();
    void fBtnChange(int fNum, bool pressed, bool withMenu);
    void menuClick();

    void receivedTrackPower(TrackPower state);
    void receivedSpeed(int speed);
    void receivedDirection(Direction dir);
    void receivedFunctionState(uint8_t func, bool state);

  private:
    String locoFullAddress;
    unsigned long previousBatteryCheck;
    int batteryLevel;
    void updateSpeed();
    void updateDirection();
    void updateFunction(int funcNumber);
    void updateTrackPower();
    void checkBattery(); 
};

class Screen_Menu : public Screen {

  public:
    Screen_Menu();
    void init(Adafruit_ST7789 *tft, WiThrottle *wiThrottle);  
    void show();
    void run() {}
    void encoderTick(bool forward);
    void encoderClick();
    void fBtnChange(int fNum, bool pressed, bool withMenu) {} 
    void menuClick();
    
  private:
    String menuItems[4] = {"Enter address", "Favourite list", "Roster list", "Toggle power"};
    int selectedMenuItem = 0;  
    int menuBoxX, menuBoxY;
    void displayMenuItems();   
};

class Screen_ChangeAddr : public Screen {

  public:
    Screen_ChangeAddr();
    void init(Adafruit_ST7789 *tft, WiThrottle *wiThrottle);  
    void show();
    void run() {}
    void encoderTick(bool forward);
    void encoderClick();
    void fBtnChange(int fNum, bool pressed, bool withMenu);
    void menuClick() {}

  private: 
    int menuBoxX, menuBoxY;
    bool inEditMode = false;
    int cursorPosition;
    void printAddress(bool old); 
    void printAddressLength(bool old); 
    void printOk(bool old); 
    void redrawCursor(int cursorPosition, bool old);
};

class Screen_List : public Screen {

  public:
    Screen_List();
    void init(Adafruit_ST7789 *tft, WiThrottle *wiThrottle);  
    void show();
    void run() {}
    void encoderTick(bool forward);
    void encoderClick();
    void fBtnChange(int fNum, bool pressed, bool withMenu) {}
    void menuClick();
    void setSource(int source);

  private:
    int menuBoxX, menuBoxY;
    int _source;
    String sourceTitle[3] = {"Favourite List", "Roster List", "Recent List"};
    loco* listItems;
    int numListItems;
    int selectedListItem;
    int listPage;
    bool pageChanged;
    void displayListItems();
};

class Screen_Server : public Screen {

  public:
    Screen_Server();
    void init(Adafruit_ST7789 *tft, WiThrottle *wiThrottle);  
    void show();
    void run() {}
    void encoderTick(bool forward);
    void encoderClick();
    void fBtnChange(int fNum, bool pressed, bool withMenu) {}
    void menuClick() {}
    
  private:
    bool mDNSfound = false;
    bool inEditMode = false;
    int cursorPosition;
    int manualIP[4];
    int inEditOctet;
    void redrawCursor(int cursorPosition, bool old);
    bool connectToServer(IPAddress ip); 
};
