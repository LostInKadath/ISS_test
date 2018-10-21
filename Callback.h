#pragma once
#include <iostream>


struct ICallback
{
	virtual void BinaryPacket(const char* data, unsigned int size) = 0;
	virtual void TextPacket(const char* data, unsigned int size) = 0;
};

struct Callback : ICallback
{
	void BinaryPacket(const char *data, unsigned int size) override;
	void TextPacket(const char *data, unsigned int size) override;
};
