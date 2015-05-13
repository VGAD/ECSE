#include "gtest/gtest.h"
#include "ECSE/Logging.h"

int main(int argc, char **argv)
{
    ECSE::init_logging(argc, argv);
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}