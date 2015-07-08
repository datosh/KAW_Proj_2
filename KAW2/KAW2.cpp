// KAW2.cpp : Defines the entry point for the console application.
//

#include "CMPEGParser.h"
#include "stdafx.h"

void printVectors(std::vector<std::vector<BYTE>> vectors)
{
	for (auto vector : vectors)
	{
		for (int i = 0; i < 0xF; ++i)
		{
			std::cout << std::hex << std::setw(2) << std::setfill('0') << int(vector[i]) << " ";
		}
		std::cout << std::endl;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	MPEG mpeg("workshop2_stream.m2v");
	
	std::cout << "MPEG object successfully created!" << std::endl;

	/*auto times = mpeg.getHeaders();
	for (auto t : times)
	{
		t.printData();
	}*/


	// print out the time between key updates
	for (auto t : mpeg.getSlices())
	{
		t.printData();
	}

	// print out the attack vectors
	auto attack_vectors = mpeg.getStuffedBytesPerFrame();
	printVectors(attack_vectors);

	getchar();

	return 0;
}

