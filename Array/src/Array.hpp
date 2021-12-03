#include "Array.h"

#define INIT_CAPACITY 16

/*-----------------------Constructors------------------------*/
template<typename T>
Array<T>::Array() : Array(INIT_CAPACITY)
{
	
}

template<typename T>
Array<T>::Array(size_t capacity)
	: Capacity(capacity), Size(0)
{
	Ptr = allocate_memory(capacity);
}

template<typename T>
Array<T>::Array(std::initializer_list<T> list) : Array(list.size() * 2)
{
	Size = list.size();
	int i = 0;
	for (auto val : list)
	{
		new(Ptr + i) T(val);
		i++;
	}
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
Array<T>::Array(Array&& rhs)
	: Ptr(std::exchange(rhs.Ptr, nullptr)),
	Capacity(std::exchange(rhs.Capacity, 0)), Size(std::exchange(rhs.Size, 0))
{
}

template<typename T>
Array<T>::Array(Array const& rhs)
{
	Ptr = allocate_memory(rhs.Capacity);
	Size = rhs.Size;
	Capacity = rhs.Capacity;

	for (ptrdiff_t i = 0; i < Size; i++)
		new(Ptr + i) T(rhs.Ptr[i]);
}

template<typename T>
Array<T>& Array<T>::operator=(const Array<T>& rhs)
{
	if (this == &rhs)
		return *this;

	if (Size != rhs.Size)
	{
		FreeMemory();
		// preserve invariants in case next line throws allocate resource in *this
		Ptr = nullptr;
		Size = 0;
		Capacity = 0;
		Ptr = allocate_memory(rhs.Capacity);
		Size = rhs.Size;
		Capacity = rhs.Capacity;
	}

	for (size_t i = 0; i < Size; i++)
		new(Ptr + i) T(rhs.Ptr[i]);
	return *this;
}

template<typename T>
Array<T>& Array<T>::operator=(Array<T>&& rhs)
{
	if (this == &rhs)
		return *this;

	FreeMemory();
	Ptr = std::exchange(rhs.Ptr, nullptr);
	Size = std::exchange(rhs.Size, 0);
	Capacity = std::exchange(rhs.Capacity, 0);
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
		if constexpr (std::is_move_constructible<T>::value)
		{
			for (int i = 0; i < Size; i++)
				new(reallocPtr + i) T(std::move(Ptr[i]));
		}
		else
		{
			for (int i = 0; i < Size; i++)
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
	bool isEnoughCapacity = EnsureCapacity();
	bool isMovable = std::is_move_constructible<T>::value;
	if (isEnoughCapacity)
	{
		if constexpr (isMovable)
		{
			for (int i = Size; i > index; i--)
			{
				new(Ptr + i) T(std::move(Ptr[i - 1]));
				Ptr[i - 1].~T();
			}
				
		}
		else
		{
			for (int i = Size; i > index; i--)
			{
				new(Ptr + i) T(Ptr[i - 1]);
				Ptr[i - 1].~T();
			}
		}
	}
	else
	{
		Capacity *= 2u;
		T* reallocPtr = allocate_memory(Capacity);
		auto indexAfterInset = index + 1;
		if constexpr (isMovable)
		{
			for (int i = 0; i < index; i++)
				new(&reallocPtr[i]) T(std::move(Ptr[i]));
			for (int i = indexAfterInset; i < Size; i++)
				new(&reallocPtr[i]) T(std::move(Ptr[i]));
		}
		else
		{
			for (size_t i = 0; i < index; i++)
				new(&reallocPtr[i]) T(Ptr[i]);
			for (size_t i = indexAfterInset; i < Size; i++)
				new(&reallocPtr[i]) T(Ptr[i]);
		}
		FreeMemory();
		Ptr = reallocPtr;
	}
	new(&Ptr[index]) T(value);
	Size++;
	return index;
}

/*-----------------------Remove------------------------*/
template<typename T>
void Array<T>::Remove(int index)
{
	bool isMovable = std::is_move_constructible<T>::value;
	Ptr[index].~T();
	Size--;
	if constexpr (isMovable)
	{
		for (int i = index; i < Size; i++)
		{
			new(&Ptr[i]) T(std::move(Ptr[i + 1]));
			Ptr[i + 1].~T();
		}
			
	}
	else
	{
		for (int i = index; i < Size; i++)
		{
			new(&Ptr[i]) T(Ptr[i + 1]);
			Ptr[i + 1].~T();
		}
			
	}
}

template<typename T>
const T& Array<T>::operator[](int index) const
{
	return Ptr[index];
}

template<typename T>
T& Array<T>::operator[](int index)
{
	return Ptr[index];
}