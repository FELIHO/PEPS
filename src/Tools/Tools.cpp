#include "Tools.hpp"


void Tools::concatenationMatrice(PnlMat* res, const PnlMat *mat1, const PnlMat *mat2){
  //assert(mat1->n == mat2->n && mat2->n == res->n);
  //assert(res->m == mat1->m + mat2->m);
  if(mat1->n != mat2->n || mat2->n != res->n){
    throw new length_error("la concatenation n'est pas possible: les matrices sont incompatibles") ;
  }
  if(res->m != mat1->m + mat2->m){
    throw new length_error("la concatenation n'est pas possible: les matrices sont incompatibles") ;
  }
  int nbRows= res->m;
  int nbColumns = res->n;
  PnlVect* currentRow = pnl_vect_create(nbColumns);
  for (int i = 0; i < mat1->m; i++) {
    pnl_mat_get_row(currentRow, mat1, i);
    pnl_mat_set_row(res, currentRow, i);
  }
  for (int i = mat1->m; i < nbRows; i++) {
    pnl_mat_get_row(currentRow, mat2, i - mat1->m);
    pnl_mat_set_row(res, currentRow, i);
  }

  pnl_vect_free(&currentRow);
}