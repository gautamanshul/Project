#include "stdafx.h"
#include "ExchangeContext.h"



//void ExchangeContext::HandleOutOfOrder(const IOrder & order)
//{
//	//Out of order can be new buy / sell, change buy / sell, cancel buy / sell
//	//The first order can be a sell order in case of a short sell.
//	
//}

uint64_t ExchangeContext::shareVolBeforeTimeStamp(uint64_t & timestamp, uint64_t orderId, uint64_t customerId)
{
	//Find the order and customer map
	auto itr = openOrders_.find(std::make_pair(orderId, customerId));
	if (itr != openOrders_.end())
	{
		//Get the sum of share volume before the timestamp irrespective of the type of order
		
		auto low = std::lower_bound(itr->second.second->begin(), itr->second.second->end(), itr->second.second->begin());
		auto upper = std::upper_bound(itr->second.second->begin(), itr->second.second->end(), timestamp);

		if (low != upper)
		{
			uint64_t sum = 0;
			for (auto it = low; it != upper; ++it)
			{
				sum += it->second->getQuantity();

			}
		}
	}
}

void ExchangeContext::HandleBuySellOrder(BuySellOrder* buySellOrder)
{
	if (getExpectedSequenceNumber() != buySellOrder->getSequenceNumber()) 
	{
		if (buySellOrder->getSequenceNumber() < getExpectedSequenceNumber()) 
		{
			//Case 0: new sequence number is less than expected sequence number. Received an earlier order
			//Add the order to the map and remove it from the setmap 
			/*orderIDCustIDtoSharesAndSeq.insert(std::make_pair(buySellOrder.getOrderId(),
																buySellOrder.getClientId()),
												std::make_pair(buySellOrder.getNumShares(),
																buySellOrder.getSequenceNumber()));*/
			
			//openOrders_[(std::make_pair(buySellOrder->getOrderId(),
				//buySellOrder->getClientId()))] = dynamic_cast<IOrder *> (buySellOrder);
			sequenceNotReceived.erase(buySellOrder->getSequenceNumber());
		}
		else if (getExpectedSequenceNumber() > buySellOrder->getSequenceNumber()) 
		{		
			//Case 1: new sequence number is greater than expected sequence number. Missed an order.
			//Update the set with the sequence number. 
			sequenceNotReceived.insert(getExpectedSequenceNumber());
			
		}
	}

	//Find the orderid and customer id in the map. 
	uint64_t orderId = buySellOrder->getOrderId();
	uint64_t customerId = buySellOrder->getClientId();
	std::shared_ptr<std::map<uint64_t, IOrder *>> timeStampMap = nullptr;

	
	auto itr = openOrders_.find(std::make_pair(orderId, customerId));
	if (itr == openOrders_.end())
	{
		std::shared_ptr<OrderSummary> orderSummary = std::make_shared<OrderSummary>(new OrderSummary());
		//Update summary
		orderSummary->updateNumShares(buySellOrder->getNumShares());
		orderSummary->updatePrice(buySellOrder->getPrice());
		orderSummary->updateSide(buySellOrder->getSide());
		orderSummary->updateLastOrderType(buySellOrder->getUpdateType());

		timeStampMap = std::make_shared<std::map<uint64_t, IOrder *>>(new std::map<uint64_t, IOrder *>);
		timeStampMap->insert({buySellOrder->getTimestamp(), dynamic_cast<IOrder *> (buySellOrder) });

		openOrders_.insert({ std::make_pair(buySellOrder->getOrderId(),
			buySellOrder->getClientId()), std::make_pair(orderSummary, timeStampMap) });

		incrementExpectedSequence();
	}
	
	//If timestamp is older, then the 
	//uint64_t newOrderTimestamp = buySellOrder.getTimestamp();
	//if (newOrderTimestamp < getTimeStampOfPreviousOrder()) 
	//{
	//	//Pending order has been received. Add the order to the map 
	//	orderIDCustIDtoSharesAndSeq.insert
	//}
}

