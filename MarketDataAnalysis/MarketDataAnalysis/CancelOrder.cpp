

#include "CancelOrder.h"
#include "ExchangeContext.h"

CancelOrder::CancelOrder(const struct OrderUpdate &order)
	: IOrder(order.entryTimestamp)
{
	// Parse member fields and make sure they follow the specification
	orderId_ = order.orderId;
	customerId_ = order.clientId;
	cancelledShares_ = order.quantity;
}
void CancelOrder::run(ExchangeContext &context)
{
	context.HandleCancelOrder(*this);
}
