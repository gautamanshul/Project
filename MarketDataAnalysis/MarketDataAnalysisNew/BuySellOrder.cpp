//#include "stdafx.h"
#include"BuySellOrder.h"
#include "ExchangeContext.h"

BuySellOrder::BuySellOrder(const struct OrderUpdate &order) : IOrder(order.entryTimestamp, order.side, order.type, order.price) {
	orderId_ = order.orderId;
	clientId_ = order.clientId;
	buyOrder_ = order.side;
	orderType_ = order.type;
	numShares_ = order.quantity;
	orderSequence_ = order.sequence;
	price_ = order.price;
}

void BuySellOrder::run(ExchangeContext& context, std::shared_ptr<IOrder> order)
{	
	context.HandleBuySellOrder(order);
}