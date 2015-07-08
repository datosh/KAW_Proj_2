// KAW2.cpp : Defines the entry point for the console application.
//

#include "CMPEGParser.h"
#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	MPEG mpeg("recording_small");
	
	std::cout << "MPEG object successfully created!" << std::endl;

	/*auto times = mpeg.getHeaders();
	for (auto t : times)
	{
		t.printData();
	}*/

	for (auto t : mpeg.getSlices())
	{
		t.printData();
	}

	// auto attack_vectors = mpeg.getStuffedBytesPerFrame();

	getchar();

	return 0;
}

