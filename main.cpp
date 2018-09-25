#include <iostream>
#include <fstream>
#include <stdexcept>

#include "Callback.h"
#include "Receiver.h"


struct FineGenerator
{
	const unsigned int blockSize = 1024;
	const unsigned int speed = 1024 * 1024 * 500 / 8;
	const unsigned int packetSize = 800;
	const unsigned int flowSize = speed;
	
	const char *letters = " abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ 0123456789 ";

	void GenerateTextFile(const char* filename)
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

	void GenerateBinaryFile(const char *filename)
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
};

int main(void)
{	
	FineGenerator gen;
//	gen.GenerateTextFile("testdata.txt");
//	gen.GenerateBinaryFile("testdata.bin");

	char *flow = new char[gen.flowSize];

	std::ifstream in("testdata.bin");
	in.read((char*)&*flow, gen.flowSize * sizeof(char));
	in.close();
	

	ICallback *callback = new Callback();
	IReceiver *receiver = new Receiver(callback);

	try
	{
		for (unsigned int i = 0; i < gen.flowSize; i += gen.blockSize)
			receiver->Receive(flow + i, gen.blockSize);
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}
	
	delete[] flow;
	
	system("pause");
	return 0;
}
