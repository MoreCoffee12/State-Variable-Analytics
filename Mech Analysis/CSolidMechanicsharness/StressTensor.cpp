#include "StdAfx.h"
#include ".\stresstensor.h"

CStressTensor::CStressTensor(void)
{
}

CStressTensor::~CStressTensor(void)
{
}

//return the xx normal stress
bool CStressTensor::GetNormalStress_xx( double *normal )
{
    if( !GetNormal_xx( normal ) )
        return false;
    return true;
}

//return the yy normal stress
bool CStressTensor::GetNormalStress_yy( double *normal )
{
    if( !GetNormal_yy( normal ) )
        return false;
    return true;
}

//return the zz normal stress
bool CStressTensor::GetNormalStress_zz( double *normal )
{
    if( !GetNormal_zz( normal ) )
        return false;
    return true;
}

//return the principal stress
bool CStressTensor::GetPrincipalStress( double *stress_1, double *stress_2, double *stress_3 )
{
    if( !GetPrincipals( stress_1, stress_2, stress_3 ) )
        return false;
    return true;
}

//return the xy shear stress
bool CStressTensor::GetShearStress_xy( double *shear )
{
    if( !GetShear_xy( shear ) )
        return false;
    return true;
}

//return the xz shear stress
bool CStressTensor::GetShearStress_xz( double *shear )
{
    if( !GetShear_xz( shear ) )
        return false;
    return true;
}

//return the yz shear stress
bool CStressTensor::GetShearStress_yz( double *shear )
{
    if( !GetShear_yz( shear ) )
        return false;
    return true;
}

//load the xx normal stress
bool CStressTensor::SetNormalStress_xx( double normal )
{
    if( !SetNormal_xx( normal ) )
        return false;
    return true;
}

//load the yy normal stress
bool CStressTensor::SetNormalStress_yy( double normal )
{
    if( !SetNormal_yy( normal ) )
        return false;
    return true;
}

//load the zz normal stress
bool CStressTensor::SetNormalStress_zz( double normal )
{
    if( !SetNormal_zz( normal ) )
        return false;
    return true;
}

//load the xy shear stress
bool CStressTensor::SetShearStress_xy( double shear )
{
    if( !SetShear_xy( shear ) )
        return false;
    return true;
}

//load the xz shear stress
bool CStressTensor::SetShearStress_xz( double shear )
{
    if( !SetShear_xz( shear ) )
        return false;
    return true;
}

//load the yz shear stress
bool CStressTensor::SetShearStress_yz( double shear )
{
    if( !SetShear_yz( shear ) )
        return false;
    return true;
}
