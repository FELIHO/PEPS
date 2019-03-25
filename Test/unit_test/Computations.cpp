#include "Computations.hpp"

using namespace std;

int main(int argc, char const *argv[])
{
    double prix,ic;
    int n_samples = 50000;
    double h = 0.1;
    int date = 20140411;
    double inv_init=100.0;
    int numberDateEstimation = 720;
    double r = 0.03;

    Computations::getPriceKozeiNative(ic, prix, n_samples, h, date, inv_init, numberDateEstimation, r);
    cout << prix << endl;

    string s1 = Computations::getDeltaKozeiNative(n_samples,h,date,inv_init,numberDateEstimation,r);
    cout << s1 << endl;

    //string s2 = Computations::getListPriceKozeiNative(n_samples,h,inv_init,numberDateEstimation,r);
    //cout << s2 << endl;



    return 0;
}
