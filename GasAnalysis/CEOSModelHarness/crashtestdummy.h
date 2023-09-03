// crashtestdummy.h: interface for the CCrashTestDummy class.
// 
// Empty object to test the parent object
//
//////////////////////////////////////////////////////////////////////
#include "string.h"
#include "math.h"
#include "logfile.h"
#include "eosmodel.h"

#ifndef _CCrashTestDummy
#define _CCrashTestDummy

class CCrashTestDummy : public CEOSMODEL
{
public:

	/*Variables
	******************************************************************/
	//class 	CLogFile		*debugLog;

	/*Functions
	******************************************************************/
					CCrashTestDummy();
	virtual			~CCrashTestDummy();
	double			GetMolarDensity_TP_USCS( double T, double P);
	double			GetMolarDensity_TP_SI( double T, double P);
	double			GetFalsePositionMolarDensity( double T, double P, double initialMD );

	double			GetFugacity_TP_USCS( double T, double P );
	double			GetFugacity_TP_SI( double T, double P ){return 0;}
	double			GetFalsePositionFugacity( double T, double P, double initialMD );
	double			EOSFugacity( double T, double MD );

	double			GetSDepart_TP_USCS( double T, double P){return 0;}
	double			GetSDepart_TP_SI( double T, double P){return 0;}
	double			GetSDepart_MT_USCS( double M, double T){return 0;}
	double			GetSDepart_MT_SI( double M, double T){return 0;}
	double			GetHDepart_TP_USCS( double T, double P){return 0;}
	double			GetHDepart_TP_SI( double T, double P){return 0;}
	double			GetHDepart_MT_USCS( double M, double T){return 0;}
	double			GetHDepart_MT_SI( double M, double T){return 0;}
	double			GetSDepartLiquid_TP_USCS( double T, double P );
	double			GetSDepartVapor_TP_USCS( double T, double P );

	double			EOS( double T, double MD );

private:

	/*Functions
	******************************************************************/
	double			GuessMolarDensity();

};

#endif
