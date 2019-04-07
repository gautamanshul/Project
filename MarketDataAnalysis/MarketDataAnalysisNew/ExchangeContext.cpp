//#include "stdafx.h"
#include "ExchangeContext.h"

uint64_t ExchangeContext::shareVolBeforeTimeStamp(uint64_t timestamp, uint64_t orderId, uint64_t customerId)
{
	//Find the order and customer map
	auto itr = openOrders_.find(std::make_pair(orderId, customerId)); //O(1) to search through hash map
	if (itr != openOrders_.end()) 
	{
		//Get the sum of share volume before the timestamp irrespective of the type of order
		
		auto low = itr->second.second.lower_bound(itr->second.second.begin()->first);
		auto upper = itr->second.second.lower_bound(timestamp);

		if (low != upper)
		{
			uint64_t sum = 0;
			for (auto it = low; it != upper; ++it) //O(n) time to add the orders together
			{
				sum += it->second->getQuantity();
			}

			std::cout << "Sum of shares " << " " << sum << std::endl;
		}
	}

	return 0;
}

void ExchangeContext::updateOrderSummary(std::shared_ptr<IOrder> buySellOrder, 
				std::shared_ptr<OrderSummary> orderSummary)
{
	if (buySellOrder->getSide() == Side::Sell)
	{
		uint64_t openSharesCount = orderSummary->getNumShares();

		if (openSharesCount == buySellOrder->getQuantity())
		{
			orderSummary->updateNumShares(0);
			orderSummary->updateLastOrderType(buySellOrder->getUpdateType());
		}
		else if (buySellOrder->getQuantity() < openSharesCount)
		{
			openSharesCount -= buySellOrder->getQuantity();
			orderSummary->updateNumShares(openSharesCount);
			orderSummary->updateLastOrderType(buySellOrder->getUpdateType());
		}
		else
		{
			//throw exception
			//	throw E("Cancel Order failed");

		}
	}
	else {
		uint64_t openSharesCount = orderSummary->getNumShares();
		openSharesCount += buySellOrder->getQuantity();
		orderSummary->updateNumShares(openSharesCount);
		orderSummary->updateLastOrderType(buySellOrder->getUpdateType());
	}
}

void ExchangeContext::HandleBuySellOrder(std::shared_ptr<IOrder> buySellOrder)
{
	//Find the orderid and customer id in the map. 
	uint64_t orderId = buySellOrder->getOrderId();
	uint64_t customerId = buySellOrder->getClientId();
	//std::map<uint64_t, IOrder *, cmpByTimeStamp> timeStampMap;
	std::shared_ptr<OrderSummary> orderSummary;
	auto itr = openOrders_.find(std::make_pair(orderId, customerId));
	/* Sample simulation
	* 1 NES = 0, POU = {1}
	* 0 NES = 2, POU = {}
	* 5 NES = 2, POU = {5}
	* 7 NES = 2, POU = {5, 7}
	* 4 NES = 2, POU = {4, 5, 7}
	* 6 NES = 2, POU = {4, 5, 6, 7}
	* 3 NES = 2, POU = {3, 4, 5, 6, 7}
	* 2 NES = 8, POU = {}
	*/
	if (itr == openOrders_.end())
	{

		orderSummary = std::make_shared<OrderSummary>();
		openOrders_[std::make_pair(buySellOrder->getOrderId(),
			buySellOrder->getClientId())].first = orderSummary; 
	}
	else {
		orderSummary = itr->second.first;
	}
	
		if (orderSummary->getExpectedSequenceNumber() == buySellOrder->getSequenceNumber()) 
		{
			
			openOrders_[std::make_pair(buySellOrder->getOrderId(),
				buySellOrder->getClientId())].second[buySellOrder->getTimestamp()] = buySellOrder;

			if (orderSummary->getFirstSummaryUpdate() == false) {
				orderSummary->updateNumShares(buySellOrder->getQuantity());
				orderSummary->updatePrice(buySellOrder->getPrice());
				orderSummary->updateSide(buySellOrder->getSide());
				orderSummary->updateLastOrderType(buySellOrder->getUpdateType());
				orderSummary->setFirstSummaryUpdate(true);
			}
			else {
				updateOrderSummary(buySellOrder, orderSummary);
			}

			orderSummary->incrementExpectedSequence();

			while (orderSummary->getTopPendingOrder() != nullptr && 
						orderSummary->getExpectedSequenceNumber() ==  orderSummary->getTopPendingOrder()->getSequenceNumber()) 
			{
				//Update the orders and handle the cases of cancel and change order
				
				if (orderSummary->getTopPendingOrder()->getUpdateType() == UpdateType::Cancel) 
				{
					HandleCancelOrder( orderSummary->getTopPendingOrder() );
				}
				else if (orderSummary->getTopPendingOrder()->getUpdateType() == UpdateType::Change) 
				{
					HandleChangeOrder(orderSummary->getTopPendingOrder());
				}
				else if (orderSummary->getTopPendingOrder()->getUpdateType() == UpdateType::New)
				{
					updateOrderSummary(orderSummary->getTopPendingOrder(), orderSummary);
					openOrders_[std::make_pair(orderSummary->getTopPendingOrder()->getOrderId(),
						orderSummary->getTopPendingOrder()->getClientId())].second[orderSummary->getTopPendingOrder()->getTimestamp()] = orderSummary->getTopPendingOrder();
					//Increment the sequence
					orderSummary->incrementExpectedSequence();
				}
				orderSummary->removeTopPendingOrder();
			}
		}
		else {
			//Out of sequence encountered. Add to priority queue.
				orderSummary->addToPendingOrderUpdates(buySellOrder);
		}
	
		return;
}

