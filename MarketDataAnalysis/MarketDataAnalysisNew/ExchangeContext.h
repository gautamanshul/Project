#ifndef EXCHANGE_CONTEXT_H
#define EXCHANGE_CONTEXT_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <algorithm>
#include <set>
#include <exception>
#include "IOrder.h"
#include "buySellOrder.h"
#include "ChangeOrder.h"
#include "CancelOrder.h"
#include "OrderSummary.h"
#include "Exception.h"
#include "OrderFilter.h"


namespace std {

	template <> struct hash<std::pair<uint64_t, uint64_t>>
	{
		inline std::size_t operator() (const std::pair<uint64_t, uint64_t> &pairs) const
		{
			std::hash <uint64_t> int_hasher;
			return int_hasher(pairs.first) ^ int_hasher(pairs.second);
		}
	};

	//template <> struct less<const uint64_t>
	//{
	//	bool operator() (const uint64_t &a, const uint64_t& b) const
	//	{
	//		
	//		return a < b;
	//	}
	//};

}

struct cmpByTimeStamp {
	bool operator() (uint64_t const& a, uint64_t const & b) const {
		return a < b;
	}
};

class ExchangeContext {
	//void UpdateSymbolVolume(const std::string & symbol);
	ExchangeContext() = default;
	
	std::unordered_map<std::pair<uint64_t, uint64_t>, std::pair<std::shared_ptr<OrderSummary>, 
														std::map<uint64_t, std::shared_ptr<IOrder>, 
														cmpByTimeStamp>>> openOrders_;
	//std::set<uint64_t> sequenceNotReceived;	
	
	uint64_t timestampOfPreviousOrder_;
	friend struct BetterFilter; 

public:
	//Deleting the copy constructors
	ExchangeContext(ExchangeContext const &) = delete;
	void operator = (ExchangeContext const &) = delete;
	static ExchangeContext & getContext()
	{
		static ExchangeContext ec;
		return ec;
	}

	
	uint64_t getTimeStampOfPreviousOrder() const { return timestampOfPreviousOrder_; }
	void setTimeStampforCurrentOrder(uint64_t & timestampOfCurrentOrder) 
	{
		timestampOfPreviousOrder_ = timestampOfCurrentOrder;
	}


	void HandleBuySellOrder(std::shared_ptr<IOrder>);
	void HandleChangeOrder(std::shared_ptr<IOrder>);
	void HandleCancelOrder(std::shared_ptr<IOrder>);
	void updateOrderSummary(std::shared_ptr<IOrder> buySellOrder, std::shared_ptr<OrderSummary> orderSummary);
	bool isQuantityChanged(uint64_t oldVolume, uint64_t newVolume);
	bool isPriceChanged(float oldPrice, float newPrice);

	uint64_t shareVolBeforeTimeStamp(uint64_t timestamp, uint64_t orderId, uint64_t customerId);
};

#endif
