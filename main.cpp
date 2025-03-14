#include <iostream>
#include <iomanip>
#include <math.h>
#include <Eigen/Dense>
#include <Eigen/PardisoSupport>

int main()
{
  typedef Eigen::SparseMatrix<double> SpMat;
  typedef Eigen::Triplet     <double> Trip;
  typedef Eigen::PardisoLU   <SpMat > Solver;

  size_t N = 11;

  SpMat A(N,N);
  Eigen::VectorXd f(N);

  f         *= 0.0;
  f((N-1)/2) = 1.0;

  std::vector<Trip> Atr;

  Atr.push_back(Trip(0,0,+2.0));
  Atr.push_back(Trip(0,1,-1.0));

  for ( size_t i=1; i<N-1; ++i ) {
    Atr.push_back(Trip(i,i-1,-1.0));
    Atr.push_back(Trip(i,i  ,+2.0));
    Atr.push_back(Trip(i,i+1,-1.0));
  }

  Atr.push_back(Trip(N-1,N-2,-1.0));
  Atr.push_back(Trip(N-1,N-1,+2.0));

  A.setFromTriplets(Atr.begin(),Atr.end());

  Solver solver;
  solver.analyzePattern(A);
  solver.factorize(A);

  Eigen::VectorXd u = solver.solve(f);

  return 0;
}
