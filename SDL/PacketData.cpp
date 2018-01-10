#include "stdafx.h"
#include "PacketData.h"


PacketData::PacketData()
	: m_readPos(0)
{
}


PacketData::~PacketData()
{
}

void PacketData::append(const void * data, std::size_t sizeInBytes)
{
	if (data != NULL && sizeInBytes > 0)
	{
		std::size_t start = m_data.size();
		m_data.resize(start + sizeInBytes);
		memcpy(&m_data[start], data, sizeInBytes);
	}
}


std::size_t PacketData::getDataSize() const
{
	return m_data.size();
}

char * PacketData::getData()
{
	return &m_data[0];
}

PacketData & PacketData::operator >> (Uint8 & data)
{
	if (checkSize(sizeof(data)))
	{
		data = *reinterpret_cast<const Uint8*>(&m_data[m_readPos]);
		m_readPos += sizeof(data);
	}
	return *this;
}

PacketData & PacketData::operator<<(Uint8 data)
{
	append(&data, sizeof(data));
	return *this;
}

PacketData & PacketData::operator >> (Uint16 & data)
{
	if (checkSize(sizeof(data)))
	{
		data = *reinterpret_cast<const Uint16*>(&m_data[m_readPos]);
		m_readPos += sizeof(data);
	}

	return *this;
}

PacketData & PacketData::operator<<(Uint16 data)
{
	Uint16 toWrite = data;
	append(&toWrite, sizeof(toWrite));
	return *this;
}

PacketData & PacketData::operator >> (Uint32 & data)
{
	if (checkSize(sizeof(data)))
	{
		data = (*reinterpret_cast<const Uint32*>(&m_data[m_readPos]));
		m_readPos += sizeof(data);
	}
	return *this;
}

PacketData & PacketData::operator<<(Uint32 data)
{
	Uint32 toWrite = data;
	append(&toWrite, sizeof(toWrite));
	return *this;
}

PacketData & PacketData::operator >> (std::string & data)
{
	// First extract string length
	Uint32 length = 0;
	*this >> length;

	data.clear();
	if ((length > 0) && checkSize(length))
	{
		// Then extract characters
		data.assign(&m_data[m_readPos], length);

		// Update reading position
		m_readPos += length;
	}
	return *this;
}

PacketData & PacketData::operator << (std::string & data)
{
	// First insert string length
	Uint32 length = static_cast<Uint32>(data.size());
	*this << length;

	// Then insert characters
	if (length > 0)
	{
		append(data.c_str(), length * sizeof(std::string::value_type));
	}
	return *this;
}

PacketData & PacketData::operator >> (int & data)
{
	if (checkSize(sizeof(data)))
	{
		data = *reinterpret_cast<const int*>(&m_data[m_readPos]);
		m_readPos += sizeof(data);
	}
	return *this;
}

PacketData & PacketData::operator<<(int data)
{
	append(&data, sizeof(data));
	return *this;
}

PacketData & PacketData::operator >> (float & data)
{
	if (checkSize(sizeof(data)))
	{
		data = *reinterpret_cast<const float*>(&m_data[m_readPos]);
		m_readPos += sizeof(data);
	}
	return *this;
}

PacketData & PacketData::operator<<(float data)
{
	append(&data, sizeof(data));
	return *this;
}

PacketData & PacketData::operator >> (double & data)
{
	if (checkSize(sizeof(data)))
	{
		data = *reinterpret_cast<const double*>(&m_data[m_readPos]);
		m_readPos += sizeof(data);
	}
	return *this;
}

PacketData & PacketData::operator<<(double data)
{
	append(&data, sizeof(data));
	return *this;
}

void PacketData::clear()
{
	m_data.clear();
	m_readPos = 0;
}

bool PacketData::checkSize(std::size_t size)
{
	if (m_readPos + size <= m_data.size())
	{
		return true;
	}
	return false;
}
