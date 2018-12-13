#ifndef TCurve_h
#define TCurve_h

const char TCurve_MAX_POINTS = 10;
const char TCurve_LENGTH_NAME = 32;

const char TCurve_NO_SETPOINT = 126;
const char TCurve_SETPOINT_OFF = 0;

typedef struct {
  char temp;
  char h;
  char m;
} CurvePoint_t;

class TCurve
{

  public:
    static CurvePoint_t newCurvePoint(char temp, char h, char m);

    TCurve();
    TCurve(const char* name);

    // Editing

    void setName(const char* name);
    /*Must be added after last one*/
    void addPoint(char t, char h, char m);
    void addPoint(CurvePoint_t point);
    void removeAllPoints();

    void setWeekPattern(char pattern);
    void setApplicableForDay(char day, bool applicable);

    // Querying

    const char* getName();
    char getNbPoints();
    CurvePoint_t& getPoint(char index);

    char getWeekPattern();
    bool isApplicableForDay(char day);

    /* TCurve_SETPOINT_OFF=0 if setpoint for OFF - TCurve_NO_SETPOINT=127 if NO setpoint */
    char getSetpoint(char h, char m);

    // Dump

    const char* dumpCurvePoint(CurvePoint_t& p);  // don't deallocate returned string
    const char* dump(); // don't deallocate returned string

  private:

    bool before(CurvePoint_t& p1, CurvePoint_t& p2);
    bool before(char h, char m, CurvePoint_t& p);

    int minutes(CurvePoint_t& p);
    int minutes(char h, char m);

    char _name[TCurve_LENGTH_NAME];
    CurvePoint_t _points[TCurve_MAX_POINTS];
    char _nbPoints = 0;
    char _weekPattern = 0;

};

#endif

