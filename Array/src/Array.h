#pragma once

#include <algorithm>
#include <cstdlib>
#include <xutility>

template <typename T> 
class Array final
{
	T* Ptr;
	size_t Capacity;
	size_t Size;

	T* allocate_memory(size_t size) { return (T*) malloc(size * sizeof(T)); }
	void FreeMemory();
	bool EnsureCapacity();

public:
	Array();
	Array(size_t capacity);

	Array(std::initializer_list<T> list);

	~Array();

	Array(Array&& rhs);
	Array(Array const& rhs);
	Array<T>& operator=(const Array<T>& rhs);
	Array<T>& operator=(Array<T>&& rhs);

	int Insert(const T& value);
	int Insert(int index, const T& value);

	void Remove(int index);

	const T& operator[](int index) const;
	T& operator[](int index);

	size_t size() const { return Size; };

	class ConstIterator
	{

	protected:
		T* Begin;
		T* Ptr;
		T* End;

	public:
		using iterator_category = std::random_access_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = T*;
		using reference = T&;

		ConstIterator(T* ptr, ptrdiff_t offset, size_t size)
			: Begin(ptr), Ptr(ptr + offset), End(ptr + size)
		{}

		const T& Get() const { return *Ptr; }

		void Previous() { Ptr--; }
		bool HasPrevious() { return Ptr - 1 >= Begin; }
		void Next() { Ptr++; }
		bool HasNext() { return Ptr + 1 >= End; }

		ConstIterator& operator+=(const difference_type _Off) noexcept
		{
			//_Verify_offset(_Off);
			Ptr += _Off;
			return *this;
		}

		ConstIterator operator+(const difference_type _Off) const noexcept
		{
			ConstIterator _Tmp = *this;
			return _Tmp += _Off;
		}

		ConstIterator& operator-=(const difference_type _Off) noexcept
		{
			return *this += -_Off;
		}

		ConstIterator operator-(const difference_type _Off) const noexcept
		{
			ConstIterator _Tmp = *this;
			return _Tmp -= _Off;
		}

		difference_type operator-(const ConstIterator& rhs) const noexcept 
		{
			return Ptr - rhs.Ptr;
		}

		/* -------------------------------Индексирование--------------------------------- */
		reference operator[](const difference_type _Off) const noexcept
		{
			return *(Begin + _Off);
		}

		reference operator[](const difference_type _Off)
		{
			return *(Begin + _Off);
		}

		/*
		reference operator[](const difference_type _Off) const noexcept
		{
			return *(*this + _Off);
		}*/

		/* -------------------------------Compare--------------------------------- */
		bool operator==(const ConstIterator& rhs) const noexcept
		{
			//_Compat(rhs);
			return Ptr == rhs.Ptr;
		}

		bool operator!=(const ConstIterator& rhs) const noexcept
		{
			return !(*this == rhs);
		}

		bool operator<(const ConstIterator& rhs) const noexcept
		{
			//_Compat(_Right);
			return Ptr < rhs.Ptr;
		}

		bool operator>(const ConstIterator& rhs) const noexcept
		{
			return rhs < *this;
		}

		bool operator<=(const ConstIterator& rhs) const noexcept
		{
			return !(rhs < *this);
		}

		bool operator>=(const ConstIterator& rhs) const noexcept
		{
			return !(*this < rhs);
		}

		friend ConstIterator operator/(const ConstIterator& lhs, const int rhs)
		{ 
			return ((ptrdiff_t)lhs.Ptr) / rhs; 
		}

		friend ConstIterator operator*(const ConstIterator& lhs, const int rhs)
		{ 
			return lhs.Ptr * rhs; 
		}

		/* -------------------------------Get Element--------------------------------- */
		const T& operator*() const
		{
			return *Ptr;
		}

		T* operator->()
		{
			return &(*Ptr);
		}

		/* -------------------------------Increment & Decrement--------------------------------- */
		ConstIterator& operator++()
		{ 
			Ptr++;
			return *this;
		}

		ConstIterator& operator--() 
		{ 
			Ptr--; 
			return *this; 
		}

		ConstIterator& operator++(int) 
		{ 
			ConstIterator tmp = *this; 
			++(*this); 
			return tmp;
		}

		ConstIterator& operator--(int d) 
		{ 
			ConstIterator tmp = *this; 
			--(*this); 
			return tmp; 
		}
	};

	ConstIterator cbegin() const { return ConstIterator(Ptr, 0, Size); }
	ConstIterator cend() const { return ConstIterator(Ptr, Size, Size); }

	class Iterator : public ConstIterator
	{

	public:
		using iterator_category = std::random_access_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = T*;
		using reference = T&;

		Iterator(T* ptr, ptrdiff_t offset, size_t size)
			: ConstIterator(ptr, offset, size)
		{}

		Iterator() {}

		void Set(const T& value)
		{
			new (Ptr) T(value);
		}

		reference operator*() const noexcept
		{
			return *Ptr;
		}

		pointer operator->() const noexcept
		{
			return &(*Ptr);
		}

		Iterator& operator++() noexcept
		{
			++Ptr;
			return *this;
		}

		Iterator operator++(int) noexcept
		{
			Iterator tmp = *this;  
			++(*this);
			return tmp;
		}

		Iterator& operator--() noexcept
		{
			Ptr--;
			return *this;
		}

		Iterator operator--(int) noexcept
		{
			Iterator tmp = *this;
			--(*this);
			return tmp;
		}

		Iterator& operator+=(const difference_type _Off) noexcept
		{
			Ptr += _Off;
			return *this;
		}

		Iterator operator+(const difference_type _Off) const noexcept
		{
			Iterator _Tmp = *this;
			return _Tmp += _Off;
		}

		Iterator& operator-=(const difference_type _Off) noexcept
		{
			Ptr -= _Off;
			return *this;
		}

		Iterator operator-(const difference_type _Off) const noexcept
		{
			Iterator _Tmp = *this;
			return _Tmp -= _Off;
		}

		difference_type operator-(const Iterator& rhs) const noexcept
		{
			return Ptr - rhs.Ptr;
		}

		/* -------------------------------Индексирование--------------------------------- */
		reference operator[](const difference_type _Off) const noexcept
		{
			return *Ptr;
		}

		reference operator[](const difference_type _Off)
		{
			return *Ptr;
		}
	};

	Iterator begin() const { return Iterator(Ptr, 0, Size); }
	Iterator end() const { return Iterator(Ptr, Size, Size); }

	Iterator ForwardIterator() { return Iterator(Ptr, 0, Size); }
	ConstIterator ForwardIterator() const { return ConstIterator(Ptr, 0, Size); }

	Iterator ReverseIterator() { return Iterator(Ptr, Size, Size); }
	ConstIterator ReverseIterator() const { return ConstIterator(Ptr, Size, Size); }

protected:
};

