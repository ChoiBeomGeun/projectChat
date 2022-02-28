#include "pch.h"
#include "SendBuffer.h"

SendBuffer::SendBuffer(unsigned bufferSize)
{
	_buffer.resize(bufferSize);
}

SendBuffer::~SendBuffer()
{
}
//=================================================================================================
// @brief ���� ���� ������ ���縦 ���� �Լ�
//=================================================================================================
void SendBuffer::CopyData(void* data, unsigned len)
{
	::memcpy(_buffer.data(), data, len);
	_writeSize = len;
}