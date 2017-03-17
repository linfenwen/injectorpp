/*
 * main.cpp
 *
 *  Created on: Mar 17, 2017
 *      Author: mazong1123
 */

#include <gmock/gmock.h>

int main(int argc, char** argv)
{
	testing::InitGoogleMock(&argc, argv);

	int result = RUN_ALL_TESTS();

	return result;
}

