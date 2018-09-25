#pragma once
#include <iostream>


struct ICallback
{
	virtual void BinaryPacket(const char* data, unsigned int size) = 0;
	virtual void TextPacket(const char* data, unsigned int size) = 0;
};


struct Callback : ICallback
{
	void BinaryPacket(const char *data, unsigned int size)
	{
		std::cout << "BINARY:\n";
		
		for (auto p = data; p != data + size; ++p)
			std::cout << *p;
		
		std::cout << '\n';
	}

	void TextPacket(const char *data, unsigned int size)
	{
		std::cout << "TEXT:\n";

		for (auto p = data; p != data + size; ++p)
			std::cout << *p;

		std::cout << '\n';
	}
};
