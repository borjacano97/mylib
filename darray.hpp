#ifndef DARRAY_HPP
#define DARRAY_HPP
#include "types.hpp"
namespace mylib
{
template<typename T/*TODO: Add support for custom allocator*/>
class darray
{
	T* _buffer;
	index_t _size;
	index_t _capacity;
public:
	class reverse_iterator;
	class iterator
	{
		friend class darray<T>;
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

		int64 operator-(const iterator& rhs) const
		{
			return ptr - rhs.ptr;
		}
	};

	class reverse_iterator
	{
		friend class darray<T>;
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

		int64 operator-(const reverse_iterator& rhs) const
		{
			return ptr - rhs.ptr;
		}
	};

public:
	constexpr darray() noexcept : _buffer(nullptr), _size(0), _capacity(0) {}
	darray(index_t initial_size) noexcept:
		_buffer(new T[initial_size]), _size(initial_size), _capacity(initial_size) 
	{}

	darray(const darray& rhs) noexcept:
		_buffer(new T[rhs._capacity]), _size(rhs._size), _capacity(rhs._capacity)
	{
		for(index_t i = 0; i < _capacity; ++i)
			_buffer[i] = rhs._buffer[i];
	}
	darray(darray&& rhs) noexcept:
		_buffer(rhs._buffer), _size(rhs._size), _capacity(rhs._capacity)
	{
		rhs._buffer = nullptr;
	}
	~darray()
	{
		delete[] _buffer;
	}

	T& operator[](index_t index) { return _buffer[index];}
	const T& operator[](index_t index) const { return _buffer[index];}

	darray& hard_clear()
	{
		_size = 0;
		return *this;
	}

	darray& clear() 
	{
		for(index_t i = 0; i < _size; ++i)
			_buffer[i].~T();
		_size = 0;
		return *this;
	}

	darray& push(const T& elem)
	{
		grow_if_needed();
		_buffer[_size++] = elem;
		return *this;
	}

	darray& push(T&& elem)
	{
		grow_if_needed();
		_buffer[_size++] = std::move(elem);
		return *this;
	}

	iterator find(const T& elem) const noexcept
	{
		for(index_t i = 0; i < _size; ++i)
			if(_buffer[i] == elem)
				return iterator(_buffer + i);
		return end();
	}
	darray& insert(const T& elem, iterator pos)
	{
		const index_t target_pos = pos - begin();
		insert(T(elem), target_pos);
		return *this;
	}
	
	darray& insert(T&& elem, iterator pos)
	{
		const index_t target_pos = pos - begin();
		insert(elem, target_pos);
		return *this;
	}
	darray& insert(const T& elem, reverse_iterator pos)
	{
		const index_t target_pos = pos.ptr - begin();
		insert(T(elem), target_pos);
		return *this;
	}
	
	darray& insert(T&& elem, reverse_iterator pos)
	{
		const index_t target_pos = pos.ptr - begin();
		insert(elem, target_pos);
		return *this;
	}
	darray& remove(iterator pos)
	{
		const index_t target_pos = pos - begin();
		remove(target_pos);
		return *this;
	}
	darray& remove(reverse_iterator pos)
	{
		const index_t target_pos = pos.ptr - _buffer;
		return remove(target_pos);
	}
	darray& remove(const T& elem)
	{
		return remove(find(elem));
	}
	T&& pop()
	{
		return std::move(_buffer[(_size--) -1]);
	}

	darray& resize(index_t new_size)
	{
		// Grow if needed
		if(new_size > _size)
		{
			index_t new_capacity = _capacity;
			while(new_capacity < new_size)
				new_capacity *= 2;
			grow(new_capacity);

			// Call the default constructor for the "new" implicit instanciated elements
			for(index_t i = _size; i < new_size; ++i)
				_buffer[i] = T();
		}
		else
		{
			//Call the destructors of the implicit deleted elements
			for(index_t i = _size-1; i >= new_size; --i)
				_buffer[i].~T();
		}

		_size = new_size;
		return *this;
	}

	iterator begin() noexcept{ return iterator(_buffer);}
	const iterator cbegin() const noexcept { return iterator(_buffer);}
	
	iterator end() noexcept{ return iterator(_buffer + _size);}
	const iterator cend() const noexcept{ return iterator(_buffer + _size);}

	iterator rbegin() noexcept{ return iterator(_buffer + _size - 1);}
	const iterator crbegin() const noexcept{ return iterator(_buffer + _size - 1);}
	
	reverse_iterator rend() noexcept{ return iterator(_buffer - 1);}
	const reverse_iterator crend() const noexcept{ return iterator(_buffer - 1);}

	constexpr index_t size() const noexcept{ return _size;}
	constexpr index_t capacity () const noexcept{ return _capacity;}
	constexpr T* data() noexcept { return _buffer;}
private:
	inline bool needs_to_grow(index_t size = 1) const
	{
		return (_size + size) >= _capacity;
	}

	void grow(index_t capacity = 0)
	{
		const index_t new_capacity = capacity ? capacity : _capacity * 2;
		T* new_buffer = new T[new_capacity];
		for(index_t i = 0; i < _size; ++i)
			new_buffer[i] = std::move(_buffer[i]);
		delete[] _buffer;
		_buffer = new_buffer;
		_capacity = new_capacity;
	}

	bool grow_if_needed(index_t n = 1)
	{
		if(needs_to_grow(n))
		{
			grow();
			return true;
		}
		return false;
	}

	void insert(T&& elem, index_t pos)
	{
		grow_if_needed();
		for(index_t i = _size; i >= pos; i--)
		{
			_buffer[i] = std::move(_buffer[i-1]);
		}
		_buffer[pos] = std::move(elem);
	}
	void remove(index_t pos)
	{
		_buffer[pos].~T();
		for(index_t i = pos; i < _size; ++i)
			_buffer[i] = _buffer[i+1];
	}
};
} // namespace mylib
#endif