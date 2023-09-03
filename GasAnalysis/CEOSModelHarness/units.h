// Units.h: interface for the CUnits class.
//
//////////////////////////////////////////////////////////////////////
#include <math.h>

#ifndef _CUNITS
#define _CUNITS

class CUnits  
{
public:

	/*Variables
	******************************************************************/
	// A unit member has three descriptors:
	// the first is the class of the unit (length, pressure, etc)
	// the second is the multiplicative change w.r.t. USCS
	// and the third is the offset (if any)
	//Length		- class 00 units
	double			cm[3];
	double			in[3];
	double			ft[3];
     double              m[3];

	//Area			- class 01 units
	double			cm2[3];
	double			in2[3];
	double			ft2[3];

	//Volume		- class 02 units
	double			m3[3];
	double			cm3[3];
	double			in3[3];
	double			ft3[3];

	//Temperature	- class 03 units
	double			R[3];
	double			K[3];

	//Pressure		- class 04 units
	double			PSIA[3];
	double			bar[3];

	//mass			- class 05 units
	double			g[3];
	double			kg[3];
	double			lb[3];

	//torque		- class 06 units
	double			ftlb[3];
	double			Nm[3];

	//force			- class 07 units
	double			lbf[3];
	double			N[3];

	//energy		- class 08 units
	double			lbf_ft[3];
	double			BTU[3];
	double			J[3];
	double			kJ[3];

	//angle			- class 09 units
	double			radians[3];
	double			degrees[3];

	/*Functions
	******************************************************************/
					CUnits();
	virtual			~CUnits();
	double			convert( double *from, double *to );

};

#endif