void ExchangeContext::HandleCancelOrder(std::shared_ptr<IOrder> cancelOrder)
{
	uint64_t orderId = cancelOrder->getOrderId();
	uint64_t customerId = cancelOrder->getClientId();
	auto itr = openOrders_.find(std::make_pair(orderId, customerId));
	std::shared_ptr<OrderSummary> orderSummary;

	if (itr == openOrders_.end())
	{
		//Out of sequence order received. Add it to priority queue
		orderSummary = std::make_shared<OrderSummary>();
		openOrders_[std::make_pair(orderId,
			customerId)].first = orderSummary;
		orderSummary->addToPendingOrderUpdates(cancelOrder);
		return;
	} else {
		orderSummary = itr->second.first;
	}

	if (orderSummary->getExpectedSequenceNumber() == cancelOrder->getSequenceNumber()) {
		
		uint64_t openSharesCount = orderSummary->getNumShares();
		if (cancelOrder->getQuantity() == openSharesCount)
		{
			orderSummary->updateNumShares(0);
			orderSummary->updateLastOrderType(cancelOrder->getUpdateType());
			itr->second.second[cancelOrder->getTimestamp()] = cancelOrder;
		}
		else if (cancelOrder->getQuantity() < openSharesCount)
		{
			openSharesCount -= cancelOrder->getQuantity();
			orderSummary->updateNumShares(openSharesCount);
			itr->second.second[cancelOrder->getTimestamp()] = cancelOrder;
		}
		else
		{
			//This order can be potentially removed as its no longer valid to save memory. 
			itr->second.second[cancelOrder->getTimestamp()] = cancelOrder;
			
			//throw exception
			//throw E("Cancel Order failed");
		}
		orderSummary->incrementExpectedSequence();		
	}
	else {
		//Out of sequence encountered. Add to priority queue.
		orderSummary->addToPendingOrderUpdates(cancelOrder);
	}

	return;
}

bool ExchangeContext:: isQuantityChanged(uint64_t oldVolume, uint64_t newVolume)
{
	return newVolume != oldVolume;
}

bool ExchangeContext:: isPriceChanged(float oldPrice, float newPrice)
{
	return newPrice != oldPrice;
}

void ExchangeContext::HandleChangeOrder(std::shared_ptr<IOrder> changeOrder)
{
	uint64_t orderId = changeOrder->getOrderId();
	uint64_t customerId = changeOrder->getClientId();
	auto itr = openOrders_.find(std::make_pair(orderId, customerId));
	std::shared_ptr<OrderSummary> orderSummary;

	if (itr == openOrders_.end())
	{
		//Out of sequence order received. Add it to priority queue
		orderSummary = std::make_shared<OrderSummary>();
		openOrders_[std::make_pair(orderId, customerId)].first = orderSummary;
		orderSummary->addToPendingOrderUpdates(changeOrder);
		return;
	}
	else {
		orderSummary = itr->second.first;
	}
	
	if (orderSummary->getExpectedSequenceNumber() == changeOrder->getSequenceNumber()) 
	{
		//Get the side. Assuming that the new order (orderId + customerId) can either be a new buy or sell order. 
		//Order summary will always have the primary type as buy or sell
			
		uint64_t openSharesCount = orderSummary->getNumShares();

		if (isQuantityChanged(orderSummary->getNumShares(), changeOrder->getQuantity()) &&
			isPriceChanged(orderSummary->getPrice(), changeOrder->getPrice())) 
		{
			orderSummary->updateNumShares(changeOrder->getQuantity());
			orderSummary->updatePrice(changeOrder->getPrice());
			itr->second.second[changeOrder->getTimestamp()] = changeOrder;

		}
		else if (isPriceChanged(orderSummary->getPrice(), changeOrder->getPrice())) {
			orderSummary->updatePrice(changeOrder->getPrice());
			itr->second.second[changeOrder->getTimestamp()] = changeOrder;
		}
		else if (isQuantityChanged(orderSummary->getNumShares(), changeOrder->getQuantity())) {
			orderSummary->updateNumShares(changeOrder->getQuantity());
			itr->second.second[changeOrder->getTimestamp()] = changeOrder;
		}
		else {
			itr->second.second[changeOrder->getTimestamp()] = changeOrder;
			//throw E("No price or quantity change");
		}

		orderSummary->incrementExpectedSequence();
		
	}
	else {
		orderSummary->addToPendingOrderUpdates(changeOrder);
	}

	return;
}

