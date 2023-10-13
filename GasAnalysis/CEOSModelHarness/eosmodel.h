// EOSMODEL.h: interface for the CEOSMODEL class.
//
// This object is the heart and soul of the equations of state
// (EOS) code.  It loads the fluid properties for all properties
// and provides the vvirtual functions below.  These are the 
// minimum virtual functions required for recip compressor cylinder
// analysis.
//
// 19 Jun 01
//
// Brian Howard
//////////////////////////////////////////////////////////////////////
//
// Revision, 13 Oct 2023
//
// Updated to include heat of vaporization functions.
//
// B. Howard
//
/////////////////////////////////////////////////////////////////////

#include "string.h"
#include "math.h"
#include "FluidProp.h"
#include "units.h"
#include "meshan.h"
#include <iostream>
#include <stdio.h>

#define FLUIDCOUNT 25
#define kCOUNT 625

#ifndef _CEOSMODEL
#define _CEOSMODEL

class CEOSMODEL  
{
public:

	/*Methods
	******************************************************************/
					CEOSMODEL();
	virtual			~CEOSMODEL();

	//Message handler methods
	bool            GetMessageStatus();
	int				GetMessageCount();
	char            *GetMessageMain( int mesno );
	char            *GetMessageAux( int mesno );
	int				GetMessagePriority( int mesno );
	bool            AddMessage( CMeshan *element,
									int priority,
									char *maintext,
									char *auxtext);
	bool			ClearMessages();

	//These function perform the calculations and are
	//called by the solvers not by the user
	virtual double      EOSFugacity(double T, 
						double MD) = 0;
	virtual double	     EOS( double T, double  MD ) = 0;

	//These methods return fugacity 
	virtual double	     GetFugacity_TP_USCS( double T,
									double P)	= 0;
	virtual double	     GetFugacity_TP_SI( double T,
									double P)	= 0;

	//These functions return molar density 
	virtual double      GetMolarDensity_TP_USCS( double T, 
									double P )	= 0;
	virtual double	     GetMolarDensity_TP_SI( double T, 
									double P )	= 0;
	
	//specific volume functions
	double			GetV_TP_SI( double T, double P );
	double			GetV_TP_USCS( double T,
									double P );

	//These functions return entropy related values
	double			GetS_TP_USCS( double T,
									double P );
	double			GetS_TP_SI( double T,
									double P );
	double			GetSIdeal_USCS( double T );
	double			GetSIdeal_SI( double T ){return 0;}
	double			GetSIdeal_USCS( double T, double P );
	double			GetSIdeal_SI( double T, double P ){return 0;}

	virtual double	     GetSDepart_TP_USCS( double T,
									double P )	= 0;
	virtual double	     GetSDepart_TP_SI( double T,
									double P )	= 0;
	virtual double	     GetSDepart_MT_USCS( double M,
									double T )	= 0;
	virtual double	     GetSDepart_MT_SI( double M,
									double T )	= 0;
	
	
	//These functions return properties associated with
	//the saturation curves
	double			GetVaporPressure_T_USCS( double T);
	double			GetVaporPressure_T_SI( double T);
	double			GetVaporPressure_T_USCS( double T, double *MDl, double *MDv );
	double			GetVaporPressure_T_SI( double T, double *MDl, double *MDv );

	double			GetSatLiqV_T_USCS( double T);
	double			GetSatLiqV_T_SI( double T);
	double			GetSatVapV_T_USCS( double T);
	double			GetSatVapV_T_SI( double T);

	double			GetSatLiqH_T_USCS( double T);
	double			GetSatLiqH_T_SI( double T);
	double			GetSatVapH_T_USCS( double T);
	double			GetSatVapH_T_SI( double T);

	double			GetSatLiqS_T_USCS( double T);
	double			GetSatLiqS_T_SI( double T){return 0;}
	double			GetSatVapS_T_USCS( double T);
	double			GetSatVapS_T_SI( double T){return 0;}

	double			GetVaporTemperature_P_USCS( double P);
	double			GetVaporTemperature_P_SI( double P);
	double			GetVaporTemperature_P_USCS( double P, double *MDl, double *MDv );

	double			GetSatLiqV_P_USCS( double P);
	double			GetSatLiqV_P_SI( double P);
	double			GetSatVapV_P_USCS( double P);
	double			GetSatVapV_P_SI( double P);

	double			GetSatLiqH_P_USCS( double P);
	double			GetSatLiqH_P_SI( double P);
	double			GetSatVapH_P_USCS( double P);
	double			GetSatVapH_P_SI( double P);

