#include "StdAfx.h"
#include ".\solid.h"

CSolid::CSolid(void)
{
}

CSolid::~CSolid(void)
{
}

//calculate the strain tensor from the stress and elastic data
//within the object
bool CSolid::CalcStrainFromStress()
{
    double Einv, v, dtemp, normal_xx, normal_yy, normal_zz, shear_xy, shear_xz, shear_yz;

    if( !GetNormalStress_xx( &normal_xx ) )
        return false;
    if( !GetNormalStress_yy( &normal_yy ) )
        return false;
    if( !GetNormalStress_zz( &normal_zz ) )
        return false;
    if( !GetShearStress_xy( &shear_xy ) )
        return false;
    if( !GetShearStress_xz( &shear_xz ) )
        return false;
    if( !GetShearStress_yz( &shear_yz ) )
        return false;
    if( !GetPoissonsRatio( &v ) )
        return false;

    //the inverse of the modulus appears often; retrieve and invert
    //the young's modulus value
    if( !GetModulus( &Einv ) )
        return false;
    Einv = ( 1.0 / Einv );

    //the normal xx strain
    dtemp = ( Einv * ( normal_xx - ( v * ( normal_yy + normal_zz ) ) ) ); 
    if( !SetNormalStrain_xx( dtemp ) )
        return false;
    //the normal yy strain
    dtemp = ( Einv * ( normal_yy - ( v * ( normal_xx + normal_zz ) ) ) ); 
    if( !SetNormalStrain_yy( dtemp ) )
        return false;
    //the normal zz strain
    dtemp = ( Einv * ( normal_zz - ( v * ( normal_yy + normal_xx ) ) ) ); 
    if( !SetNormalStrain_zz( dtemp ) )
        return false;
    //the shear xy strain
    dtemp = ( Einv * ( 1 + v ) * shear_xy );
    if( !SetShearStrain_xy( dtemp ) )
        return false;
    //the shear xz strain
    dtemp = ( Einv * ( 1 + v ) * shear_xz );
    if( !SetShearStrain_xz( dtemp ) )
        return false;
    //the shear yz strain
    dtemp = ( Einv * ( 1 + v ) * shear_yz );
    if( !SetShearStrain_yz( dtemp ) )
        return false;

    return true;
}

//calculate the stress tensor from the strain tensor and elastic data
//within the object
bool CSolid::CalcStressFromStrain()
{
    double E, Einv, v, dtemp, normal_xx, normal_yy, normal_zz, shear_xy, shear_xz, shear_yz;

    if( !GetNormalStrain_xx( &normal_xx ) )
        return false;
    if( !GetNormalStrain_yy( &normal_yy ) )
        return false;
    if( !GetNormalStrain_zz( &normal_zz ) )
        return false;
    if( !GetShearStrain_xy( &shear_xy ) )
        return false;
    if( !GetShearStrain_xz( &shear_xz ) )
        return false;
    if( !GetShearStrain_yz( &shear_yz ) )
        return false;
    if( !GetPoissonsRatio( &v ) )
        return false;

    //retrive the young's modulus value
    if( !GetModulus( &E ) )
        return false;
    //this term appears in the normal stresses
    Einv = ( E / ( ( 1 + v ) * ( 1- ( 2 * v ) ) ) );

    //the normal xx stress
    dtemp = Einv * ( ( ( 1- v ) * normal_xx ) + ( v * ( normal_yy + normal_zz ) )  );
    if( !SetNormalStress_xx( dtemp ) )
        return false;
    //the normal yy stress
    dtemp = Einv * ( ( ( 1- v ) * normal_yy ) + ( v * ( normal_xx + normal_zz ) )  );
    if( !SetNormalStress_yy( dtemp ) )
        return false;
    //the normal zz stress
    dtemp = Einv * ( ( ( 1- v ) * normal_zz ) + ( v * ( normal_xx + normal_yy ) )  );
    if( !SetNormalStress_zz( dtemp ) )
        return false;
    //the shear xy stress
    dtemp = ( E * shear_xy ) / ( 1 + v ) ;
    if( !SetShearStress_xy( dtemp ) )
        return false;
    //the shear xz stress
    dtemp = ( E * shear_xz ) / ( 1 + v ) ;
    if( !SetShearStress_xz( dtemp ) )
        return false;
    //the shear yz stress
    dtemp = ( E * shear_yz ) / ( 1 + v ) ;
    if( !SetShearStress_yz( dtemp ) )
        return false;

    return true;
}
