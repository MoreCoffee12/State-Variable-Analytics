#pragma once
#include "tensor.h"

class CStressTensor : public CTensor
{

public:
    CStressTensor(void);
    ~CStressTensor(void);

    //object access methods
    bool GetNormalStress_xx( double *normal );
    bool GetNormalStress_yy( double *normal );
    bool GetNormalStress_zz( double *normal );
    bool GetPrincipalStress( double *stress_1, double *stress_2, double *stress_3 );
    bool GetShearStress_xy( double *shear );
    bool GetShearStress_xz( double *shear );
    bool GetShearStress_yz( double *shear );

    bool SetNormalStress_xx( double normal );
    bool SetNormalStress_yy( double normal );
    bool SetNormalStress_zz( double normal );
    bool SetShearStress_xy( double shear );
    bool SetShearStress_xz( double shear );
    bool SetShearStress_yz( double shear );

private:

};
