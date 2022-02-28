#pragma once
//=================================================================================================
// @brief ���� ���۸� ����  Ŭ����
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

