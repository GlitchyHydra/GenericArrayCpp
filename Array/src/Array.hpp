#include "Array.h"

#define INIT_CAPACITY 16

/*-----------------------Constructors------------------------*/
template<typename T>
Array<T>::Array() : Array(INIT_CAPACITY)
{
	//TODO choose size from sizeof
}

template<typename T>
Array<T>::Array(size_t capacity)
	: Capacity(capacity), Size(0)
{
	Ptr = allocate_memory(capacity);
}

/*-----------------------Destructor------------------------*/
template<typename T>
void Array<T>::FreeMemory()
{
	//12.4.16 
	for (ptrdiff_t i = 0; i < Size; ++i)
		Ptr[i].~T();
	free(Ptr);
}

template<typename T>
Array<T>::~Array()
{
	FreeMemory();
}

template<typename T>
Array<T>::Array(Array&& other)
	: Ptr(std::exchange(other.Ptr, nullptr)),
	Capacity(std::exchange(other.Capacity, 0)), Size(std::exchange(other.Size, 0))
{
}

template<typename T>
Array<T>::Array(const Array& other)
{
	//TODO
}

template<typename T>
Array<T>& Array<T>::operator=(const Array<T>& rhs)
{
	if (this == &rhs)
		return this;

	if (Size != rhs.Size)
	{
		FreeMemory();
		// preserve invariants in case next line throws allocate resource in *this
		Ptr = nullptr;
		Size = 0;
		Capacity = 0;
		Ptr = allocate_memory(Capacity);
		Size = rhs.Size;
		Capacity = rhs.Capacity;
	}

	std::copy(rhs.Ptr, rhs.Ptr + rhs.Size, Ptr);
	return *this;
}

template<typename T>
Array<T>& Array<T>::operator=(const Array<T>&& other)
{
	if (this == &other)
		return *this;
	//free self allocated memory
	FreeMemory();
	//move from rhs
	Ptr = std::exchange(other.Ptr, nullptr);
	Size = std::exchange(other.Size, 0);
	Capacity = std::exchange(other.Capacity, 0);
	return *this;
}

template<typename T>
bool Array<T>::EnsureCapacity()
{
	return Size + 1 < Capacity;
}

/*-----------------------Insert------------------------*/
template<typename T>
int Array<T>::Insert(const T& value)
{
	bool isEnoughCapacity = EnsureCapacity();
	if (!isEnoughCapacity)
	{
		Capacity *= 2u;
		T* reallocPtr = allocate_memory(Capacity);
		//C++17
		//std::is_trivially_copyable https://en.cppreference.com/w/cpp/types/is_trivially_copyable
		//memcpy implementation against cycle?
		bool isMovable = std::is_move_constructible<T>::value;
		if (isMovable)
		{
			//move if support
			for (ptrdiff_t i = 0; i < Size; i++)
				new(reallocPtr + i) T(std::move(Ptr[i]));
		}
		else
		{
			//copy if does not
			for (ptrdiff_t i = 0; i < Size; i++)
				new(reallocPtr + i) T(Ptr[i]);
		}
		FreeMemory();
		Ptr = reallocPtr;
	}
	//new placement | new (address) (type) initializer
	//https://youtu.be/Ng2RVKjS9A4?t=9366
	new(Ptr + Size) T(value);
	Size++;
	return Size - 1;
}

template<typename T>
int Array<T>::Insert(int index, const T& value)
{
	if (index > Size - 1)
		return Insert(value);

	bool isEnoughCapacity = EnsureCapacity();
	//C++17
	bool isMovable = std::is_move_constructible<T>::value;
	if (isEnoughCapacity)
	{
		if (isMovable)
			//move to right
			for (ptrdiff_t i = Size; i > index; i--)
				new(Ptr + i) T(std::move(Ptr[i - 1]));
		else
			for (ptrdiff_t i = Size; i > index; i--)
				new(Ptr + i) T(Ptr[i - 1]);
	}
	else
	{
		Capacity *= 2u;
		T* reallocPtr = allocate_memory(Capacity);
		if (isMovable)
		{
			for (ptrdiff_t i = 0; i < Size; i++)
				new(&reallocPtr[i]) T(data[i]);
		}
		else
		{
			if (index == 0)
				std::memcpy(reallocPtr + 1, Ptr, Size);
			else
			{
				//first half before index
				for (ptrdiff_t i = 0; i < Size; i++)
					new(&reallocPtr[i]) T(data[i]);
				std::memcpy(reallocPtr, Ptr, index);
				//second half after index
				std::memcpy(reallocPtr + index + 1, Ptr, Size - index);
			}
			//in index
			new(&reallocPtr[index]) T(value);
		}
		FreeMemory();
	}
	new(&Ptr[index]) T(value);
	return 0;
}

/*-----------------------Remove------------------------*/
template<typename T>
void Array<T>::Remove(int index)
{
	bool isMovable = std::is_move_constructible<T>::value;
	Ptr[index].~T();
	Size--;
	if (isMovable)
	{
		//move if support
		for (ptrdiff_t i = Size; i > index; i--)
			new(&Ptr[i - 1]) T(std::move(Ptr[i]));
	}
	else
	{
		//copy if does not
		for (ptrdiff_t i = Size; i > index; i--)
			new(&Ptr[i - 1]) T(Ptr[i]);
	}
}

template<typename T>
const T& Array<T>::operator[](int index) const
{
	return const Ptr[index];
}

template<typename T>
T& Array<T>::operator[](int index)
{
	return Ptr[index];
}

/*---------------------------- Iterator -----------------------------*/
/*
template<typename T>
Array<T>::ConstIterator Array<T>::ForwardIterator()
{
	return nullptr;
}

template<typename T>
Array<T>::ConstIterator Array<T>::ReverseIterator()
{
	return nullptr;
}
*/