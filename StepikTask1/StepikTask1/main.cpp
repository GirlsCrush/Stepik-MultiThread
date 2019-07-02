#include <map>
#include <iostream>
#include <regex>


std::string derivative(std::string polynomial) {
	std::cmatch result;
	std::regex reg("([-+]?)([\\d]*)([\\*]?)([x]?)([\\^]?)([\\d]*)");
	std::map<size_t, int> coefs;

	while (std::regex_search(polynomial.c_str(), result, reg) && polynomial != "")
	{
		if (result[4] != "") {
			int coef = 1;
			if (result[1] == "-")
				coef = -1;
			if (result[2] != "")
				coef *= std::stoi(result[2]);
			size_t power = 0;
			if (result[5] != "") {
				power = std::stoi(result[6]);
				coef *= power--;
			}
			coefs[power] += coef;
		}
		polynomial = result.suffix();
	}
	std::string res;
	for (auto it = coefs.rbegin(); it != coefs.rend() && it->first != 1 && it->first != 0; ++it)
		res += (it->second > 0 ? "+" : "") + std::to_string(it->second) + "*x^" + std::to_string(it->first);

	if (coefs.count(1) != 0 && coefs[1] != 0)
		res += (coefs[1] > 0 ? "+" : "") + std::to_string(coefs[1]) + "*x";
	if (coefs.count(0) != 0 && coefs[0] != 0)
		res += (coefs[0] > 0 ? "+" : "") + std::to_string(coefs[0]);
	if (res.front() == '+')
		res.erase(0, 1);

	return res;
}

int main(char** argv, int argc) {
	std::cout << derivative("-3*x^2-2*x") << std::endl;
	return 0;
}
