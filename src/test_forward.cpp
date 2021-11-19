
#include <Rcpp.h>
// autodiff include
#include <autodiff_forward.h>

using namespace Rcpp;
using namespace autodiff;


// The single-variable function for which derivatives are needed
dual f1(dual x) {
  return 1 + x + x*x + 1/x + log(x);
}

// The scalar function for which the gradient is needed
real f2(const ArrayXreal& x) {
  return (x * x.exp()).sum(); // sum([xi * exp(xi) for i = 1:5])
}

// The scalar function for which the gradient is needed
dual f3(dual x, dual y, dual z) {
  return 1 + x + y + z + x*y + y*z + x*z + x*y*z + exp(x/y + y/z);
}

real xsq_real(real x) {
  return x*x;
}

dual xsq_dual(dual x) {
  return x*x;
}

//' test for y = x^2 in real mode
//' 
//' @param x double
//' @export
// [[Rcpp::export]]
List test_real(double x) {
  real xreal = static_cast<Real<1,double>>(x);
  real y = xsq_real(xreal);
  double dydx = derivative(xsq_real, wrt(xreal), at(xreal));
  
  List l = List::create(
    Named("y") = y.val(),
    Named("dydx") = dydx
  );
  return l;
}

//' test for y = x^2 in real mode
//' 
//' @param x double
//' @export
// [[Rcpp::export]]
List test_dual(double x) {
  dual xdual = static_cast<Dual<double,double>>(x);
  dual y = xsq_dual(xdual);
  double dydx = derivative(xsq_dual, wrt(xdual), at(xdual));
  
  List l = List::create(
    Named("y") = y.val,
    Named("dydx") = dydx
  );
  return l;
}

//' derivative of 1 + x + x*x + 1/x + log(x) in forward mode
//' 
//' @export
// [[Rcpp::export]]
List test_forward_derivative() {
  dual x = 2.0;                                 // the input variable x
  dual y = f1(x);                                // the output variable u

  double dydx = derivative(f1, wrt(x), at(x));   // evaluate the derivative du/dx

  // std::cout << "y = " << y << std::endl;        // print the evaluated output u
  // std::cout << "dy/dx = " << dydx << std::endl; // print the evaluated derivative du/dx
  List l = List::create(
    Named("y") = y.val,
    Named("dydx") = dydx
  );
  return l;
}

//' gradient of (x * x.exp()).sum() in forward mode
//' 
//' @export
// [[Rcpp::export]]
List test_forward_gradient() {
  using Eigen::VectorXd;
  
  ArrayXreal x(5);                            // the input array x with 5 variables
  x << 1, 2, 3, 4, 5;                         // x = [1, 2, 3, 4, 5]
  
  real u;                                     // the output scalar u = f(x) evaluated together with gradient below
  
  VectorXd g = gradient(f2, wrt(x), at(x), u); // evaluate the function value u and its gradient vector g = du/dx
  
  // std::cout << "u = " << u << std::endl;      // print the evaluated output u
  // std::cout << "g = \n" << g << std::endl;    // print the evaluated gradient vector g = du/dx
  List l = List::create(
    Named("y") = u.val(),
    Named("g") = g
  );
  return l;
}

//' partial derivatives of 1 + x + y + z + x*y + y*z + x*z + x*y*z + exp(x/y + y/z) in forward mode
//' 
//' @examples
//' test_forward_derivative_multi()
//' @export
// [[Rcpp::export]]
List test_forward_derivative_multi() {
  
  dual x = 1.0;
  dual y = 2.0;
  dual z = 3.0;
  
  dual u = f3(x, y, z);
  
  double dudx = derivative(f3, wrt(x), at(x, y, z));
  double dudy = derivative(f3, wrt(y), at(x, y, z));
  double dudz = derivative(f3, wrt(z), at(x, y, z));
  
  // std::cout << "u = " << u << std::endl;         // print the evaluated output u = f(x, y, z)
  // std::cout << "du/dx = " << dudx << std::endl;  // print the evaluated derivative du/dx
  // std::cout << "du/dy = " << dudy << std::endl;  // print the evaluated derivative du/dy
  // std::cout << "du/dz = " << dudz << std::endl;  // print the evaluated derivative du/dz
  
  List l = List::create(
    Named("u") = u.val,
    Named("dudx") = dudx,
    Named("dudy") = dudy,
    Named("dudz") = dudz
  );
  return l;
}