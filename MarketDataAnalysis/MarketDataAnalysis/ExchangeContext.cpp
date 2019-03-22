#include "ExchangeContext.h"



void ExchangeContext::HandleOutOfOrder(const IOrder & order)
{
	
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
		//Allocate memory for order summary.
		std::shared_ptr<OrderSummary> orderSummary = std::make_shared<OrderSummary>(new OrderSummary());
		orderSummary->updateNumShares(buySellOrder->getNumShares());
		orderSummary->updatePrice(buySellOrder->getNumShares());
		orderSummary->updateSide(buySellOrder->getSide());
		orderSummary->updateLastOrderType(buySellOrder->getUpdateType);

		timeStampMap = std::make_shared<std::map<uint64_t, IOrder *>>(new std::map<uint64_t, IOrder *>);
		timeStampMap->insert({buySellOrder->getTimestamp(), dynamic_cast<IOrder *> (buySellOrder) });

		openOrders_.insert({ std::make_pair(buySellOrder->getOrderId(),
			buySellOrder->getClientId()), std::make_pair(orderSummary, timeStampMap) });
	}
	else {
		//If present, then update the order summary and insert the order details in a map (tree) based on timestamp
		auto summary = itr->second.first;
	//	timeStampMap = std::make_shared<std::map<uint64_t, IOrder *>>(new std::map<uint64_t, IOrder *>);
		
		itr->second.second->insert({ buySellOrder->getTimestamp(), dynamic_cast<IOrder *> (buySellOrder) });
	}

	incrementExpectedSequence();

	//If timestamp is older, then the 
	//uint64_t newOrderTimestamp = buySellOrder.getTimestamp();
	//if (newOrderTimestamp < getTimeStampOfPreviousOrder()) 
	//{
	//	//Pending order has been received. Add the order to the map 
	//	orderIDCustIDtoSharesAndSeq.insert
	//}
}

void ExchangeContext::HandleCancelOrder(const CancelOrder &cancelOrder)
{
	uint64_t orderId = cancelOrder.getOrderId();
	uint64_t customerId = cancelOrder.getCustomerId();
	auto itr = openOrders_.find(std::make_pair(orderId, customerId));
	if (itr == openOrders_.end())
	{
		// throw an exception
	}
	// Full cancel
	if (BuySellOrder * buySellOrder = dynamic_cast<BuySellOrder *>(itr->second)) {
		uint64_t openSharesCount = buySellOrder->getNumShares();
		if (cancelOrder.getCanceledShareVol() == openSharesCount)
		{
			openOrders_.erase(itr);
		}
		else if (cancelOrder.getCanceledShareVol() < openSharesCount)
		{
			openSharesCount -= cancelOrder.getCanceledShareVol();
			buySellOrder->setNumShares(openSharesCount);
		}
		else
		{
			//throw exception
			throw E("Cancel Order failed");
		}
	} else {
		//throw exception
		throw E("Retrieved order is not of type BuySellOrder");
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

void ExchangeContext::HandleChangeOrder(const ChangeOrder &changeOrder)
{
	uint64_t orderId = changeOrder.getOrderId();
	uint64_t customerId = changeOrder.getCustomerId();

	auto itr = openOrders_.find(std::make_pair(orderId, customerId));
	if (itr == openOrders_.end())
	{
		// throw an exception
		throw E("Couldn't find the order id. Bad request.");
	}

	if (BuySellOrder * buySellOrder = dynamic_cast<BuySellOrder *>(itr->second)) {
		uint64_t openSharesCount = buySellOrder->getNumShares();

	if (BuySellOrder * buySellOrder = dynamic_cast<BuySellOrder *>(itr->second)) {
		if (isQuantityChanged(buySellOrder->getNumShares(), changeOrder.getChangedShareVol()) && 
				isPriceChanged(buySellOrder->getPrice(), changeOrder.getPrice())) {
			buySellOrder->setNumShares(changeOrder.getChangedShareVol());
			buySellOrder->setPrice(changeOrder.getPrice());

		} else if (isPriceChanged(buySellOrder->getPrice(), changeOrder.getPrice())) {
			buySellOrder->setPrice(changeOrder.getPrice());
		}
		else if (isQuantityChanged(buySellOrder->getNumShares(), changeOrder.getChangedShareVol())) {
			buySellOrder->setNumShares(changeOrder.getChangedShareVol());
		}
		else {
			throw E("No price or quantity change");
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

