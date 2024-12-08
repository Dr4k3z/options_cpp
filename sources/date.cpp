#include "../headers/date.h"
#include "../headers/rapidcsv.h"

#include <iostream>
#include <algorithm> 
#include <chrono>
#include <ctime>
#include <iomanip>
#include <ostream>

Date::Date(std::string datetime){
       std::stringstream ss(datetime);
       std::string token;

       // Extract year
       std::getline(ss, token, '/');
       year = std::stoi(token);

       // Extract month
       std::getline(ss, token, '/');
       month = std::stoi(token);

       // Extract day
       std::getline(ss, token, '/');
       day = std::stoi(token);

       // How to manage possibile errors in reading dates?
       if (day > 31 || month > 12 || year < 0){
              throw std::runtime_error("Error reading dates from string!");
       }

       weekday = UNKNOWN;
}

int Date::diff(const Date& p1, const Date& p2){
       int year_diff = abs(p1.year - p2.year);
       int month_diff = abs(p1.month - p2.month);
       int day_diff = abs(p1.day - p2.day);
       return year_diff*365+month_diff*30+day_diff;
}

int Date::dayDifference(const Date& p, bool trading){
       return diff(*this,p);
}

bool Date::isLeapYear() const{
       return (year % 4 == 0) && ((year % 100 != 0) || (year % 400 == 0));
}

bool Date::isWeekend() const{
       //if (weekday==UNKNOWN){ initializeWeekDay(); } this is no longer required

       if (weekday==Sunday || weekday==Saturday){
              return true;
       }
       return false;
}

// This function only works with dates after 2018
void Date::initializeWeekDay(){
       int d = 1;
       Date begin(2018,1,1); // this date was a monday

       //std::cout << "Initializationg of weekday..." << std::endl;

       for (;begin < *this; ++begin){
              if (d == 6){ d = 0; }
              else { d++; }
              //begin.print();
       }
       weekday = static_cast<WeekDay>(d);
}

Date &Date::operator++(){
       // list of months with 31 days
       std::vector<int> months31 = {1,3,5,7,8,10,12};

       if (day == 31){
              day = 1;
              month++;
       }else if (day == 30 && std::find(months31.begin(), months31.end(), month)==months31.end()){
              day = 1;
              month++;
       }else if (day == 29 && month == 2){
              day = 1;
              month++;
       }else if (day == 28 && month == 2 && !isLeapYear()){
              day = 1; 
              month++;
       }else{
              day++;
       }

       if (month>12){
              month = 1;
              year++;
       }

       if (weekday != UNKNOWN){
              if (weekday == Saturday){
                     weekday = Sunday;
              }else{
                     weekday = static_cast<WeekDay>(weekday+1);
              }
       }
       return *this;
}

Date &Date::operator=(const Date& p){
       if (&p != this){
              year = p.year;
              month = p.month;
              day = p.day;
              weekday = p.weekday;
       }
       return *this;
}

bool Date::operator<(const Date& p) const{
       if (year < p.year){ return true; }
       if (year == p.year && month < p.month){ return true; }
       if (year == p.year && month == p.month && day < p.day){ return true; }
       return false;
}

bool Date::operator>(const Date& p) const{
       if (*this < p){ return false; }
       else { return true; }
}

bool Date::operator==(const Date& p) const{
       if (p.year == year && p.month == month && p.day == day){
              return true;
       }
       return false;
}

bool Date::operator!=(const Date& p) const{
       return !(*this == p);
}

std::ostream& operator<<(std::ostream& stream, const Date& date){
       stream << date.year << "/" << date.month << "/" << date.day;
       return stream;
}

Month int2Month(int i){
       switch (i){
              case 1: return January;
              case 2: return Febraury;
              case 3: return March;
              case 4: return April;
              case 5: return May;
              case 6: return June;
              case 7: return July;
              case 8: return August;
              case 9: return September;
              case 10: return October;
              case 11: return November;
              case 12: return December;
              default: throw std::runtime_error("int2month unknown month");
       }
}

std::string Date::month_name() const{
       Month m = int2Month(month);
       switch (m){
              case January: return "January";
              case Febraury: return "Febraury";       
              case March: return "March";
              case April: return "April";
              case May: return "May";
              case June: return "June";
              case July: return "July";
              case August: return "August";
              case September: return "September";
              case October: return "October";
              case November: return "November";
              case December: return "December";
              default: throw std::runtime_error("unknown month");
       }
}

std::string Date::day_name() const{
       //WeekDay d = int2Day(day);
       switch (weekday){
              case Sunday: return "Sunday";
              case Monday: return "Monday";
              case Tuesday: return "Tuesday";
              case Wednesday: return "Wednesday";
              case Thursday: return "Thursday";
              case Friday: return "Friday";
              case Saturday: return "Saturday";
              case UNKNOWN: return "fuck";
              default: throw std::runtime_error("unknown day");
       }
}

void Date::print(bool format){
       if (format){
              std::cout << day_name() << " " << day << "th " << month_name() << " " << year << std::endl;
       }else{
              std::cout << year << "/" << month << "/" << day << std::endl;
       }
}

// Not very elegant I know
Date Date::max(const Date& p1, const Date& p2){
       if (p1 > p2){
              return p1;
       }else{
              return p2;
       }
}

Date Date::min(const Date& p1, const Date& p2){
       if (p1 < p2){
              return p1;
       }else{
              return p2;
       }
}

Date Date::today(){
       auto now = std::chrono::system_clock::now();
       std::time_t now_time = std::chrono::system_clock::to_time_t(now);
       std::tm* local_time = std::localtime(&now_time);
       std::ostringstream oss;
       oss << std::put_time(local_time, "%Y/%m/%d");
       return Date::create(oss.str());
}      