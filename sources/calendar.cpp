#include "../headers/calendar.h"
#include <stdexcept>

std::vector<Date> Calendar::readFromCsv(const std::string& filename){
       try {
              rapidcsv::Document doc(filename);
              std::vector<std::string> col = doc.GetColumn<std::string>("Date");

              std::vector<Date> res;

              for (std::string str : col){
                     Date d = Date::create(str);
                     //d.print(true);
                     res.push_back(d);
              }

              if (res.size() == 0){
                     std::cout << "Warning! No holidays were read from the csv file\n";
              }
              return res;
       }catch (const std::exception& e) {
              std::cerr << e.what() << "\n";
              throw std::runtime_error("Check if the csv file actaully exists");
       }
}

Calendar Calendar::createFromCsv(const std::string& filename){
       Calendar cal;
       cal.holidays = cal.readFromCsv(filename);
       return cal;
}

// We assume p2 > p1
int Calendar::tradingDays(const Date& p1, const Date& p2) const{
       Date p = p1; //ugly ugly ugly
       if (p1 == p2){
              std::cout << "The are the same date!" << std::endl;
              return 0;
       }else if (p1 > p2){
              throw std::runtime_error("The time to maturity is negative!");
       }

       int counter=1;
       for (;p<p2;++p){
              if (isTradingDay(p)){ counter++; }
       }

       // This must be fixed better, currently sucks
       if (!isTradingDay(p2)){ counter--; }

       return counter;
}

void Calendar::print() const{
       for(Date d : holidays){
              d.print();
              std::cout << std::endl;
       }
}

bool Calendar::operator==(const Calendar& p) const{
       size_t n = holidays.size();
       assert(n == p.holidays.size() && "Warning! calendars have different number of holidays");

       for (int i=0;i<n;i++){
              if (holidays[i] != p.holidays[i]){
                     return false;
              }
       }
       return true;
}

bool Calendar::operator!=(const Calendar& p) const{
       return !(*this == p);
}