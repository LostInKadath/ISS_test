#pragma once
#include <memory>

struct Buffer
{
	Buffer()
		: _data(std::unique_ptr<char[]>(new char[MINSIZE])), _size(0), _capacity(MINSIZE)
	{}

	Buffer(Buffer const& buffer)
		: _data(std::unique_ptr<char[]>(new char[buffer._capacity])), _size(buffer._size), _capacity(buffer._capacity)
	{
		for (unsigned int i = 0; i < _size; ++i)
			_data[i] = buffer._data[i];
	}

	~Buffer() = default;

	unsigned int size() const { return _size; }

	unsigned int capacity() const { return _capacity; }

	bool empty() const { return size() == 0; }

	const char* data() const { return _data.get(); }

	void append(const char *ptr, unsigned int sz)
	{
		if (_size + sz >= _capacity)
			resize(_capacity + MINSIZE);

		for (unsigned int i = 0; i < sz; ++i)
			_data[_size + i] = ptr[i];

		_size += sz;
	}

	void shrinkToFit()
	{
		_capacity = MINSIZE;
		_size = 0;

		auto newData = std::unique_ptr<char[]>(new char[_capacity]);
		_data = std::move(newData);
	}

private:
	static const unsigned int MINSIZE = 1024;

	std::unique_ptr<char[]> _data;
	unsigned int _size = 0;
	unsigned int _capacity = 0;

	void resize(unsigned int newCapacity)
	{
		_capacity = newCapacity;

		auto newData = std::unique_ptr<char[]>(new char[_capacity]);

		for (unsigned int i = 0; i < _size; ++i)
			newData[i] = _data[i];

		_data = std::move(newData);
	}
};
