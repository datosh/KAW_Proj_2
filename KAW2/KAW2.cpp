// KAW2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>

enum MPEGType
{
	sequence_header,
	group_of_picture,
	extention,
	picture,
	slice,
	key_update,
};

class MPEGDataField
{
public:
	MPEGDataField(INT pos, MPEGType type, std::vector<BYTE>::iterator data_begin, std::vector<BYTE>::iterator data_end);
	~MPEGDataField();

public:
	INT m_position;
	MPEGType m_type;
	std::vector<BYTE> m_data;
};

MPEGDataField::MPEGDataField(INT pos, MPEGType type, std::vector<BYTE>::iterator data_begin, std::vector<BYTE>::iterator data_end)
{
	m_position = pos;
	m_type = type;
	m_data = std::vector<BYTE>(data_begin, data_end);
}

MPEGDataField::~MPEGDataField()
{
}

std::vector<BYTE> readFile(const char* filename)
{
	// open the file:
	std::streampos fileSize;
	std::ifstream file(filename, std::ios::binary);

	// get its size:
	file.seekg(0, std::ios::end);
	fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	// read the data:
	std::vector<BYTE> fileData(fileSize);
	file.read((char*)&fileData[0], fileSize);
	return fileData;
}

std::vector<INT> getHeader(std::vector<BYTE> data)
{
	std::vector<INT> headerPos;

	for (auto i = 0; i < data.size() - 2; ++i)
	{
		if (data[i] == 0x00)
		{
			SHORT buff = data[i + 1] << 8 | data[i + 2];
			if (buff == 0x0001)
			{
				headerPos.push_back(i);
			}
		}
	}

	return headerPos;
}

int _tmain(int argc, _TCHAR* argv[])
{
	std::vector<BYTE> fileData = readFile("recording");
	auto fileDataSize = fileData.size();
	auto headerPos = getHeader(fileData);

	for (auto i = 0; i < 20; ++i)
	{
		std::cout << headerPos[i] << std::endl;
	}

	std::vector<MPEGDataField> MPEGdata;
	for (auto pos : headerPos)
	{
		auto type = fileData[pos + 3];
		switch (type)
		{
		case 0xB3:
			MPEGdata.push_back(MPEGDataField(pos, sequence_header, fileData.begin() + pos, fileData.begin() + pos + 12));
			break;
		case 0xB5:
			MPEGdata.push_back(MPEGDataField(pos, extention, fileData.begin() + pos, fileData.begin() + pos + 9));
			break;
		case 0xB8:
			MPEGdata.push_back(MPEGDataField(pos, group_of_picture, fileData.begin() + pos, fileData.begin() + pos + 7));
			break;
		case 0xB2:
			if (fileData[pos + 1] != 0xFF)
				break;
			MPEGdata.push_back(MPEGDataField(pos, key_update, fileData.begin() + pos, fileData.begin() + pos + 4));
			break;
		case 0x00:
			MPEGdata.push_back(MPEGDataField(pos, picture, fileData.begin() + pos, fileData.begin() + pos + 7));
			break;
		default:
			break;
		}
	}

	std::cout << MPEGdata.size() << std::endl;

	for (auto data : MPEGdata)
	{
		if (data.m_type == sequence_header)
		{
			for (auto b : data.m_data)
			{
				std::cout << std::hex << (int)b << " ";
			}
			std::cout << std::endl;
		}
	}

	getchar();

	return 0;
}

