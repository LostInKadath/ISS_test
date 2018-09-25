#pragma once
#include <stdexcept>
#include "Callback.h"


struct IReceiver
{
	virtual void Receive(const char* data, unsigned int size) = 0;
};


struct Receiver : IReceiver
{
	using BinaryPacketSizeT = long;         // 4B

	Receiver(ICallback *callback) : callback(callback)
	{}

	void Receive(const char* data, unsigned int size)
	{
		if (callback == nullptr)
		    throw std::exception("No customer for callback");

		for (auto currentPtr = data; currentPtr < data + size; )
		{
			// Check if this is new packet:
			if (type == NONE)
			{
				switch (type = CheckPacket(currentPtr))
				{
				case TEXT:
					packetSize = 0;
					break;

				case BINARY:
					++currentPtr;

					// Check if we can read packetSize now or shall wait for the next block
					if (currentPtr + sizeof(BinaryPacketSizeT) <= data + size)
					{
						packetSize = *((BinaryPacketSizeT*)currentPtr);
						currentPtr += sizeof(BinaryPacketSizeT);
					}
					break;

				default:
					throw std::exception("Unknown type of packet");
					break;
				}

				startPacketPtr = currentPtr;
			}

			// Handle the proper packet
			switch (type)
			{
			case TEXT:
				if (startPacketPtr + textMarkerSize <= currentPtr && TextIsFinished(currentPtr - textMarkerSize))
				{
					callback->TextPacket(startPacketPtr, packetSize - textMarkerSize);

					packetSize = 0;
					type = NONE;
				}
				else
				{
					++packetSize;
					++currentPtr;
				}
				break;

			case BINARY:
				// We don't know packetSize yet
				if (packetSize == 0)
				{
					// If we cannot read packetSize and have to wait for the next block
					if (startPacketPtr + sizeof(BinaryPacketSizeT) > data + size)
						currentPtr = data + size;
					else
					{
						packetSize = *((BinaryPacketSizeT*)startPacketPtr);
						currentPtr = startPacketPtr + sizeof(BinaryPacketSizeT);
						startPacketPtr = currentPtr;
					}
				}

				// We know packetSize but part of data is in the next block
				else if (startPacketPtr + packetSize > data + size)
					currentPtr = data + size;

				// We know packetSize and can send the whole packet
				else
				{
					callback->BinaryPacket(startPacketPtr, packetSize);

					currentPtr += packetSize - (currentPtr - startPacketPtr);
					packetSize = 0;
					type = NONE;
				}
				break;

			default:
				break;
			}
		}
	}

private:
	enum PacketType { NONE, TEXT, BINARY };

	static const char binaryMarker = 0x24;                // '$'
	static constexpr char textMarker[5] = "\r\n\r\n";
	static const unsigned int textMarkerSize = 4;

	PacketType type = NONE;
	unsigned int packetSize = 0;
	const char *startPacketPtr = nullptr;

	ICallback *callback;


	PacketType CheckPacket(const char* data)
	{
		return (*data == binaryMarker) ? BINARY : TEXT;
	}

	bool TextIsFinished(const char *data)
	{
		for (unsigned int i = 0; i < textMarkerSize; ++i)
			if (data[i] != textMarker[i])
				return false;
		return true;
	}
};
