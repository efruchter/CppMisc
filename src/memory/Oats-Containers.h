#pragma once

#include <assert.h>
#include <memory>

namespace oats
{
	inline const int index1d(const int col, const int row, const int columnCount)
	{
		return col + (row * columnCount);
	}

	template <class DATA_TYPE, int CAPACITY>
	struct array_c
	{
		DATA_TYPE _buffer[CAPACITY];
		const int _capacity = CAPACITY;

		array_c(const int _) { }
		array_c() { }

		inline const int size() const
		{
			return CAPACITY;
		}

		inline const DATA_TYPE operator[](const int index) const
		{
			return _buffer[index];
		}

		inline DATA_TYPE& operator[](const int index)
		{
			return _buffer[index];
		}

		inline DATA_TYPE* ptr() const
		{
			return _buffer;
		}
	};

	template <class DATA_TYPE>
	struct dynarray
	{
		const int EXPANSION_FACTOR = 2;

		std::unique_ptr<DATA_TYPE[]> _buffer;
		int _size;
		int _capacity;

		dynarray(const int initialCapacity)
		{
			_buffer = std::unique_ptr<DATA_TYPE[]>(new DATA_TYPE[initialCapacity]);
			_size = 0;
			_capacity = initialCapacity;
		}

		void insert(const int index, const DATA_TYPE t)
		{
			int topIndex = index > _size ? index : _size;
			if (topIndex > _capacity - 1)
			{
				int cap = _capacity > 0 ? _capacity : 1;
				while (cap < topIndex + 1)
					cap *= EXPANSION_FACTOR;
				reserve(cap);
			}

			for (int i = _size; i > index; i--)
				_buffer[i] = _buffer[i - 1];

			_buffer[index] = t;
			_size = topIndex + 1;
		}

		inline void push_back(DATA_TYPE t)
		{
			insert(_size, t);
		}

		inline void push_front(DATA_TYPE t)
		{
			insert(0, t);
		}

		inline const int size() const
		{
			return _size;
		}

		inline const DATA_TYPE operator[](const int index) const
		{
			return _buffer[index];
		}

		inline DATA_TYPE& operator[](const int index)
		{
			return _buffer[index];
		}

		inline DATA_TYPE* ptr() const
		{
			return _buffer;
		}

		void reserve(const int capacity)
		{
			if (_capacity >= capacity)
				return;

			auto temp = std::unique_ptr<DATA_TYPE[]>(new DATA_TYPE[capacity]);

			for (int i = 0; i < _capacity; i++)
				temp[i] = _buffer[i];

			_buffer = std::move(temp);
			_capacity = capacity;
		}
	};

	template <typename DATA_TYPE, typename CONTAINER_TYPE>
	struct ringbuffer
	{
		CONTAINER_TYPE _buffer;
		int _count;
		int _dequeue_index;

		ringbuffer() : _buffer(0)
		{
			_count = 0;
			_dequeue_index = 0;
		}

		ringbuffer(const int capacity) : _buffer(capacity)
		{
			_count = 0;
			_dequeue_index = 0;
		}

		inline const int abs_index(const int relative_index) const
		{
			return (_dequeue_index + relative_index) % _buffer._capacity;
		}

		inline const int size() const
		{
			return _count;
		}

		inline const int capacity() const
		{
			return _buffer._capacity;
		}

		inline const DATA_TYPE operator[](const int index) const
		{
			return _buffer[abs_index(index)];
		}

		inline DATA_TYPE& operator[](const int index)
		{
			return _buffer[abs_index(index)];
		}

		DATA_TYPE dequeue()
		{
			DATA_TYPE t = _buffer[_dequeue_index];
			_dequeue_index = abs_index(1);
			_count--;
			return t;
		}

		void enqueue(const DATA_TYPE d)
		{
			if (_count == capacity())
				dequeue();
			_buffer[abs_index(_count)] = d;
			_count++;
		}
	};

	template<typename DATA_TYPE> using ringbuffer_heap = ringbuffer<DATA_TYPE, dynarray<DATA_TYPE>>;
	template<typename DATA_TYPE, int SIZE> using ringbuffer_c = ringbuffer<DATA_TYPE, array_c<DATA_TYPE, SIZE>>;
}

