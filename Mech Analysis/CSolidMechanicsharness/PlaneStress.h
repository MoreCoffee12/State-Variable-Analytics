#pragma once

#include "math.h"
#include "stresstensor.h"

//////////////////////////////////////////////////////////////////
// object for managing plane stress calcluations
// 
// Brian Howard, September 2007
//
//////////////////////////////////////////////////////////////////

class CPlaneStress : public CStressTensor
{
public:
    //members

    //methods
    CPlaneStress(void);
    ~CPlaneStress(void);

    bool CalcPrincipalsMaxShear();
    bool GetPrincipal_1( double *normal );
    bool GetPrincipal_2( double *normal );
    bool GetShear_max( double *shear );

    bool SetPrincipal_1( double normal ) {return false; }
    bool SetPrincipal_2( double normal ) {return false; }
    bool SetShear_max( double shear ) {return false; }

private:

    //members
    double Principal_1;
    double Principal_2;
    double Shear_max;

};
