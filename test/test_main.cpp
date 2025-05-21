#include "gtest/gtest.h"
#include "util/get_exchange_rate.hpp"
#include "util/currencies_enum.hpp"
#include "util/convert_currency.hpp"
#include "util/convert_unit.hpp"
#include "util/strip_and_to_lower.hpp"
#include "util/process_input_original.hpp"
#include "util/process_input_target.hpp"

TEST(units, convert_unit)
{
	EXPECT_EQ(units::convert_unit(1, units::kg, units::g), 1000);
	EXPECT_EQ(units::convert_unit(1, units::kg, units::jin), 2);
}

TEST(util, strip_and_to_lower)
{
	std::string str = " Hello World! ";
	util::strip_and_to_lower(str);
	EXPECT_EQ(str, "hello world!");
	str = "kahf AOLH KJXKZ ";
	util::strip_and_to_lower(str);
	EXPECT_EQ(str, "kahf aolh kjxkz");
}

TEST(user_interaction, process_input_original)
{
	currencies::currencies currency;
	units::units unit;
	double price, weight;
	user_interaction::process_input("4.99usd/lb", price, currency, weight, unit);
	EXPECT_EQ(price, 4.99);
	EXPECT_EQ(currency, currencies::usd);
	EXPECT_EQ(weight, 1);
	EXPECT_EQ(unit, units::lb);
	user_interaction::process_input("4.99uSd/1lb", price, currency, weight, unit);
	EXPECT_EQ(price, 4.99);
	EXPECT_EQ(currency, currencies::usd);
	EXPECT_EQ(weight, 1);
	EXPECT_EQ(unit, units::lb);
	user_interaction::process_input("4.99usd/1.0lB", price, currency, weight, unit);
	EXPECT_EQ(price, 4.99);
	EXPECT_EQ(currency, currencies::usd);
	EXPECT_EQ(weight, 1);
	EXPECT_EQ(unit, units::lb);
}

TEST(user_interaction, process_input_target)
{
	currencies::currencies currency;
	units::units unit;
	user_interaction::process_input("usd/lb", currency, unit);
	EXPECT_EQ(currency, currencies::usd);
	EXPECT_EQ(unit, units::lb);
	user_interaction::process_input("cny/kg", currency, unit);
	EXPECT_EQ(currency, currencies::cny);
	EXPECT_EQ(unit, units::kg);
}

// test to ensure get_exchange_rate parses the data correctly
TEST(get_exchange_rate, get_exchange_rate_us_to_eur)
{
 	// mock lambda function to pass into the get_exchange_rate function
	auto mock_fetcher = [](const std::string& from_currency) -> std::string {
		return R"({
            "usd": {
                "eur": 0.91,
            }
        })";
	};

	currencies::currencies from = currencies::usd;
	currencies::currencies to = currencies::eur;

	double rate = get_exchange_rate(from, to, mock_fetcher);

	EXPECT_NEAR(rate, 0.91, 0.0001);
}

TEST(get_exchange_rate, get_exchange_rate_jpy_to_usd)
{
	auto mock_fetcher = [](const std::string& from_currency) -> std::string {
		return R"({
            "jpy": {
                "usd": 0.0012
            }
        })";
	};

	currencies::currencies from = currencies::jpy;
	currencies::currencies to = currencies::usd;

	double rate = get_exchange_rate(from, to, mock_fetcher);

	EXPECT_NEAR(rate, 0.0012, 0.0001);
}

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}