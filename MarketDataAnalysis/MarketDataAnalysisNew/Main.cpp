#include "OrderFactory.h"
#include "ExchangeContext.h"
#include "OrderFilter.h"

void testHashAndMap()
{
	OrderUpdate feedOrder;
	feedOrder.clientId = 1;
	feedOrder.entryTimestamp = 100;
	feedOrder.orderId = 1;
	feedOrder.price = 10;
	feedOrder.quantity = 20;
	feedOrder.sequence = 0;
	feedOrder.side = Side::Buy;
	feedOrder.type = UpdateType::New;

	auto & exchangeContext = ExchangeContext::getContext();

	std::shared_ptr<IOrder> order = OrderFactory::GenerateOrder(feedOrder);
	order->run(exchangeContext, order);

	feedOrder.clientId = 1;
	feedOrder.entryTimestamp = 101;
	feedOrder.orderId = 1;
	feedOrder.price = 10;
	feedOrder.quantity = 10;
	feedOrder.sequence = 1;
	feedOrder.side = Side::Sell;
	feedOrder.type = UpdateType::New;

	order = OrderFactory::GenerateOrder(feedOrder);
	order->run(exchangeContext, order);

	feedOrder.clientId = 1;
	feedOrder.entryTimestamp = 103;
	feedOrder.orderId = 1;
	feedOrder.price = 10;
	feedOrder.quantity = 10;
	feedOrder.sequence = 3;
	feedOrder.side = Side::Sell;
	feedOrder.type = UpdateType::New;

	order = OrderFactory::GenerateOrder(feedOrder);
	order->run(exchangeContext, order);

	feedOrder.clientId = 1;
	feedOrder.entryTimestamp = 104;
	feedOrder.orderId = 1;
	feedOrder.price = 10;
	feedOrder.quantity = 10;
	feedOrder.sequence = 4;
	feedOrder.side = Side::Sell;
	feedOrder.type = UpdateType::New;

	order = OrderFactory::GenerateOrder(feedOrder);
	order->run(exchangeContext, order);

	feedOrder.clientId = 1;
	feedOrder.entryTimestamp = 102;
	feedOrder.orderId = 1;
	feedOrder.price = 10;
	feedOrder.quantity = 10;
	feedOrder.sequence = 2;
	feedOrder.side = Side::Sell;
	feedOrder.type = UpdateType::New;

	order = OrderFactory::GenerateOrder(feedOrder);
	order->run(exchangeContext, order);
}

void testinOrderCancel()
{
	OrderUpdate feedOrder;
	feedOrder.clientId = 1;
	feedOrder.entryTimestamp = 100;
	feedOrder.orderId = 1;
	feedOrder.price = 10;
	feedOrder.quantity = 20;
	feedOrder.sequence = 0;
	feedOrder.side = Side::Buy;
	feedOrder.type = UpdateType::New;

	auto & exchangeContext = ExchangeContext::getContext();

	std::shared_ptr<IOrder> order = OrderFactory::GenerateOrder(feedOrder);
	order->run(exchangeContext, order);

	feedOrder.clientId = 1;
	feedOrder.entryTimestamp = 101;
	feedOrder.orderId = 1;
	feedOrder.price = 10;
	feedOrder.quantity = 10;
	feedOrder.sequence = 1;
	feedOrder.side = Side::Sell;
	feedOrder.type = UpdateType::Cancel;

	order = OrderFactory::GenerateOrder(feedOrder);
	order->run(exchangeContext, order);

	feedOrder.clientId = 1;
	feedOrder.entryTimestamp = 103;
	feedOrder.orderId = 1;
	feedOrder.price = 10;
	feedOrder.quantity = 10;
	feedOrder.sequence = 3;
	feedOrder.side = Side::Sell;
	feedOrder.type = UpdateType::New;

	order = OrderFactory::GenerateOrder(feedOrder);
	order->run(exchangeContext, order);

	feedOrder.clientId = 1;
	feedOrder.entryTimestamp = 104;
	feedOrder.orderId = 1;
	feedOrder.price = 10;
	feedOrder.quantity = 10;
	feedOrder.sequence = 4;
	feedOrder.side = Side::Sell;
	feedOrder.type = UpdateType::New;

	order = OrderFactory::GenerateOrder(feedOrder);
	order->run(exchangeContext, order);

	feedOrder.clientId = 1;
	feedOrder.entryTimestamp = 102;
	feedOrder.orderId = 1;
	feedOrder.price = 10;
	feedOrder.quantity = 10;
	feedOrder.sequence = 2;
	feedOrder.side = Side::Sell;
	feedOrder.type = UpdateType::New;

	order = OrderFactory::GenerateOrder(feedOrder);
	order->run(exchangeContext, order);
}

