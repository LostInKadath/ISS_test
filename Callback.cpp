#include "Callback.h"

void Callback::BinaryPacket(const char *data, unsigned int size)
{
	std::cout << "BINARY:\n";

	for (auto p = data; p != data + size; ++p)
		std::cout << *p;

	std::cout << '\n';
}

void Callback::TextPacket(const char *data, unsigned int size)
{
	std::cout << "TEXT:\n";

	for (auto p = data; p != data + size; ++p)
		std::cout << *p;

	std::cout << '\n';
}