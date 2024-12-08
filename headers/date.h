#ifndef __CppOptions__Date__
#define __CppOptions__Date__

/*
       This class implements Date as tuple of year,month and day.
       At the moment, it only works in the yy/mm/dd format. 
       Instances of the class can be create only thru factory methods. 
       For most of the methods implementation, refer to date.cpp
*/

#include <string>
#include <iostream>
#include <vector>

enum WeekDay{
       Sunday,
       Monday,
       Tuesday,
       Wednesday,
       Thursday,
       Friday,
       Saturday,
       UNKNOWN
};

enum Month{
       January, 
       Febraury,
       March,
       April, 
       May,
       June,
       July,
       August, 
       September,
       October,
       November,
       December
};

class Date{
private:
       int year;
       int month;
       int day;
       WeekDay weekday;

       //--------------------
       //Private Constructos: objects instance can be created only thru factory methods
       Date(int year, int month, int day) : year(year), month(month), day(day), weekday(UNKNOWN) {}
       Date(std::string datetime);

       //--------------------
       //The following methods initializes the weekday member. It's automatically called by the factory methods
       void initializeWeekDay();
public:
       //--------------------
       //Factory Methods
       static Date create(int y, int m, int d){
              Date factory(y,m,d);
              factory.initializeWeekDay();
              return factory;
       }

       static Date create(std::string datetime){
              Date factory(datetime);
              factory.initializeWeekDay();
              return factory;
       }

       //--------------------
       //Copy Constructor and Assignment Operator
       Date(const Date& p) : year(p.year), month(p.month), day(p.day), weekday(p.weekday) {}
       Date &operator=(const Date& p);

       bool isLeapYear() const;
       bool isWeekend() const;

       //--------------------
       // Methods for visualization
       std::string month_name() const;
       std::string day_name() const;
       void print(bool text = false);
       std::string to_string() const {
              std::string res = std::to_string(year)+"-"+std::to_string(month)+"-"+std::to_string(day);
              return res;
       }

       //--------------------
       // Methods for computation
       static int diff(const Date& p1, const Date& p2);
       int dayDifference(const Date& p, bool trading = true);
       void add(int days);
       void subtract(int days);
       inline static Date max(const Date& p1, const Date& p2);
       inline static Date min(const Date& p1, const Date& p2);
       static Date today();
       static Date tomorrow();

       //--------------------
       // Methods for importing and format change

       //--------------------
       // Operator overloading
       bool operator==(const Date& p) const;
       bool operator!=(const Date& p) const;
       Date &operator+(int days);
       Date &operator++();
       Date &operator-(int days);
       Date &operator--();
       bool operator>(const Date& p) const;
       bool operator<(const Date& p) const;
       friend std::ostream& operator<<(std::ostream& stream, const Date& date);

       ~Date() {}
};

#endif