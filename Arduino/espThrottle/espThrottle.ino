#include "espThrottle.h"

void setup() {

  Serial.begin(115200);
  Serial.print("espThrottle v");
  Serial.println(VERSION);
  Serial.println();
  
  tft.init(TFT_HEIGHT, TFT_WIDTH);
  tft.setRotation(TFT_ROTATION);
  tft.setTextWrap(false);
  tft.setFont(&FreeMono9pt7b);
  tft.fillScreen(ST77XX_BLACK);
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  Serial.println("Display init complete");

  encoder.attachSingleEdge(ENCODER_B, ENCODER_A);
  encoderCount = 0;
  encoder.setCount(encoderCount);
  pinMode(ENCODER_BTN, INPUT_PULLUP);
  encoderBtnPressed = false;
  Serial.println("Encoder init complete");

  initBatteryADC();
  Serial.print("ADC init complete, Vref: "); Serial.println(vref);

  for(int i = 0; i < 4; i++) 
    pinMode(fBtns[i], INPUT_PULLUP);
  pinMode(BTN_MENU, INPUT_PULLUP);
  Serial.println("Buttons init complete");

  EEPROM.begin(EEPROM_SIZE);
  Serial.println("EEPROM init complete");

  screenLoco.init(&tft, &wiThrottle);
  screenMenu.init(&tft, &wiThrottle);
  screenChangeAddr.init(&tft, &wiThrottle);
  screenList.init(&tft, &wiThrottle);
  screenServer.init(&tft, &wiThrottle);  
  currentScreen = &screenServer;
  redrawScreen = true;
  Serial.println("Screens init complete");

  wc.setAPName("espThrottle-WiFi");
  wc.setConnectionTimeoutSecs(5);
  wc.setAPCallback(configModeCallback);  
  Serial.println("WiFiManager init complete");
  Serial.println();
  
  if (!wc.autoConnect()) {
      wc.startConfigurationPortal(AP_LOOP);
  }
  Serial.println("Connected to WiFi");
}

void loop() {

  // screen refresh required
  if(redrawScreen) {
    currentScreen->show();
    redrawScreen = false;
  }

  currentScreen->run();
  wiThrottle.check();

  // check encoder position
  int64_t newEncoderCount = encoder.getCount(); 
  if(newEncoderCount != encoderCount) {
    bool forward = (newEncoderCount > encoderCount) ? true : false;
    currentScreen->encoderTick(forward);
    encoderCount = newEncoderCount;
  }

  // check encoder button
  int encoderButtonStatus = digitalRead(ENCODER_BTN);
  if(encoderButtonStatus == HIGH) {
    if(encoderBtnPressed) {
      currentScreen->encoderClick();
    }
    encoderBtnPressed = false;
  }
  else encoderBtnPressed = true;

  // check fButton 
  int buttonMenuStatus = digitalRead(BTN_MENU);
  
  for(int i = 0; i < 4; i++) {
    
    int fButtonStatus = digitalRead(fBtns[i]);
    
    // button pressed
    if(fButtonStatus == LOW) {
      
      // button wasn't already pressed
      if(!fBtnsPressed[i]) {
        currentScreen->fBtnChange(i, true, !buttonMenuStatus);
        fBtnsPressed[i] = true;
        discardMenuBtn = true;
      }      
    }
    
    // button released
    else {

        // button was pressed
      if(fBtnsPressed[i]) {
        currentScreen->fBtnChange(i, false, !buttonMenuStatus);
        fBtnsPressed[i] = false;
       }
    }
  }

  // check menu button
  if(buttonMenuStatus == HIGH && menuBtnPressed) {
    if(!discardMenuBtn) currentScreen->menuClick();
    else discardMenuBtn = false;
    menuBtnPressed = false;
  }
  if(buttonMenuStatus == LOW && !menuBtnPressed) menuBtnPressed = true;
}

void configModeCallback(WiFiConnect *mWiFiConnect) {
  
  Serial.println("No WiFi configured or available, entering AP mode...");
  tft.drawBitmap(70, 17, wifisetup, 100, 100, ST77XX_WHITE);
}

void initBatteryADC() {
  
  esp_adc_cal_characteristics_t adc_chars;
  esp_adc_cal_value_t val_type = esp_adc_cal_characterize((adc_unit_t)ADC_UNIT_1, (adc_atten_t)ADC1_CHANNEL_6, (adc_bits_width_t)ADC_WIDTH_BIT_12, 1100, &adc_chars);
  if(val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) vref = adc_chars.vref;
}
