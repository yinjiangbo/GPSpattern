#ifndef GPS_INFO_HPP
#define GPS_INFO_HPP
#include <time.h>
#include <vector>
#include <iostream>

#include <queue>
using namespace std;

using std::vector;
struct week_data {
   long routeid;
   int year;
   int month;
   int day;
   int week;
   int hour;
   double vel;
};

struct median{
    vector<double> vel_arr;
    double medianVel;
};

struct route_meanVel{
    string routeid;
    double vel;
};

class gps_pattern
{
public:
      gps_pattern();
      tm * convertTime(long t);
      week_data convert2weekdata(long t, double vel);
      vector<week_data> loadGPS(const string& filename);
      void getmedian(vector<week_data> weekdatas, double arrays[][24]);
      void write();
      void generateXML();
};

#endif  // GPS_INFO_HPP
