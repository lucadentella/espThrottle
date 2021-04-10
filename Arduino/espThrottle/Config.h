// WiThrottle
#define DEVICE_NAME   "espThrottle"
#define SERVER_PORT   12090

// When changing direction, set speed to zero
#define CHANGE_DIR_SPEED_ZERO   true

// Enable/Disable functions
#define ENTER_ADDRESS_ENABLED   true
#define FAVOURITE_LIST_ENABLED  true
#define ROSTER_LIST_ENABLED     true
#define POWER_CONTROL_ENABLED   true

// Fixed address mode:
// uses the first address in the Favourite List
// and disables the menu
#define FIXED_ADDRESS_MODE      false

// Favourite list
#define FAV_LIST_ITEMS  2
loco favLocos[FAV_LIST_ITEMS] {
  {"D345", 1234, 'L'},
  {"E412", 3, 'S'},
};
