#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_random.h"


#include "BlackScholesModel.hpp"

using namespace std;

int main(int argc, char **argv)
{
    PnlMat *past =  pnl_mat_create_from_scalar(5, 3, 11);

    // PnlMat* subPast = pnl_mat_new();
    // pnl_mat_extract_subblock(subPast,past,0,past->m-1,0,past->n);

    // assert(subPast->m == past->m -1);

    // PnlVect *V = pnl_vect_new();
    // pnl_vect_resize(V, 5);
    //pnl_vect_print(V);

    // PnlMat* M =pnl_mat_create_from_double(3,5,1);
    // PnlVect* V = pnl_vect_new();
    // pnl_mat_get_row(V, M,2);
    // pnl_vect_print(V);

    // PnlMat *A =  pnl_mat_create_from_scalar(5, 3, 11);
    // PnlMat* B =pnl_mat_create_from_double(2,3,1);
    // PnlVect U = pnl_vect_wrap_mat_row(A, 0);
    // pnl_mat_print(B);
    // cout<<endl;
    // pnl_mat_set_row(B, &U, 0);
    // pnl_mat_print(A);
    // cout<<endl;
    // pnl_mat_print(B);
    // pnl_mat_set(A,0,0,33);
    // cout<<endl;
    // pnl_mat_print(A);
    // cout<<endl;
    // pnl_mat_print(B);
    // cout<<endl;
    // pnl_mat_free(&A);
    // pnl_mat_print(B);
    

    char const *fileData = "market_kozei.dat";
    PnlMat* Data = pnl_mat_create_from_file(fileData);
    pnl_mat_print(Data);


}
