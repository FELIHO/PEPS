#include "Computations.hpp"

int main(int argc, char const *argv[])
{
    double prix,ic;
    int n_samples = 50000;
    double h = 0.1;
    int date = 20190324;
    double inv_init=100.0;
    int numberDateEstimation = 720;
    double r = 0.03;

    Computations::getPriceKozeiNative(ic, prix, n_samples, h, date, inv_init, numberDateEstimation, r);
    cout << prix << endl;

    string s = Computations::getDeltaKozeiNative(n_samples,h,date,inv_init,numberDateEstimation,r);
    cout << s << endl;

    // string s = Computations::getListPriceKozeiNative(n_samples,h,inv_init,numberDateEstimation,r);
    // cout << s << endl;



    return 0;
}
