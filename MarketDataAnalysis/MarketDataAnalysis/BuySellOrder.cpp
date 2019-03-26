#include "stdafx.h"
#include"BuySellOrder.h"
#include "ExchangeContext.h"

BuySellOrder::BuySellOrder(const struct OrderUpdate &order) : IOrder(order.entryTimestamp, order.side, order.type, order.price) {
	orderid_ = order.orderId;
	clientid_ = order.clientId;
	buyOrder_ = order.side;
	orderType_ = order.type;
	numShares_ = order.quantity;
	orderSequence = order.sequence;
	price_ = order.price;
}

void BuySellOrder::run(ExchangeContext& context)
{	
	context.HandleBuySellOrder(this);
}