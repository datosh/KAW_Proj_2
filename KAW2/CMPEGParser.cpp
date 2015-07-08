#include "CMPEGParser.h"

/* 
 * See spec for how these data were extracted: 
 * http://dvd.sourceforge.net/dvdinfo/mpeghdrs.html#seq
 */
void MPEGHeader::printData()
{
	if (m_type == group_of_picture)
	{
		INT hour = (m_data[0] >> 2) & 0x1F;
		INT minute = ((m_data[0] & 0x03) << 4) | (m_data[1] >> 4) & 0x0F;
		INT second = ((m_data[1] & 0x07) << 3) | (m_data[2] >> 5) & 0x07;
		std::cout
			<< "Time of GOP@" << m_position
			<< " " << hour << ":" << minute << ":" << second
			<< std::endl;
	}
	else if (m_type == key_update)
	{
		std::cout << "Key Update" << std::endl;
	}
	
}

inline BOOL MPEGHeader::is(MPEGType type)
{
	return m_type == type;
}

void MPEGSlice::printData()
{
	INT lineBreakCntr = 0;
	for (auto b : m_data)
	{
		std::cout << std::hex << (int)b << " ";
		lineBreakCntr += 1;
		if (lineBreakCntr == 0xF) {
			std::cout << std::endl;
			lineBreakCntr = 0;
		}
	}
	std::cout << std::endl;
}

std::vector<MPEGHeader> MPEG::getHeaderByType(MPEGType type)
{
	std::vector<MPEGHeader> result;

	for (auto data : m_header)
	{
		if (data.is(type))
		{
			result.push_back(data);
		}
	}

	return result;
}


std::vector<std::vector<BYTE>> getChunks(std::vector<std::vector<BYTE>> input)
{
	std::vector<std::vector<BYTE>> results;

	for (auto i : input)
	{
		if (i.size() < 16)
		{
			continue;
		}

		while (true)
		{
			//results.push_back(i.begin(), i.begin() + 16);
		}
	}
}

std::vector<BYTE> getMatch(std::vector<std::vector<BYTE>> intermediate)
{

	return std::vector<BYTE>();
}

std::vector<std::vector<BYTE>> MPEG::getStuffedBytesPerFrame()
{

	std::vector<std::vector<BYTE>> intermediate;
	std::vector<std::vector<BYTE>> attackVectors;

	for (auto slice : m_slices)
	{
		// Process the intermediate list when a delimited is found
		if (slice.isFirstAfterKey())
		{
			attackVectors.push_back(getMatch(intermediate));
			intermediate.clear();
		}

		// Save the last 16 bytes of data
		auto data = slice.getData();
		intermediate.push_back(std::vector<BYTE>(data.begin(), data.end()));
	}

	return attackVectors;
}

/*
 * Finds all occurences of the bytes string 0x00 0x00 0x01
 * and saves the indexes into the data vector in a vector.
 */
std::vector<INT> getHeader(std::vector<BYTE> data)
{
	INT ignoreLastNBytes = 100;

	std::vector<INT> headerPos;

	for (auto i = 0; i < data.size() - ignoreLastNBytes; ++i)
	{
		if (data[i] == 0x00)
		{
			SHORT buff = data[i + 1] << 8 | data[i + 2];
			if (buff == 0x0001)
			{
				headerPos.push_back(i);
				i += 5;					// TODO: determine better metric to skip bytes
			}
		}
	}

	return headerPos;
}

MPEG::MPEG(const char * filename)
{
	// open the file
	std::ifstream file(filename, std::ios::binary);

	if (!file)
	{
		std::cerr << "UNABLE TO OPEN FILE: " << filename << std::endl;
		exit(-1);
	}

	// get the size
	std::streampos fileSize;
	file.seekg(0, std::ios::end);
	fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	// read the data in array
	m_data = std::vector<BYTE>(fileSize);
	file.read((char*)&m_data[0], fileSize);

	std::cout << "File fully read into memory!" << std::endl;

	auto headerPos = getHeader(m_data);

	std::cout << "Header positions (" << headerPos.size() << ") in file data found!" << std::endl;

	// Used to flag a slice as the first with a new key
	BOOL nextKey = false;
	// Ignore the broken bytes at the beginning. Start after the first Seq header was found
	BOOL foundSeqHeader = false;
	for (std::vector<INT>::size_type i = 0; i != headerPos.size() - 1; i++)
	{
		auto pos = headerPos[i];

		auto type = m_data[pos + 3];
		
		// Save all the header data.
		switch (type)
		{
		case 0xB3:
			m_header.push_back(MPEGHeader(pos, sequence_header, m_data.begin() + pos + 4, m_data.begin() + pos + 12));
			foundSeqHeader = true;
			break;
		case 0xB5:
			m_header.push_back(MPEGHeader(pos, extention, m_data.begin() + pos + 4, m_data.begin() + pos + 9));
			break;
		case 0xB8:
			m_header.push_back(MPEGHeader(pos, group_of_picture, m_data.begin() + pos + 4, m_data.begin() + pos + 7));
			break;
		case 0xB2:
			if (m_data[pos + 4] == 0xFF)
			{
				m_header.push_back(MPEGHeader(pos, key_update, m_data.begin() + pos + 4, m_data.begin() + pos + 4));
				nextKey = true;
			}
			break;
		case 0x00:
			m_header.push_back(MPEGHeader(pos, picture, m_data.begin() + pos + 4, m_data.begin() + pos + 7));
			break;
			// Save the slice
		default:
			if (foundSeqHeader)
			{
				m_slices.push_back(MPEGSlice(pos, m_data[pos + 4], m_data.begin() + pos + 4, m_data.begin() + headerPos[i+1], nextKey));
				nextKey = false;
			}
			break;
		}
	}
}

MPEG::~MPEG()
{
}