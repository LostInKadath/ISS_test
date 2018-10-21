#pragma once
#include <memory>

struct Buffer
{
	Buffer();
	Buffer(Buffer const& buffer);
	~Buffer() = default;

	unsigned int size() const;
	unsigned int capacity() const;

	bool empty() const;

	const char* data() const;

	void append(const char *ptr, unsigned int sz);
	void shrinkToFit();

private:
	static const unsigned int MINSIZE = 1024;

	std::unique_ptr<char[]> _data;
	unsigned int _size = 0;
	unsigned int _capacity = 0;

	void resize(unsigned int newCapacity);
};
