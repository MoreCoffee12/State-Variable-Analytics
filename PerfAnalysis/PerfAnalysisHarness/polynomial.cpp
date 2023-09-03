/***************************************************************************
  File Name:      polynomial.cpp

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

#include "polynomial.h"
#include "math_algorithm.h"

#pragma warning( disable: 4275 )  // non dll-interface class 'std::_Complex_base<float>' used as base for dll-interface class 'std::complex<float>'
#include "nr.h"

/* This notice is intended to satisfy the licensing terms for Numerical Recipes,
   which state, "you must... include our Copyright notice within your machine-
   readable distribution (you need not display it when your program runs)."  */
__declspec(dllexport) char* supplementalNotice =
   "This product incorporates software bearing the following notice: "
   "Copyright (C) Numerical Recipes Software 1987, 1988, 1992, 1997, 2002";


/*************************** PUBLIC METHOD *******************************
  Function Name:  Polynomial::operator()

  Purpose:        evaluate the polynomial at x

  Author:         Ivan Johnson BNMIN

  Inputs:         x     the value at which the polynomial is to be evaluated

  Outputs:        Returns the value of the polynomial at x.

  Change History:
**************************************************************************/
double Polynomial::operator() (double x) const
{
   int degree = m_coefficients.size() - 1;  // degree of polynomial
   double p = 0;
   for ( int j = degree ; j >= 0 ; --j )
   {
      p = p*x + m_coefficients[j];
   }

   return p;
};


/*************************** PUBLIC METHOD *******************************
  Function Name:  ShiftedPolynomial::operator()

  Purpose:        evaluate the polynomial at x

  Author:         Ivan Johnson BNMIN

  Inputs:         x     the value at which the polynomial is to be evaluated

  Outputs:        Returns the value of the polynomial at x.

  Change History:
**************************************************************************/
double ShiftedPolynomial::operator() (double x) const
{
   x -= m_x_shift;
   return m_polynomial(x);
};


