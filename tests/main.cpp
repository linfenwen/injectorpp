#include "gmock/gmock.h"

int main(int argc, char** argv)
{
    testing::InitGoogleMock(&argc, argv);

    int result = RUN_ALL_TESTS();

    return result;
}