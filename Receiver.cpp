#include "Receiver.h"

Receiver::Receiver(ICallback *callback) : callback(callback)
{}

void Receiver::Receive(const char* data, unsigned int size)
{
	if (callback == nullptr)
		throw std::runtime_error("No customer for callback");

	for (auto currentPtr = data; currentPtr < data + size; )
	{
		switch (type)
		{
		case NONE:                // Check if this is new packet:
		{
			switch (type = CheckPacket(currentPtr))
			{
			case TEXT:
				startPacketPtr = currentPtr;
				packetSize = 0;
				break;

			case BINARY:
				++currentPtr;
				startPacketPtr = currentPtr;
				packetSize = 0;
				break;

			default:
				throw std::logic_error("Unknown type of packet");
			}
			break;
		}

		case TEXT:                // Handling the proper text packet
		{
			if (buffer.empty())
			{
				// Data contains the whole packet, we can send it
				if (startPacketPtr + textMarkerSize <= currentPtr && TextIsFinished(currentPtr - textMarkerSize + 1))
				{
					callback->TextPacket(startPacketPtr, packetSize - textMarkerSize + 1);

					currentPtr++;
					packetSize = 0;
					type = NONE;
				}
				// Text packet is not finished but the current block is over
				else if (currentPtr >= data + size - 1)
				{
					buffer.append(startPacketPtr, packetSize);
					currentPtr = data + size;
				}
				// Continue searching the end of the packet
				else
				{
					++packetSize;
					++currentPtr;
				}
			}
			else
			{
				buffer.append(currentPtr, sizeof(char));

				// Buffer contains the whole text packet, we can send it
				if (buffer.size() >= textMarkerSize && TextIsFinished(buffer.data() + buffer.size() - textMarkerSize))
				{
					callback->TextPacket(buffer.data(), buffer.size() - textMarkerSize + 1);

					currentPtr++;
					buffer.shrinkToFit();
					packetSize = 0;
					type = NONE;
				}
				// Continue searching the end of the packet
				else
				{
					++packetSize;
					++currentPtr;
				}
			}
			break;
		}

		case BINARY:              // Handling the proper text packet
		{
			if (buffer.empty())
			{
				// We don't know size of packet yet
				if (packetSize == 0)
				{
					// Size of packet can be read from data
					if (currentPtr + sizeof(BinaryPacketSizeT) <= data + size)
					{
						packetSize = *((BinaryPacketSizeT*)currentPtr);
						currentPtr += sizeof(BinaryPacketSizeT);
					}
					else     // We memorize this piece of packet and wait for the next block
					{
						auto pieceSize = data + size - currentPtr;
						buffer.append(currentPtr, pieceSize);
						currentPtr = data + size;
					}
				}
				// Data flow contains the whole binary packet, we can send it
				else if (currentPtr + packetSize <= data + size)
				{
					callback->BinaryPacket(currentPtr, packetSize);

					currentPtr += packetSize;
					packetSize = 0;
					type = NONE;
					buffer.shrinkToFit();
				}
				// Binary packet is not finished, but the current block is over
				else
				{
					auto pieceSize = data + size - currentPtr;
					buffer.append(currentPtr, pieceSize);
					currentPtr = data + size;
				}
			}
			else        // There's something in the buffer
			{
				// We don't know size of packet
				if (packetSize == 0)
				{
					auto pieceSize = sizeof(BinaryPacketSizeT) - buffer.size();

					buffer.append(currentPtr, pieceSize);
					packetSize = *((BinaryPacketSizeT*)buffer.data());

					buffer.shrinkToFit();
					currentPtr += pieceSize;
				}
				// Buffer contains a part of binary packet
				else
				{
					auto packetLeftSize = packetSize - buffer.size();

					// Binary packet ends within the current block
					if (currentPtr + packetLeftSize <= data + size)
					{
						buffer.append(currentPtr, packetLeftSize);
						callback->BinaryPacket(buffer.data(), buffer.size());

						currentPtr += packetLeftSize;
						packetSize = 0;
						type = NONE;
						buffer.shrinkToFit();
					}
					// Binary packet continues in the next block
					else
					{
						auto pieceSize = data + size - currentPtr;
						buffer.append(currentPtr, pieceSize);
						currentPtr = data + size;
					}
				}
			}
			break;

		default:
			break;
		}
		}
	}
}

Receiver::PacketType Receiver::CheckPacket(const char* data)
{
	return (*data == binaryMarker) ? BINARY : TEXT;
}

bool Receiver::TextIsFinished(const char *data)
{
	for (unsigned int i = 0; i < textMarkerSize; ++i)
		if (data[i] != textMarker[i])
			return false;
	return true;
}

