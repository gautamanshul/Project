#include "OrderFilter.h"
#include "ExchangeContext.h"


std::vector<std::shared_ptr<IOrder>> BetterFilter::filter(uint64_t orderId, uint64_t customerId, ExchangeContext & ec, ISpecification<std::shared_ptr<IOrder>> & spec)
{
	OrderList result;
	auto itr = ec.openOrders_.find(std::make_pair(orderId, customerId)); //Time complexity to search in a hash map is O(1)

	for (auto & p : itr->second.second)
	{
		//Check for the type of order through dynamic type casting

		if (spec.is_satisfied(p.second))
		{
			result.push_back(p.second);
		}
	}

	return result;
}