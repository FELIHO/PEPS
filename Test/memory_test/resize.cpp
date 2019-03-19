#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"


using namespace std;

int main(int argc, char **argv)
{
    PnlMat* m = pnl_mat_create_from_double(3,10,1);
    pnl_mat_resize(m,13,134);

    pnl_mat_free(&m);
}