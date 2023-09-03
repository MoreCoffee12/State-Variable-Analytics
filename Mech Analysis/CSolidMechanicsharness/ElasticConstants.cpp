#include "StdAfx.h"
#include ".\elasticconstants.h"

CElasticConstants::CElasticConstants(void)
{
    E = 1000;
    Poissons = 1001;
}

CElasticConstants::~CElasticConstants(void)
{
}

//return the modulus
bool CElasticConstants::GetModulus( double *modulus )
{
    *modulus = E;
    return true;
}

//return Poisson's ratio
bool CElasticConstants::GetPoissonsRatio( double *poissons )
{
    *poissons = Poissons;
    return true;
}

//load the modulus
bool CElasticConstants::SetModulus( double modulus )
{
    //check that it is positive
    if( modulus <=0 )
        return false;

    E = modulus;
    return true;
}

//load Poisson's ratio
bool CElasticConstants::SetPoissonsRatio( double v )
{
    //check that it is positive
    if( v <=0 )
        return false;

    Poissons = v;
    return true;
}
