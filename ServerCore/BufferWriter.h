#pragma once

//=================================================================================================
// @brief 버퍼 쓰기를 도와주는  클래스
//=================================================================================================

class BufferWriter
{
public:
	BufferWriter();
	BufferWriter(BYTE* buffer, unsigned size, unsigned pos = 0) : _buffer(buffer) , _size(size) , _pos(pos) {}
	~BufferWriter();

	BYTE* Buffer() { return _buffer; }
	unsigned Size() { return _size; }
	unsigned WriteSize() { return _pos; }
	unsigned FreeSize() { return _size - _pos; }

	template<typename T>
	bool Write(T* src) { return Write(src, sizeof(T)); }
	bool Write(void* src, unsigned len);

	template<typename T>
	T* Reserve();

	template<typename T>
	BufferWriter& operator<<(T&& src);

private:
	BYTE* _buffer = nullptr;
	unsigned _size = 0;
	unsigned _pos = 0;
};

template<typename T>
T* BufferWriter::Reserve()
{
	if (FreeSize() < sizeof(T))
		return nullptr;

	T* ret = reinterpret_cast<T*>(&_buffer[_pos]);
	_pos += sizeof(T);
	return ret;
}

template<typename T>
BufferWriter& BufferWriter::operator<<(T&& src)
{
	using nonRefType = std::remove_reference_t<T>;
	*reinterpret_cast<nonRefType*>(&_buffer[_pos]) = std::forward<nonRefType>(src);
	_pos += sizeof(T);
	return *this;
}