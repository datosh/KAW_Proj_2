#pragma once

#include "stdafx.h"

class MPEGSlice
{
public:
	MPEGSlice(INT pos, INT slice, std::vector<BYTE>::iterator begin, std::vector<BYTE>::iterator end, BOOL first=FALSE);
	~MPEGSlice();

	void printData();
	std::vector<BYTE> getData() { return m_data; }
	BOOL isFirstAfterKey() { return m_firstAfterNewKey; }

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

inline MPEGSlice::~MPEGSlice()
{
}

enum MPEGType
{
	sequence_header,
	group_of_picture,
	extention,
	picture,
	key_update,
};

class MPEGHeader
{
public:
	MPEGHeader(INT pos, MPEGType type, std::vector<BYTE>::iterator begin, std::vector<BYTE>::iterator end);
	~MPEGHeader();

	void printData();
	BOOL is(MPEGType type);

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

inline MPEGHeader::~MPEGHeader()
{
}

class MPEG
{
public:
	MPEG(const char * filename);
	~MPEG();

	std::vector<MPEGHeader> getHeaderByType(MPEGType type);
	std::vector<MPEGHeader> getHeaders() { return m_header; }
	std::vector<MPEGSlice> getSlices() { return m_slices; }
	std::vector<std::vector<BYTE>> getStuffedBytesPerFrame();

private:
	std::vector<BYTE> m_data;
	std::vector<MPEGHeader> m_header;
	std::vector<MPEGSlice> m_slices;
};

std::vector<INT> getHeader(std::vector<BYTE> data);