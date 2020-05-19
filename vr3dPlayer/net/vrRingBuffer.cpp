#include "vrRingBuffer.h"
template<typename T>
vrRingBuffer<T>::vrRingBuffer()
{
}
template<typename T>
vrRingBuffer<T>::~vrRingBuffer()
{
}

template<typename T>
bool vrRingBuffer<T>::vr_ring_buffer_push(const T& data)
{
	return vr_ring_buffer_push_data(std::forward<T>(data));
}

template<typename T>
bool vrRingBuffer<T>::vr_ring_buffer_push(T&& data)
{
	return vr_ring_buffer_push_data(data);
}

template<typename T>
bool vrRingBuffer<T>::vr_ring_buffer_pop(T& data)
{
	if (_numDatas)
	{
		data = std::move(_buffer[_getPos]);
		//data = _buffer[_getPos];
		vr_ring_buffer_add(_getPos);
		_numDatas--;

		return true;
	}

	return false;
}

template<typename T>
bool vrRingBuffer<T>::vr_ring_buffer_is_full()
{
	return _numDatas == _capacity;
}

template<typename T>
bool vrRingBuffer<T>::vr_ring_buffer_is_empty()
{
	return _numDatas == 0;
}

template<typename T>
int vrRingBuffer<T>::vr_ring_buffer_size()
{
	return _numDatas;
}

template<typename T>
void vrRingBuffer<T>::vr_ring_buffer_add(int& pos)
{
	pos = (((pos + 1) == _capacity) ? 0 : (pos + 1));
}


template<typename T>
template<typename U>
inline bool vrRingBuffer<T>::vr_ring_buffer_push_data(U&& data)
{
	if (_numDatas < _capacity)
	{
		_buffer[_putPos] = std::forward<U>(data);
		//_buffer[_putPos] = std::move(data);
		vr_ring_buffer_add(_putPos);
		_numDatas++;

		return true;
	}

	return false;
}