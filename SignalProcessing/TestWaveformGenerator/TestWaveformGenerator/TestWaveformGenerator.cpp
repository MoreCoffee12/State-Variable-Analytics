// TestWaveformGenerator.cpp : Defines the entry point for the console application.
//
///////////////////////////////////////////////////////////////////////////////////
// This application generates test files for use in the signal processing
// harness programs.  
//
// B. Howard
// 8 October 2012
///////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <Windows.h>
#include <time.h>
#include <math.h>
#define TWOPI (2.0*3.1415926535897932384626433832795)

///////////////////////////////////////////////////////////////////////////////////
// Reference http://jonsson.eu/programs/cweb/sgfilter/
///////////////////////////////////////////////////////////////////////////////////
double gauss(double x, double w, double xa) {
   return(exp(-pow(((x-xa)/w),2)));
}

///////////////////////////////////////////////////////////////////////////////////
// Reference http://jonsson.eu/programs/cweb/sgfilter/
///////////////////////////////////////////////////////////////////////////////////
double func(double x) {
   double retval=gauss(x,0.007,0.2); /* $x_1=0.2$, $w_1=0.007$ */
   retval+=gauss(x,0.01,0.4);       /* $x_2=0.4$, $w_2=0.01$ */
   retval+=gauss(x,0.02,0.6);       /* $x_3=0.6$, $w_3=0.02$ */
   retval+=gauss(x,0.04,0.8);       /* $x_4=0.8$, $w_4=0.04$ */
   retval*=4.0;
   retval+=cos(3.0*x)*pow(sin(pow(x,3)),2);
   return(retval);
}

///////////////////////////////////////////////////////////////////////////////////
// Reference http://jonsson.eu/programs/cweb/sgfilter/
// Modified to output a .csv file
// 
// Command Line Structure:
// <filename> <noise variance>  or simply <noise variance>
///////////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[]) {
   int k, mm=1024;
   double var=1.0,xmax=2.5, x1, x2, u, v, f, z;
   char strFileName[MAX_PATH];
   
   // Initialize variables
   strcpy_s( strFileName, "DataOut.csv");

   // Read first argument as variance.
   if (argc == 2) 
   {
       sscanf(argv[1],"%lf",&var); 
   }

   // And the second as a file name.
   if (argc == 3)
   {
       strcpy_s( strFileName, argv[1]);
       sscanf(argv[2],"%lf",&var);
   }

   // Try to open the file
   FILE *fOut = fopen(strFileName, "w");
   if( fOut == NULL )
   {
      fprintf( stdout, "Failed to open file %s.  Error return: %s\n", strFileName, std::strerror(errno));
      return errno;
   }

   // Update the display
   fprintf( stdout, "  variance: %1.8f\n", var );
   fprintf( stdout, " File Name: %s\n", strFileName );

   // Initialize random number generator.
   srand((unsigned)time(NULL)); 
   for (k=0;k<mm-1;k+=2) {
      x1=xmax*k/((double)mm-1);
      x2=xmax*(k+1)/((double)mm-1);
      u=((float)rand())/RAND_MAX; /* Uniformly distributed over $[0,1]$ */
      v=((float)rand())/RAND_MAX; /* Uniformly distributed over $[0,1]$ */
      if (u>0.0) { /* Apply the Box--Muller algorithm on |u| and |v| */
         f=sqrt(-2*log(u));
         z=TWOPI*v;
         u=f*cos(z); /* Normally distributed with E(|u|)=0 and Var(|u|)=1 */
         v=f*sin(z); /* Normally distributed with E(|u|)=0 and Var(|u|)=1 */
         fprintf(fOut,"%1.8f, %1.8f\n",x1,func(x1)+var*u); /* $f(x_1)$ */
         fprintf(fOut,"%1.8f, %1.8f\n",x2,func(x2)+var*v); /* $f(x_2)$ */
      }
   }

   // Housekeeping;
   fclose(fOut);

   return(0);
}