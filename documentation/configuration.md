# Configuration :gear:

espThrottle behavior can be configured by editing the **config.h** file:
![](https://github.com/lucadentella/espThrottle/raw/main/images/config-h.png)

## WiThrottle parameters

    #define DEVICE_NAME   "espThrottle"
    #define SERVER_PORT   12090
You can change the **device name** (i.e. the name that identifies this throttle in JMRI) and the **TCP port** the server is listening on.

## Functions

    #define ENTER_ADDRESS_ENABLED   true
    #define FAVOURITE_LIST_ENABLED  true
    #define ROSTER_LIST_ENABLED     true
    #define POWER_CONTROL_ENABLED   true
Each menu **function** can be enabled / disabled independently. 

## Fixed address mode

    #define FIXED_ADDRESS_MODE      false
The throttle can be configured in **fixed address** mode: all the menu functions (see above) are automatically disabled and the throttle controls the first locomotive of the *favourite list*.

## Speed when changing direction

    #define CHANGE_DIR_SPEED_ZERO   true
If true, when changing direction the speed is automatically set to zero. If not, the current speed is maintained.

## Favourite list

    #define FAV_LIST_ITEMS  2
    loco favLocos[FAV_LIST_ITEMS] {
      {"D345", 1234, 'L'},
      {"E412", 3, 'S'},
    };

A list of favourite locos can be configured. The list will be available in the throttle menu (if FAVOURITE_LIST_ENABLED is true, see above).

You can add as many entries as you like:

 - update the **number of entries** (FAV_LIST_ITEMS)
 - each entry contains the descriptive **name**, the DCC **address** and the address **format** (**S** for *short*, **L** for *long*).

 
