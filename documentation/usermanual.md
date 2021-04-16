# User manual

## User controls

espThrottle has a slide switch, 5 buttons and a rotary encoder (with a pushbotton):

![](https://github.com/lucadentella/espThrottle/raw/main/images/use-00.png)

## Power on and server selection

Turn on espThrottle with the slide switch.

Choose the WiThrottle server you like to connect to.
espThrottle is able to **auto-detect** running servers which broadcast their IP address via mDNS (JMRI has this feature). Otherwise you can **manually** enter the server IP address:

![](https://github.com/lucadentella/espThrottle/raw/main/images/use-01.png)

By turning the rotary encoder, you can highlight (yellow) **mDNS**, **Manual** or one **octet** of the manual IP address.

If mDNS or Manual is selected and you press the encoder button, a **connection** to that server is attempted. 

If one octet is highlighted and you press the encoder button, you can **edit** the value (octet becomes red). Values can be increased/decreased by turning the rotary encoder and confirmed by pressing the encoder button again:

![](https://github.com/lucadentella/espThrottle/raw/main/images/use-02.png)

## Main menu

Using the main menu you can choose **which locomotive** the throttle will control:

 - by entering the address manually
 - by selecting the loco from a favourite list
 - by selecting the loco from [JMRI roster](https://github.com/lucadentella/espThrottle/tree/main/documentation/rostergroup.md)

You can also turn the **track power** ON / OFF.

![](https://github.com/lucadentella/espThrottle/raw/main/images/use-03.png)

One or more functions can be disabled by editing the [configuration file](https://github.com/lucadentella/espThrottle/tree/main/documentation/configuration.md). The throttle can also be put in "**fixed address** mode".

You can **open/close** the main menu pressing the corresponding button.

## Entering address

The loco address can be entered manually using the rotary encoder (similar to how the IP address is changed):

![](https://github.com/lucadentella/espThrottle/raw/main/images/use-04.png)

When editing the address:

 - press F0 to add 10
 - press F1 to add 100
 - press F2 to substract 10
 - press F3 to substract 100

## Loco control

In the locomotive screen:

![](https://github.com/lucadentella/espThrottle/raw/main/images/use-05.png)

 - press F0-F1-F2-F3 to turn ON/OFF the corresponding decoder **function**
 - press **MENU** + F0-F1-F2-F3 to turn ON/OFF function F4-F5-F6-F7
 - turn the rotary encoder to change the **speed**
 - press the encoder button to change the **direction**

## Battery charging

With the device **turned on**, connect a USB-C cable to the throttle.
A blue led will turn on and the battery icon on the display with change into "charging mode":

![](https://github.com/lucadentella/espThrottle/raw/main/images/use-06.png)