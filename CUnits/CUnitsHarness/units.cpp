// Units.cpp: implementation of the CUnits class.
//
//////////////////////////////////////////////////////////////////////

#include "units.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUnits::CUnits()
{
	//Load linear data
	//Length	- class 00 units
	cm[0]		= 0.00;
	cm[1]		= 2.54;
	cm[2]		= 0.00;

	in[0]		= 0.00;
	in[1]		= 1.00;
	in[2]		= 0.00;

	ft[0]		= 0.00;
	ft[1]		= ( 1.00 / 12.00 );
	ft[2]		= 0.00;

     m[0]           = 0.00;
     m[1]           = ( 1.00 / 39.3700787402 );
     m[2]           = 0.00;

	//Load area data
	//Area		- class 01 units
	cm2[0]		= 1.00;
	cm2[1]		= pow(2.54, 2);
	cm2[2]		= 0.00;

	in2[0]		= 1.00;
	in2[1]		= pow(1.0, 2.0);
	in2[2]		= 0.00;

	ft2[0]		= 1.00;
	ft2[1]		= pow( ( 1.0 / 12.0 ), 2);
	ft2[2]		= 0.00;

	//Load volume data
	//Volume	- class 02 units
	m3[0]		= 2.00;
	m3[1]		= pow(0.0254, 3);
	m3[2]		= 0.00;

	cm3[0]		= 2.00;
	cm3[1]		= pow(2.54, 3);
	cm3[2]		= 0.00;

	in3[0]		= 2.00;
	in3[1]		= 1.00;
	in3[2]		= 0.00;

	ft3[0]		= 2.00;
	ft3[1]		= (1.0 / pow(12.0, 3.0));
	ft3[2]		= 0.00;

	//Load temperature data
	//Temperature	- class 03 units
	R[0]		= 3.00;
	R[1]		= 1.00;
	R[2]		= 0.00;

	K[0]		= 3.00;
	K[1]		= ( 5.0 / 9.0 );
	K[2]		= 0.00;

	//Load pressure data
	//Pressure		- class 04 units
	PSIA[0]		= 4.00;
	PSIA[1]		= 1.00;
	PSIA[2]		= 0.00;

	bar[0]		= 4.00;
	bar[1]		= 0.068947572928;
	bar[2]		= 0.00;

	//Load mass data
	//Mass			- class 05 units
	lb[0]		= 5.00;
	lb[1]		= 1.00;
	lb[2]		= 0.00;

	g[0]		= 5.00;
	g[1]		= 453.59237;
	g[2]		= 0.00;

	kg[0]		= 5.00;
	kg[1]		= 0.45359237;
	kg[2]		= 0.00;

	//Torque		- class 06 units
	ftlb[0]		= 6.00;
	ftlb[1]		= 1.00;
	ftlb[2]		= 0.00;

	Nm[0]		= 6.00;
	Nm[1]		= 1.35581794;
	Nm[2]		= 0.00;

	//Force		- class 07 units
	lbf[0]		= 7.00;
	lbf[1]		= 1.00;
	lbf[2]		= 0.00;

	N[0]		= 7.00;
	N[1]		= 4.44822161526;
	N[2]		= 0.00;

	//Energy	- class 08 units
	lbf_ft[0]	= 8.00;
	lbf_ft[1]	= 778.169262266;
	lbf_ft[2]	= 0.00;

	BTU[0]		= 8.00;
	BTU[1]		= 1.00;
	BTU[2]		= 0.00;

	J[0]		= 8.00;
	J[1]		= 1055.05585262;
	J[2]		= 0.00;

	kJ[0]		= 8.00;
	kJ[1]		= 1.05505585262;
	kJ[2]		= 0.00;

	//angle			- class 09 units
	radians[0]	= 9.00;
	radians[1]	= 1.00;
	radians[2]	= 0.00;

	degrees[0]	= 9.00;
	degrees[1]	= ( 180 / ( 3.1415926535897932384626433832795 ) );
	degrees[2]	= 0.00;
}

CUnits::~CUnits()
{

}

double CUnits::convert( double *from, double *to )
{
	
	//Local variables
	double		dtemp;

	//Check to see that we are consistent with our units
	switch( int(from[0]) )
	{
	case 0:
		if(from[0] == to[0])
		{
			//convert whatever the from is
			//to USCS
			dtemp	= ( in[1]/from[1]);
			return ( ( to[1]/in[1] ) * dtemp);
		}
		else
			return -1;
		break;

	case 1:
		if(from[0] == to[0])
		{
			//convert whatever the from is
			//to USCS
			dtemp	= ( in2[1]/from[1]);
			return ( ( to[1]/in2[1] ) * dtemp);
		}
		else
			return -1;
		break;

	case 2:
		if(from[0] == to[0])
		{
			//convert whatever the from is
			//to USCS
			dtemp	= ( in3[1]/from[1]);
			return ( ( to[1]/in3[1] ) * dtemp);
		}
		else
			return -1;
		break;

	case 3:
		if(from[0] == to[0])
		{
			//convert whatever the from is
			//to USCS
			dtemp	= ( R[1]/from[1]);
			return ( ( to[1]/R[1] ) * dtemp);
		}
		else
			return -1;
		break;

	case 4:
		if(from[0] == to[0])
		{
			//convert whatever the from is
			//to USCS
			dtemp	= ( PSIA[1]/from[1]);
			return ( ( to[1]/PSIA[1] ) * dtemp);
		}
		else
			return -1;
		break;

	case 5:
		if(from[0] == to[0])
		{
			//convert whatever the from is
			//to USCS
			dtemp	= ( lb[1]/from[1]);
			return ( ( to[1]/lb[1] ) * dtemp);
		}
		else
			return -1;
		break;

	case 6:
		if(from[0] == to[0])
		{
			//convert whatever the from is
			//to USCS
			dtemp	= ( ftlb[1]/from[1]);
			return ( ( to[1]/ftlb[1] ) * dtemp);
		}
		else
			return -1;
		break;

	case 7:
		if(from[0] == to[0])
		{
			//convert whatever the from is
			//to USCS
			dtemp	= ( lbf[1]/from[1]);
			return ( ( to[1]/lbf[1] ) * dtemp);
		}
		else
			return -1;
		break;

	case 8:
		if(from[0] == to[0])
		{
			//convert whatever the from is
			//to USCS (BTU's)
			dtemp	= ( BTU[1]/from[1]);
			return ( ( to[1]/BTU[1] ) * dtemp);
		}
		else
			return -1;
		break;

	case 9:
		if(from[0] == to[0])
		{
			//convert whatever the from is
			//to radians
			dtemp	= ( radians[1]/from[1]);
			return ( ( to[1]/radians[1] ) * dtemp);
		}
		else
			return -1;
		break;

	default:
		return -1;
	}
}
