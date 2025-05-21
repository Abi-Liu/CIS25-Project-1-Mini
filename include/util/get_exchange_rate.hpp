#pragma once

#include <string>
#include <functional>

#include "currencies_enum.hpp"	// For currencies::currencies

// Namespace for everything related to the currencies
namespace currencies
{
	// Get the exchange rate between two currencies by calling an API, an example use of advanced features
	double get_exchange_rate(const currencies from, const currencies to, const std::function<std::string(const std::string&)>& fetcher);

    // we can break this out to another hpp and cpp file, but I'm putting it here for now
    std::string fetch_exchange_rate(const std::string& from_currency);
}