void testOutOfOrderCancel() {
	OrderUpdate feedOrder;
	feedOrder.clientId = 1;
	feedOrder.entryTimestamp = 100;
	feedOrder.orderId = 1;
	feedOrder.price = 10;
	feedOrder.quantity = 20;
	feedOrder.sequence = 0;
	feedOrder.side = Side::Buy;
	feedOrder.type = UpdateType::New;

	auto & exchangeContext = ExchangeContext::getContext();

	std::shared_ptr<IOrder> order = OrderFactory::GenerateOrder(feedOrder);
	order->run(exchangeContext, order);

	feedOrder.clientId = 1;
	feedOrder.entryTimestamp = 101;
	feedOrder.orderId = 1;
	feedOrder.price = 10;
	feedOrder.quantity = 10;
	feedOrder.sequence = 1;
	feedOrder.side = Side::Sell;
	feedOrder.type = UpdateType::New;

	order = OrderFactory::GenerateOrder(feedOrder);
	order->run(exchangeContext, order);

	feedOrder.clientId = 1;
	feedOrder.entryTimestamp = 103;
	feedOrder.orderId = 1;
	feedOrder.price = 10;
	feedOrder.quantity = 10;
	feedOrder.sequence = 3;
	feedOrder.side = Side::Sell;
	feedOrder.type = UpdateType::Cancel;

	order = OrderFactory::GenerateOrder(feedOrder);
	order->run(exchangeContext, order);

	feedOrder.clientId = 1;
	feedOrder.entryTimestamp = 104;
	feedOrder.orderId = 1;
	feedOrder.price = 10;
	feedOrder.quantity = 10;
	feedOrder.sequence = 4;
	feedOrder.side = Side::Sell;
	feedOrder.type = UpdateType::New;

	order = OrderFactory::GenerateOrder(feedOrder);
	order->run(exchangeContext, order);

	feedOrder.clientId = 1;
	feedOrder.entryTimestamp = 102;
	feedOrder.orderId = 1;
	feedOrder.price = 10;
	feedOrder.quantity = 10;
	feedOrder.sequence = 2;
	feedOrder.side = Side::Sell;
	feedOrder.type = UpdateType::New;

	order = OrderFactory::GenerateOrder(feedOrder);
	order->run(exchangeContext, order);
}

