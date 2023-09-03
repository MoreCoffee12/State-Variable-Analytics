#include "StdAfx.h"
#include ".\solidstress.h"

CSolidStress::CSolidStress(void)
{

    PrincipalStress_1 = 1.1;
    PrincipalStress_2 = 2.2;
    PrincipalStress_3 = 3.3;
    ShearStress_max = 4.4;

}

CSolidStress::~CSolidStress(void)
{
}

//return the largest principal stress
bool CSolidStress::GetPrincipalStress_1( double *normal )
{
    double dtemp1, dtemp2;   

    if( !GetPrincipalStress( normal, &dtemp1, &dtemp2 ) )
        return false;

    return true;
}

//return the middle principal stress
bool CSolidStress::GetPrincipalStress_2( double *normal )
{
    double dtemp1, dtemp2;   

    if( !GetPrincipalStress( &dtemp1, normal, &dtemp2 ) )
        return false;

    return true;
}

//return the smallest principal stress
bool CSolidStress::GetPrincipalStress_3( double *normal )
{
    double dtemp1, dtemp2;   

    if( !GetPrincipalStress( &dtemp1, &dtemp2, normal ) )
        return false;

    return true;
}

//return the maximum shear stress
bool CSolidStress::GetShearStress_max( double *shear )
{
    return false;
}

