#pragma once
#include <stdexcept>

template <typename T>
class vector
{
public:

	class const_iterator {
	public:
		const_iterator();
		const_iterator(const T* elem);

		const_iterator& operator++();
		const_iterator& operator++(int);
		const_iterator& operator--();

		operator const T*() const;

	private:
		const T* elem;
	};

	class iterator {
	public:
		iterator();
		iterator(T* elem);

		iterator& operator++();
		iterator& operator--();

		operator const_iterator();

		operator T*();

	private:
		T* elem;
	};

	vector();

	vector(vector&& another);

	vector& operator=(vector&& another);

	vector(const vector& another);

	vector& operator = (const vector& another);

	~vector();

	size_t size() const;

	size_t capacity() const;

	bool empty() const;

	void push_back(const T& value);

	void pop_back();

	void reserve(size_t capacity);

	void resize(size_t size);

	void clear();

	bool erase(const T& value);

	T& operator[] (size_t index);

	const T& operator[] (size_t index) const;

	const_iterator begin() const;

	const_iterator end() const;

	iterator begin();

	iterator end();

private:
	T* data;
	size_t _size;
	size_t _capacity;

	void copy(const vector& another);
};

template<typename T>
inline vector<T>::iterator::iterator() {}

template<typename T>
inline vector<T>::iterator::iterator(T * elem) {
	this->elem = elem;
}

template<typename T>
inline vector<T>::iterator::operator T*() {
	return elem;
}


template<typename T>
inline typename vector<T>::iterator& vector<T>::iterator::operator++() {
	elem++;
	return *this;
}

template<typename T>
inline typename vector<T>::iterator& vector<T>::iterator::operator--() {
	elem--;
	return *this;
}

template<typename T>
inline vector<T>::iterator::operator typename vector<T>::const_iterator() { 
	return const_iterator(elem); 
}

template<typename T>
inline vector<T>::const_iterator::const_iterator() {}

template<typename T>
inline vector<T>::const_iterator::const_iterator(const T * i_elem)
	: elem(i_elem)
{
}

template<typename T>
inline vector<T>::const_iterator::operator const T*() const {
	return elem;
}

template<typename T>
inline typename vector<T>::const_iterator& vector<T>::const_iterator::operator++() {
	++elem;
	return *this;
}

template<typename T>
inline typename vector<T>::const_iterator& vector<T>::const_iterator::operator++(int) {
	++elem;
	return *this;
}

template<typename T>
inline typename vector<T>::const_iterator& vector<T>::const_iterator::operator--() {
	elem--;
	return *this;
}

template<typename T>
inline vector<T>::vector()
{
	_size = 0;
	_capacity = 0;
	data = nullptr;
}

template<typename T>
inline vector<T>::vector(vector && another)
{
	_size = another._size;
	_capacity = another._capacity;
	data = another.data;
	another.data = nullptr;
}

template<typename T>
inline vector<T>& vector<T>::operator=(vector&& another)
{
	if (&another != this) {
		delete[] data;
		_size = another._size;
		_capacity = another._capacity;
		data = another.data;
		another.data = nullptr;
	}
	return *this;
}

template<typename T>
inline vector<T>::vector(const vector & another) {
	copy(another);
}

template<typename T>
inline vector<T>& vector<T>::operator = (const vector& another) {
	copy(another);
	return *this;
}

template<typename T>
inline vector<T>::~vector()
{
	delete[] data;
}

template<typename T>
inline size_t vector<T>::size() const
{
	return _size;
}

template<typename T>
inline size_t vector<T>::capacity() const
{
	return _capacity;
}

template<typename T>
inline bool vector<T>::empty() const {
	return _size == 0;
}

template<typename T>
inline void vector<T>::push_back(const T & value) {
	if (_size + 1 > _capacity) {
		reserve((_size + 1) * 2);
	}
	data[_size] = value;
	_size++;
}

template<typename T>
inline void vector<T>::pop_back() {
	_size--;
}

template<typename T>
inline void vector<T>::reserve(size_t capacity) {
	this->_capacity = capacity;
	T* newArr = new T[capacity];
	if (_size > capacity) {
		_size = capacity;
	}
	for (size_t i = 0; i < _size; i++) {
		newArr[i] = data[i];
	}
	T* toDelete = data;
	data = newArr;
	if (toDelete) {
		delete[] toDelete;
	}
}

template<typename T>
inline void vector<T>::resize(size_t size) {
	if (size > _capacity) {
		reserve(size);
	}
	this->_size = size;
}

template<typename T>
inline void vector<T>::clear() {
	_size = 0;
	_capacity = 0;
	delete[] data;
}

template<typename T>
inline bool vector<T>::erase(const T & value) {
	int index = -1;
	for (size_t i = 0; i < _size; i++) {
		if (data[i] == value) {
			index = i;
		}
	}
	if (index != -1) {
		_size--;
		for (size_t i = index; i < _size; i++) {
			data[i] = data[i + 1];
		}
		return true;
	}
	return false;
}

template<typename T>
inline T& vector<T>::operator[] (size_t index)
{
	return const_cast<T&>(static_cast<const vector*>(this)->operator[](index));
}

template<typename T>
inline const T& vector<T>::operator[] (size_t index) const
{
#ifdef _DEBUG
	if (index >= _size) {
		throw std::out_of_range("vector index out of bounds");
	}
	else {
		return data[index];
	}
#else
	return data[index];
#endif
}



template<typename T>
inline typename vector<T>::const_iterator vector<T>::begin() const {
	return const_iterator(data);
}

template<typename T>
inline typename vector<T>::const_iterator vector<T>::end() const {
	return const_iterator(data + _size);;
}

template<typename T>
inline typename vector<T>::iterator vector<T>::begin() {
	return iterator(data);
}

template<typename T>
inline typename vector<T>::iterator vector<T>::end() {
	return iterator(data + _size);
}

template<typename T>
inline void vector<T>::copy(const vector & another) {
	_size = another._size;
	_capacity = another._capacity;
	data = new T[_capacity];
	for (size_t i = 0; i < _size; i++) {
		data[i] = another[i];
	}
}
