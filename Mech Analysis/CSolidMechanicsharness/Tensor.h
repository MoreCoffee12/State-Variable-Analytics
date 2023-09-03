#pragma once

#include "nr.h"
using namespace std;

class CTensor
{
public:
    CTensor(void);
    ~CTensor(void);

    //member access methods
    bool GetNormal_xx( double *normal );
    bool GetNormal_yy( double *normal );
    bool GetNormal_zz( double *normal );
    bool GetShear_xy( double *shear );
    bool GetShear_xz( double *shear );
    bool GetShear_yz( double *shear );

    bool SetNormal_xx( double normal );
    bool SetNormal_yy( double normal );
    bool SetNormal_zz( double normal );
    bool SetShear_xy( double shear );
    bool SetShear_xz( double shear );
    bool SetShear_yz( double shear );

    //tensor properties
    bool GetPrincipals( double *principal_1, double *principal_2, double *principal_3 );
    bool GetInvariants( double *invariant_1, double *invariant_2, double *invariant_3 );

private:

    //members
    double Normal_xx;
    double Normal_yy;
    double Normal_zz;

    double Shear_xy;
    double Shear_xz;
    double Shear_yz;

};
