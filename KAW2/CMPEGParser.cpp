#include "CMPEGParser.h"

/* 
 * See spec for how these data were extracted: 
 * http://dvd.sourceforge.net/dvdinfo/mpeghdrs.html#seq
 */
void MPEGHeader::printData(MPEGType type)
{
	if (type == group_of_picture)
	{
		INT hour = (m_data[0] >> 2) & 0x1F;
		INT minute = ((m_data[0] & 0x03) << 4) | (m_data[1] >> 4) & 0x0F;
		INT second = ((m_data[1] & 0x07) << 3) | (m_data[2] >> 5) & 0x07;
		std::cout
			<< "Time of GOP@" << m_position
			<< " " << hour << ":" << minute << ":" << second;
	}
	else if (type == key_update)
	{
		std::cout << "Key Update" << std::endl;
	}
	
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
		}
	}
	std::cout << std::endl;
}