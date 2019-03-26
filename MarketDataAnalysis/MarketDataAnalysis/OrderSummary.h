#pragma once
#include "OrderUpdate.h"

struct OrderSummary
{
public:
	//Delete the defaul destructor
	OrderSummary() {}
	//OrderSummary(const struct OrderUpdate &order);
	
	uint64_t getNumShares() const { return numShares_; }
	//uint64_t getSequenceNumber() const { return orderSequence; }	
	float getPrice() const { return price_; }
	void updateNumShares(uint64_t numOfShares) { numShares_ = numOfShares; }
	void updatePrice(float  price) { price_ = price; }
	void updateSide(Side  side) { buyOrder_ = side; }
	void updateLastOrderType(UpdateType orderType) { orderType_ = orderType; }
	Side getSide() const { return buyOrder_; }

	// More methods can be definted to access other fields
	virtual ~OrderSummary() = default;

protected:
	
	UpdateType orderType_;
	Side buyOrder_;
	uint64_t numShares_ = 0;
	float price_ = 0;
	uint64_t canceledShares = 0;

};