#include <string>	// For string, 
#include <stdexcept>// For runtime_error
#include <vector> // for vector
#include <functional>

#ifdef _WIN32 // Windows MSVC
#include <windows.h>// For _popen, _pclose
#define popen _popen
#define pclose _pclose
#else // Linux, MacOS
#include <stdio.h>	// For popen, pclose
#endif

#include "util/currencies_enum.hpp"	// For currencies::currencies
#include "util/currencies_names.hpp"	// For currencies::names
#include "util/get_exchange_rate.hpp"

using namespace std;

// Namespace for everything related to the currencies
namespace currencies
{
	// Get the exchange rate between two currencies by calling an API, an example use of advanced features
	// refactored to use dependency injection to enable testing
	double get_exchange_rate(const currencies from, const currencies to, const function<string(const string&)>& fetcher)
	{
		// Example use of static_cast
		string from_currency = names[static_cast<int>(from)];
		string to_currency = names[static_cast<int>(to)];

        string res = fetcher(from_currency);

		if (res.length() == 0)
		{
			throw runtime_error("Error: Failed to fetch exchange rate, please check your internet connection");
		}

		try
		{
			size_t s = res.find("\"" + to_currency + "\":");// Find the start of name/value pairs
			s += to_currency.length() + 3;					// Skip the name
			size_t e = res.find(",", s);					// Find the end of the name/value pairs
			return stod(res.substr(s, e - s));				// Convert the value to double
		}
		catch (exception)
		{
			throw runtime_error("Error: Failed to process fetched exchange rate");
		}
	}

    string fetch_exchange_rate(const string& from_currency) {
		std::string cmd = "curl -s \"https://latest.currency-api.pages.dev/v1/currencies/" + from_currency + ".min.json\"";
		FILE* pipe = popen(cmd.c_str(), "r");
		if (!pipe)
			throw runtime_error("Error: Failed to execute curl command, please make sure curl is installed and added to PATH");

		vector<char> buf(1024);
		string res;
		while (fgets(buf.data(), static_cast<int>(buf.size()), pipe) != nullptr)
		{
			res += buf.data();
		}
		pclose(pipe);

		if (res.empty())
			throw runtime_error("Error: Failed to fetch exchange rate, please check your internet connection");

		return res;
    }
}