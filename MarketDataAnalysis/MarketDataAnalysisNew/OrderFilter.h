#ifndef ORDER_FILTER_H
#define ORDER_FILTER_H

#include "IOrder.h"
#include <map>
#include <memory>
#include <utility>
#include <vector>

class ExchangeContext;
//Specification pattern for data access


template <typename T>
struct ISpecification
{
	virtual bool is_satisfied(T item) = 0;
};

struct cmpTimeStamp {
	bool operator() (uint64_t const& a, uint64_t const & b) const {
		return a < b;
	}
};

template <typename T> struct IFilter
{
	//virtual std::vector<std::shared_ptr<IOrder>> filter(std::map<uint64_t, T > items, ISpecification<T> & spec) = 0;
	virtual std::vector<std::shared_ptr<IOrder>> filter(uint64_t orderId, uint64_t customerId, ExchangeContext & ec, ISpecification<T> & spec) = 0;
};

struct BetterFilter : IFilter<std::shared_ptr<IOrder>>
{
	typedef  std::vector<std::shared_ptr<IOrder>> OrderList;

	std::vector<std::shared_ptr<IOrder>> filter(uint64_t orderId, uint64_t customerId, ExchangeContext & ec, ISpecification<std::shared_ptr<IOrder>> & spec) override;
};

struct SideSpecification : ISpecification<std::shared_ptr<IOrder>>
{
	Side side;
	explicit SideSpecification(const Side side) : side{ side } {}

	bool is_satisfied(std::shared_ptr<IOrder> item) override
	{
		return item->getSide() == side;
	}
};

struct PriceSpecification : ISpecification<std::shared_ptr<IOrder>>
{
	float price;
	explicit PriceSpecification(const float price) : price{ price } {}

	bool is_satisfied(std::shared_ptr<IOrder> item) override
	{
		return item->getPrice() == price;
	}
};

struct TimeStampSpecification : ISpecification<std::shared_ptr<IOrder>>
{
	uint64_t timeStamp;
	explicit TimeStampSpecification(const uint64_t timeStamp) : timeStamp{ timeStamp } {}

	bool is_satisfied(std::shared_ptr<IOrder> item) override
	{
		return item->getTimestamp() < timeStamp;
	}
};

template <typename T> struct AndSpecification : ISpecification <T>
{
	ISpecification<T> & first;
	ISpecification<T> & second;
	ISpecification<T> & third;

	AndSpecification(ISpecification<T> & first, ISpecification<T> & second, ISpecification<T> & third) : first{ first }, second{ second }, third{ third } {}

	bool is_satisfied(T item) override {
		return first.is_satisfied(item) && second.is_satisfied(item) && third.is_satisfied(item);
	}
};


#endif