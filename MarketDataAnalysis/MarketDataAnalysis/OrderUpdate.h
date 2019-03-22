#pragma once

#include <cstdint>
enum class UpdateType : uint64_t {
	New = 0,
	Change = 1,
	Cancel = 2
};

enum class Side : uint64_t {
	Buy = 0,
	Sell = 1
};

struct OrderUpdate {
	uint64_t sequence;
	UpdateType type;
	uint64_t clientId;
	uint64_t orderId;
	uint64_t entryTimestamp;
	Side 	  side;
	uint64_t price;
	uint64_t quantity;
};