void testASecondOrder() {
	OrderUpdate feedOrder;
	feedOrder.clientId = 1;
	feedOrder.entryTimestamp = 100;
	feedOrder.orderId = 2;
	feedOrder.price = 10;
	feedOrder.quantity = 20;
	feedOrder.sequence = 0;
	feedOrder.side = Side::Buy;
	feedOrder.type = UpdateType::New;

	auto & exchangeContext = ExchangeContext::getContext();

	std::shared_ptr<IOrder> order = OrderFactory::GenerateOrder(feedOrder);
	order->run(exchangeContext, order);

	feedOrder.clientId = 1;
	feedOrder.entryTimestamp = 101;
	feedOrder.orderId = 2;
	feedOrder.price = 10;
	feedOrder.quantity = 10;
	feedOrder.sequence = 1;
	feedOrder.side = Side::Sell;
	feedOrder.type = UpdateType::New;

	order = OrderFactory::GenerateOrder(feedOrder);
	order->run(exchangeContext, order);

	feedOrder.clientId = 1;
	feedOrder.entryTimestamp = 103;
	feedOrder.orderId = 2;
	feedOrder.price = 10;
	feedOrder.quantity = 10;
	feedOrder.sequence = 3;
	feedOrder.side = Side::Sell;
	feedOrder.type = UpdateType::Cancel;

	order = OrderFactory::GenerateOrder(feedOrder);
	order->run(exchangeContext, order);

	feedOrder.clientId = 1;
	feedOrder.entryTimestamp = 104;
	feedOrder.orderId = 2;
	feedOrder.price = 10;
	feedOrder.quantity = 10;
	feedOrder.sequence = 4;
	feedOrder.side = Side::Sell;
	feedOrder.type = UpdateType::New;

	order = OrderFactory::GenerateOrder(feedOrder);
	order->run(exchangeContext, order);

	feedOrder.clientId = 1;
	feedOrder.entryTimestamp = 102;
	feedOrder.orderId = 2;
	feedOrder.price = 10;
	feedOrder.quantity = 10;
	feedOrder.sequence = 2;
	feedOrder.side = Side::Sell;
	feedOrder.type = UpdateType::New;

	order = OrderFactory::GenerateOrder(feedOrder);
	order->run(exchangeContext, order);

	return;
}



void testInOrderChange() {
	OrderUpdate feedOrder;
	feedOrder.clientId = 1;
	feedOrder.entryTimestamp = 100;
	feedOrder.orderId = 1;
	feedOrder.price = 10;
	feedOrder.quantity = 20;
	feedOrder.sequence = 0;
	feedOrder.side = Side::Buy;
	feedOrder.type = UpdateType::New;

	auto & exchangeContext = ExchangeContext::getContext();

	std::shared_ptr<IOrder> order = OrderFactory::GenerateOrder(feedOrder);
	order->run(exchangeContext, order);

	feedOrder.clientId = 1;
	feedOrder.entryTimestamp = 101;
	feedOrder.orderId = 1;
	feedOrder.price = 10;
	feedOrder.quantity = 30;
	feedOrder.sequence = 1;
	feedOrder.side = Side::Buy;
	feedOrder.type = UpdateType::Change;

	order = OrderFactory::GenerateOrder(feedOrder);
	order->run(exchangeContext, order);

	feedOrder.clientId = 1;
	feedOrder.entryTimestamp = 103;
	feedOrder.orderId = 1;
	feedOrder.price = 10;
	feedOrder.quantity = 10;
	feedOrder.sequence = 3;
	feedOrder.side = Side::Sell;
	feedOrder.type = UpdateType::Cancel;

	order = OrderFactory::GenerateOrder(feedOrder);
	order->run(exchangeContext, order);

	feedOrder.clientId = 1;
	feedOrder.entryTimestamp = 104;
	feedOrder.orderId = 1;
	feedOrder.price = 10;
	feedOrder.quantity = 10;
	feedOrder.sequence = 4;
	feedOrder.side = Side::Sell;
	feedOrder.type = UpdateType::New;

	order = OrderFactory::GenerateOrder(feedOrder);
	order->run(exchangeContext, order);

	feedOrder.clientId = 1;
	feedOrder.entryTimestamp = 102;
	feedOrder.orderId = 1;
	feedOrder.price = 10;
	feedOrder.quantity = 10;
	feedOrder.sequence = 2;
	feedOrder.side = Side::Sell;
	feedOrder.type = UpdateType::New;

	exchangeContext.shareVolBeforeTimeStamp(103, 1, 1);

	order = OrderFactory::GenerateOrder(feedOrder);
	order->run(exchangeContext, order);

	
}

