#include "pch.h"
#include "BufferWriter.h"

BufferWriter::BufferWriter()
{

}

BufferWriter::~BufferWriter()
{

}

bool BufferWriter::Write(void* src, unsigned len)
{
	if (FreeSize() < len)
		return false;

	::memcpy(&_buffer[_pos], src, len);
	_pos += len;
	return true;
}