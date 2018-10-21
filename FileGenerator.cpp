#include "FileGenerator.h"

void FileGenerator::GenerateTextFile(const char* filename)
{
	char *flow = new char[flowSize];

	for (unsigned int i = 0; i < flowSize; ++i)
		flow[i] = letters[i % 66];

	for (unsigned int i = packetSize; i < flowSize; i += packetSize)
	{
		flow[i + 0] = '\r';
		flow[i + 1] = '\n';
		flow[i + 2] = '\r';
		flow[i + 3] = '\n';
	}

	std::ofstream out(filename);
	out.write((char*)&*flow, flowSize * sizeof(char));
	out.close();

	delete[] flow;
}

void FileGenerator::GenerateBinaryFile(const char *filename)
{
	std::ofstream out(filename, std::ios::binary);
	char b = 0x24;

	for (unsigned int i = 0; i < flowSize / packetSize; ++i)
	{
		out.write((char*)&b, sizeof(char));
		out.write((char*)&packetSize, sizeof(long));

		for (unsigned int j = 0; j < packetSize; ++j)
			out.write((char*)&letters[j % 66], sizeof(char));
	}
	out.close();
}
