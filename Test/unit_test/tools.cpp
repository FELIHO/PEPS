#include <gtest/gtest.h>
#include "pnl/pnl_matrix.h"
#include "Tools.hpp"

using namespace std;

PnlMat *mat1 =  pnl_mat_create_from_scalar(12, 10, 5);
PnlMat *mat2 = pnl_mat_create_from_scalar(5, 10, 5);

TEST(Concatenation,equal) {
    PnlMat *res = pnl_mat_create_from_zero(17, 10);
    PnlMat* expexted_res = pnl_mat_create_from_double(17,10,5);
    Tools::concatenationMatrice(res, mat1, mat2);
    bool b = pnl_mat_isequal( res , expexted_res,0);
    ASSERT_TRUE(b);
    pnl_mat_free(&res);
    pnl_mat_free(&expexted_res);
}

TEST(Concatenation,incompatible_size_m) {
    PnlMat *res = pnl_mat_create_from_zero(16, 10);
    ASSERT_ANY_THROW(Tools::concatenationMatrice(res, mat1, mat2));
}

TEST(Concatenation,incompatible_size_n) {
    PnlMat *res = pnl_mat_create_from_zero(17, 9);
    ASSERT_ANY_THROW(Tools::concatenationMatrice(res, mat1, mat2));
}
 
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

