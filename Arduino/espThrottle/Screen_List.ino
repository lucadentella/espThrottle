#define ITEM_X            20
#define FIRST_ITEM_Y      35
#define LIST_ITEM_SPACE   25
#define SHOW_NUM_ITEMS    3

#define SOURCE_FAVOURITE  0
#define SOURCE_ROSTER     1
#define SOURCE_RECENT     2

Screen_List::Screen_List() {}

void Screen_List::init(Adafruit_ST7789 *tft, WiThrottle *wiThrottle) {

  _tft = tft;
  _wiThrottle = wiThrottle;
  menuBoxX = (TFT_WIDTH - MENU_BOX_WIDTH) / 2;
  menuBoxY = (TFT_HEIGHT - MENU_BOX_HEIGHT) / 2;  
}

void Screen_List::show() {

  // clear dialog area
  _tft->fillRect(menuBoxX + 1, menuBoxY + 1, MENU_BOX_WIDTH - 2, MENU_BOX_HEIGHT - 2, ST77XX_BLACK);

  // print screen title
  _tft->setCursor(menuBoxX + 10, menuBoxY + 20);
  _tft->setFont(&FreeMonoBold12pt7b);
  _tft->setTextColor(ST77XX_GREEN);
  _tft->print(sourceTitle[_source]);

  // reset list 
  selectedListItem = 0;
  listPage = 0;  

  // print list items
  displayListItems();
}

void Screen_List::encoderTick(bool forward) {

  if(forward) selectedListItem++;
  else selectedListItem--;

  if(selectedListItem == numListItems) selectedListItem = 0;
  else if(selectedListItem == -1) selectedListItem = numListItems - 1;

  int newListPage = selectedListItem / SHOW_NUM_ITEMS;
  if(newListPage != listPage) pageChanged = true;
  else pageChanged = false;
  listPage = newListPage;
  
  displayListItems();
}

void Screen_List::encoderClick() {

  locoName = listItems[selectedListItem].locoName;
  locoAddress = listItems[selectedListItem].locoAddress;
  locoAddressLength = listItems[selectedListItem].locoAddressLength;

  currentScreen = &screenLoco;
  redrawScreen = true;
}

void Screen_List::menuClick() {
  
  currentScreen = &screenMenu;
  redrawScreen = true;  

}

void Screen_List::setSource(int source) {

  _source = source;
  
  if(_source == SOURCE_FAVOURITE) {
    numListItems = FAV_LIST_ITEMS;
    listItems = favLocos;
  }

  else if(_source == SOURCE_ROSTER) {
    numListItems = rosterDelegate.getRosterLocosNum();
    listItems = rosterDelegate.getRosterLocos();
  }
}

void Screen_List::displayListItems() {

  // get start element based on page
  int startElement = listPage * SHOW_NUM_ITEMS;
  int endElement = (SHOW_NUM_ITEMS < numListItems - startElement) ? SHOW_NUM_ITEMS : numListItems - startElement;

  // clear list area and set font if page changed
  if(pageChanged) _tft->fillRect(menuBoxX + 1, FIRST_ITEM_Y, MENU_BOX_WIDTH - 2, MENU_BOX_HEIGHT - 2 - FIRST_ITEM_Y, ST77XX_BLACK);
  _tft->setFont(&FreeMono12pt7b);
  
  for(int i = 0; i < endElement; i++) {
    _tft->setCursor(ITEM_X, FIRST_ITEM_Y + LIST_ITEM_SPACE * (i + 1));
    if((i + startElement) == selectedListItem) _tft->setTextColor(ST77XX_YELLOW);
    else _tft->setTextColor(ST77XX_WHITE);
    _tft->print(listItems[startElement + i].locoName); 
  }
}
