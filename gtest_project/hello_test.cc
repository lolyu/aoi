#include <gtest/gtest.h>
#include "test_target.cpp"
#include <iostream>

/*
TEST(TestSuiteName, TestName) {
  ... test body ...
}
*/

TEST(HelloTest, BasicAssertions)
{
    EXPECT_STRNE("HELLO", "WORLD");
    ASSERT_EQ(6, squareRoot(36.0));
}

TEST(HelloTest, PositiveNos)
{
    ASSERT_EQ(18.0, squareRoot(324.0));
}

TEST(HelloTest, NegativeNos)
{
    ASSERT_EQ(-1.0, squareRoot(-15.0));
}

TEST(HelloTest, NonFatalAssertionFail)
{
    EXPECT_EQ("Hello", "World") << "Test NonFatal Assertion";
    std::cout << "Hello NonFatal Assertion" << std::endl;
}

TEST(HelloTest, FatalAssertionFail)
{
    ASSERT_EQ("HELLO", "WORLD") << "Test Fatal Assertion";
    std::cout << "Hello Fatal Assertion" << std::endl;
}

class TimeTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        t0 = Time(1, 10);
        t1 = Time(2, 20);
        t2 = t0 + t1;
        // fixture is called per TEST_F test
        std::cout << "TimeTest::SetUp called once" << std::endl;
    }

    Time t0;
    Time t1;
    Time t2;
};

TEST_F(TimeTest, TimeAdditionTest)
{
    int minutes = t0.getMin() + t1.getMin();
    ASSERT_EQ(t2.getMin(), minutes %= 60);
    ASSERT_EQ(t2.getHour(), t0.getHour() + t1.getHour() + minutes / 60);
    std::cout << &t2 << std::endl;
}

TEST_F(TimeTest, TimeShowTest)
{
    std::cout << "Hour " << t2.getHour() << " Minute " << t2.getMin() << std::endl;
    std::cout << &t2 << std::endl;
}

int main(int argc, char *argv[])
{
    /*
    the test invocation of RUN_ALL_TESTS
    1. Saves the state of all googletest flags. 
    2. Creates a test fixture object for the first test.
    3. Initializes it via SetUp().
    4. Runs the test on the fixture object.
    5. Cleans up the fixture via TearDown().
    6. Deletes the fixture.
    7. Restores the state of all googletest flags.
    8. Repeats the above steps for the next test, until all tests have run.
    */
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}