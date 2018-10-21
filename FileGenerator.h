#pragma once
#include <fstream>

struct FileGenerator
{
	const unsigned int blockSize = 1024;
	const unsigned int speed = 1024 * 1024 * 500 / 8;
	const unsigned int packetSize = 800;
	const unsigned int flowSize = speed;

	const char *letters = " abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ 0123456789 ";

	void GenerateTextFile(const char* filename);
	void GenerateBinaryFile(const char *filename);
};
