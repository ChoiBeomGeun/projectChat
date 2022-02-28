#pragma once

//=================================================================================================
// @brief 버퍼 읽기를 도와주는  클래스
//=================================================================================================


class BufferReader
{
public:
	BufferReader();
	BufferReader(BYTE* buffer, unsigned size, unsigned pos = 0);
	~BufferReader();

	BYTE* Buffer() { return _buffer; }
	unsigned Size() { return _size; }
	unsigned ReadSize() { return _pos; }
	unsigned FreeSize() { return _size - _pos; }

	template<typename T>
	bool Peek(T* dest) { return Peek(dest, sizeof(T)); }
	bool Peek(void* dest, unsigned len);

	template<typename  T>
	bool Read(T* dest) { return Read(dest, sizeof(T)); }
	bool Read(void* dest, unsigned len);




	template <typename T>
	BufferReader& operator>>(OUT T& dest);
private:
	BYTE* _buffer = nullptr;
	unsigned _size = 0;
	unsigned _pos = 0;
};

template <typename T>
BufferReader& BufferReader::operator>>(T& dest)
{
	dest = *reinterpret_cast<T*>(&_buffer[_pos]);

	_pos += sizeof(T);

	return *this;
}

