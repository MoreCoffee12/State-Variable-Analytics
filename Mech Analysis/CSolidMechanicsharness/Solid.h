#pragma once
#include "solidstrain.h"
#include "solidstress.h"
class CSolid :  public CSolidStrain, public CSolidStress, public CElasticConstants
{
public:
    CSolid(void);
    ~CSolid(void);

    //calculation calls
    bool CalcStrainFromStress();
    bool CalcStressFromStrain();

};
