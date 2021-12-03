#pragma once

#include "Array.hpp"

#include <iostream>
#include <string>

#include "gtest/gtest.h"

class ArrayTest : public ::testing::Test
{
#define INITIAL_STR_CAPACITY 7
#define INITIAL_INT_CAPACITY 101
public:
	ArrayTest()
	{

	}

	~ArrayTest() override
	{

	}

	Array<int> IntArray;
	Array<std::wstring> StrArray;

	const std::wstring Strs[INITIAL_STR_CAPACITY] = { L"Es", L"kommt", L"für", L"mich", L"nicht", L"in", L"frage" };
	const std::wstring MiddleStr = L"Redewendung";
	ptrdiff_t StrInsertIndex = INITIAL_STR_CAPACITY / 2;
	ptrdiff_t IntInsertIndex = INITIAL_INT_CAPACITY / 2;

	void SetUp() override
	{
		IntArray = Array<int>(INITIAL_INT_CAPACITY);
		StrArray = Array<std::wstring>(INITIAL_STR_CAPACITY);

		auto exclusiveValue = 101;
		for (int i = 0; i < exclusiveValue; i++)
			IntArray.Insert(i);
		IntArray.Insert(IntInsertIndex, -IntInsertIndex);
		
		for (auto str : Strs)
			StrArray.Insert(str);
		StrArray.Insert(StrInsertIndex, MiddleStr);
	};

	void TearDown() override
	{

	};
};

//EXPECT_* when you want the test to continue to reveal more errors after the assertion failure
	//EXPECT_EQ(StrArray.size(), 0);
//ASSERT_* when continuing after failure doesn’t make sense
	//ASSERT_NE(n, nullptr)

TEST_F(ArrayTest, IsSizeCorrect)
{
	EXPECT_EQ(IntArray.size(), INITIAL_INT_CAPACITY + 1);
	EXPECT_EQ(StrArray.size(), INITIAL_STR_CAPACITY + 1);
}

TEST_F(ArrayTest, IsInsertCorrect)
{
	for (int i = 0; i < IntInsertIndex; i++)
		EXPECT_EQ(IntArray[i], i);
	for (int i = IntInsertIndex + 1; i < IntArray.size(); i++)
		EXPECT_EQ(IntArray[i], i - 1);
	EXPECT_EQ(IntArray[IntInsertIndex], -IntInsertIndex);
	
	//assert before last inserted in between
	for (int i = 0; i < StrInsertIndex; i++)
		EXPECT_EQ(StrArray[i], Strs[i]);
	//assert after last inserted in between
	for (int i = StrInsertIndex + 1; i < StrArray.size(); i++)
		EXPECT_EQ(StrArray[i], Strs[i - 1]);
	//assert last inserted
	EXPECT_EQ(StrArray[StrInsertIndex], MiddleStr);
}

TEST_F(ArrayTest, IsRemoveCorrect)
{
	IntArray.Remove(IntInsertIndex);
	StrArray.Remove(StrInsertIndex);

	ptrdiff_t i = 0;
	for (auto val : IntArray)
	{
		EXPECT_EQ(val, i);
		i++;
	}
	
	i = 0;
	for (auto val : StrArray)
	{
		EXPECT_EQ(val, Strs[i]);
		i++;
	}
}

TEST_F(ArrayTest, IsAssignCorrect)
{
	IntArray.Remove(IntInsertIndex);
	StrArray.Remove(StrInsertIndex);

	auto DuplicateIntArray = IntArray;
	auto DuplicateStrArray = StrArray;

	auto IntArrayIterator = IntArray.cbegin();
	auto StrArrayIterator = StrArray.cbegin();

	for (auto dup_val : DuplicateIntArray)
	{
		auto val = IntArrayIterator.Get();
		EXPECT_EQ(dup_val, val);
		IntArrayIterator.Next();
	}

	for (auto dup_val : DuplicateStrArray)
	{
		auto val = StrArrayIterator.Get();
		EXPECT_EQ(dup_val, val);
		EXPECT_NE(dup_val.c_str(), val.c_str());
		StrArrayIterator.Next();
	}
}

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}