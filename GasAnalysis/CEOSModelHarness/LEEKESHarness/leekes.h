// LEEKES.h: interface for the CLEEKES class.
// 
// Implementation of the Starling modified BWR equation of state
//
//////////////////////////////////////////////////////////////////////
#include "string.h"
#include "math.h"
#include "eosmodel.h"

#ifndef _CLEEKES
#define _CLEEKES

class CLEEKES : public CEOSMODEL
{
public:

	/*Variables
	******************************************************************/
	//class 	CLogFile		*debugLog;

	/*Functions
	******************************************************************/
					CLEEKES();
	virtual			~CLEEKES();
	double			GetMolarDensity_TP_USCS( double T, double P );
	double			GetMolarDensity_TP_SI( double T, double P ){ return 0;}
	double			GetFugacity_TP_USCS( double T, double P );
	double			GetFugacity_TP_SI( double T, double P ){ return 0; }
	double			EOSFugacity( double T, double MD );

	double			GetSDepart_TP_USCS( double T, double P );
	double			GetSDepart_TP_SI( double T, double P ){return 0;}

	virtual double	GetHDepart_TP_USCS( double T, double P );
	virtual double	GetHDepart_TP_SI( double T,	double P ){return 0;}
	
     double			GetSDepart_MT_USCS( double M, double T ){return 0;}
	double			GetSDepart_MT_SI( double M, double T ){return 0;}

     double			GetHDepart_MT_USCS( double M, double T ){return 0;}
	double			GetHDepart_MT_SI( double M,	double T ){return 0;}

     double			EOS( double T, double MD );

	//Lee-Kessler object query functions
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

     //Lee-Kessler specific call
     bool                CalcSimple( double T, double P, double *SimpleVr );
     bool                CalcDeviation( double T, double P, double *RefVr );

protected:

	/*Variables
	******************************************************************/
    double			sb1;
	double			sb2;
	double			sb3;
	double			sb4;

	double			sc1;
	double			sc2;
	double			sc3;
	double			sc4;

	double			sd1;
	double			sd2;

	double			sbeta;
	double			sgamma;

	double			rb1;
	double			rb2;
	double			rb3;
	double			rb4;
	double			rB;

	double			rc1;
	double			rc2;
	double			rc3;
	double			rc4;
	double			rC;

	double			rd1;
	double			rd2;
	double			rD;

	double			rbeta;
	double			rgamma;
    double romega;

	double			sVr;
	double			sTr;
	double			sZ;
	double			rVr;
	double			rTr;
	double			rZ;

	double			Z;

	/*Functions
	******************************************************************/
	double			GetFalsePositionMolarDensity( double T, double P, double inititalMD );
	double			GetFalsePositionFugacity( double T, double P, double initialMD ){return 0;}

private:

	/*Functions
	******************************************************************/
	double			GuessMolarDensity_TP_USCS( double T, double P );
     double			RightSide( double *Vr, 
                                        double B,
                                        double C,
                                        double D,
                                        double beta,
                                        double gamma,
                                        double c4,
                                        double *Tr );
     double			RightSideRef(){return 0;}
     bool                CalcLeeKesler( bool Simple, double T, double P, double *Vr );
     bool                CalcLeeKeslerBCD( bool Simple, double Tr, 
                                        double *B, 
                                        double *C, 
                                        double *D );
     bool                CalcLeeKeslerVaporPressure( double Tr, 
                                                       double term1, 
                                                       double term2, 
                                                       double term3,
                                                       double term4,
                                                       double *Pvpr );
     bool                CalcReferenceVaporPressure( double Tr, double *Pvpr );
     bool                CalcSimpleVaporPressure( double Tr, double *Pvpr );
     bool                SolveSpecificVolume( double T, 
                                                  double P,
                                                  bool Simple,
                                                  double *V, 
                                                  double Vinit);

};

#endif
