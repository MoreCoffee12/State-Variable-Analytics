#include ".\planestress.h"

CPlaneStress::CPlaneStress(void)
{

    //definition of plane stress   
    SetNormalStress_zz( (double)0 );
    SetShearStress_xz( (double)0 );
    SetShearStress_yz( (double)0 );

    Principal_1 = -4;
    Principal_2 = -5;
    Shear_max = -6;
}

CPlaneStress::~CPlaneStress(void)
{
}

//calculate the principals and maximum shear
bool CPlaneStress::CalcPrincipalsMaxShear()
{
    double dtemp, dtemp2, normal_xx, normal_yy, shear_xy;

    //retrieve the values from the tensor
    if( !GetNormalStress_xx( &normal_xx ) )
        return false;
    if( !GetNormalStress_yy( &normal_yy ) )
        return false;
    if( !GetShearStress_xy( &shear_xy ) )
        return false;

    //calculate the first and second terms in the equation
    dtemp = ( ( normal_xx + normal_yy ) / 2 );
    dtemp2 = ( ( normal_xx - normal_yy ) / 2 ) * ( ( normal_xx - normal_yy ) / 2 );
    dtemp2 = sqrt( dtemp2 + ( shear_xy * shear_xy ) );
    
    //calculate the principals
    Principal_1 = dtemp + dtemp2;
    Principal_2 = dtemp - dtemp2;

    //order according to magnitude
    if( Principal_2 > Principal_1 )
    {
        dtemp = Principal_1;
        Principal_1 = Principal_2;
        Principal_2 = dtemp;
    }

    //calculate the maximum shear
    Shear_max = ( 0.50 * ( Principal_1 - Principal_2 ) );

    //success
    return true;
}

//return the largest principal stress
bool CPlaneStress::GetPrincipal_1( double *normal )
{
    *normal = Principal_1;
    return true;
}

//return the smallest principal stress
bool CPlaneStress::GetPrincipal_2( double *normal )
{
    *normal = Principal_2;
    return true;
}

//return the maximum shear stress
bool CPlaneStress::GetShear_max( double *shear )
{
    *shear = Shear_max;
    return true;
}
