#include <fstream>
#include <iostream>
#include <omp.h>
#include <cmath>
#include <cstdio>
#include <chrono>
#include <string>
#include <armadillo>
#include <vector>
#include "data_dir.hpp"
#include "wigner/gaunt.hpp"

using namespace std::chrono;
using namespace arma;

static const double kB = 1.3806504e-23;         // J/K
static const double NA = 6.02214179e23;         // 1/mol
static const double EHARTREE = 4.35974434e-18;  // J/Hartree
static const double AMU = 1.660538921e-27;      // kg/amu
static const double HBAR = 1.054571726e-34;     // J.s
static const double HBAR1 = HBAR / EHARTREE;    // Hartree.s
static const double HBAR2 = HBAR * 1e20 / AMU;  // amu.Å^2/s
static const double SCH4 = 186.25; // J/mol.K

std::string getDirectory(std::string sysname="METH-CHA") {
    if (sysname == "") {
        std::cout << "Enter system name:" << std::endl;
        std::cin >> sysname;
    }
    return dataRoot() + '/' + sysname;
}

Col<double> getCoefficients() {
    std::string dirname = getDirectory();
    std::string filename = dirname+'/'+"vdat.txt";
    std::ifstream is(filename);
    if (is.fail())
    {
        std::cout << "cannot open file " << filename;
    }
    double theta, phi, v;
    std::vector<double> my_vec;
    while (is) {
        if (!(is >> theta >> phi >> v)) {
            break;
        }
        my_vec.push_back(v);
    }
    Col<double> cvec = conv_to<vec>::from(my_vec);
    is.close();
    return cvec;
}

std::vector<double> getMomentOfInertia() {
    std::string dirname = getDirectory();
    std::string filename = dirname+'/'+"I.txt";
    std::ifstream is(filename);
    double val;
    std::vector<double> Ivec;
    while (is) {
        if (!(is >> val)) {
            break;
        }
        Ivec.push_back(val);
    }
    return Ivec;
}

double getReducedI(std::vector<double> Ivec) {
    double result = 0;
    for (double i : Ivec) {
        result += pow(i, -1);
    }
    return pow(result, -1);
}

double getPartitionFunction(double T, Mat<double>& H) {
    Col<double> eigval = eig_sym(H);
    double Q = 0;
    double b = pow(kB * T, -1) * EHARTREE;
    for (double e : eigval) {
        Q += exp(-b * e);
    }
    std::cout << std::endl << "Q predicted by eig_sym: " << Q << std::endl;
    Mat<double> bH = -b*H;
    Mat<double> expbH = expmat_sym(bH);
    double tr = trace(expbH);
    std::cout << pow(b,-1) << std::endl;
    return tr;
}

Mat<double> getHamiltonian(int lmax, double I, Col<double>& ahat) {
    int Lmax = sqrt(ahat.size());
    Mat<double> H = zeros<mat>(lmax*lmax, lmax*lmax);
    #pragma omp parallel
    {
    double g;
    double a;
    #pragma omp for
    for (int l = 0; l < lmax; l++) {
        for (int m = -l; m < l+1; m++) {
            int j = l*l + l + m;
            for (int ll = 0; ll < l+1; ll++) {
                for (int mm = -ll; mm < ll+1; mm++) {
                    int i = ll*ll + ll + mm;
                    if (j < i) {
                        continue;
                    }
                    for (int K = -ll; K < ll+1; K++) {
                    for (int L = 0; L < Lmax; L++) {
                        for (int M = -L; M < L+1; M++) {
                            int k = L*L + L + M;
                            g = wigner::gaunt<double>(l,ll,L,m,mm,M);
                            a = ahat(k);
                        }
                    }
                    if (ll == l && mm == m) {
                        H(i,j) /= 2.0;
                        H(i,j) += HBAR1*HBAR2 * l*(l+1.0) /(2.0*I);
                    }
                    }
                }
            }
        }
    }
    }
    return H;
}

int main() {
    auto start = high_resolution_clock::now();
    std::vector<double> Ivec = getMomentOfInertia();
    double I = getReducedI(Ivec);
    std::cout << "Printing I:" << std::endl;
    for (double i : Ivec) {
        std::cout << i << '\t';
    }
    I = 3.2;
    std::cout << std::endl << "Reduced I: " << I << std::endl;
    std::cout << "Rotational Constant: = " << HBAR1*HBAR2/2.0/I << std::endl;
    std::cout << "kT = " << kB*300 / EHARTREE << std::endl;
    std::cout << "Ratio = " << HBAR1*HBAR2/2.0/I *EHARTREE/(kB*300) << std::endl;
    std::cout << "Qapprox = " << kB*300/EHARTREE / (HBAR1*HBAR2/2.0/I) << std::endl;

    Col<double> ahat = getCoefficients();
    std::string dirname = getDirectory();
    std::cout << "Directory is: " << dirname << std::endl;

    Mat<double> H = getHamiltonian(20, I, ahat);
    double Q = getPartitionFunction(300, H);

    std::cout << std::endl;
    std::cout << "Q = " << Q << std::endl;
    std::cout << std::endl;
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout << std::endl << duration.count()/1e6 << " secs" << std::endl;
    return 0;
}
