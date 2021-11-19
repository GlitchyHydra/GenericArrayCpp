#pragma once

#include "Array.hpp"

#include <iostream>
#include <string>

#include "gtest/gtest.h"

class ArrayTest : public ::testing::Test
{
public:
	ArrayTest()
	{

	}

	~ArrayTest() override
	{

	}



	Array<int> IntArray;

	Array<std::string> StrArray;

	void SetUp() override
	{

	};

	void TearDown() override
	{

	};
};

TEST_F(ArrayTest, IsInitializedCorrect)
{
	//EXPECT_* when you want the test to continue to reveal more errors after the assertion failure
	EXPECT_EQ(IntArray.size(), 0);
	EXPECT_EQ(StrArray.size(), 0);
	//ASSERT_* when continuing after failure doesn’t make sense
	//ASSERT_NE(n, nullptr)
}

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}