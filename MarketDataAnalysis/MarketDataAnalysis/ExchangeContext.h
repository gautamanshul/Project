#pragma once
#include "stdafx.h"
#include <iostream>
#include <vector>
#include <unordered_map>
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


class ExchangeContext {
	//void UpdateSymbolVolume(const std::string & symbol);
	ExchangeContext() = default;
	
	std::unordered_map<std::pair<uint64_t, uint64_t>, std::pair<std::shared_ptr<OrderSummary>, std::shared_ptr<std::map<uint64_t, IOrder *>>>> openOrders_;
	std::set<uint64_t> sequenceNotReceived;
	static uint64_t expectedSequenceNumber_;
	static uint64_t timestampOfPreviousOrder_;

public:
	//Deleting the copy constructors
	ExchangeContext(ExchangeContext const &) = delete;
	void operator = (ExchangeContext const &) = delete;
	static ExchangeContext & getContext()
	{
		static ExchangeContext ec;
		return ec;
	}

	uint64_t getExpectedSequenceNumber() const { return expectedSequenceNumber_; }
	void incrementExpectedSequence() { ++expectedSequenceNumber_; }
	uint64_t getTimeStampOfPreviousOrder() const { return timestampOfPreviousOrder_; }
	void setTimeStampforCurrentOrder(uint64_t & timestampOfCurrentOrder) 
	{
		timestampOfPreviousOrder_ = timestampOfCurrentOrder;
	}


	void HandleBuySellOrder(BuySellOrder* buySellOrder);
	void HandleChangeOrder(ChangeOrder* changeOrder);
	void HandleCancelOrder(CancelOrder * cancelOrder);
	uint64_t shareVolBeforeTimeStamp(uint64_t & timestamp, uint64_t orderId, uint64_t customerId);
};


uint64_t ExchangeContext::timestampOfPreviousOrder_ = 0;
uint64_t ExchangeContext::expectedSequenceNumber_ = 0;