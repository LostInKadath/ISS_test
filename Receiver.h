#pragma once
#include <stdexcept>
#include "Callback.h"
#include "Buffer.h"


struct IReceiver
{
	virtual void Receive(const char* data, unsigned int size) = 0;
};

struct Receiver : IReceiver
{
	using BinaryPacketSizeT = long;         // 4B

	Receiver(ICallback*);

	void Receive(const char* data, unsigned int size) override;

private:
	enum PacketType { NONE, TEXT, BINARY };

	static const char binaryMarker = 0x24;                // '$'
	static constexpr char textMarker[5] = "\r\n\r\n";
	static const unsigned int textMarkerSize = 4;

	PacketType type = NONE;
	unsigned int packetSize = 0;
	const char *startPacketPtr = nullptr;

	Buffer buffer;
	ICallback *callback;

	PacketType CheckPacket(const char* data);
	bool TextIsFinished(const char *data);
};
