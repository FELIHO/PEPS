#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_random.h"


#include "BlackScholesModel.hpp"

using namespace std;


int main(int argc, char **argv)
{
    PnlMat *past =  pnl_mat_create_from_scalar(10, 30, 1);

    PnlMat* subPast = pnl_mat_new();
    pnl_mat_extract_subblock(subPast,past,0,past->m-1,0,past->n);

    cout << "past->m : " << past->m << endl;
    cout << "subPast->m : " << subPast->m << endl;
}
