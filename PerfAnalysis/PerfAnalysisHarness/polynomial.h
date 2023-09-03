/***************************************************************************
  File Name:      polynomial.h

  Description:    classes and functions for defining, fitting, and evaluating polynomials

  Author:         Ivan Johnson BNMIN

  Contents:
     class Polynomial
     class ShiftedPolynomial
     function FitPolynomial

  Creation Date:  2006-06-09

  Change History:

Unpublished Copyright 2006 Bently Nevada 
All Rights Reserved
***************************************************************************/

#ifndef POLYNOMIAL_H_INCLUDED_87E3B7F0
#define POLYNOMIAL_H_INCLUDED_87E3B7F0

#include "MathDll.h"
#include <vector>


/******************************** CLASS *************************************
  Name:      Polynomial

  Description:
   A function object representing a polynomial of arbitrary degree

  Author:    Ivan Johnson BNMIN

  Change History:
*****************************************************************************/

#pragma warning( push )  
#pragma warning( disable: 4251 )  // 'm_coefficients' : class 'std::vector<double,class std::allocator<double> >' needs to have dll-interface to be used by clients of class 'Polynomial'

class Polynomial // : public std::unary_function<double, double>  // commented out to avoid VC6 compiler bug
{
public:
   Polynomial()
   : m_coefficients()
   {
   }

   Polynomial(const double coefficients[], size_t numCoefficients)
   : m_coefficients(coefficients, coefficients + numCoefficients)
   {
   }

   // evaluate the polynomial at x
   double operator() (double x) const;

private:
   std::vector<double> m_coefficients;
};

#pragma warning( pop )  


/******************************** CLASS *************************************
  Name:      ShiftedPolynomial

  Description:
   A function object representing a polynomial of arbitrary degree.  It differs 
   from class Polynomial in that the polynomial is formulated as 
      p(x) = a0 + a1*(x - x0) + a2*(x - x0)^2 + ...
   instead of the conventional form
      p(x) = a0 + a1*x + a2*x^2 + ...
   In some cases this can allow the polynomial to be more accurately 
   evaluated, with less floating-point roundoff error.

  Author:    Ivan Johnson BNMIN

  Change History:
*****************************************************************************/
class ShiftedPolynomial
{
public:
   ShiftedPolynomial()
   : m_polynomial()
   , m_x_shift()
   {
   }

   ShiftedPolynomial(const Polynomial& polynomial, double x_shift)
   : m_polynomial(polynomial)
   , m_x_shift(x_shift)
   {
   }

   // evaluate the polynomial at x
   double operator() (double x) const;

private:
   Polynomial m_polynomial;
   double m_x_shift;
};


// fit a polynomial to the data
MATH_API void FitPolynomial(const double x[],
                            const double y[], 
                            size_t ndata, 
                            int degree, 
                            ShiftedPolynomial* pResult);


#endif //POLYNOMIAL_H_INCLUDED_87E3B7F0
