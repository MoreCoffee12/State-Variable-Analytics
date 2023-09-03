#pragma once

#include "straintensor.h"
#include "elasticconstants.h"

//////////////////////////////////////////////////////////////////
// object for managing plane strain calcluations
// 
// Brian Howard, October 2007
//
//////////////////////////////////////////////////////////////////

// note - because of the relationships between the stress and strain
// tensors, the stress tensor forms the base for the strain class
// as well as the stress object.
class CSolidStrain : public CStrainTensor
{

public:
    CSolidStrain(void);
    ~CSolidStrain(void);

    //object members access methods
    bool GetPrincipalStrain_1( double *normal );
    bool GetPrincipalStrain_2( double *normal );
    bool GetPrincipalStrain_3( double *normal );
    bool GetShearStrain_max( double *shear );

private:

    //members
    double PrincipalStrain_1;
    double PrincipalStrain_2;
    double PrincipalStrain_3;
    double ShearStrain_max;

};
