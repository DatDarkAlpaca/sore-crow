#pragma once
#include <uuid_v4.h>

namespace sore
{
	inline std::string generateUUID()
	{
		UUIDv4::UUIDGenerator<std::mt19937_64> uuidGenerator;
		UUIDv4::UUID uuid = uuidGenerator.getUUID();

		return uuid.str();
	}
}