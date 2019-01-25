#include "Option/Kozei.hpp"
#include "Option/Asian.hpp"
#include "AssetModel/BlackScholesModel.hpp"

#include <iostream>
#include <string>
#include <fstream>


using namespace std;


	int payoff_test_Kozei() {
		int size = 30;
		double r = 0.1;
		PnlMat *rho = pnl_mat_create(size, size);
		pnl_mat_set_id(rho);
		double sigma = 0.2;

	    PnlRng *Pnlrng = pnl_rng_create(PNL_RNG_MERSENNE);
    	pnl_rng_sseed(Pnlrng, time(NULL));
    	RandomGen *rng = new PnlRnd(Pnlrng); 



		Kozei *test_kozei = new Kozei(100);

		PnlMat* mat_s0 = pnl_mat_create_from_file("matrice.txt");
		PnlVect* niveaux_initaux = pnl_vect_create(test_kozei->size_);
		PnlMat* past = pnl_mat_create(mat_s0->m - 2, mat_s0->n);
		PnlMat* path = pnl_mat_create(17, size);
		for (int i = 0; i < test_kozei->size_; i++) {

			pnl_vect_set(niveaux_initaux, i, 1.0 / 3 * (pnl_mat_get(mat_s0, 0, i) + pnl_mat_get(mat_s0, 1, i) + pnl_mat_get(mat_s0, 2, i)));
		}
		pnl_mat_set_row(past, niveaux_initaux, 0);

		PnlVect *vect_i = pnl_vect_create(mat_s0->n);
		for (int i = 3;i < mat_s0->m;i++) {
			pnl_mat_get_row(vect_i, mat_s0, i);
			pnl_mat_set_row(past, vect_i, i - 2);
		}


		BlackScholesModel *bs_model = new BlackScholesModel(size, r, rho, pnl_vect_create_from_double(size, sigma), niveaux_initaux);
		bs_model->asset(path, 9.0 / 2, test_kozei->T_, 16, rng, past);

		double payoff_kozei = test_kozei->payoff(path);

		cout << payoff_kozei << endl;


		return 0;
	}