void testShareVolumeBeforeTimeStamp() {
	OrderUpdate feedOrder;
	feedOrder.clientId = 1;
	feedOrder.entryTimestamp = 100;
	feedOrder.orderId = 1;
	feedOrder.price = 10;
	feedOrder.quantity = 20;
	feedOrder.sequence = 0;
	feedOrder.side = Side::Buy;
	feedOrder.type = UpdateType::New;

	auto & exchangeContext = ExchangeContext::getContext();

	std::shared_ptr<IOrder> order = OrderFactory::GenerateOrder(feedOrder);
	order->run(exchangeContext, order);

	feedOrder.clientId = 1;
	feedOrder.entryTimestamp = 101;
	feedOrder.orderId = 1;
	feedOrder.price = 10;
	feedOrder.quantity = 30;
	feedOrder.sequence = 1;
	feedOrder.side = Side::Buy;
	feedOrder.type = UpdateType::Change;

	order = OrderFactory::GenerateOrder(feedOrder);
	order->run(exchangeContext, order);

	feedOrder.clientId = 1;
	feedOrder.entryTimestamp = 103;
	feedOrder.orderId = 1;
	feedOrder.price = 10;
	feedOrder.quantity = 10;
	feedOrder.sequence = 3;
	feedOrder.side = Side::Sell;
	feedOrder.type = UpdateType::Cancel;

	order = OrderFactory::GenerateOrder(feedOrder);
	order->run(exchangeContext, order);

	feedOrder.clientId = 1;
	feedOrder.entryTimestamp = 104;
	feedOrder.orderId = 1;
	feedOrder.price = 10;
	feedOrder.quantity = 10;
	feedOrder.sequence = 4;
	feedOrder.side = Side::Sell;
	feedOrder.type = UpdateType::New;

	order = OrderFactory::GenerateOrder(feedOrder);
	order->run(exchangeContext, order);

	feedOrder.clientId = 1;
	feedOrder.entryTimestamp = 102;
	feedOrder.orderId = 1;
	feedOrder.price = 10;
	feedOrder.quantity = 10;
	feedOrder.sequence = 2;
	feedOrder.side = Side::Sell;
	feedOrder.type = UpdateType::New;

	order = OrderFactory::GenerateOrder(feedOrder);
	order->run(exchangeContext, order);

	exchangeContext.shareVolBeforeTimeStamp(103, 1, 1); 
}


void testFirstOutOfOrderSeq() 
{
	OrderUpdate feedOrder;

	feedOrder.clientId = 1;
	feedOrder.entryTimestamp = 101;
	feedOrder.orderId = 1;
	feedOrder.price = 10;
	feedOrder.quantity = 30;
	feedOrder.sequence = 1;
	feedOrder.side = Side::Buy;
	feedOrder.type = UpdateType::Change;

	auto & exchangeContext = ExchangeContext::getContext();

	std::shared_ptr<IOrder> order = OrderFactory::GenerateOrder(feedOrder);
	order->run(exchangeContext, order);

	feedOrder.clientId = 1;
	feedOrder.entryTimestamp = 103;
	feedOrder.orderId = 1;
	feedOrder.price = 10;
	feedOrder.quantity = 10;
	feedOrder.sequence = 3;
	feedOrder.side = Side::Sell;
	feedOrder.type = UpdateType::Cancel;

	order = OrderFactory::GenerateOrder(feedOrder);
	order->run(exchangeContext, order);

	feedOrder.clientId = 1;
	feedOrder.entryTimestamp = 104;
	feedOrder.orderId = 1;
	feedOrder.price = 10;
	feedOrder.quantity = 10;
	feedOrder.sequence = 4;
	feedOrder.side = Side::Sell;
	feedOrder.type = UpdateType::New;

	order = OrderFactory::GenerateOrder(feedOrder);
	order->run(exchangeContext, order);

	feedOrder.clientId = 1;
	feedOrder.entryTimestamp = 102;
	feedOrder.orderId = 1;
	feedOrder.price = 10;
	feedOrder.quantity = 10;
	feedOrder.sequence = 2;
	feedOrder.side = Side::Sell;
	feedOrder.type = UpdateType::New;

	order = OrderFactory::GenerateOrder(feedOrder);
	order->run(exchangeContext, order);

	feedOrder.clientId = 1;
	feedOrder.entryTimestamp = 100;
	feedOrder.orderId = 1;
	feedOrder.price = 10;
	feedOrder.quantity = 20;
	feedOrder.sequence = 0;
	feedOrder.side = Side::Buy;
	feedOrder.type = UpdateType::New;

	order = OrderFactory::GenerateOrder(feedOrder);
	order->run(exchangeContext, order);	

	return;
}

