#include "StdAfx.h"
#include ".\tensor.h"

CTensor::CTensor(void)
{

    Normal_xx = -0.1;
    Normal_yy = -0.2;
    Normal_zz = -0.3;
    Shear_xy = -0.4;
    Shear_xz = -0.5;
    Shear_yz = -0.6;

}

CTensor::~CTensor(void)
{
}

//return the xx normal
bool CTensor::GetNormal_xx( double *normal )
{
    *normal = Normal_xx;
    return true;
}

//return the yy normal
bool CTensor::GetNormal_yy( double *normal )
{
    *normal = Normal_yy;
    return true;
}

//return the zz normal
bool CTensor::GetNormal_zz( double *normal )
{
    *normal = Normal_zz;
    return true;
}

//return the principal strains
bool CTensor::GetPrincipals( double *strain_1, double *strain_2, double *strain_3 )
{

    //locals.  primarily used by the eigenvalue solver
    DP a_d[3*3];
    int nrot;
    double dtemp;
    Vec_DP d(3);
    Mat_DP v(3,3);

    //transfer the values from the object to the local array
    a_d[0] = Normal_xx;
    a_d[1] = Shear_xy;
    a_d[2] = Shear_xz;
    a_d[3] = a_d[1];
    a_d[4] = Normal_yy;
    a_d[5] = Shear_yz;
    a_d[6] = a_d[2];
    a_d[7] = a_d[5];
    a_d[8] = Normal_zz;

    Mat_DP a(a_d, 3, 3 );

    NR::jacobi( a, d, v, nrot);

    *strain_1 = d[0];
    *strain_2 = d[1];
    *strain_3 = d[2];
    
    //order principals
    if( *strain_1 < *strain_3 )
    {
        dtemp = *strain_1;
        *strain_1 = *strain_3;
        *strain_3 = dtemp;
    }
    if( *strain_1 < *strain_2 )
    {
        dtemp = *strain_1;
        *strain_1 = *strain_2;
        *strain_2 = dtemp;
    }
    if( *strain_2 < *strain_3 )
    {
        dtemp = *strain_2;
        *strain_2 = *strain_3;
        *strain_3 = dtemp;
    }
    return true;

}

//return the xy shear
bool CTensor::GetShear_xy( double *shear )
{
    *shear = Shear_xy;
    return true;
}

//return the xz shear
bool CTensor::GetShear_xz( double *shear )
{
    *shear = Shear_xz;
    return true;
}

//return the yz shear
bool CTensor::GetShear_yz( double *shear )
{
    *shear = Shear_yz;
    return true;
}

//return the invariants for this strain tensor
bool CTensor::GetInvariants( double *invariant_1, double *invariant_2, double *invariant_3 )
{
    *invariant_1 = ( Normal_xx + Normal_yy + Normal_zz );

    *invariant_2 = ( ( Normal_xx * Normal_yy ) + ( Normal_yy * Normal_zz ) + ( Normal_xx * Normal_zz) );
    *invariant_2 = *invariant_2 + ( -( Shear_xy * Shear_xy ) - ( Shear_yz * Shear_yz ) - ( Shear_xz * Shear_xz ) );

    *invariant_3 = ( Normal_xx * Normal_yy * Normal_zz );
    *invariant_3 = ( *invariant_3 - ( Normal_xx * Shear_yz * Shear_yz ) );
    *invariant_3 = ( *invariant_3 - ( Normal_yy * Shear_xz * Shear_xz ) );
    *invariant_3 = ( *invariant_3 - ( Normal_zz * Shear_xy * Shear_xy ) );
    *invariant_3 = ( *invariant_3 + ( 2 * ( Shear_xy * Shear_xz * Shear_yz ) ) );

    return true;
}

//load the xx normal
bool CTensor::SetNormal_xx( double normal )
{
    Normal_xx = normal;
    return true;
}

//load the yy normal
bool CTensor::SetNormal_yy( double normal )
{
    Normal_yy = normal;
    return true;
}

//load the zz normal
bool CTensor::SetNormal_zz( double normal )
{
    Normal_zz = normal;
    return true;
}

//load the xy shear
bool CTensor::SetShear_xy( double shear )
{
    Shear_xy = shear;
    return true;
}

//load the xz shear
bool CTensor::SetShear_xz( double shear )
{
    Shear_xz = shear;
    return true;
}

//load the yz shear
bool CTensor::SetShear_yz( double shear )
{
    Shear_yz = shear;
    return true;
}

