// BWRS.h: interface for the CBWRS class.
// 
// Implementation of the Starling modified BWR equation of state
//
//////////////////////////////////////////////////////////////////////
#include "string.h"
#include "math.h"
#include "eosmodel.h"

#ifndef _CBWRS
#define _CBWRS

class CBWRS : public CEOSMODEL
{
public:

	/*Variables
	******************************************************************/
	//class 	CLogFile		*debugLog;

	/*Functions
	******************************************************************/
					CBWRS();
	virtual			~CBWRS();
	double			GetMolarDensity_TP_USCS( double T, double P );
	double			GetMolarDensity_TP_SI( double T, double P );
	double			GetFugacity_TP_USCS( double T, double P );
	double			GetFugacity_TP_SI( double T, double P ){ return 0; }
	double			EOSFugacity( double T, double MD );

	double			GetSDepart_TP_USCS( double T, double P );
	double			GetSDepart_TP_SI( double T, double P ){return 0;}
	double			GetSDepart_MT_USCS( double M, double T );
	double			GetSDepart_MT_SI( double M, double T ){return 0;}

	double			GetHDepart_TP_USCS( double T, double P );
	double			GetHDepart_TP_SI( double T,	double P ){return 0;}
	double			GetHDepart_MT_USCS( double M, double T );
	double			GetHDepart_MT_SI( double M,	double T ){return 0;}
	
	double			EOS( double T, double MD );

	//BRWS object query functions
	double			GetBo( int fluidindex );
	double			GetAo( int fluidindex );
	double			GetCo( int fluidindex );
	double			GetDo( int fluidindex );
	double			GetEo( int fluidindex );
	double			Getb( int fluidindex );
	double			Geta( int fluidindex );
	double			Getd( int fluidindex );
	double			GetAlpha( int fluidindex );
	double			Getc( int fluidindex );
	double			GetGamma( int fluidindex );

protected:

	/*Functions
	******************************************************************/
	double			GetFalsePositionMolarDensity( double T, double P, double inititalMD );
	double			GetFalsePositionFugacity( double T, double P, double initialMD ){return 0;}
private:

	/*Functions
	******************************************************************/
	double			GuessMolarDensity_TP_USCS( double T, double P );

};

#endif
