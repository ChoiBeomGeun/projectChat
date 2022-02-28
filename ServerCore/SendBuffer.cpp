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
// @brief 샌드 버퍼 데이터 복사를 위한 함수
//=================================================================================================
void SendBuffer::CopyData(void* data, unsigned len)
{
	::memcpy(_buffer.data(), data, len);
	_writeSize = len;
}