void ExchangeContext::HandleCancelOrder(CancelOrder *cancelOrder)
{
	uint64_t orderId = cancelOrder->getOrderId();
	uint64_t customerId = cancelOrder->getCustomerId();
	auto itr = openOrders_.find(std::make_pair(orderId, customerId));
	if (itr == openOrders_.end())
	{
		// throw an exception
	}
	else {

		//If present, then update the order summary and insert the order details in a map (tree) based on timestamp
		auto orderSummary = itr->second.first;
		//	timeStampMap = std::make_shared<std::map<uint64_t, IOrder *>>(new std::map<uint64_t, IOrder *>);
		//Get the side. Assuming that the new order (orderId + customerId) can either be a new buy or sell order. 
		//Order summary will always have the primary type as buy or sell
		if (cancelOrder->getSide() == orderSummary->getSide())
		{
			uint64_t openSharesCount = orderSummary->getNumShares();
			if (cancelOrder->getCanceledShareVol() == openSharesCount)
			{
				orderSummary->updateNumShares(0);
				orderSummary->updateLastOrderType(cancelOrder->getUpdateType());
				itr->second.second->insert({ cancelOrder->getTimestamp(), dynamic_cast<IOrder *> (cancelOrder) });
			}
			else if (cancelOrder->getCanceledShareVol() < openSharesCount)
			{
				openSharesCount -= cancelOrder->getCanceledShareVol();
				orderSummary->updateNumShares(openSharesCount);
				itr->second.second->insert({ cancelOrder->getTimestamp(), dynamic_cast<IOrder *> (cancelOrder) });
			}
			else
			{
				//throw exception
				throw E("Cancel Order failed");
			}
			
		}
		else {
			//throw exception
			throw E("Side mismatch");
		}
		
	}
}

bool isQuantityChanged(uint64_t oldVolume, uint64_t newVolume)
{
	return newVolume != oldVolume;
}

bool isPriceChanged(float oldPrice, float newPrice)
{
	return newPrice != oldPrice;
}

void ExchangeContext::HandleChangeOrder(ChangeOrder *changeOrder)
{
	uint64_t orderId = changeOrder->getOrderId();
	uint64_t customerId = changeOrder->getCustomerId();
	auto itr = openOrders_.find(std::make_pair(orderId, customerId));
	if (itr == openOrders_.end())
	{
		// throw an exception
	}
	else {

		//If present, then update the order summary and insert the order details in a map (tree) based on timestamp
		auto orderSummary = itr->second.first;
		//	timeStampMap = std::make_shared<std::map<uint64_t, IOrder *>>(new std::map<uint64_t, IOrder *>);
		//Get the side. Assuming that the new order (orderId + customerId) can either be a new buy or sell order. 
		//Order summary will always have the primary type as buy or sell
		if (changeOrder->getSide() == orderSummary->getSide())
		{
			uint64_t openSharesCount = orderSummary->getNumShares();
			
			if (isQuantityChanged(orderSummary->getNumShares(), changeOrder->getChangedShareVol()) &&
				isPriceChanged(orderSummary->getPrice(), changeOrder->getPrice())) {
				orderSummary->updateNumShares(changeOrder->getChangedShareVol());
				orderSummary->updatePrice(changeOrder->getPrice());
				itr->second.second->insert({ changeOrder->getTimestamp(), dynamic_cast<IOrder *> (changeOrder) });

			}
			else if (isPriceChanged(orderSummary->getPrice(), changeOrder->getPrice())) {
				orderSummary->updatePrice(changeOrder->getPrice());
				itr->second.second->insert({ changeOrder->getTimestamp(), dynamic_cast<IOrder *> (changeOrder) });
			}
			else if (isQuantityChanged(orderSummary->getNumShares(), changeOrder->getChangedShareVol())) {
				orderSummary->updateNumShares(changeOrder->getChangedShareVol());
				itr->second.second->insert({ changeOrder->getTimestamp(), dynamic_cast<IOrder *> (changeOrder) });
			}
			else {
				throw E("No price or quantity change");
			}

		}
		else {
			//throw exception
			throw E("Side mismatch");
		}

		//// Complete fill
		//if (changeOrder.getChangedShareVol() == openSharesCount)
		//{
		//	UpdateSymbolVolume(symbol, executeOrder.getExecShareVol());
		//	openOrders_.erase(itr);
		//}
		//// partial fill
		//else if (executeOrder.getExecShareVol() < openSharesCount)
		//{
		//	UpdateSymbolVolume(symbol, executeOrder.getExecShareVol());
		//	openSharesCount -= executeOrder.getExecShareVol();
		//}
		//else
		//{
		//	// throw an exception. executed share count cannot be more than what is open on the order
		//	throw E("Executed share count cannot be more than what is open on the order");
		//}
	}
}

