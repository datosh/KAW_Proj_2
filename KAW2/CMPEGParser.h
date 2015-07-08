#pragma once

#include "stdafx.h"

class MPEGSlice
{
public:
	MPEGSlice(INT pos, INT slice, std::vector<BYTE>::iterator begin, std::vector<BYTE>::iterator end, BOOL first=FALSE);
	~MPEGSlice();

	void printData();

private:
	BOOL m_firstAfterNewKey;
	INT m_position;
	INT m_sliceNumber;
	std::vector<BYTE> m_data;
};

inline MPEGSlice::MPEGSlice(INT pos, INT slice, std::vector<BYTE>::iterator begin, std::vector<BYTE>::iterator end, BOOL first)
{
	m_firstAfterNewKey = first;
	m_position = pos;
	m_sliceNumber = slice;
	m_data = std::vector<BYTE>(begin, end);
}

MPEGSlice::~MPEGSlice()
{
}

enum MPEGType
{
	sequence_header,
	group_of_picture,
	extention,
	picture,
	slice,
	key_update,
	unvalid_type,
};

class MPEGHeader
{
public:
	MPEGHeader(INT pos, MPEGType type, std::vector<BYTE>::iterator begin, std::vector<BYTE>::iterator end);
	~MPEGHeader();

	void printData(MPEGType type = unvalid_type);

private:
	INT m_position;
	MPEGType m_type;
	std::vector<BYTE> m_data;
};

inline MPEGHeader::MPEGHeader(INT pos, MPEGType type, std::vector<BYTE>::iterator begin, std::vector<BYTE>::iterator end)
{
	m_position = pos;
	m_type = type;
	m_data = std::vector<BYTE>(begin, end);
}

MPEGHeader::~MPEGHeader()
{
}

class MPEG
{
public:
	MPEG(const char * filename);
	~MPEG();

private:
	std::vector<MPEGHeader> m_header;
	std::vector<MPEGSlice> m_slices;
};

MPEG::MPEG(const char * filename)
{

}

MPEG::~MPEG()
{
}