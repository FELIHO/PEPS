#include "ForwardTest.hpp"
#include "Kozei.hpp"
#include <vector>

using namespace std;

PnlVect* niveauxInitiaux =pnl_vect_create_from_double(30,100);
Kozei *kozei = new Kozei(100,niveauxInitiaux);

double fdStep = 0.1;
int nbSamples = 50000;
double r = 0.04879;
double rho = 0;
double sigmaValue = 0.2;
double spotValue = 100;
ForwardTest* forwardTest = new ForwardTest(kozei,r,rho,sigmaValue,spotValue,nbSamples,fdStep);

int main(int argc, char const *argv[])
{
    forwardTest->setRebalancementFrequence(26);
    for(int i =0; i<100; i++){
        double PL = forwardTest->ProfitAndLoss();
        cout << PL << endl;
    }
    return 0;
}
