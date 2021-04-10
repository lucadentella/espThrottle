#define VERSION "1.1"

// Loco struct
typedef struct {
  String locoName;
  int locoAddress;
  char locoAddressLength;
} loco;

#include <EEPROM.h>               // EEPROM to store parameters (server IP...)
#include <Adafruit_GFX.h>         // TFT display library (GFX + HAL + Font)
#include <Adafruit_ST7789.h>
#include <Fonts/FreeMono9pt7b.h>
#include <Fonts/FreeMono12pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <SPI.h>
#include <ESP32Encoder.h>         // Encoder library
#include <WiFi.h>                 // WiFi, mDNS and WiThrottle
#include <ESPmDNS.h>
#include <WiThrottle.h>
#include <esp_adc_cal.h>          // ADC calibration
#include "WiFiConnect.h"          // WiFi manager

#include "Config.h"
#include "Screens.h"
#include "RosterDelegate.h"
#include "Icons.h"

// Display
#define TFT_MOSI      19
#define TFT_SCLK      18
#define TFT_CS        5
#define TFT_RST       23
#define TFT_DC        16
#define TFT_BL        4
#define TFT_HEIGHT    135
#define TFT_WIDTH     240
#define TFT_ROTATION  3

// Encoder
#define ENCODER_A     36
#define ENCODER_B     37
#define ENCODER_BTN   27

// Buttons
#define BTN_F0        12
#define BTN_F1        13
#define BTN_F2        15
#define BTN_F3        17
#define BTN_MENU      2

// Battery monitor
#define BATTERY_PIN            34
#define BATTERY_CHECK_INTERVAL 2000

// EEPROM
#define EEPROM_SIZE   10

// Global objects
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
ESP32Encoder encoder;
WiFiClient client;
WiThrottle wiThrottle;
RosterDelegate rosterDelegate;
WiFiConnect wc;

// Screens
Screen_Loco screenLoco;
Screen_Menu screenMenu;
Screen_ChangeAddr screenChangeAddr;
Screen_List screenList;
Screen_Server screenServer;
Screen* currentScreen;
bool redrawScreen;

// Screen positions
#define MENU_BOX_WIDTH          220
#define MENU_BOX_HEIGHT         120
#define ST77XX_GRAY             0xAD55

// Global variables
bool connected = false;

int vref = 1100;
int64_t encoderCount;
bool encoderBtnPressed;

int fBtns[4] = {BTN_F0, BTN_F1, BTN_F2, BTN_F3};
bool fBtnsPressed[4];
bool menuBtnPressed;
bool discardMenuBtn;

// Track info
TrackPower trackPower = PowerUnknown;

// Selected loco
int locoAddress = 1;
char locoAddressLength = 'S';
String locoName = "";
int locoSpeed;
Direction locoDirection;
bool locoFunctions[8];
