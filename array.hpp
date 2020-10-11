#ifdef ARRAY_HPP
#define ARRAY_HPP
#include "types.hpp"

namespace mylib
{
template<typename T, uint64 CAPACITY>
class array
{
	T _buffer[CAPACITY];
public:
	class iterator
	{
		T* ptr;
		constexpr iterator(const iterator& rhs) noexcept : ptr(rhs.ptr){}
		constexpr iterator(iterator&& rhs) noexcept : ptr(rhs.ptr){ rhs.ptr = nullptr;}
		constexpr iterator(T* ptr) noexcept : ptr(ptr){}

	public:
		constexpr iterator() noexcept : ptr(nullptr){}

		constexpr iterator& operator=(const iterator& rhs){ ptr = rhs.ptr; return *this;}
		constexpr iterator& operator=(iterator&& rhs){ ptr = rhs.ptr; return *this;}

		bool operator==(const iterator& rhs) const noexcept { return ptr == rhs.ptr;}
		bool operator!=(const iterator& rhs) const noexcept { return ptr != rhs.ptr;}

		T& operator*() const { return *ptr;}
		T& operator->(){return *ptr;}

		iterator& operator++() noexcept		{++ptr; return *this;}
		iterator operator++(int) noexcept	{return iterator(ptr++);}

		iterator operator+(index_t offset) const noexcept
		{
			return iterator(ptr + offset);
		}

		iterator& operator--() noexcept { --ptr; return *this;}
		iterator operator--(int) noexcept { return iterator(ptr--);}
		iterator operator-(index_t offset) const noexcept
		{
			return iterator(ptr - offset);
		}

	};

	class reverse_iterator
	{
		T* ptr;
		constexpr reverse_iterator(const reverse_iterator& rhs) noexcept : ptr(rhs.ptr){}
		constexpr reverse_iterator(reverse_iterator&& rhs) noexcept : ptr(rhs.ptr){ rhs.ptr = nullptr;}
		constexpr reverse_iterator(T* ptr) noexcept : ptr(ptr){}
	public:
		constexpr reverse_iterator() noexcept : ptr(nullptr){}

		constexpr reverse_iterator& operator=(const reverse_iterator& rhs) noexcept { ptr = rhs.ptr; return *this;}
		constexpr reverse_iterator& operator=(reverse_iterator&& rhs) noexcept { ptr = rhs.ptr; return *this;}

		bool operator==(const reverse_iterator& rhs) const { return ptr == rhs.ptr;}
		bool operator!=(const reverse_iterator& rhs) const { return ptr != rhs.ptr;}

		T& operator*() const { return *ptr;}
		T& operator->(){return *ptr;}

		reverse_iterator& operator++(){ --ptr; return *this;}
		reverse_iterator operator++(int){ return iterator(ptr--);}
		reverse_iterator operator+(index_t offset) const noexcept
		{
			return reverse_iterator(ptr - offset);
		}

		reverse_iterator& operator--(){ ++ptr; return *this;}
		reverse_iterator operator--(int){ return iterator(ptr++);}
		reverse_iterator operator-(index_t offset)
		{
			return reverse_iterator(ptr + offset);
		}
	};
public:

	constexpr array() noexcept{}

	value_type& operator[](index_t index)
	{
		return _buffer[index];
	}
	const value_type& operator[](index_t index) const
	{
		return _buffer[index];
	}

	constexpr iterator begin() noexcept{ return iterator(_buffer);}
	constexpr const iterator cbegin() const noexcept{ return iterator(_buffer);}
	
	constexpr iterator end() noexcept { return iterator(&_buffer[CAPACITY]);}
	constexpr const iterator cend() const noexcept { return iterator(&_buffer[CAPACITY]);}

	constexpr reverse_iterator begin() noexcept{return reverse_iterator(_buffer + CAPACITY - 1);}
	constexpr const reverse_iterator cbegin() const noexcept{ return reverse_iterator(_buffer + CAPACITY - 1);}
	
	constexpr reverse_iterator end() noexcept { return reverse_iterator(_buffer - 1);}
	constexpr const reverse_iterator cend() const noexcept { return reverse_iterator(_buffer - 1);}

	constexpr index_t size() const noexcept { return CAPACITY;}
	constexpr index_t capacity() const noexcept { return CAPACITY;}
};
} // namespace mylib
#endif