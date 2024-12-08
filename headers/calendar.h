#ifndef __CppOptions__Calendar__
#define __CppOptions__Calendar__

/*
       This class build a bit more structure around the Holidays object,
       which is basically a vector of Dates. It enables to count trading days
*/

#include "Date.h"
#include "rapidcsv.h"

class Calendar{
private:
       std::vector<Date> holidays;
       std::vector<Date> readFromCsv(const std::string& filename);
public:
       //--------------------
       //Configuration methods
       static Calendar createFromCsv(const std::string& finename);
       static Calendar noHolidays(){ return Calendar(); }
       inline void loadHolidaysFromCsv(const std::string& filename) { holidays = readFromCsv(filename); }
       void addHoliday(const Date& d);
       void removeHoliday(const std::string& holiday);

       //--------------------
       //Getters and Setters
       void setHolidays(const std::vector<Date>& p){ holidays = p; }
       inline std::vector<Date> getHolidays() const{ return holidays; }

       //--------------------
       //Binary Checking methods
       bool isHoliday(const Date& p) const{
              return std::find(holidays.begin(), holidays.end(), p)!=holidays.end();
       }

       bool isTradingDay(const Date& p) const{
              if (p.isWeekend() || isHoliday(p)){
                     return false;
              }else{
                     return true;
              }
       }

       //---------------------
       //Methods for computations and visualisation
       int tradingDays(const Date& p1, const Date& p2) const;
       void print() const;

       //---------------------
       //Operator overloading
       bool operator==(const Calendar& p) const;
       bool operator!=(const Calendar& p) const;
};

#endif