	double			GetSatLiqS_P_USCS( double P);
	double			GetSatLiqS_P_SI( double P);
	double			GetSatVapS_P_USCS( double P );
	double			GetSatVapS_P_SI( double P );

	//These function will return temperature as a function
	//of two other state variables
    double			GetT_SP_USCS( double S, 
                                    double P);
    double			GetT_SP_SI( double S, 
                                    double P);
    double			GetT_HP_USCS( double H, 
                                    double P);
    double			GetT_HP_SI( double H, 
                                    double P);
    double         GetT_VP_SI( double V,
                                    double P);
    double         GetT_VP_USCS( double V,
                                    double P);

	//These methodss will return pressure as a function
	//of other state variables
	double			GetP_MT_USCS( double MD, 
								double T);
	double			GetP_MT_SI( double MD, 
								double T){return 0;}
	double			GetP_VS_USCS( double V, 
								double S);
	double			GetP_VS_SI( double V, 
								double S);
	double			GetP_VT_USCS( double V, 
								double T);
	double			GetP_VT_SI( double V, 
								double T);

	//These methods will return enthalpy as a function
	//of two other state variables
    double          GetH_PS_USCS( double P, double S );
    double          GetH_PS_SI( double P, double S );
	double			GetH_TP_USCS( double T, double P);
	double			GetH_TP_SI( double T, double P);
    double          GetH_TS_USCS( double T, double S );
    double          GetH_TS_SI( double T, double S );
	double			GetHIdeal_USCS( double T );
	double			GetHIdeal_SI( double T );

	// Revision, 13 Oct 2023, enthalpy of vaporization
	double GetHvap_SI(int iFluidindex, double T);
	double GetHvap_USCS(int iFluidindex, double T);

	// The EOS must provide the departure functions
	virtual double  GetHDepart_TP_USCS( double T, double P ) = 0;
	virtual double  GetHDepart_TP_SI( double T, double P ) = 0;
	virtual double  GetHDepart_MT_USCS( double M, double T ) = 0;
	virtual double  GetHDepart_MT_SI( double M, double T ) = 0;

	//object definition methods
	bool            SetMixtureData( double *MixArray );
	bool            SetMaxIterations( int maxiters );
	bool            SetPrecision( double prec );

	//Object query methods
    bool            GetkCount( int *iCount );
    bool            GetkValue( int iIndex, double *dblValue );
    int				GetFluidCount();
	int				GetTPGLIndex( int fluidindex );
	int	            *GetCASNo( int fluidindex );
	char			*GetFormula( int fluidindex );
	char			*GetName( int fluidindex );
	char			*GetName_Alt01( int fluidindex );
    double          dblGetTrace();

	//mixture and pure fluid mole weight query functions
	double			GetMolWeight( int fluidindex );
	double			GetMolWeight_mx();


	//pure fluid critical temperature query functions
	double			GetTempCrit_USCS( int fluidindex );
	double			GetTempCrit_SI( int fluidindex );

	//mixture psuedo critical temperature query functions
	double			GetTempCrit_mx_USCS();
	double			GetTempCrit_mx_SI();

	//pure fluid critical pressure query functions
	double			GetPresCrit_USCS( int fluidindex );
	double			GetPresCrit_SI( int fluidindex );

	//mixture psuedo critical pressure query functions
	double			GetPresCrit_mx_USCS();
	double			GetPresCrit_mx_SI();

	//pure fluid critical pressure query functions
	double			GetVolCrit_USCS( int fluidindex );
	double			GetVolCrit_SI( int fluidindex );

	//mixture psuedo critical valume query functions
	double			GetVolCrit_mx_USCS();
	double			GetVolCrit_mx_SI();

	//pure fluid and mixture eccentricity functions
	double			GetEcc( int fluidindex );
	double			GetEcc_mx();

	//percent mole weight query functions
	double			GetPercentMW( int fluidindex );

	//the gas specific constants
	double			GetR_USCS( int fluidindex );
	double			GetR_SI( int fluidindex ){return 0;}

    // Bo Parameter
    double          GetBo_USCS( int fluidindex );
    double          GetBo_mx_USCS();

    // Ao Parameter
    double          GetAo_USCS( int fluidindex );
    double          GetAo_mx_USCS();

    // Co Parameter
    double          GetCo_USCS( int fluidindex );
    double          GetCo_mx_USCS();

    // Do Parameter
    double          GetDo_USCS( int fluidindex );
    double          GetDo_mx_USCS();

