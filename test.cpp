#include <functional>
#include <limits>
#include <map>
#include <iostream>
#include <vector>
#include <stdio.h>



#include <string.h>
#include "gps_pattern.hpp"

int main() {
   gps_pattern pattern;
   //tm * p=pattern.convertTime(1211748272);
   //cout<<1900 + p->tm_year<<" "<<1 + p->tm_mon<<" "<<p->tm_mday<<" "<<p->tm_wday<<" "<<p->tm_hour<<endl;

    pattern.write();

   return 0;

}
