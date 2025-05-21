#include "util/currencies_enum.hpp"	// For currencies::currencies
#include "util/get_exchange_rate.hpp"// For get_exchange_rate and fetch_exchange_rate

// Namespace for everything related to the currencies
namespace currencies
{
	// Convert one currency to another, an example use of default argument
	double convert_currency(const double value, const currencies from, const currencies to)
	{
		return value * get_exchange_rate(from, to, fetch_exchange_rate);
	}
}