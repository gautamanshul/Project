#ifndef ORDER_FACTORY_H
#define ORDER_FACTORY_H

#include <memory>
#include "OrderUpdate.h"
#include "IOrder.h"
#include "BuySellOrder.h"
#include "ChangeOrder.h"
#include "CancelOrder.h"
#include <iostream>

namespace OrderFactory
{
	std::shared_ptr<IOrder> GenerateOrder(const OrderUpdate & newOrder) 
	{
		std::shared_ptr<IOrder> order;

		switch (newOrder.type)
		{
			case (UpdateType::New):
				order.reset(static_cast<IOrder*>(new BuySellOrder(newOrder)));
				break;
			case (UpdateType::Change):
				order.reset(static_cast<IOrder*>(new ChangeOrder(newOrder)));
				break;
			case (UpdateType::Cancel):
				order.reset(static_cast<IOrder*>(new CancelOrder(newOrder)));
				break;

		}

		return order;
	}
}

#endif
