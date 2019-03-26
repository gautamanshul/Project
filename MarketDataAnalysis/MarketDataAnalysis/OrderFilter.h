#pragma once
#include "OrderUpdate.h"
#include "IOrder.h"
#include <map>
//Specification pattern for data access

template <typename T> struct ISpecification
{
	virtual bool is_satisfied(T* item) = 0;
};

template <typename T> class IFilter
{
	virtual std::map<uint64_t, T *> filter(std::map<uint64_t, T *> items, ISpecification<T> & spec) = 0;
 };

class BetterFilter : public IFilter<IOrder>
{
	typedef  std::map<uint64_t, IOrder *> timeStampOrderMap;
	std::map<uint64_t, IOrder *> filter(std::map<uint64_t, IOrder *> items, ISpecification<IOrder> & spec)
	{
		timeStampOrderMap result;
		for (auto & p : items)
		{
			//Check for the type of order through dynamic type casting
			
			if (spec.is_satisfied(p.second))
			{
				result.insert(p);
			}
		}

		return result;
	}
};

class SideSpecification : ISpecification<IOrder>
{
	Side side;
	explicit SideSpecification(const Side side) : side{side} {}

	bool is_satisfied(IOrder * item) override
	{
		return item->getSide() == side;
	}
};

class PriceSpecification : ISpecification<IOrder>
{
	float price;
	explicit PriceSpecification(const float price) : price{ price } {}

	bool is_satisfied(IOrder * item) override
	{
		return item->getPrice() == price;
	}
};