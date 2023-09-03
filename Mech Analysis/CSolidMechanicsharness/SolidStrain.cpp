#include "StdAfx.h"
#include ".\solidstrain.h"

CSolidStrain::CSolidStrain(void)
{
    PrincipalStrain_1 = -4;
    PrincipalStrain_2 = -5;
    PrincipalStrain_3 = -6;
    ShearStrain_max = -10;
}

CSolidStrain::~CSolidStrain(void)
{
}

//return the largest principal strain
bool CSolidStrain::GetPrincipalStrain_1( double *strain )
{
    double dtemp1, dtemp2;   

    if( !GetPrincipalStrain( strain, &dtemp1, &dtemp2 ) )
        return false;

    return true;
}

//return the largest principal strain
bool CSolidStrain::GetPrincipalStrain_2( double *strain )
{
    double dtemp1, dtemp2;   

    if( !GetPrincipalStrain( &dtemp1, strain, &dtemp2 ) )
        return false;

    return true;
}

//return the largest principal strain
bool CSolidStrain::GetPrincipalStrain_3( double *strain )
{
    double dtemp1, dtemp2;   

    if( !GetPrincipalStrain( &dtemp1, &dtemp2, strain ) )
        return false;

    return true;
}

