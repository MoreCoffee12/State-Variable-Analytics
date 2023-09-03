#include "StdAfx.h"
#include ".\straintensor.h"

CStrainTensor::CStrainTensor(void)
{
}

CStrainTensor::~CStrainTensor(void)
{
}

//return the xx normal Strain
bool CStrainTensor::GetNormalStrain_xx( double *normal )
{
    if( !GetNormal_xx( normal ) )
        return false;
    return true;
}

//return the yy normal Strain
bool CStrainTensor::GetNormalStrain_yy( double *normal )
{
    if( !GetNormal_yy( normal ) )
        return false;
    return true;
}

//return the zz normal Strain
bool CStrainTensor::GetNormalStrain_zz( double *normal )
{
    if( !GetNormal_zz( normal ) )
        return false;
    return true;
}

//return the principal strains
bool CStrainTensor::GetPrincipalStrain( double *strain_1, double *strain_2, double *strain_3 )
{
    if( !GetPrincipals( strain_1, strain_2, strain_3 ) )
        return false;
    return true;
}

//return the xy shear Strain
bool CStrainTensor::GetShearStrain_xy( double *shear )
{
    if( !GetShear_xy( shear ) )
        return false;
    return true;
}

//return the xz shear Strain
bool CStrainTensor::GetShearStrain_xz( double *shear )
{
    if( !GetShear_xz( shear ) )
        return false;
    return true;
}

//return the yz shear Strain
bool CStrainTensor::GetShearStrain_yz( double *shear )
{
    if( !GetShear_yz( shear ) )
        return false;
    return true;
}

//return the strain invariants
bool CStrainTensor::GetStrainInvariants( double *Inv1, double *Inv2, double *Inv3 )
{
    if( !GetInvariants( Inv1, Inv2, Inv3 ) )
        return false;
    return true;
}

//load the xx normal Strain
bool CStrainTensor::SetNormalStrain_xx( double normal )
{
    if( !SetNormal_xx( normal ) )
        return false;
    return true;
}

//load the yy normal Strain
bool CStrainTensor::SetNormalStrain_yy( double normal )
{
    if( !SetNormal_yy( normal ) )
        return false;
    return true;
}

//load the zz normal Strain
bool CStrainTensor::SetNormalStrain_zz( double normal )
{
    if( !SetNormal_zz( normal ) )
        return false;
    return true;
}

//load the xy shear Strain
bool CStrainTensor::SetShearStrain_xy( double shear )
{
    if( !SetShear_xy( shear ) )
        return false;
    return true;
}

//load the xz shear Strain
bool CStrainTensor::SetShearStrain_xz( double shear )
{
    if( !SetShear_xz( shear ) )
        return false;
    return true;
}

//load the yz shear Strain
bool CStrainTensor::SetShearStrain_yz( double shear )
{
    if( !SetShear_yz( shear ) )
        return false;
    return true;
}
