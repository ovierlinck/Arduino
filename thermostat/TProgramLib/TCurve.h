#ifndef TCurve_h
#define TCurve_h

const char TCurve_MAX_POINTS = 10;
const char TCurve_LENGTH_NAME = 32;

const char TCurve_NO_SETPOINT = 126;
const char TCurve_SETPOINT_OFF = 0;

typedef struct {
  unsigned char temp;
  unsigned char h;
  unsigned char m;
} CurvePoint_t;

class TCurve
{

  public:
    static CurvePoint_t newCurvePoint(unsigned char temp, unsigned char h, unsigned char m);

    TCurve();
    TCurve(const char* name);

    // Editing

    void setName(const char* name);
    /*Must be added after last one*/
    void addPoint(unsigned char t, unsigned char h, unsigned char m);
    void addPoint(CurvePoint_t point);
    void removeAllPoints();

    void setWeekPattern(unsigned char pattern);
    void setApplicableForDay(unsigned char day, bool applicable);

    // Querying

    const char* getName();
    unsigned char getNbPoints();
    CurvePoint_t& getPoint(unsigned char index);

    unsigned char getWeekPattern();
    bool isApplicableForDay(unsigned char day);

    /* TCurve_SETPOINT_OFF=0 if setpoint for OFF - TCurve_NO_SETPOINT=127 if NO setpoint */
    unsigned char getSetpoint(unsigned char h, unsigned char m);

    // Dump

    const char* dumpCurvePoint(CurvePoint_t& p);  // don't deallocate returned string
    const char* dump(); // don't deallocate returned string

  private:

    bool before(CurvePoint_t& p1, CurvePoint_t& p2);
    bool before(unsigned char h, unsigned char m, CurvePoint_t& p);

    int minutes(CurvePoint_t& p);
    int minutes(unsigned char h, unsigned char m);

    char _name[TCurve_LENGTH_NAME];
    CurvePoint_t _points[TCurve_MAX_POINTS];
    unsigned char _nbPoints = 0;
    unsigned char _weekPattern = 0;

};

#endif

