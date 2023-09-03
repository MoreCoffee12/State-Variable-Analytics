#pragma once
#include "tensor.h"

class CStrainTensor : public CTensor
{
public:
    CStrainTensor(void);
    ~CStrainTensor(void);

    //object access methods
    bool GetNormalStrain_xx( double *normal );
    bool GetNormalStrain_yy( double *normal );
    bool GetNormalStrain_zz( double *normal );
    bool GetPrincipalStrain( double *strain_1, double *strain_2, double *strain_3 );
    bool GetShearStrain_xy( double *shear );
    bool GetShearStrain_xz( double *shear );
    bool GetShearStrain_yz( double *shear );
    bool GetStrainInvariants( double *Inv1, double *Inv2, double *Inv3 );

    bool SetNormalStrain_xx( double normal );
    bool SetNormalStrain_yy( double normal );
    bool SetNormalStrain_zz( double normal );
    bool SetShearStrain_xy( double shear );
    bool SetShearStrain_xz( double shear );
    bool SetShearStrain_yz( double shear );

private:

};
