#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <math.h>
#include "ccranksliderharness\crankslider.h"
#include "CSolidMechanicsharness\Solid.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
// housekeeping code below this marker
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
int MechShowVersion()
{
	return 000;
}

int MechShowBuild()
{
	return 043;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
// solid mechanics below this marker
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

//return the largest principal strain from a strain tensor
bool CGetStrainPrincipal_1( double normalstrain_xx, double normalstrain_yy, double normalstrain_zz,
                                           double shearstrain_xy, double shearstrain_xz, double shearstrain_yz,
                                           double *principal_1 )
{

    //locals
    double dtemp = -1;
    class CSolid *solid;
    solid = new CSolid();

    //transfer the strain tensor and material information   
    if( !solid->SetNormalStrain_xx( normalstrain_xx ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetNormalStrain_yy( normalstrain_yy ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetNormalStrain_zz( normalstrain_zz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStrain_xy( shearstrain_xy ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStrain_xz( shearstrain_xz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStrain_yz( shearstrain_yz ) )
	{
		delete solid;
		return false;
	}

    //perform the calculation and return the value
    if( !solid->GetPrincipalStrain_1( principal_1 ) )
	{
		delete solid;
		return false;
	}

    //end of function housekeeping
    delete solid;

    return true;
}

//return the middle principal strain from a strain tensor
bool CGetStrainPrincipal_2( double normalstrain_xx, double normalstrain_yy, double normalstrain_zz,
                                           double shearstrain_xy, double shearstrain_xz, double shearstrain_yz,
                                           double *principal_2 )
{

    //locals
    double dtemp = -1;
    class CSolid *solid;
    solid = new CSolid();

    //transfer the strain tensor and material information   
    if( !solid->SetNormalStrain_xx( normalstrain_xx ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetNormalStrain_yy( normalstrain_yy ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetNormalStrain_zz( normalstrain_zz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStrain_xy( shearstrain_xy ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStrain_xz( shearstrain_xz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStrain_yz( shearstrain_yz ) )
	{
		delete solid;
		return false;
	}

    //perform the calculation and return the value
    if( !solid->GetPrincipalStrain_2( principal_2 ) )
	{
		delete solid;
		return false;
	}

    //end of function housekeeping
    delete solid;

    return true;
}

//return the largest principal strain from a strain tensor
bool CGetStrainPrincipal_3( double normalstrain_xx, double normalstrain_yy, double normalstrain_zz,
                                           double shearstrain_xy, double shearstrain_xz, double shearstrain_yz,
                                           double *principal_3 )
{

    //locals
    double dtemp = -1;
    class CSolid *solid;
    solid = new CSolid();

    //transfer the strain tensor and material information   
    if( !solid->SetNormalStrain_xx( normalstrain_xx ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetNormalStrain_yy( normalstrain_yy ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetNormalStrain_zz( normalstrain_zz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStrain_xy( shearstrain_xy ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStrain_xz( shearstrain_xz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStrain_yz( shearstrain_yz ) )
	{
		delete solid;
		return false;
	}

    //perform the calculation and return the value
    if( !solid->GetPrincipalStrain_3( principal_3 ) )
	{
		delete solid;
		return false;
	}

    //end of function housekeeping
    delete solid;

    return true;
}


//return the normal strain in the x direction from a stress tensor
bool CGetNormalStrain_xxFromStressTensor( double normalstress_xx, double normalstress_yy, double normalstress_zz,
                                           double shearstress_xy, double shearstress_xz, double shearstress_yz,
                                           double modulus, double poissons, double *normalstrain_xx )
{
    //locals
    double dtemp = -1;
    class CSolid *solid;
    solid = new CSolid();

    //transfer the stress tensor and material information   
    if( !solid->SetNormalStress_xx( normalstress_xx ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetNormalStress_yy( normalstress_yy ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetNormalStress_zz( normalstress_zz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStress_xy( shearstress_xy ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStress_xz( shearstress_xz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStress_yz( shearstress_yz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetModulus( modulus ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetPoissonsRatio( poissons ) )
	{
		delete solid;
		return false;
	}

    //perform the transform.  To do - trap on a false
    if( !solid->CalcStrainFromStress() )
	{
		delete solid;
		return false;
	}

    //retrieve the desired value
    if( !solid->GetNormalStrain_xx( normalstrain_xx ) )
	{
		delete solid;
		return false;
	}

    //end of function housekeeping
    delete solid;

    return true;
}


//return the normal strain in the y direction from a stress tensor
bool CGetNormalStrain_yyFromStressTensor( double normalstress_xx, double normalstress_yy, double normalstress_zz,
                                           double shearstress_xy, double shearstress_xz, double shearstress_yz,
                                           double modulus, double poissons, double *normalstrain_yy )
{
    //locals
    double dtemp = -1;
    class CSolid *solid;
    solid = new CSolid();

    //transfer the stress tensor and material information   
    if( !solid->SetNormalStress_xx( normalstress_xx ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetNormalStress_yy( normalstress_yy ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetNormalStress_zz( normalstress_zz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStress_xy( shearstress_xy ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStress_xz( shearstress_xz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStress_yz( shearstress_yz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetModulus( modulus ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetPoissonsRatio( poissons ) )
	{
		delete solid;
		return false;
	}

    //perform the transform.  To do - trap on a false
    if( !solid->CalcStrainFromStress() )
	{
		delete solid;
		return false;
	}

    //retrieve the desired value
    if( !solid->GetNormalStrain_yy( normalstrain_yy ) )
	{
		delete solid;
		return false;
	}

    //end of function housekeeping
    delete solid;

    return true;
}

//return the normal strain in the z direction from a stress tensor
bool CGetNormalStrain_zzFromStressTensor( double normalstress_xx, double normalstress_yy, double normalstress_zz,
                                           double shearstress_xy, double shearstress_xz, double shearstress_yz,
                                           double modulus, double poissons, double *normalstrain_zz )
{
    //locals
    double dtemp = -1;
    class CSolid *solid;
    solid = new CSolid();

    //transfer the stress tensor and material information   
    if( !solid->SetNormalStress_xx( normalstress_xx ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetNormalStress_yy( normalstress_yy ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetNormalStress_zz( normalstress_zz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStress_xy( shearstress_xy ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStress_xz( shearstress_xz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStress_yz( shearstress_yz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetModulus( modulus ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetPoissonsRatio( poissons ) )
	{
		delete solid;
		return false;
	}

    //perform the transform.  To do - trap on a false
    if( !solid->CalcStrainFromStress() )
	{
		delete solid;
		return false;
	}

    //retrieve the desired value
    if( !solid->GetNormalStrain_zz( normalstrain_zz ) )
	{
		delete solid;
		return false;
	}

    //end of function housekeeping
    delete solid;

    return true;
}

//return the shear strain xy from a stress tensor
bool CGetShearStrain_xyFromStressTensor( double normalstress_xx, double normalstress_yy, double normalstress_zz,
                                           double shearstress_xy, double shearstress_xz, double shearstress_yz,
                                           double modulus, double poissons, double *shearstrain_xy )
{
    //locals
    double dtemp = -1;
    class CSolid *solid;
    solid = new CSolid();

    //transfer the stress tensor and material information   
    if( !solid->SetNormalStress_xx( normalstress_xx ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetNormalStress_yy( normalstress_yy ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetNormalStress_zz( normalstress_zz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStress_xy( shearstress_xy ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStress_xz( shearstress_xz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStress_yz( shearstress_yz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetModulus( modulus ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetPoissonsRatio( poissons ) )
	{
		delete solid;
		return false;
	}

    //perform the transform.  To do - trap on a false
    if( !solid->CalcStrainFromStress() )
	{
		delete solid;
		return false;
	}

    //retrieve the desired value
    if( !solid->GetShearStrain_xy( shearstrain_xy ) )
	{
		delete solid;
		return false;
	}

    //end of function housekeeping
    delete solid;

    return true;
}

//return the shear strain xz from a stress tensor
bool CGetShearStrain_xzFromStressTensor( double normalstress_xx, double normalstress_yy, double normalstress_zz,
                                           double shearstress_xy, double shearstress_xz, double shearstress_yz,
                                           double modulus, double poissons, double *shearstrain_xz )
{
    //locals
    double dtemp = -1;
    class CSolid *solid;
    solid = new CSolid();

    //transfer the stress tensor and material information   
    if( !solid->SetNormalStress_xx( normalstress_xx ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetNormalStress_yy( normalstress_yy ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetNormalStress_zz( normalstress_zz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStress_xy( shearstress_xy ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStress_xz( shearstress_xz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStress_yz( shearstress_yz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetModulus( modulus ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetPoissonsRatio( poissons ) )
	{
		delete solid;
		return false;
	}

    //perform the transform.  To do - trap on a false
    if( !solid->CalcStrainFromStress() )
	{
		delete solid;
		return false;
	}

    //retrieve the desired value
    if( !solid->GetShearStrain_xz( shearstrain_xz ) )
	{
		delete solid;
		return false;
	}

    //end of function housekeeping
    delete solid;

    return true;
}

//return the shear strain yz from a stress tensor
bool CGetShearStrain_yzFromStressTensor( double normalstress_xx, double normalstress_yy, double normalstress_zz,
                                           double shearstress_xy, double shearstress_xz, double shearstress_yz,
                                           double modulus, double poissons, double *shearstrain_yz )
{
    //locals
    double dtemp = -1;
    class CSolid *solid;
    solid = new CSolid();

    //transfer the stress tensor and material information   
    if( !solid->SetNormalStress_xx( normalstress_xx ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetNormalStress_yy( normalstress_yy ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetNormalStress_zz( normalstress_zz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStress_xy( shearstress_xy ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStress_xz( shearstress_xz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStress_yz( shearstress_yz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetModulus( modulus ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetPoissonsRatio( poissons ) )
	{
		delete solid;
		return false;
	}

    //perform the transform.  To do - trap on a false
    if( !solid->CalcStrainFromStress() )
	{
		delete solid;
		return false;
	}

    //retrieve the desired value
    if( !solid->GetShearStrain_yz( shearstrain_yz ) )
	{
		delete solid;
		return false;
	}

    //end of function housekeeping
    delete solid;

    return true;
}

//return the normal stress in the x direction from a strain tensor
bool CGetNormalStress_xxFromStrainTensor( double normalstrain_xx, double normalstrain_yy, double normalstrain_zz,
                                           double shearstrain_xy, double shearstrain_xz, double shearstrain_yz,
                                           double modulus, double poissons, double *normalstress_xx )
{
    //locals
    double dtemp = -1;
    class CSolid *solid;
    solid = new CSolid();

    //transfer the stress tensor and material information   
    if( !solid->SetNormalStrain_xx( normalstrain_xx ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetNormalStrain_yy( normalstrain_yy ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetNormalStrain_zz( normalstrain_zz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStrain_xy( shearstrain_xy ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStrain_xz( shearstrain_xz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStrain_yz( shearstrain_yz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetModulus( modulus ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetPoissonsRatio( poissons ) )
	{
		delete solid;
		return false;
	}

    //perform the transform.  To do - trap on a false
    if( !solid->CalcStressFromStrain() )
	{
		delete solid;
		return false;
	}

    //retrieve the desired value
    if( !solid->GetNormalStress_xx( normalstress_xx ) )
	{
		delete solid;
		return false;
	}

    //end of function housekeeping
    delete solid;

    return true;
}

//return the normal stress in the y direction from a strain tensor
bool CGetNormalStress_yyFromStrainTensor( double normalstrain_xx, double normalstrain_yy, double normalstrain_zz,
                                           double shearstrain_xy, double shearstrain_xz, double shearstrain_yz,
                                           double modulus, double poissons, double *normalstress_yy )
{
    //locals
    double dtemp = -1;
    class CSolid *solid;
    solid = new CSolid();

    //transfer the stress tensor and material information   
    if( !solid->SetNormalStrain_xx( normalstrain_xx ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetNormalStrain_yy( normalstrain_yy ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetNormalStrain_zz( normalstrain_zz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStrain_xy( shearstrain_xy ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStrain_xz( shearstrain_xz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStrain_yz( shearstrain_yz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetModulus( modulus ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetPoissonsRatio( poissons ) )
	{
		delete solid;
		return false;
	}

    //perform the transform.  To do - trap on a false
    if( !solid->CalcStressFromStrain() )
	{
		delete solid;
		return false;
	}

    //retrieve the desired value
    if( !solid->GetNormalStress_yy( normalstress_yy ) )
	{
		delete solid;
		return false;
	}

    //end of function housekeeping
    delete solid;

    return true;
}

//return the normal stress in the z direction from a strain tensor
bool CGetNormalStress_zzFromStrainTensor( double normalstrain_xx, double normalstrain_yy, double normalstrain_zz,
                                           double shearstrain_xy, double shearstrain_xz, double shearstrain_yz,
                                           double modulus, double poissons, double *normalstress_zz )
{
    //locals
    double dtemp = -1;
    class CSolid *solid;
    solid = new CSolid();

    //transfer the stress tensor and material information   
    if( !solid->SetNormalStrain_xx( normalstrain_xx ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetNormalStrain_yy( normalstrain_yy ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetNormalStrain_zz( normalstrain_zz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStrain_xy( shearstrain_xy ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStrain_xz( shearstrain_xz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStrain_yz( shearstrain_yz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetModulus( modulus ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetPoissonsRatio( poissons ) )
	{
		delete solid;
		return false;
	}

    //perform the transform.  To do - trap on a false
    if( !solid->CalcStressFromStrain() )
	{
		delete solid;
		return false;
	}

    //retrieve the desired value
    if( !solid->GetNormalStress_zz( normalstress_zz ) )
	{
		delete solid;
		return false;
	}

    //end of function housekeeping
    delete solid;

    return true;
}

//return the shear stress on the xy face from a strain tensor
bool CGetShearStress_xyFromStrainTensor( double normalstrain_xx, double normalstrain_yy, double normalstrain_zz,
                                           double shearstrain_xy, double shearstrain_xz, double shearstrain_yz,
                                           double modulus, double poissons, double *shearstress_xy )
{
    //locals
    double dtemp = -1;
    class CSolid *solid;
    solid = new CSolid();

    //transfer the stress tensor and material information   
    if( !solid->SetNormalStrain_xx( normalstrain_xx ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetNormalStrain_yy( normalstrain_yy ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetNormalStrain_zz( normalstrain_zz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStrain_xy( shearstrain_xy ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStrain_xz( shearstrain_xz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStrain_yz( shearstrain_yz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetModulus( modulus ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetPoissonsRatio( poissons ) )
	{
		delete solid;
		return false;
	}

    //perform the transform.  To do - trap on a false
    if( !solid->CalcStressFromStrain() )
	{
		delete solid;
		return false;
	}

    //retrieve the desired value
    if( !solid->GetShearStress_xy( shearstress_xy ) )
	{
		delete solid;
		return false;
	}

    //end of function housekeeping
    delete solid;

    return true;
}

//return the shear stress on the xz face from a strain tensor
bool CGetShearStress_xzFromStrainTensor( double normalstrain_xx, double normalstrain_yy, double normalstrain_zz,
                                           double shearstrain_xy, double shearstrain_xz, double shearstrain_yz,
                                           double modulus, double poissons, double *shearstress_xz )
{
    //locals
    double dtemp = -1;
    class CSolid *solid;
    solid = new CSolid();

    //transfer the stress tensor and material information   
    if( !solid->SetNormalStrain_xx( normalstrain_xx ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetNormalStrain_yy( normalstrain_yy ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetNormalStrain_zz( normalstrain_zz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStrain_xy( shearstrain_xy ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStrain_xz( shearstrain_xz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStrain_yz( shearstrain_yz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetModulus( modulus ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetPoissonsRatio( poissons ) )
	{
		delete solid;
		return false;
	}

    //perform the transform.  To do - trap on a false
    if( !solid->CalcStressFromStrain() )
	{
		delete solid;
		return false;
	}

    //retrieve the desired value
    if( !solid->GetShearStress_xz( shearstress_xz ) )
	{
		delete solid;
		return false;
	}

    //end of function housekeeping
    delete solid;

    return true;
}

//return the shear stress on the yz face from a strain tensor
bool CGetShearStress_yzFromStrainTensor( double normalstrain_xx, double normalstrain_yy, double normalstrain_zz,
                                           double shearstrain_xy, double shearstrain_xz, double shearstrain_yz,
                                           double modulus, double poissons, double *shearstress_yz )
{
    //locals
    double dtemp = -1;
    class CSolid *solid;
    solid = new CSolid();

    //transfer the stress tensor and material information   
    if( !solid->SetNormalStrain_xx( normalstrain_xx ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetNormalStrain_yy( normalstrain_yy ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetNormalStrain_zz( normalstrain_zz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStrain_xy( shearstrain_xy ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStrain_xz( shearstrain_xz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStrain_yz( shearstrain_yz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetModulus( modulus ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetPoissonsRatio( poissons ) )
	{
		delete solid;
		return false;
	}

    //perform the transform.  To do - trap on a false
    if( !solid->CalcStressFromStrain() )
	{
		delete solid;
		return false;
	}

    //retrieve the desired value
    if( !solid->GetShearStress_yz( shearstress_yz ) )
	{
		delete solid;
		return false;
	}

    //end of function housekeeping
    delete solid;

    return true;
}

//return the largest principal stress from a stress tensor
bool CGetStressPrincipal_1( double normalstress_xx, double normalstress_yy, double normalstress_zz,
                                           double shearstress_xy, double shearstress_xz, double shearstress_yz,
                                           double *principal_1 )
{

    //locals
    double dtemp = -1;
    class CSolid *solid;
    solid = new CSolid();

    //transfer the stress tensor and material information   
    if( !solid->SetNormalStress_xx( normalstress_xx ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetNormalStress_yy( normalstress_yy ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetNormalStress_zz( normalstress_zz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStress_xy( shearstress_xy ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStress_xz( shearstress_xz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStress_yz( shearstress_yz ) )
	{
		delete solid;
		return false;
	}

    //perform the calculation and return the value
    if( !solid->GetPrincipalStress_1( principal_1 ) )
	{
		delete solid;
		return false;
	}

    //end of function housekeeping
    delete solid;

    return true;
}

//return the middle principal stress from a stress tensor
bool CGetStressPrincipal_2( double normalstress_xx, double normalstress_yy, double normalstress_zz,
                                           double shearstress_xy, double shearstress_xz, double shearstress_yz,
                                           double *principal_2 )
{

    //locals
    double dtemp = -1;
    class CSolid *solid;
    solid = new CSolid();

    //transfer the stress tensor and material information   
    if( !solid->SetNormalStress_xx( normalstress_xx ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetNormalStress_yy( normalstress_yy ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetNormalStress_zz( normalstress_zz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStress_xy( shearstress_xy ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStress_xz( shearstress_xz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStress_yz( shearstress_yz ) )
	{
		delete solid;
		return false;
	}

    //perform the calculation and return the value
    if( !solid->GetPrincipalStress_2( principal_2 ) )
	{
		delete solid;
		return false;
	}

    //end of function housekeeping
    delete solid;

    return true;
}

//return the largest principal stress from a stress tensor
bool CGetStressPrincipal_3( double normalstress_xx, double normalstress_yy, double normalstress_zz,
                                           double shearstress_xy, double shearstress_xz, double shearstress_yz,
                                           double *principal_3 )
{

    //locals
    double dtemp = -1;
    class CSolid *solid;
    solid = new CSolid();

    //transfer the stress tensor and material information   
    if( !solid->SetNormalStress_xx( normalstress_xx ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetNormalStress_yy( normalstress_yy ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetNormalStress_zz( normalstress_zz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStress_xy( shearstress_xy ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStress_xz( shearstress_xz ) )
	{
		delete solid;
		return false;
	}
    if( !solid->SetShearStress_yz( shearstress_yz ) )
	{
		delete solid;
		return false;
	}

    //perform the calculation and return the value
    if( !solid->GetPrincipalStress_3( principal_3 ) )
	{
		delete solid;
		return false;
	}

    //end of function housekeeping
    delete solid;

    return true;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
// Applied mechanics code below this marker
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
double CDisplacement_USCS(double stroke, double l, double CrankAngleDegrees)
{
		    
	//local variables
	class CCrankSlider		*crankslidercurrent;
	double				temp;

	//initialize local variables
	crankslidercurrent		= new CCrankSlider();
	temp					= 0;

	/********************************************************************
	* We'll load the data									   *
	*********************************************************************/
     crankslidercurrent->SetStroke_USCS( stroke );
     crankslidercurrent->SetConnRodLength_USCS( l );
     crankslidercurrent->SetCrankAngleDegrees( CrankAngleDegrees );
     if( !crankslidercurrent->CalcSliderDisplacement() )
		return -1;

	temp = crankslidercurrent->GetSliderDisplacement_USCS();

	/********************************************************************
	* End-of-program housekeeping functions					        *
	*********************************************************************/
	//Free all elements in the strand
	delete crankslidercurrent;

    //Return a Value
    return temp;
    
}

double CConnRodAngle_Degrees(double stroke, double l, double CrankAngleDegrees)
{
    	//local variables
	class CCrankSlider		*crankslidercurrent;
	double				temp;

	//initialize local variables
	crankslidercurrent		= new CCrankSlider();
	temp					= 0;

	/********************************************************************
	* We'll load the data									   *
	*********************************************************************/
     crankslidercurrent->SetStroke_USCS( stroke );
     crankslidercurrent->SetConnRodLength_USCS( l );
     crankslidercurrent->SetCrankAngleDegrees( CrankAngleDegrees );
	if( !crankslidercurrent->CalcConnRodAngle() )
		return -1;

	temp					= crankslidercurrent->GetConnRodAngleDegrees();

	/********************************************************************
	* End-of-program housekeeping functions					        *
	*********************************************************************/
	//Free all elements in the strand
	delete crankslidercurrent;

    //Return a Value
    return temp;
    
}

double CVelocity_USCS(double stroke, 
                    double l,
                    double CrankAngleDegrees,
				double RPM)

{
	
     //local variables
	class CCrankSlider		     *crankslidercurrent;
	double					temp;

	//initialize local variables
	crankslidercurrent		     = new CCrankSlider();
	temp					     = 0;

     /********************************************************************
	* We'll load the data									   *
	*********************************************************************/
     crankslidercurrent->SetStroke_USCS( stroke );
     crankslidercurrent->SetConnRodLength_USCS( l );
     crankslidercurrent->SetCrankAngleDegrees( CrankAngleDegrees );
     crankslidercurrent->SetRPM( RPM );
	if( !crankslidercurrent->CalcSliderVelocity() )
		return -1;

	temp					     = crankslidercurrent->GetSliderVelocity_USCS();

	/********************************************************************
	* End-of-program housekeeping functions					        *
	*********************************************************************/
	//Free all elements in the strand
	delete crankslidercurrent;

    //Return a Value
    return temp;

}
    

double CAcceleration_USCS(double stroke,
					 double l,
					 double CrankAngleDegrees,
					 double RPM)
{

     //local variables
	class CCrankSlider		     *crankslidercurrent;
	double					temp;

	//initialize local variables
	crankslidercurrent		     = new CCrankSlider();
	temp					     = 0;

	/********************************************************************
	* We'll load the data									   *
	*********************************************************************/
     crankslidercurrent->SetStroke_USCS( stroke );
     crankslidercurrent->SetConnRodLength_USCS( l );
     crankslidercurrent->SetCrankAngleDegrees( CrankAngleDegrees );
     crankslidercurrent->SetRPM( RPM );
	if( !crankslidercurrent->CalcSliderAcceleration() )
		return -1;

	temp					     = crankslidercurrent->GetSliderAcceleration_USCS();

	/********************************************************************
	* End-of-program housekeeping functions					        *
	*********************************************************************/
	//Free all elements in the strand
	delete crankslidercurrent;

    //Return a Value
    return temp;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// This is the wrapper function to calculate the acceleration of the reciprocating components 
// allowing for angular acceleration of the crankshaft.
//
// Revision, 28 August 2014
// Added this function
//////////////////////////////////////////////////////////////////////////////////////////////////////
double CAccelerationWithAngAcc_USCS(double stroke,
					 double l,
					 double CrankAngleDegrees,
					 double PistonAngleDegrees,
					 double RPM,
                     double RPM01)
{
    // Local variables
	class CCrankSlider *crankslidercurrent;
	double dTemp;

	// Initialize local variables
	crankslidercurrent = new CCrankSlider();
	dTemp = 0;

	// Load the data
    if( !crankslidercurrent->SetStroke_USCS( stroke ) )
    {
        delete crankslidercurrent;
        return -1;
    }
    if( !crankslidercurrent->SetConnRodLength_USCS( l ) )
    {
        delete crankslidercurrent;
        return -1;
    }
    if( !crankslidercurrent->SetCrankAngleDegrees( CrankAngleDegrees ) )
    {
        delete crankslidercurrent;
        return -1;
    }
    if( !crankslidercurrent->SetPistonAngleDegrees( PistonAngleDegrees ) )
    {
        delete crankslidercurrent;
        return -1;
    }
    if( !crankslidercurrent->SetRPM( RPM ) )
    {
        delete crankslidercurrent;
        return -1;
    }
    if( !crankslidercurrent->SetRPM01( RPM01 ) )
    {
        delete crankslidercurrent;
        return -1;
    }

    // The object will refresh all members (including the acceleration)
    // before returning a value
	dTemp = crankslidercurrent->GetSliderAccelerationWithAngAcc_USCS();

	// Clear member
	delete crankslidercurrent;

    // Return a Value
    return dTemp;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Calculates the gas load acting on the throw
//////////////////////////////////////////////////////////////////////////////////////////////////////
double CGasLoad_USCS(double stroke,
                         double l,
                         double CrankAngleDegrees,
                         double RPM,
                         double PistonAreaCrankEnd_01,
                         double PistonAreaHeadEnd_01,
                         double PistonAreaCrankEnd_02,
                         double PistonAreaHeadEnd_02,
                         double PressureGageCrankEnd_01,
                         double PressureGageHeadEnd_01,
                         double PressureGageCrankEnd_02,
                         double PressureGageHeadEnd_02,
                         double AmbientPressure,
                         double PistonAssyMass,
                         double CrossheadMass,
                         double ConnRodSmallEndMass,
                         double ConnRodLargeEndMass,
                         bool CCW, 
                         bool Right )

{
     //local variables
	class CCrankSlider *crankslidercurrent;
	double dTemp;

	//initialize local variables
	crankslidercurrent  = NULL;
	dTemp = 0;


	//Instantiate the class
	crankslidercurrent = new CCrankSlider();


    //////////////////////////////////////////////////////////////////////////////////////////////////////
	// We'll load the data	
    //////////////////////////////////////////////////////////////////////////////////////////////////////
     crankslidercurrent->SetStroke_USCS( stroke );
     crankslidercurrent->SetConnRodLength_USCS( l );
     crankslidercurrent->SetCrankAngleDegrees( CrankAngleDegrees );
     crankslidercurrent->SetRPM( RPM );

     crankslidercurrent->SetPistonAreaCrankEnd_USCS( PistonAreaCrankEnd_01, 1 );
     crankslidercurrent->SetPistonAreaHeadEnd_USCS( PistonAreaHeadEnd_01, 1 );
     crankslidercurrent->SetPressureAbsCrankEnd_USCS( ( PressureGageCrankEnd_01 + AmbientPressure ), 1 );
     crankslidercurrent->SetPressureAbsHeadEnd_USCS( ( PressureGageHeadEnd_01 + AmbientPressure ), 1 );

     crankslidercurrent->SetPistonAreaCrankEnd_USCS( PistonAreaCrankEnd_02, 2 );
     crankslidercurrent->SetPistonAreaHeadEnd_USCS( PistonAreaHeadEnd_02, 2 );
     crankslidercurrent->SetPressureAbsCrankEnd_USCS( ( PressureGageCrankEnd_01 + AmbientPressure ), 2 );
     crankslidercurrent->SetPressureAbsHeadEnd_USCS( ( PressureGageCrankEnd_01 + AmbientPressure ), 2 );

     if( crankslidercurrent->bSetAmbientPressure_USCS( AmbientPressure ) == false )
     {
         return -1;
     }

     crankslidercurrent->SetCCW ( CCW );
     crankslidercurrent->SetRight ( Right );

     if( !crankslidercurrent->CalcSliderGasLoad() )
     {
          delete crankslidercurrent;
		return -1;
     }

	dTemp = crankslidercurrent->GetSliderGasLoad_USCS();

	/********************************************************************
	* End-of-program housekeeping functions						   *
	*********************************************************************/
	//Free all elements in the strand
	delete crankslidercurrent;

    //Return a Value
    return dTemp;
    
}

/***********************
 * Function Definition *
 *---------------------*******************************
 * Calculates the inertial load acting on the throw  *
 *****************************************************/
double CInertiaLoad_USCS(double stroke,
                         double l,
                         double CrankAngleDegrees,
                         double RPM,
                         double PistonAssyWeight,
                         double CrossheadWeight )

{
     //local variables
	class CCrankSlider			*crankslidercurrent;
	double					temp;

	//initialize local variables
	crankslidercurrent		     = NULL;
	temp					     = 0;


	//Instantiate the class
	crankslidercurrent		     = new CCrankSlider();


	/********************************************************************
	* We'll load the data.  A more robust app would check for success   *
     * after each parameter is loaded                                    *
	*********************************************************************/
     crankslidercurrent->SetStroke_USCS( stroke );
     crankslidercurrent->SetConnRodLength_USCS( l );
     crankslidercurrent->SetCrankAngleDegrees( CrankAngleDegrees );
     crankslidercurrent->SetRPM( RPM );
     
     crankslidercurrent->SetPistonAndRodWeight_USCS( PistonAssyWeight );
     crankslidercurrent->SetCrossheadWeight_USCS( CrossheadWeight );

	if( !crankslidercurrent->CalcSliderInertiaLoad() )
     {
          delete crankslidercurrent;
		return -1;
     }

	temp					     = crankslidercurrent->GetSliderInertiaLoad_USCS();

	/********************************************************************
	* End-of-program housekeeping functions						   *
	*********************************************************************/
	//Free all elements in the strand
	delete crankslidercurrent;

    //Return a Value
    return temp;
    
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// This is a wrapper function that returns the inertia load, allowing for angular acceleration
// of the crankshaft.
//
// Revision, 28 August 2014
// Added this function
//////////////////////////////////////////////////////////////////////////////////////////////////////
double CInertiaLoadWithAngAcc_USCS(double stroke,
                         double l,
                         double CrankAngleDegrees,
                         double PistonAngleDegrees,
                         double RPM,
                         double RPM01,
                         double PistonAssyWeight,
                         double CrossheadWeight )

{
    // Local variables
	class CCrankSlider *crankslidercurrent;
	double dTemp;

	// Initialize local variables
	crankslidercurrent = NULL;
	dTemp = 0;

	// Instantiate the class
	crankslidercurrent = new CCrankSlider();

	// Load the data into the object. 
    if( !crankslidercurrent->SetStroke_USCS( stroke ) )
    {
        delete crankslidercurrent;
        return -1;
    }
    if( !crankslidercurrent->SetConnRodLength_USCS( l ) )
    {
        delete crankslidercurrent;
        return -1;
    }
    if( !crankslidercurrent->SetCrankAngleDegrees( CrankAngleDegrees ) )
    {
        delete crankslidercurrent;
        return -1;
    }
    if( !crankslidercurrent->SetPistonAngleDegrees( PistonAngleDegrees ) )
    {
        delete crankslidercurrent;
        return -1;
    }
    if( !crankslidercurrent->SetRPM( RPM ) )
    {
        delete crankslidercurrent;
        return -1;
    }
    if( !crankslidercurrent->SetRPM01( RPM01 ) )
    {
        delete crankslidercurrent;
        return -1;
    }
    if( !crankslidercurrent->SetPistonAndRodWeight_USCS( PistonAssyWeight ) )
    {
        delete crankslidercurrent;
        return -1;
    }
    if( !crankslidercurrent->SetCrossheadWeight_USCS( CrossheadWeight ) )
    {
        delete crankslidercurrent;
        return -1;
    }

	// The object will update internal members before returning a value
    dTemp = crankslidercurrent->GetSliderInertiaLoadWithAngAcc_USCS();

	// Release memory
	delete crankslidercurrent;

    //Return a Value
    return dTemp;
    
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Definition 
//////////////////////////////////////////////////////////////////////////////////////////////////////
// Calculates the combined load acting on the throw
//////////////////////////////////////////////////////////////////////////////////////////////////////
double CCombinedLoad_USCS(double stroke,
                         double l,
                         double CrankAngleDegrees,
                         double RPM,
                         double PistonAreaCrankEnd_01,
                         double PistonAreaHeadEnd_01,
                         double PistonAreaCrankEnd_02,
                         double PistonAreaHeadEnd_02,
                         double PressureGageCrankEnd_01,
                         double PressureGageHeadEnd_01,
                         double PressureGageCrankEnd_02,
                         double PressureGageHeadEnd_02,
                         double AmbientPressure,
                         double PistonAssyWeight,
                         double CrossheadWeight,
                         double ConnRodSmallEndMass,
                         double ConnRodLargeEndMass,
                         bool CCW, 
                         bool Right )

{
    //local variables
	class CCrankSlider *crankslidercurrent;
	double temp;

	//initialize local variables
	crankslidercurrent = NULL;
	temp = 0;

	//Instantiate the class
	crankslidercurrent = new CCrankSlider();

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    // We'll load the data
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    crankslidercurrent->SetStroke_USCS( stroke );
    crankslidercurrent->SetConnRodLength_USCS( l );
    crankslidercurrent->SetCrankAngleDegrees( CrankAngleDegrees );
    crankslidercurrent->SetRPM( RPM );

    crankslidercurrent->SetPistonAreaCrankEnd_USCS( PistonAreaCrankEnd_01, 1 );
    crankslidercurrent->SetPistonAreaHeadEnd_USCS( PistonAreaHeadEnd_01, 1 );
    crankslidercurrent->SetPressureAbsCrankEnd_USCS( ( PressureGageCrankEnd_01 + AmbientPressure ), 1 );
    crankslidercurrent->SetPressureAbsHeadEnd_USCS( ( PressureGageHeadEnd_01 + AmbientPressure ), 1 );

    crankslidercurrent->SetPistonAreaCrankEnd_USCS( PistonAreaCrankEnd_02, 2 );
    crankslidercurrent->SetPistonAreaHeadEnd_USCS( PistonAreaHeadEnd_02, 2 );
    crankslidercurrent->SetPressureAbsCrankEnd_USCS( ( PressureGageCrankEnd_01 + AmbientPressure ), 2 );
    crankslidercurrent->SetPressureAbsHeadEnd_USCS( ( PressureGageCrankEnd_01 + AmbientPressure ), 2 );

    if( crankslidercurrent->bSetAmbientPressure_USCS( AmbientPressure ) == false )
    {
        return -1;
    }

    crankslidercurrent->SetPistonAndRodWeight_USCS( PistonAssyWeight );
    crankslidercurrent->SetCrossheadWeight_USCS( CrossheadWeight );

    crankslidercurrent->SetCCW ( CCW );
    crankslidercurrent->SetRight ( Right );

	if( !crankslidercurrent->CalcSliderCombinedLoad() )
    {
        delete crankslidercurrent;
		return -1;
    }

	temp = crankslidercurrent->GetSliderCombinedLoad_USCS();

    //Free memory
	delete crankslidercurrent;

    //Return a Value
    return temp;
    
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Calculates the combined load acting on the throw allowing for angular acceleration of the
// crankshaft.
//
// Revision, 28 August 2014
// Added this function
//////////////////////////////////////////////////////////////////////////////////////////////////////
double CCombinedLoadWithAngAcc_USCS(double stroke,
                         double l,
                         double CrankAngleDegrees,
                         double PistonAngleDegrees,
                         double RPM,
                         double RPM01,
                         double PistonAreaCrankEnd_01,
                         double PistonAreaHeadEnd_01,
                         double PistonAreaCrankEnd_02,
                         double PistonAreaHeadEnd_02,
                         double PressureGageCrankEnd_01,
                         double PressureGageHeadEnd_01,
                         double PressureGageCrankEnd_02,
                         double PressureGageHeadEnd_02,
                         double AmbientPressure,
                         double PistonAssyWeight,
                         double CrossheadWeight,
                         double ConnRodSmallEndMass,
                         double ConnRodLargeEndMass,
                         bool CCW, 
                         bool Right )

{

    // Local variables
	class CCrankSlider *crankslidercurrent;
	double dTemp;

	// Initialize local variables
	crankslidercurrent = NULL;
	dTemp = 0;

	// Instantiate the class
	crankslidercurrent = new CCrankSlider();

    // Load the data, beginning with basic kinematics
        if( !crankslidercurrent->SetStroke_USCS( stroke ) )
    {
        delete crankslidercurrent;
        return -1;
    }
    if( !crankslidercurrent->SetConnRodLength_USCS( l ) )
    {
        delete crankslidercurrent;
        return -1;
    }
    if( !crankslidercurrent->SetCrankAngleDegrees( CrankAngleDegrees ) )
    {
        delete crankslidercurrent;
        return -1;
    }
    if( !crankslidercurrent->SetPistonAngleDegrees( PistonAngleDegrees ) )
    {
        delete crankslidercurrent;
        return -1;
    }
    if( !crankslidercurrent->SetRPM( RPM ) )
    {
        delete crankslidercurrent;
        return -1;
    }
    if( !crankslidercurrent->SetRPM01( RPM01 ) )
    {
        delete crankslidercurrent;
        return -1;
    }

    // Load the piston areas and pressures for the inner cylinder
    if( !crankslidercurrent->SetPistonAreaCrankEnd_USCS( PistonAreaCrankEnd_01, 1 ) )
    {
        delete crankslidercurrent;
        return -1;
    }
    if( !crankslidercurrent->SetPistonAreaHeadEnd_USCS( PistonAreaHeadEnd_01, 1 ) )
    {
        delete crankslidercurrent;
        return -1;
    }
    if( !crankslidercurrent->SetPressureAbsCrankEnd_USCS( ( PressureGageCrankEnd_01 + AmbientPressure ), 1 ) )
    {
        delete crankslidercurrent;
        return -1;
    }
    if( !crankslidercurrent->SetPressureAbsHeadEnd_USCS( ( PressureGageHeadEnd_01 + AmbientPressure ), 1 ) )
    {
        delete crankslidercurrent;
        return -1;
    }

    // Load the piston areas and pressures for the outer cylinder.
    if( !crankslidercurrent->SetPistonAreaCrankEnd_USCS( PistonAreaCrankEnd_02, 2 ) )
    {
        delete crankslidercurrent;
        return -1;
    }
    if( !crankslidercurrent->SetPistonAreaHeadEnd_USCS( PistonAreaHeadEnd_02, 2 ) )
    {
        delete crankslidercurrent;
        return -1;
    }
    if( !crankslidercurrent->SetPressureAbsCrankEnd_USCS( ( PressureGageCrankEnd_02 + AmbientPressure ), 2 ) )
    {
        delete crankslidercurrent;
        return -1;
    }
    if( !crankslidercurrent->SetPressureAbsHeadEnd_USCS( ( PressureGageHeadEnd_02 + AmbientPressure ), 2 ) )
    {
        delete crankslidercurrent;
        return -1;
    }

    // Load the ambient pressure
    if( crankslidercurrent->bSetAmbientPressure_USCS( AmbientPressure ) == false )
    {
        delete crankslidercurrent;
        return -1;
    }

    // Load the reciprocating weights
    if( !crankslidercurrent->SetPistonAndRodWeight_USCS( PistonAssyWeight ) )
    {
        delete crankslidercurrent;
        return -1;
    }
    if( !crankslidercurrent->SetCrossheadWeight_USCS( CrossheadWeight ) )
    {
        delete crankslidercurrent;
        return -1;
    }

    // Crankshaft rotation and throw orientation
    if( !crankslidercurrent->SetCCW ( CCW ) )
    {
        delete crankslidercurrent;
        return -1;
    }
    if( !crankslidercurrent->SetRight ( Right ) )
    {
        delete crankslidercurrent;
        return -1;
    }

	// Internal members are updated prior to returning a value
    dTemp = crankslidercurrent->GetSliderCombinedLoadWithAngAcc_USCS();

    //Free memory
	delete crankslidercurrent;

    //Return a Value
    return dTemp;
    
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Calculates the vertical force on the crosshead 
//////////////////////////////////////////////////////////////////////////////////////////////////////
double CVerticalForce_USCS(double stroke,
                         double l,
                         double CrankAngleDegrees,
                         double RPM,
                         double PistonAreaCrankEnd_01,
                         double PistonAreaHeadEnd_01,
                         double PistonAreaCrankEnd_02,
                         double PistonAreaHeadEnd_02,
                         double PressureGageCrankEnd_01,
                         double PressureGageHeadEnd_01,
                         double PressureGageCrankEnd_02,
                         double PressureGageHeadEnd_02,
                         double AmbientPressure,
                         double PistonAssyWeight,
                         double CrossheadWeight,
                         double ConnRodSmallEndMass,
                         double ConnRodLargeEndMass,
                         bool CCW, 
                         bool Right )

{
     //local variables
	class CCrankSlider *crankslidercurrent;
	double temp;

	//initialize local variables
	crankslidercurrent = NULL;
	temp = 0;

	//Instantiate the class
	crankslidercurrent = new CCrankSlider();

    //////////////////////////////////////////////////////////////////////////////////////////////////////
	// We'll load the data
    //////////////////////////////////////////////////////////////////////////////////////////////////////
     crankslidercurrent->SetStroke_USCS( stroke );
     crankslidercurrent->SetConnRodLength_USCS( l );
     crankslidercurrent->SetCrankAngleDegrees( CrankAngleDegrees );
     crankslidercurrent->SetRPM( RPM );

     crankslidercurrent->SetPistonAreaCrankEnd_USCS( PistonAreaCrankEnd_01, 1 );
     crankslidercurrent->SetPistonAreaHeadEnd_USCS( PistonAreaHeadEnd_01, 1 );
     crankslidercurrent->SetPressureAbsCrankEnd_USCS( ( PressureGageCrankEnd_01 + AmbientPressure ), 1 );
     crankslidercurrent->SetPressureAbsHeadEnd_USCS( ( PressureGageHeadEnd_01 + AmbientPressure ), 1 );

     crankslidercurrent->SetPistonAreaCrankEnd_USCS( PistonAreaCrankEnd_02, 2 );
     crankslidercurrent->SetPistonAreaHeadEnd_USCS( PistonAreaHeadEnd_02, 2 );
     crankslidercurrent->SetPressureAbsCrankEnd_USCS( ( PressureGageCrankEnd_01 + AmbientPressure ), 2 );
     crankslidercurrent->SetPressureAbsHeadEnd_USCS( ( PressureGageCrankEnd_01 + AmbientPressure ), 2 );

     if( crankslidercurrent->bSetAmbientPressure_USCS( AmbientPressure ) == false )
     {
         return -1;
     }

     crankslidercurrent->SetPistonAndRodWeight_USCS( PistonAssyWeight );
     crankslidercurrent->SetCrossheadWeight_USCS( CrossheadWeight );

     crankslidercurrent->SetCCW ( CCW );
     crankslidercurrent->SetRight ( Right );

	if( !crankslidercurrent->CalcCrossheadVerticalForce() )
     {
          delete crankslidercurrent;
		return -1;
     }

	temp = crankslidercurrent->GetCrossheadVerticalForce_USCS();

	//Free memory
	delete crankslidercurrent;

    //Return a Value
    return temp;
    
}

/***********************
 * Function Definition *
 *---------------------*******************************
 * Calculates the radial force acting on the crank   *
 * shaft main bearing given the geometry and the     *
 * force.                                            *
 *****************************************************/
double CMainBearingRadialForce_USCS (double stroke, 
                                        double l, 
                                        double CrankAngleDegrees, 
                                        double RPM,
                                        double PistonAreaCrankEnd_01,
                                        double PistonAreaHeadEnd_01,
                                        double PistonAreaCrankEnd_02,
                                        double PistonAreaHeadEnd_02,
                                        double PressureGageCrankEnd_01,
                                        double PressureGageHeadEnd_01,
                                        double PressureGageCrankEnd_02,
                                        double PressureGageHeadEnd_02,
                                        double AmbientPressure,
                                        double PistonAssyWeight,
                                        double CrossheadWeight,
                                        double ConnRodSmallEndMass,
                                        double ConnRodLargeEndMass,
                                        bool CCW, 
                                        bool Right )
{

	//local variables
	class CCrankSlider			*crankslidercurrent;
	double					temp;

	//initialize local variables
	crankslidercurrent		     = NULL;
	temp					     = 0;


	//Instantiate the class
	crankslidercurrent		     = new CCrankSlider();


	/********************************************************************
	* We'll load the data									   *
	*********************************************************************/
     crankslidercurrent->SetStroke_USCS( stroke );
     crankslidercurrent->SetConnRodLength_USCS( l );
     crankslidercurrent->SetCrankAngleDegrees( CrankAngleDegrees );
     crankslidercurrent->SetRPM( RPM );

     crankslidercurrent->SetPistonAreaCrankEnd_USCS( PistonAreaCrankEnd_01, 1 );
     crankslidercurrent->SetPistonAreaHeadEnd_USCS( PistonAreaHeadEnd_01, 1 );
     crankslidercurrent->SetPressureAbsCrankEnd_USCS( PressureGageCrankEnd_01, 1 );
     crankslidercurrent->SetPressureAbsHeadEnd_USCS( PressureGageHeadEnd_01, 1 );

     crankslidercurrent->SetPistonAreaCrankEnd_USCS( PistonAreaCrankEnd_02, 2 );
     crankslidercurrent->SetPistonAreaHeadEnd_USCS( PistonAreaHeadEnd_02, 2 );
     crankslidercurrent->SetPressureAbsCrankEnd_USCS( PressureGageCrankEnd_01, 2 );
     crankslidercurrent->SetPressureAbsHeadEnd_USCS( PressureGageCrankEnd_01, 2 );

     if( crankslidercurrent->bSetAmbientPressure_USCS( AmbientPressure ) == false )
     {
         return -1;
     }

     crankslidercurrent->SetPistonAndRodWeight_USCS( PistonAssyWeight );
     crankslidercurrent->SetCrossheadWeight_USCS( CrossheadWeight );

     crankslidercurrent->SetCCW ( CCW );
     crankslidercurrent->SetRight ( Right );

	//turn the engine over...
     if( !crankslidercurrent->CalcMainBearingRadialForce() )
     {
          delete crankslidercurrent;
		return -1;
     }

	temp					     = crankslidercurrent->GetMainBearingRadialForce_USCS();

	/********************************************************************
	* End-of-program housekeeping functions						   *
	*********************************************************************/
	//Free all elements in the strand
	delete crankslidercurrent;

    //Return a Value
    return temp;
    
}

/***********************
 * Function Definition *
 *---------------------********************************
 * Calculates the tangential force acting on the crank*
 * shaft main bearing given the geometry and the	    *
 * force.									    *
 ******************************************************/
double CMainBearingTangentialForce_USCS (double stroke, 
                                        double l, 
                                        double CrankAngleDegrees, 
                                        double RPM,
                                        double PistonAreaCrankEnd_01,
                                        double PistonAreaHeadEnd_01,
                                        double PistonAreaCrankEnd_02,
                                        double PistonAreaHeadEnd_02,
                                        double PressureGageCrankEnd_01,
                                        double PressureGageHeadEnd_01,
                                        double PressureGageCrankEnd_02,
                                        double PressureGageHeadEnd_02,
                                        double AmbientPressure,
                                        double PistonAssyWeight,
                                        double CrossheadWeight,
                                        double ConnRodSmallEndMass,
                                        double ConnRodLargeEndMass,
                                        bool CCW, 
                                        bool Right )
{
	//local variables
	class CCrankSlider			*crankslidercurrent;
	double					temp;

	//initialize local variables
	crankslidercurrent		     = NULL;
	temp					     = 0;


	//Instantiate the class
	crankslidercurrent		     = new CCrankSlider();

     //load crankshaft and cylinder location information
     crankslidercurrent->SetCCW( CCW );
     crankslidercurrent->SetRight( Right );


	/********************************************************************
	* We'll load the data									   *
	*********************************************************************/
     crankslidercurrent->SetStroke_USCS( stroke );
     crankslidercurrent->SetConnRodLength_USCS( l );
     crankslidercurrent->SetCrankAngleDegrees( CrankAngleDegrees );
     crankslidercurrent->SetRPM( RPM );

     crankslidercurrent->SetPistonAreaCrankEnd_USCS( PistonAreaCrankEnd_01, 1 );
     crankslidercurrent->SetPistonAreaHeadEnd_USCS( PistonAreaHeadEnd_01, 1 );
     crankslidercurrent->SetPressureAbsCrankEnd_USCS( PressureGageCrankEnd_01, 1 );
     crankslidercurrent->SetPressureAbsHeadEnd_USCS( PressureGageHeadEnd_01, 1 );

     crankslidercurrent->SetPistonAreaCrankEnd_USCS( PistonAreaCrankEnd_02, 2 );
     crankslidercurrent->SetPistonAreaHeadEnd_USCS( PistonAreaHeadEnd_02, 2 );
     crankslidercurrent->SetPressureAbsCrankEnd_USCS( PressureGageCrankEnd_01, 2 );
     crankslidercurrent->SetPressureAbsHeadEnd_USCS( PressureGageCrankEnd_01, 2 );

     if( crankslidercurrent->bSetAmbientPressure_USCS( AmbientPressure ) == false )
     {
         return -1;
     }

     crankslidercurrent->SetPistonAndRodWeight_USCS( PistonAssyWeight );
     crankslidercurrent->SetCrossheadWeight_USCS( CrossheadWeight );

     crankslidercurrent->SetCCW ( CCW );
     crankslidercurrent->SetRight ( Right );

	//turn the engine over...
	if( !crankslidercurrent->CalcMainBearingTangentForce() )
     {
          delete crankslidercurrent;
		return -1;
     }

	temp					     = crankslidercurrent->GetMainBearingTangentForce_USCS();

	/********************************************************************
	* End-of-program housekeeping functions						   
	*********************************************************************/
	//Free all elements in the strand
	delete crankslidercurrent;

     //Return a Value
     return temp;
    
}

/***********************
 * Function Definition *
 *---------------------********************************
 * Calculates the torque force acting on the crank    
 * shaft main bearing given the geometry and the	    
 * force.									    
 ******************************************************/
double CMainBearingTorque_USCS (double stroke, 
                                        double l, 
                                        double CrankAngleDegrees, 
                                        double RPM,
                                        double PistonAreaCrankEnd_01,
                                        double PistonAreaHeadEnd_01,
                                        double PistonAreaCrankEnd_02,
                                        double PistonAreaHeadEnd_02,
                                        double PressureGageCrankEnd_01,
                                        double PressureGageHeadEnd_01,
                                        double PressureGageCrankEnd_02,
                                        double PressureGageHeadEnd_02,
                                        double AmbientPressure,
                                        double PistonAssyWeight,
                                        double CrossheadWeight,
                                        double ConnRodSmallEndMass,
                                        double ConnRodLargeEndMass,
                                        bool CCW, 
                                        bool Right )
{
	//local variables
	class CCrankSlider			*crankslidercurrent;
	double					temp;

	//initialize local variables
	crankslidercurrent		     = NULL;
	temp					     = 0;


	//Instantiate the class
	crankslidercurrent		     = new CCrankSlider();

     //load crankshaft and cylinder location information
     crankslidercurrent->SetCCW( CCW );
     crankslidercurrent->SetRight( Right );


	/********************************************************************
	* We'll load the data									   *
	*********************************************************************/
     crankslidercurrent->SetStroke_USCS( stroke );
     crankslidercurrent->SetConnRodLength_USCS( l );
     crankslidercurrent->SetCrankAngleDegrees( CrankAngleDegrees );
     crankslidercurrent->SetRPM( RPM );

     crankslidercurrent->SetPistonAreaCrankEnd_USCS( PistonAreaCrankEnd_01, 1 );
     crankslidercurrent->SetPistonAreaHeadEnd_USCS( PistonAreaHeadEnd_01, 1 );
     crankslidercurrent->SetPressureAbsCrankEnd_USCS( PressureGageCrankEnd_01, 1 );
     crankslidercurrent->SetPressureAbsHeadEnd_USCS( PressureGageHeadEnd_01, 1 );

     crankslidercurrent->SetPistonAreaCrankEnd_USCS( PistonAreaCrankEnd_02, 2 );
     crankslidercurrent->SetPistonAreaHeadEnd_USCS( PistonAreaHeadEnd_02, 2 );
     crankslidercurrent->SetPressureAbsCrankEnd_USCS( PressureGageCrankEnd_01, 2 );
     crankslidercurrent->SetPressureAbsHeadEnd_USCS( PressureGageCrankEnd_01, 2 );

     if( crankslidercurrent->bSetAmbientPressure_USCS( AmbientPressure ) == false )
     {
         return -1;
     }

     crankslidercurrent->SetPistonAndRodWeight_USCS( PistonAssyWeight );
     crankslidercurrent->SetCrossheadWeight_USCS( CrossheadWeight );

     crankslidercurrent->SetCCW ( CCW );
     crankslidercurrent->SetRight ( Right );

	//turn the engine over...
	if( !crankslidercurrent->CalcMainBearingTorque() )
     {
          delete crankslidercurrent;
		return -1;
     }

	temp					     = crankslidercurrent->GetMainBearingTorque_USCS();

	/********************************************************************
	* End-of-program housekeeping functions						   *
	*********************************************************************/
	//Free all elements in the strand
	delete crankslidercurrent;

     //Return a Value
     return temp;
    
}
