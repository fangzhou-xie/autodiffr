
#include <Rcpp.h>
// autodiff include
#include <autodiff_reverse.h>

using namespace Rcpp;
using namespace autodiff;

// The single-variable function for which derivatives are needed
autodiff::var f1(autodiff::var x) {
  return 1 + x + x*x + 1/x + log(x);
}

// The scalar function for which the gradient is needed
autodiff::var f2(const ArrayXvar& x) {
  return (x * x.exp()).sum(); // sum([xi * exp(xi) for i = 1:5])
}

// The multi-variable function for which derivatives are needed
autodiff::var f3(autodiff::var x, autodiff::var y, autodiff::var z) {
  return 1 + x + y + z + x*y + y*z + x*z + x*y*z + exp(x/y + y/z);
}

//' derivative of 1 + x + x*x + 1/x + log(x) in reserse mode
//' 
//' @export
// [[Rcpp::export]]
List test_reverse_derivative() {
  autodiff::var x = 2.0;   // the input variable x
  autodiff::var u = f1(x);  // the output variable u
  
  auto [ux] = derivatives(u, wrt(x)); // evaluate the derivative of u with respect to x
  
  // cout << "u = " << u << endl;  // print the evaluated output variable u
  // cout << "ux = " << ux << endl;  // print the evaluated derivative ux
  List l = List::create(
    Named("y") = static_cast<double>(u),
    Named("dydx") = ux
  );
  return l;
}

//' gradient of (x * x.exp()).sum() in reverse mode
//' 
//' @export
// [[Rcpp::export]]
List test_reverse_gradient() {
  using Eigen::VectorXd;
  
  VectorXvar x(5);                       // the input vector x with 5 variables
  x << 1, 2, 3, 4, 5;                    // x = [1, 2, 3, 4, 5]
  
  autodiff::var y = f2(x);                          // the output variable y
  
  VectorXd dydx = gradient(y, x);        // evaluate the gradient vector dy/dx
  
  // std::cout << "y = " << y << std::endl;           // print the evaluated output y
  // std::cout << "dy/dx = \n" << dydx << std::endl;  // print the evaluated gradient vector dy/dx
  List l = List::create(
    Named("y") = static_cast<double>(y),
    Named("g") = dydx
  );
  return l;
}

//' partial derivatives of 1 + x + y + z + x*y + y*z + x*z + x*y*z + exp(x/y + y/z) in reverse mode
//'
//' @export
// [[Rcpp::export]]
List test_reverse_derivative_multi() {
  
  autodiff::var x = 1.0;         // the input variable x
  autodiff::var y = 2.0;         // the input variable y
  autodiff::var z = 3.0;         // the input variable z
  autodiff::var u = f3(x, y, z);  // the output variable u
  
  auto [ux, uy, uz] = derivatives(u, wrt(x, y, z)); // evaluate the derivatives of u with respect to x, y, z
  
  // cout << "u = " << u << endl;    // print the evaluated output u
  // cout << "ux = " << ux << endl;  // print the evaluated derivative ux
  // cout << "uy = " << uy << endl;  // print the evaluated derivative uy
  // cout << "uz = " << uz << endl;  // print the evaluated derivative uz
  List l = List::create(
    Named("u") = static_cast<double>(u),
    Named("dudx") = ux,
    Named("dudy") = uy,
    Named("dudz") = uz
  );
  return l;
}