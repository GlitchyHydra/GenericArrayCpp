#pragma once

#include <algorithm>
#include <cstdlib>

template <typename T> 
class Array final
{
	T* Ptr;
	size_t Capacity;
	size_t Size;

	using pointer = T*;

	T* allocate_memory(size_t size) { return (T*) malloc(size * sizeof(T)); }
	void FreeMemory();
	bool EnsureCapacity();

public:
	Array();
	Array(size_t capacity);

	~Array();

	Array(Array&& rhs);
	Array(const Array& rhs);
	Array<T>& operator=(const Array<T>& rhs);
	Array<T>& operator=(const Array<T>&& rhs);

	int Insert(const T& value);
	int Insert(int index, const T& value);

	void Remove(int index);

	const T& operator[](int index) const;
	T& operator[](int index);

	size_t size() const { return Size; };

	class Iterator
	{
		T* Begin;
		T* Ptr;
		T* End;
		
	public:
		//using iterator_category = std::bidirectional_iterator_tag;

		Iterator(T* ptr, ptrdiff_t offset, size_t size)
			: Begin(ptr), Ptr(ptr + offset), End(ptr + size)
		{}

		const T& Get() const { *Ptr; }
		void Set(const T& value) { new (Ptr) T(value); };

		void Previous() { Ptr--; }
		bool HasPrevious() { return Ptr - 1 >= Begin; }
		void Next() { Ptr++; }
		bool HasNext() { return Ptr + 1 < End; }

		T& operator*() const { return *Ptr; }
		T* operator->() { return Ptr; }
		Iterator& operator++() { Ptr++; return *this; }
		Iterator& operator--() { Ptr--; return *this; }
		Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
		Iterator& operator--(int) { Iterator tmp = *this; --(*this); return tmp; }
		friend bool operator== (const Iterator& a, const Iterator& b) { return a.Ptr == b.Ptr; };
		friend bool operator!= (const Iterator& a, const Iterator& b) { return a.Ptr != b.Ptr; };
	};

	Iterator begin() const { return Iterator(Ptr, 0, Size); }
	Iterator end() const { return Iterator(Ptr, Size, Size); }

	class ConstIterator
	{
		T* Begin;
		T* Ptr;
		T* End;

	public:
		ConstIterator(T* ptr, ptrdiff_t offset, size_t size)
			: Begin(ptr), Ptr(ptr + offset), End(ptr + Size)
		{}

		const T& Get() const { *Ptr; }

		void Previous() { Ptr--; }
		bool HasPrevious() { return Ptr - 1 >= Begin; }
		void Next() { Ptr++; }
		bool HasNext() { return Ptr + 1 < End; }

		const T& operator*() const { return *Ptr; }
		T* operator->() { return Ptr; }
		ConstIterator& operator++() { Ptr++; return *this; }
		ConstIterator& operator--() { Ptr--; return *this; }
		ConstIterator operator++(int) { ConstIterator tmp = *this; ++(*this); return tmp; }
		ConstIterator& operator--(int d) { ConstIterator tmp = *this; --(*this); return tmp; }
		friend bool operator== (const ConstIterator& lhs, const ConstIterator& rhs) { return lhs.Ptr == rhs.Ptr; };
		friend bool operator!= (const ConstIterator& lhs, const ConstIterator& rhs) { return lhs.Ptr != rhs.Ptr; };
	};

	ConstIterator cbegin() const { return ConstIterator(Ptr, 0, Size); }
	ConstIterator cend() const { return ConstIterator(Ptr, Size, Size); }

	Iterator ForwardIterator() { return Iterator(Ptr, 0, Size); }
	ConstIterator ForwardIterator() const { return ConstIterator(Ptr, 0, Size); }
	Iterator ReverseIterator() { return Iterator(Ptr, 0, Size); }
	ConstIterator ReverseIterator() const { return ConstIterator(Ptr, Size, Size); }

protected:
};

