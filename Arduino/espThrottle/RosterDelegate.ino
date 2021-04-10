void RosterDelegate::receivedTrackPower(TrackPower state) {

  trackPower = state;
}

void RosterDelegate::receivedRosterEntries(int rosterSize) {     

  if(rosterSize > MAX_ROSTER_ENTRIES) rosterLocosNum = MAX_ROSTER_ENTRIES;
  else rosterLocosNum = rosterSize;
}
  
void RosterDelegate::receivedRosterEntry(int index, String name, int address, char length) {

  if(index < MAX_ROSTER_ENTRIES) {
    rosterLocos[index].locoName = name;
    rosterLocos[index].locoAddress = address;
    rosterLocos[index].locoAddressLength = length;
  }
}

int RosterDelegate::getRosterLocosNum() {

  return rosterLocosNum;
}

loco* RosterDelegate::getRosterLocos() {

  return rosterLocos;
}
