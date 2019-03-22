

#include "ChangeOrder.h"
#include "ExchangeContext.h"

ChangeOrder::ChangeOrder(const struct OrderUpdate &order)
	: IOrder(order.entryTimestamp)
{
	// Parse member fields and make sure they follow the specification
	orderId_ = order.orderId;
	customerId_ = order.clientId;
	numberOfShares_ = order.quantity;
	buyOrder_ = order.side;
	price_ = order.price;
}
void ChangeOrder::run(ExchangeContext &context)
{
	context.HandleChangeOrder(*this);
}

