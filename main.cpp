#include "headers/options.h"
#include "headers/black_scholes.h"

template<typename T>
void write_csv(const std::string& filename, const std::vector<T>& strikes, const std::vector<T>& data){
	std::ofstream file;
	file.open(filename);

	size_t n = strikes.size();
	assert(n == data.size() && "Warning! Strikes and data have different sizes");

	file << "STRIKE" << "," << "DATA" << "\n";

	for (int i=0;i<n;i++){
		file << strikes[i] << "," << data[i] << "\n";
	}

	file.close();
}

int main(){
	Date value_date = Date::create(2024,7,31);
	Date expiry_date = Date::create(2024,8,1);
	Calendar calendar = Calendar::noHolidays();

	std::string filename = "../../assets/NIFTY_put_data_1_aug_2024.csv";
	OptionChain* chain = OptionChain::createFromCsv(value_date,expiry_date,calendar,European,Put,filename);

	float S = 24950.0; // NIFTY value at the 2024-7-31
	float rate = 0.1; // Risk-free rate
	std::vector<float> strikes = chain->getStrikes();
	std::vector<float> implied_vols = BlackScholes::calibrate(chain,S,rate);
	write_csv("../../assets/implied_vols.csv",strikes,implied_vols);

	return 0;
}