#include "UnitTest++/UnitTest++.h"

TEST(Sanity)
{
    CHECK_EQUAL(1, 1);
}

TEST(Breakit)
{
    CHECK_EQUAL(1, 2);
}

int main()
{
    return UnitTest::RunAllTests();
}
