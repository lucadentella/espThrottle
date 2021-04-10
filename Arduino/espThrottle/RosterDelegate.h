#define MAX_ROSTER_ENTRIES  10

class RosterDelegate : public WiThrottleDelegate {
  
  public:
    void receivedTrackPower(TrackPower state);
    void receivedRosterEntries(int rosterSize);
    void receivedRosterEntry(int index, String name, int address, char length);
    int getRosterLocosNum();
    loco* getRosterLocos();
    

  private:
    loco rosterLocos[MAX_ROSTER_ENTRIES];
    int rosterLocosNum = 0;
};
