#include "../headers/options.h"
#include "../headers/options_european.h"
#include "../headers/options_american.h"
#include <cassert>
#include <memory>

float Option::time2maturity(Units unit) const{
       int tmt = cal.tradingDays(value_date, expiry_date);
       switch (unit){
       case Days:
              return tmt;
       case Months:
              return tmt/30.0;
       case Years:
              return tmt/365.0;
       default:
              return cal.tradingDays(value_date, expiry_date);
       }
}

// Options chain methods

float string2float(std::string str){
       try{
              std::remove(str.begin(),str.end(),',');

              if (str == "-"){
                     return 0;
              }

              return std::stof(str);
       }catch (std::exception& e){
              std::cerr << e.what() << "\n";
              std::string errmsg = "Error with the following string:  "+str+"\n";
              throw std::runtime_error(errmsg);
       }
}

OptionChain* OptionChain::createFromCsv(const Date& value_date, const Date& expiry_date, const Calendar& cal, const OptionClass option_class,const OptionType type, const std::string& filename){
       OptionChain* chain = new OptionChain(value_date, expiry_date, cal, option_class, type);

       try{
              rapidcsv::Document doc(filename);
              std::vector<std::string> strikes = doc.GetColumn<std::string>("STRIKE");
              std::vector<std::string> prices = doc.GetColumn<std::string>("LTP"); // last traded price
              std::vector<std::string> bids = doc.GetColumn<std::string>("BID");
              std::vector<std::string> asks = doc.GetColumn<std::string>("ASK");

              size_t n = strikes.size();
              assert(n == bids.size() && "Warning! bids size is different from strikes");
              assert(n == asks.size() && "Warning! asks size is different from strikes");

              for (int i=0;i<n;i++){
                     float K = string2float(strikes[i]);
                     float p = string2float(prices[i]);
                     float b = string2float(bids[i]);
                     float a = string2float(asks[i]);

                     // I dont really like this part. Okay new there shouldn't be any memory leak, but still...
                     /*std::unique_ptr<Option> option;
                     if (type == Call){
                            option = std::make_unique<EuropeanCallOption>(K, expiry_date, cal);
                     }else{
                            option = std::make_unique<EuropeanPutOption>(K, expiry_date, cal);
                     }*/


                     // Remove nested ifs
                     std::unique_ptr<Option> option;
                     if (option_class == European){
                            if (type == Call){
                                   option = std::make_unique<EuropeanCallOption>(K, expiry_date, cal);
                            }else{
                                   option = std::make_unique<EuropeanPutOption>(K, expiry_date, cal);
                            }
                     }else if (option_class == American){
                            if (type == Call){
                                   option = std::make_unique<AmericanCallOption>(K, expiry_date, cal);
                            }else{
                                   option = std::make_unique<AmericanPutOption>(K, expiry_date, cal);
                            }
                     }

                     option->setValueDate(value_date); // I dont like this
                     option->setPrice(p); option->setBidPrice(b); option->setAskPrice(a);

                     chain->options.push_back(std::move(option)); 
              }

              return chain;
       }catch (std::exception& e){
              std::cerr << "Error reading csv file: " << e.what() << "\n";
              throw std::runtime_error("Check if the csv file actaully exists");
       }
}

void OptionChain::addOption(std::unique_ptr<Option> option){
       // check for expiry_date, calendar and type
       if (option->getExpiryDate() != expiry_date){
              throw std::invalid_argument("Option expiry date does not match chain expiry date");
       } 
       if (option->getCalendar() != cal){
              throw std::invalid_argument("Option calendar does not match chain calendar");
       }
       if (option->getClass() != option_class){
              throw std::invalid_argument("Option class does not match chain class");
       }
       if (option->getType() != type){
              throw std::invalid_argument("Option type does not match chain type");
       }

       options.push_back(std::move(option));
}

void OptionChain::print(){      
       std::string type_name;
       if (type == Call){
              type_name = "Call";
       }else{
              type_name = "Put";
       }

       std::cout << "Option Chain " << type_name << " - Expiry date: " << expiry_date << "\n"; 

       for (const auto& option : options){
              std::cout << "Bid: " << option->getBidPrice() << "  Ask: " << option->getAskPrice() << "  Strike: " << option->getStrike() << "\n";
       }
}

std::vector<float> OptionChain::getStrikes() const{
       size_t n = options.size();
       std::vector<float> strikes(n);

       for (int i=0;i<n;i++){
              strikes[i] = options[i]->getStrike();
       }

       return strikes;
}