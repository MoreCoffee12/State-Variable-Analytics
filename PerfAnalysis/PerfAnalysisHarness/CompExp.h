// CompExp.h: interface for the CCompExp class.
//
//////////////////////////////////////////////////////////////////////
#include "math.h"
#include "units.h"
#include "crankslider.h"

#define PI 3.1415926535897932384626433832795

class CCompExpCurve : public CCrankSlider   
{
public:

	/*Members
	******************************************************************/

     /*Methods
	******************************************************************/
				                    CCompExpCurve();
	virtual                            ~CCompExpCurve();

     //Workhorse functions
     bool bCalcPressure();
     bool bCalcPressure( double dSuctionValveCloseAngle );
     bool                               CalcPressureLeak();
	
     
     //Object access functions
     bool                               SetClearance_Percent( double inputclearance );
     bool                               SetnComp( double inputn );
     bool                               SetnExp( double inputn );
     bool                               SetPressureAbsDischarge_USCS( double inputpressure );
     bool                               SetPressureAbsDischarge_SI(){return false;}
     bool                               SetPressureAbsSuction_USCS( double inputpressure );
     bool                               SetPressureAbsSuction_SI(){return false;}
     bool                               SetBoreDiameter_USCS( double inputbore );
     bool                               SetBoreDiameter_SI(){return false;}
     bool                               SetRodDiameter_USCS( double inputrod );
     bool                               SetRodDiameter_SI(){return false;}
     bool bSetHeadEnd( bool bIsHeadEnd, bool bUseDefaultSuctionValveCloseAngle );
     bool                               SetLeakSuction( double inputleak );
     bool                               SetLeakDischarge( double inputleak );

     double                             GetClearance_Percent();
     double                             GetClearance_USCS();
     double                             GetClearance_SI();
     double                             GetnComp();
     double                             GetnExp();
     double GetDisplacedVolume_USCS();
     double GetDisplacedVolume_SI();
     double                             GetPressureAbsDischarge_USCS();
     double                             GetPressureAbsDischarge_SI();
     double                             GetPressureAbsSuction_USCS();
     double                             GetPressureAbsSuction_SI();
     double                             GetPressureAbsCrankAngle_USCS();
     double                             GetPressureAbsCrankAngle_SI();
     double                             GetBoreDiameter_USCS();
     double                             GetBoreDiameter_SI();
     double                             GetRodDiameter_USCS();
     double                             GetRodDiameter_SI();
     bool                               GetHeadEnd();
     double                             GetLeakSuction();
     double                             GetLeakDischarge();

     bool bGetTheoreticalCompressionCurveGage_USCS( bool bSteplessUnloader,
                                            double dSuctionValveClosureAngle,
                                            double *dPressureCurveArray,
                                            long lPressureArrayCount );
     bool bGetSuctionValveCloseAngle( double *dSuctionValveCloseAngle );
     bool bSetSuctionValveCloseAngle( double dSuctionValveCloseAngle );

private:

	/*Members
	******************************************************************/
    bool                               HeadEnd;

    double		                    Clearance_Percent;
    double dClearance_USCS;
    double dClearance_SI;
	double		                    nexp;
	double		                    ncomp;
	double		                    PressureAbsSuction_USCS;
	double		                    PressureAbsSuction_SI;
	double		                    PressureAbsDischarge_USCS;
    double                             PressureAbsDischarge_SI;
    double                             PressureAbsCrankAngle_USCS;
    double                             PressureAbsCrankAngle_SI;
	double		                    BoreDiameter_USCS;
    double                             BoreDiameter_SI;
    double                             RodDiameter_USCS;
    double                             RodDiameter_SI;
    double                             LeakSuction;
    double                             LeakDischarge;
    double dSuctionValveCloseAngle;
    class                              CUnits units;

    /*Methods
	******************************************************************/
	bool		                         validitycheck();
    bool                               ToSI();
};

