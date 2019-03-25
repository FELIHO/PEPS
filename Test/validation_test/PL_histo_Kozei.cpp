#include "ForwardTest.hpp"
#include "Kozei.hpp"
#include <vector>

using namespace std;

Kozei *kozei = new Kozei(100);

double fdStep = 0.1;
int nbSamples = 50000;
double r = 0.04879;
double rho = 0;
double sigmaValue = 0.2;
double spotValue = 100;
ForwardTest* forwardTest;


int main(int argc, char const *argv[])
{
    for(int i =0; i<100; i++){
        forwardTest = new ForwardTest(kozei,r,rho,sigmaValue,spotValue,nbSamples,fdStep);
        forwardTest->setRebalancementFrequence(13);
        double PL = forwardTest->ProfitAndLoss();
        cout << PL << endl;
    }
    return 0;
}
