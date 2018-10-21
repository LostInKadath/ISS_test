#include <iostream>
#include <fstream>
#include <stdexcept>

#include "Callback.h"
#include "Receiver.h"
#include "FileGenerator.h"


int main(void)
{	
	FileGenerator gen;
//	gen.GenerateTextFile("testdata.txt");
//	gen.GenerateBinaryFile("testdata.bin");

	char *flow = new char[gen.flowSize];

	std::ifstream in("testdata.bin");
	in.read((char*)&*flow, gen.flowSize * sizeof(char));
	in.close();
	

	ICallback *callback = new Callback();
	IReceiver *receiver = new Receiver(callback);

	try {
		for (unsigned int i = 0; i < gen.flowSize; i += gen.blockSize)
			receiver->Receive(flow + i, gen.blockSize);
	}
	catch (std::exception &e) {
		std::cerr << e.what() << '\n';
	}
	
	delete[] flow;
	
	system("pause");
	return 0;
}
