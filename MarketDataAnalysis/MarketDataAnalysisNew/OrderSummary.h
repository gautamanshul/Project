#ifndef ORDER_SUMMARY_H
#define ORDER_SUMMARY_H

#include "OrderUpdate.h"
#include <queue>
#include <vector>
#include <memory>

struct compare {
	bool operator()(const std::shared_ptr<IOrder> a, const std::shared_ptr<IOrder> b)
	{
		return a->getSequenceNumber() > b->getSequenceNumber();
	}
};

struct OrderSummary
{
public:

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
	uint64_t getExpectedSequenceNumber() const { return expectedSequenceNumber_; }
	void incrementExpectedSequence() { ++expectedSequenceNumber_; }
	
	std::shared_ptr<IOrder> getTopPendingOrder() {
		if (!pendingOrderUpdates.empty()) {
			return pendingOrderUpdates.top();
		}
		else {
			return nullptr;
		}
	}
	void removeTopPendingOrder() { pendingOrderUpdates.pop(); }

	void addToPendingOrderUpdates(std::shared_ptr<IOrder> order)
	{
		pendingOrderUpdates.push(order);
	}
	
	void setFirstSummaryUpdate(bool update) { firstSummaryUpdate_ = update; }
	bool getFirstSummaryUpdate() const { return firstSummaryUpdate_; }
protected:

	UpdateType orderType_;
	Side buyOrder_;
	uint64_t numShares_ = 0;
	float price_ = 0;
	uint64_t canceledShares = 0;
	uint64_t expectedSequenceNumber_ = 0;
	bool firstSummaryUpdate_ = false;
	std::priority_queue<std::shared_ptr<IOrder>, std::vector<std::shared_ptr<IOrder>>, compare> pendingOrderUpdates;
};

#endif
