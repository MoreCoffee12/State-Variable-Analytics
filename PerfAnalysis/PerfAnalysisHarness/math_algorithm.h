/***************************************************************************
  File Name:      math_algorithm.h

  Description:    functions for computing numeric values from sequences

  Author:         Ivan Johnson BNMIN

  Contents:
      mean

  Creation Date:  2006-06-09

  Change History:

Unpublished Copyright 2006 Bently Nevada 
All Rights Reserved
***************************************************************************/

#ifndef MATH_ALGORITHM_H_INCLUDED_87E3B7F0
#define MATH_ALGORITHM_H_INCLUDED_87E3B7F0

#include <assert.h>
#include <iterator>
#include <numeric>


/*************************** FUNCTION ************************************
  Function Name:  mean

  Purpose:        Compute the arithmetic mean (also known as the average) of
                  the elements in a sequence

  Author:         Ivan Johnson BNMIN

  Inputs:         first, last   pointers specifying the sequence of items to be
                                averaged; normally the value_type will be some 
                                numeric type such as double or std::complex

  Outputs:        Returns the mean

  Notes:          Ideally the template parameter would be the iterator type,
                  rather than the value type, so that the function could handle
                  iterators other than pointers.  VC6 template limitations make
                  this inconvenient at present.  If necessary, the function
                  could be overloaded to get the desired effect.

  Change History:
**************************************************************************/
template<typename value_type>
value_type mean(const value_type* first, const value_type* last) 
{
   double sum = std::accumulate( first, last, value_type() );
   int count = std::distance(first, last);
   assert( count != 0 );
   return sum / count;
}


#endif //MATH_ALGORITHM_H_INCLUDED_87E3B7F0