    // Eo Parameter
    double          GetEo_USCS( int fluidindex );
    double          GetEo_mx_USCS();

    // b Parameter
    double          Getb_USCS( int fluidindex );
    double          Getb_mx_USCS();

    // a Parameter
    double          Geta_USCS( int fluidindex );
    double          Geta_mx_USCS();

	//engine limits
	double			GetTmin_USCS( int fluidindex );
	double			GetTmin_SI( int fluidindex );
	double			GetTmax_USCS( int fluidindex );
	double			GetTmax_SI( int fluidindex );
	double			GetPmin_USCS( int fluidindex );
	double			GetPmin_SI( int fluidindex );
	double			GetPmax_USCS( int fluidindex );
	double			GetPmax_SI( int fluidindex );

	//the enthalpy of formation
	double			Gethfo_USCS( int fluidindex );
	double			Gethfo_SI( int fluidindex );
	double			Gethfo_USCS( );
	double			Gethfo_SI( );

	//the entropy of formation
	double			Getsfo_USCS( int fluidindex );
	double			Getsfo_SI( int fluidindex );
	double			Getsfo_USCS( );
	double			Getsfo_SI( );

	//the enthalpy from JANAF correlation
	double			Geth_h298_USCS( double T );
	double			Geth_h298_SI( double T );

	//the entropy from JANAF correlation
	double			Getso_USCS( double T );
	double			Getso_SI( double T );

	//heating values
	double			GetLHV_SI( int fluidindex );
	double			GetLHV_USCS( int fluidindex );
	double			GetLHV_mx_SI();
	double			GetLHV_mx_USCS();
	double			GetHHV_SI( int fluidindex );
	double			GetHHV_USCS( int fluidindex );
	double			GetHHV_mx_SI();
	double          GetHHV_mx_USCS();

     //dipole moments
     double         GetDipole_SI( int fluidindex );
     double         GetDipole_USCS( int fluidindex ){ return -1; }

     //viscocity data
     double         GetViscosityGas_TP_SI( double T, double P );
     double         GetViscosityGas_TP_USCS( double T, double P ); 

     //Specific heat data
     double GetCp_TP_SI( double T, double P);
     double GetCp_TP_USCS( double T, double P);
     double GetCv_TP_SI( double T, double P);
     double GetCv_TP_USCS( double T, double P);

protected:

	/*Members
	******************************************************************/
	double			percentcomp[FLUIDCOUNT];
	FluidProp       FluidList[FLUIDCOUNT];
	FluidProp       Mixture;
	double			A[12];
	double			B[12];
	int				FluidCount;
	double			r_USCS;
	double			r_SI;
	double			Precision;
	int				MaxIterations;
	char			errchar[2];
	CUnits			units;

	double          Tr;
	double			Pr;
	bool            uniary;




	// Methods
    ///////////////////////////////////////////////////////////////////
	bool			ToUSCS();
	bool			ToUSCS( int fluidindex );
	bool			ValidityCheck();
	bool			ValidityCheckFluid( int fluidindex );
	class			CMeshan *messagelist;

	// message handler methods
	class           CMeshan *AddMessage( class CMeshan *element );
	void            RemoveMessage( class CMeshan *element );



	//These functions find the molar densiities from different
	//initial guesses
	virtual double	GetFalsePositionMolarDensity( double T,
									double P, double initialMD )	= 0;
	
	//These functions find the fugacity from different
	//initial guesses
	virtual double	GetFalsePositionFugacity( double T,
									double P, double initialMD )	= 0;

private:

	//Members
    ///////////////////////////////////////////////////////////////////
    int _ikCount;    
    double _k[kCOUNT];
    double _dblTrace;

    //Methods
    ///////////////////////////////////////////////////////////////////
	bool			_bValidityCheckMixture( double *mixarray );
    bool            _bCalc_Ao( int iArrayIndex, double *Ao);
    bool            _bCalc_Bo( int iArrayIndex, double *Bo);
    bool            _bCalc_Co( int iArrayIndex, double *Co);
    bool            _bCalc_Do( int iArrayIndex, double *Do);
    bool            _bCalc_Eo( int iArrayIndex, double *Eo);
    bool            _bCalc_b( int iArrayIndex, double *b);
    bool            _bCalc_a( int iArrayIndex, double *a);
    bool            _bCalc_d( int iArrayIndex, double *d);
    bool            _bCalc_alpha( int iArrayIndex, double *alpha);
    bool            _bCalc_c( int iArrayIndex, double *c);
    bool            _bCalc_gamma( int iArrayIndex, double *gamma);

};

#endif
