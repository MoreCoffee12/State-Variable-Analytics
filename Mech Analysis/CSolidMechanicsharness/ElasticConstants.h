#pragma once

class CElasticConstants
{
public:
    CElasticConstants(void);
    ~CElasticConstants(void);

    //object member access methods
    bool GetModulus( double *E );
    bool GetPoissonsRatio( double *v );

    bool SetModulus ( double E );
    bool SetPoissonsRatio ( double v );

private:

    double E;
    double Poissons;

};
