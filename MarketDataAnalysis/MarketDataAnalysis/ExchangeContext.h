#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>
#include <memory>
#include <string>
#include <set>
#include "IOrder.h"
#include "buySellOrder.h"
#include "ChangeOrder.h"
#include "CancelOrder.h"

uint64_t ExchangeContext::timestampOfPreviousOrder_ = 0;
uint64_t ExchangeContext::expectedSequenceNumber_ = 0;

class ExchangeContext {
	//void UpdateSymbolVolume(const std::string & symbol);
	ExchangeContext() = default;
	
	std::map<std::pair<uint64_t, uint64_t>, IOrder *> openOrders_; 
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
	void HandleChangeOrder(const ChangeOrder& changeOrder);
	void HandleCancelOrder(const CancelOrder& cancelOrder);
};

