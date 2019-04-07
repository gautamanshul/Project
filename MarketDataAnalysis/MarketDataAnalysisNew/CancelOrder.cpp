
//#include "stdafx.h"
#include "CancelOrder.h"
#include "ExchangeContext.h"

CancelOrder::CancelOrder(const struct OrderUpdate &order)
	: IOrder(order.entryTimestamp, order.side, order.type)
{
	// Parse member fields and make sure they follow the specification
	orderId_ = order.orderId;
	clientId_ = order.clientId;
	cancelledShares_ = order.quantity;
	orderSequence_ = order.sequence;
	buyOrder_ = order.side;
	updateType_ = order.type;
}
void CancelOrder::run(ExchangeContext &context, std::shared_ptr<IOrder> order)
{
	context.HandleCancelOrder(order);
}
