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

	//������ݣ���ֵ
	bool vr_ring_buffer_push(const T& data);

	//������ݣ���ֵ
	bool vr_ring_buffer_push(T&& data);

	//��������
	bool vr_ring_buffer_pop(T& data);

	//�жϻ��λ������Ƿ���
	bool vr_ring_buffer_is_full();

	//�жϻ��λ������Ƿ��
	bool vr_ring_buffer_is_empty();

	//��ȡ���λ�������Ԫ�ظ���
	int vr_ring_buffer_size();

private:
	template <typename U>
	bool vr_ring_buffer_push_data(U&& data);

	//����������λ�õ���1
	void vr_ring_buffer_add(int& pos);

	int _capacity = 0;
	int _putPos = 0;
	int _getPos = 0;

	std::vector<T> _buffer;
	std::atomic_int _numDatas;
};

#endif // !_VR_RING_BUFFER_H_
