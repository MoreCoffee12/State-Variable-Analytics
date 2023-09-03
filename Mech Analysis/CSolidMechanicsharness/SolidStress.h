#pragma once

#include "stresstensor.h"

// object for managing stress calcluations
// 
// Brian Howard, September 2007
//

class CSolidStress : public CStressTensor
{
public:
    CSolidStress(void);
    ~CSolidStress(void);

    //member access methods
    bool GetPrincipalStress_1( double *normal );
    bool GetPrincipalStress_2( double *normal );
    bool GetPrincipalStress_3( double *normal );
    bool GetShearStress_max( double *shear );

private:

    //members
    double PrincipalStress_1;
    double PrincipalStress_2;
    double PrincipalStress_3;
    double ShearStress_max;

};
