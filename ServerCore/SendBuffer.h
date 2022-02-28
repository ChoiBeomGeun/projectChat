#pragma once
//=================================================================================================
// @brief 전송 버퍼를 위한  클래스
//=================================================================================================
class SendBuffer : enable_shared_from_this<SendBuffer>
{
public:
	SendBuffer(unsigned bufferSize);
	~SendBuffer();

	BYTE* Buffer() { return _buffer.data(); }
	unsigned WriteSize() { return _writeSize; }
	unsigned Capacity() { return static_cast<unsigned>(_buffer.size()); }

	void CopyData(void* data, unsigned len);

private:
	vector<BYTE>	_buffer;
	unsigned			_writeSize = 0;
};

