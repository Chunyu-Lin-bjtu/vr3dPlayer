#ifndef _VR_RING_BUFFER_H_
#define _VR_RING_BUFFER_H_

#include <vector>	//for std::vector
#include <atomic>	//for std::atomic_int

template <typename T>
class vrRingBuffer
{
public:
	vrRingBuffer();
	~vrRingBuffer();

	//添加数据，左值
	bool vr_ring_buffer_push(const T& data);

	//添加数据，右值
	bool vr_ring_buffer_push(T&& data);

	//弹出数据
	bool vr_ring_buffer_pop(T& data);

	//判断环形缓冲区是否满
	bool vr_ring_buffer_is_full();

	//判断环形缓冲区是否空
	bool vr_ring_buffer_is_empty();

	//获取环形缓冲区内元素个数
	int vr_ring_buffer_size();

private:
	template <typename U>
	bool vr_ring_buffer_push_data(U&& data);

	//待插入数据位置递增1
	void vr_ring_buffer_add(int& pos);

	int _capacity = 0;
	int _putPos = 0;
	int _getPos = 0;

	std::vector<T> _buffer;
	std::atomic_int _numDatas;
};

#endif // !_VR_RING_BUFFER_H_
