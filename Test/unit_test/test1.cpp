#include "Kozei.hpp"
#include <gtest/gtest.h>

TEST(KozeiTest,equal) { 
    ASSERT_EQ(6, 6.0);
}

TEST(Kozei,none_equal) { 
    ASSERT_EQ(6, 5);
}
 
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