void testBetterFilter() {
	OrderUpdate feedOrder;
	feedOrder.clientId = 1;
	feedOrder.entryTimestamp = 100;
	feedOrder.orderId = 1;
	feedOrder.price = 10;
	feedOrder.quantity = 20;
	feedOrder.sequence = 0;
	feedOrder.side = Side::Buy;
	feedOrder.type = UpdateType::New;

	auto & exchangeContext = ExchangeContext::getContext();

	std::shared_ptr<IOrder> order = OrderFactory::GenerateOrder(feedOrder);
	order->run(exchangeContext, order);

	feedOrder.clientId = 1;
	feedOrder.entryTimestamp = 101;
	feedOrder.orderId = 1;
	feedOrder.price = 10;
	feedOrder.quantity = 30;
	feedOrder.sequence = 1;
	feedOrder.side = Side::Buy;
	feedOrder.type = UpdateType::Change;

	order = OrderFactory::GenerateOrder(feedOrder);
	order->run(exchangeContext, order);

	feedOrder.clientId = 1;
	feedOrder.entryTimestamp = 103;
	feedOrder.orderId = 1;
	feedOrder.price = 10;
	feedOrder.quantity = 10;
	feedOrder.sequence = 3;
	feedOrder.side = Side::Sell;
	feedOrder.type = UpdateType::Cancel;

	order = OrderFactory::GenerateOrder(feedOrder);
	order->run(exchangeContext, order);

	feedOrder.clientId = 1;
	feedOrder.entryTimestamp = 104;
	feedOrder.orderId = 1;
	feedOrder.price = 10;
	feedOrder.quantity = 10;
	feedOrder.sequence = 4;
	feedOrder.side = Side::Sell;
	feedOrder.type = UpdateType::New;

	order = OrderFactory::GenerateOrder(feedOrder);
	order->run(exchangeContext, order);

	feedOrder.clientId = 1;
	feedOrder.entryTimestamp = 102;
	feedOrder.orderId = 1;
	feedOrder.price = 10;
	feedOrder.quantity = 10;
	feedOrder.sequence = 2;
	feedOrder.side = Side::Buy;
	feedOrder.type = UpdateType::New;

	order = OrderFactory::GenerateOrder(feedOrder);
	order->run(exchangeContext, order);

	BetterFilter bf;
	SideSpecification buy(Side::Buy);


	auto buySideOrders = bf.filter(1, 1, exchangeContext, buy);

	PriceSpecification price(10);

	TimeStampSpecification timeStamp(102);
	
	AndSpecification <std::shared_ptr<IOrder>> buySideAndPrice{ buy, price, timeStamp };

	auto buyOrderAndPrice = bf.filter(1, 1, exchangeContext, buySideAndPrice);

	for (auto & a : buyOrderAndPrice)
	{
		std::cout << "Sequence number " << a->getSequenceNumber() << " "  << "Price " << a->getPrice() << std::endl;
	}

}



int main()
{
	//testFirstOutOfOrderSeq();

	//testASecondOrder();
	//testHashAndMap();

	//testinOrderCancel();

	//testOutOfOrderCancel();

	//testInOrderChange();

	//testShareVolumeBeforeTimeStamp();

	testBetterFilter();

	return 0;
}