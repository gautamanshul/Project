// MarketDataAnalysis.cpp : Defines the entry point for the console application.
//
#include "OrderFactory.h"

#include "stdafx.h"
#include "ExchangeContext.h"

TEST(DatabaseTests, IsSingletonTest)
{
	auto& db = SingletonDatabase::get();
	auto& db2 = SingletonDatabase::get();
	ASSERT_EQ(1, db.instance_count);
	ASSERT_EQ(1, db2.instance_count);
}

TEST(RecordFinderTests, SingletonTotalPopulationTest)
{
	SingletonRecordFinder rf;
	std::vector<std::string> names{ "Seoul", "Mexico City" };
	int tp = rf.total_population(names);
	EXPECT_EQ(17500000 + 17400000, tp);
}

TEST(RecordFinderTests, DependantTotalPopulationTest)
{
	DummyDatabase db{};
	ConfigurableRecordFinder rf{ db };
	EXPECT_EQ(4, rf.total_population(
		std::vector<std::string>{"alpha", "gamma"}));
}

int main(int ac, char* av[])
{
	const OrderUpdate feedOrder;
	
	auto & exchangeContext = ExchangeContext::getContext();

	std::shared_ptr<IOrder> order = OrderFactory::GenerateOrder(feedOrder);
	order->run(exchangeContext);

	return 0;
}

