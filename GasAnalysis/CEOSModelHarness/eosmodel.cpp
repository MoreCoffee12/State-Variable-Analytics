//////////////////////////////////////////////////////////////////////
//
// EOSMODEL.cpp: implementation of the CEOSMODEL class.
//
//////////////////////////////////////////////////////////////////////

#include "EOSMODEL.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEOSMODEL::CEOSMODEL()
{

     //local variables
     int                 arrayindex;
     double              dtemp01;
     double              dtemp02;
     bool                bTemp;
  
     /*  Data used below comes from a variety of sources:
     Data                Description                        Source
     -------------------------------------------------------------------------
     molewt              Molecular Weight                   1,4
     omega               Eccentric Factor                   1
     R                   Gas Constant                       1

     (Note: these three values were only available in SI.  They will be
     converted to their USCS equivalents with a call ToUSCS()
     Vc                  Critical Volume                    1
     Tc                  Critical Temperature               1
     Pc                  Critical Pressure                  1
     Dipole              Dipole Moment, debye               7

     Bo...gamma          Component EOS Parameters, BWRS     2
     B0...F              Ideal Entropy Fit Params           3
     A[0]...B[11]        Corrosponding States, Lee-Kesler   3
     hfo,sfo             Enthalpy/entropy of formation      6,7
     LHV                 Lower heating values               6
     A..E                Correlation for ideal gas Cp       6,7

     1.   Poling, Bruce, et al.  The Properties of Gases and Liquids.  5th ed.
           New York : McGraw-Hill, 2001.

     2.   Starling, Kenneth.  Fluid Properties for Light Petroleum Systems.
           Houston : Gulf Publishing Co., 1973.

     3.   Edmister, Wayne, and Byung IK Lee.  Applied Hydrocarbon Thermodynamics. 
           2nd ed.  Houston : Gulf Publishing Co., 1981.

     4.   Reynolds, W.C.  Thermodynamic Properties in SI.
           Stanform : Department of Mechanical Engineering, 1979.

     5.   Lindeburg, Michael F.  Mechanical Engineering Reference Manual
           10th ed.  Belmont : Professional Publications, Inc., 1998.

     6.   Perry, Robert H. and Don W. Green.  Perry's Chemical Engineer's Handbook.
           7th ed.  New York : McGraw-Hill, 1997.

     7.   Yaws, Carl L.  Chemical Properties Handbook.
           New York : McGraw Hill. 1999.

     8.


     9.   Crane Engineering Department.  Technical Paper No. 410 - Flow 
           of Fluids Through Valves, Fittings and Pipe.  
           Joliet : Crane Co., 1988.

     10.  Mott, Robert L.  Applied Fluid Mechanics.  2nd ed.
           Columbus : Charles E. Merill Publishing Co. 19XX.

     11.  NIST Suppertrap Version 3.1; NIST website
        

     -----------------------------------------------------------------------------
     Units are:
  
     Description                   USCS                     SI
     -----------------------------------------------------------------------------
     Pressure                      PSIA                     bar
     Temperature                   R                        K
     Volume                        ft3/lb                   cm3/g
     Gas Constant                  (psia-ft3)/(lbmol-R)     (bar-cm3)/(gmol-K)
     Entropy                       BTU/(lb-R)               kJ/(Kg-K)
     Enthalpy                      BTU/lb                   kJ/kg
     Entropy of formation          BTU/(lbmol-R)            J/(gmol-K)
     Enthalpy of formation         BTU/lbmol                J/gmol
     Dipole Moment                 -                        Debye, 3.162 * 10^-25 (J-m3)^(1/2)
     Viscosity (absolute)          lb/ft-sec                kg/m-sec
     -----------------------------------------------------------------------------*/

     //Initialize local variables
     arrayindex          = 0;
     dtemp01             = 0;
     dtemp02             = 0;
     
     //Initialize object variables
     r_USCS              = 10.740;
     r_SI                = 83.145;
     _ikCount            = kCOUNT;
     
     //House Keeping Variables
     FluidCount          = FLUIDCOUNT;
     MaxIterations       = 200;
     Precision           = 0.000001;
     _dblTrace           = 1E-20;
     
     //mesage handler variables
     messagelist         = NULL;
     strcpy_s( errchar, "*" );
     uniary              = false;
     
     //Begin the long process of filling each fluid object
     
     //Initialize generalized parameter functions
     //used by the lee-kesler generalized e-o-s
     //to keep things simple, the index of the array
     //corresponds to the indice of the A or B
     A[0]  = 0;
     A[1]  = 0.44369;
     A[2]  = 1.28438;
     A[3]  = 0.3563060;
     A[4]  = 0.5449790;
     A[5]  = 0.5286290;
     A[6]  = 0.4840110;
     A[7]  = 0.0705233;
     A[8]  = 0.5040870;
     A[9]  = 0.0307452;
     A[10] = 0.0732828;
     A[11] = 0.0064500;

     B[0]  = 0;
     B[1]  = 0.1154490;
     B[2]  = -0.9207310;
     B[3]  = 1.7087100;
     B[4]  = -0.2708960;
     B[5]  = 0.3492610;
     B[6]  = 0.7541300;
     B[7]  = -0.0444480;
     B[8]  = 1.3224500;
     B[9]  = 0.1794330;
     B[10] = 0.4634920;
     B[11] = -0.0221430;

     //interaction parameters for BWRS components
     arrayindex = 0;     
     _k[( FluidCount * arrayindex ) + 0 ] = 0;
     _k[( FluidCount * arrayindex ) + 1 ] = 0.01;
     _k[( FluidCount * arrayindex ) + 2 ] = 0.023;
     _k[( FluidCount * arrayindex ) + 3 ] = 0.0275;
     _k[( FluidCount * arrayindex ) + 4 ] = 0.031;
     _k[( FluidCount * arrayindex ) + 5 ] = 0.036;
     _k[( FluidCount * arrayindex ) + 6 ] = 0.041;
     _k[( FluidCount * arrayindex ) + 7 ] = 0.050;
     _k[( FluidCount * arrayindex ) + 8 ] = 0.060;
     _k[( FluidCount * arrayindex ) + 9 ] = 0.070;
     _k[( FluidCount * arrayindex ) + 10 ] = 0.010;
     _k[( FluidCount * arrayindex ) + 11 ] = 0.021;
     _k[( FluidCount * arrayindex ) + 12 ] = 0.050;
     _k[( FluidCount * arrayindex ) + 13 ] = 0.050;
     _k[( FluidCount * arrayindex ) + 14 ] = 0.025;
     _k[( FluidCount * arrayindex ) + 15 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 16 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 17 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 18 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 19 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 20 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 21 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 22 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 23 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 24 ] = 0.000;

     arrayindex = 1;     
     _k[( FluidCount * arrayindex ) + 0 ] = 0.010;
     _k[( FluidCount * arrayindex ) + 1 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 2 ] = 0.031;
     _k[( FluidCount * arrayindex ) + 3 ] = 0.004;
     _k[( FluidCount * arrayindex ) + 4 ] = 0.0045;
     _k[( FluidCount * arrayindex ) + 5 ] = 0.005;
     _k[( FluidCount * arrayindex ) + 6 ] = 0.006;
     _k[( FluidCount * arrayindex ) + 7 ] = 0.007;
     _k[( FluidCount * arrayindex ) + 8 ] = 0.0085;
     _k[( FluidCount * arrayindex ) + 9 ] = 0.010;
     _k[( FluidCount * arrayindex ) + 10 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 11 ] = 0.003;
     _k[( FluidCount * arrayindex ) + 12 ] = 0.048;
     _k[( FluidCount * arrayindex ) + 13 ] = 0.045;
     _k[( FluidCount * arrayindex ) + 14 ] = 0.070;
     _k[( FluidCount * arrayindex ) + 15 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 16 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 17 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 18 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 19 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 20 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 21 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 22 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 23 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 24 ] = 0.000;

     arrayindex = 2;     
     _k[( FluidCount * arrayindex ) + 0 ] = 0.023;
     _k[( FluidCount * arrayindex ) + 1 ] = 0.0031;
     _k[( FluidCount * arrayindex ) + 2 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 3 ] = 0.003;
     _k[( FluidCount * arrayindex ) + 4 ] = 0.0035;
     _k[( FluidCount * arrayindex ) + 5 ] = 0.004;
     _k[( FluidCount * arrayindex ) + 6 ] = 0.0045;
     _k[( FluidCount * arrayindex ) + 7 ] = 0.005;
     _k[( FluidCount * arrayindex ) + 8 ] = 0.0065;
     _k[( FluidCount * arrayindex ) + 9 ] = 0.008;
     _k[( FluidCount * arrayindex ) + 10 ] = 0.0031;
     _k[( FluidCount * arrayindex ) + 11 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 12 ] = 0.045;
     _k[( FluidCount * arrayindex ) + 13 ] = 0.040;
     _k[( FluidCount * arrayindex ) + 14 ] = 0.100;
     _k[( FluidCount * arrayindex ) + 15 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 16 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 17 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 18 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 19 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 20 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 21 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 22 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 23 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 24 ] = 0.000;

     arrayindex = 3;     
     _k[( FluidCount * arrayindex ) + 0 ] = 0.0275;
     _k[( FluidCount * arrayindex ) + 1 ] = 0.004;
     _k[( FluidCount * arrayindex ) + 2 ] = 0.003;
     _k[( FluidCount * arrayindex ) + 3 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 4 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 5 ] = 0.0008;
     _k[( FluidCount * arrayindex ) + 6 ] = 0.001;
     _k[( FluidCount * arrayindex ) + 7 ] = 0.0015;
     _k[( FluidCount * arrayindex ) + 8 ] = 0.0018;
     _k[( FluidCount * arrayindex ) + 9 ] = 0.002;
     _k[( FluidCount * arrayindex ) + 10 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 11 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 12 ] = 0.050;
     _k[( FluidCount * arrayindex ) + 13 ] = 0.036;
     _k[( FluidCount * arrayindex ) + 14 ] = 0.110;
     _k[( FluidCount * arrayindex ) + 15 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 16 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 17 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 18 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 19 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 20 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 21 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 22 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 23 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 24 ] = 0.000;

     arrayindex = 4;     
     _k[( FluidCount * arrayindex ) + 0 ] = 0.031;
     _k[( FluidCount * arrayindex ) + 1 ] = 0.0045;
     _k[( FluidCount * arrayindex ) + 2 ] = 0.0035;
     _k[( FluidCount * arrayindex ) + 3 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 4 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 5 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 6 ] = 0.001;
     _k[( FluidCount * arrayindex ) + 7 ] = 0.0015;
     _k[( FluidCount * arrayindex ) + 8 ] = 0.0018;
     _k[( FluidCount * arrayindex ) + 9 ] = 0.002;
     _k[( FluidCount * arrayindex ) + 10 ] = 0.0045;
     _k[( FluidCount * arrayindex ) + 11 ] = 0.0031;
     _k[( FluidCount * arrayindex ) + 12 ] = 0.050;
     _k[( FluidCount * arrayindex ) + 13 ] = 0.034;
     _k[( FluidCount * arrayindex ) + 14 ] = 0.120;
     _k[( FluidCount * arrayindex ) + 15 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 16 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 17 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 18 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 19 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 20 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 21 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 22 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 23 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 24 ] = 0.000;

     arrayindex = 5;     
     _k[( FluidCount * arrayindex ) + 0 ] = 0.036;
     _k[( FluidCount * arrayindex ) + 1 ] = 0.0050;
     _k[( FluidCount * arrayindex ) + 2 ] = 0.004;
     _k[( FluidCount * arrayindex ) + 3 ] = 0.0008;
     _k[( FluidCount * arrayindex ) + 4 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 5 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 6 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 7 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 8 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 9 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 10 ] = 0.0050;
     _k[( FluidCount * arrayindex ) + 11 ] = 0.0045;
     _k[( FluidCount * arrayindex ) + 12 ] = 0.050;
     _k[( FluidCount * arrayindex ) + 13 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 14 ] = 0.134;
     _k[( FluidCount * arrayindex ) + 15 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 16 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 17 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 18 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 19 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 20 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 21 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 22 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 23 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 24 ] = 0.000;

     arrayindex = 6;     
     _k[( FluidCount * arrayindex ) + 0 ] = 0.041;
     _k[( FluidCount * arrayindex ) + 1 ] = 0.0060;
     _k[( FluidCount * arrayindex ) + 2 ] = 0.0045;
     _k[( FluidCount * arrayindex ) + 3 ] = 0.0010;
     _k[( FluidCount * arrayindex ) + 4 ] = 0.0015;
     _k[( FluidCount * arrayindex ) + 5 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 6 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 7 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 8 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 9 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 10 ] = 0.0060;
     _k[( FluidCount * arrayindex ) + 11 ] = 0.0045;
     _k[( FluidCount * arrayindex ) + 12 ] = 0.050;
     _k[( FluidCount * arrayindex ) + 13 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 14 ] = 0.148;
     _k[( FluidCount * arrayindex ) + 15 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 16 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 17 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 18 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 19 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 20 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 21 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 22 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 23 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 24 ] = 0.000;

     arrayindex = 7;     
     _k[( FluidCount * arrayindex ) + 0 ] = 0.050;
     _k[( FluidCount * arrayindex ) + 1 ] = 0.0070;
     _k[( FluidCount * arrayindex ) + 2 ] = 0.0050;
     _k[( FluidCount * arrayindex ) + 3 ] = 0.0015;
     _k[( FluidCount * arrayindex ) + 4 ] = 0.0015;
     _k[( FluidCount * arrayindex ) + 5 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 6 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 7 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 8 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 9 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 10 ] = 0.0070;
     _k[( FluidCount * arrayindex ) + 11 ] = 0.0050;
     _k[( FluidCount * arrayindex ) + 12 ] = 0.050;
     _k[( FluidCount * arrayindex ) + 13 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 14 ] = 0.172;
     _k[( FluidCount * arrayindex ) + 15 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 16 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 17 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 18 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 19 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 20 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 21 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 22 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 23 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 24 ] = 0.000;

     arrayindex = 8;     
     _k[( FluidCount * arrayindex ) + 0 ] = 0.060;
     _k[( FluidCount * arrayindex ) + 1 ] = 0.0085;
     _k[( FluidCount * arrayindex ) + 2 ] = 0.0065;
     _k[( FluidCount * arrayindex ) + 3 ] = 0.0018;
     _k[( FluidCount * arrayindex ) + 4 ] = 0.0018;
     _k[( FluidCount * arrayindex ) + 5 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 6 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 7 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 8 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 9 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 10 ] = 0.0085;
     _k[( FluidCount * arrayindex ) + 11 ] = 0.0065;
     _k[( FluidCount * arrayindex ) + 12 ] = 0.050;
     _k[( FluidCount * arrayindex ) + 13 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 14 ] = 0.200;
     _k[( FluidCount * arrayindex ) + 15 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 16 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 17 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 18 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 19 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 20 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 21 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 22 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 23 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 24 ] = 0.000;

     arrayindex = 9;     
     _k[( FluidCount * arrayindex ) + 0 ] = 0.070;
     _k[( FluidCount * arrayindex ) + 1 ] = 0.010;
     _k[( FluidCount * arrayindex ) + 2 ] = 0.008;
     _k[( FluidCount * arrayindex ) + 3 ] = 0.002;
     _k[( FluidCount * arrayindex ) + 4 ] = 0.002;
     _k[( FluidCount * arrayindex ) + 5 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 6 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 7 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 8 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 9 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 10 ] = 0.010;
     _k[( FluidCount * arrayindex ) + 11 ] = 0.008;
     _k[( FluidCount * arrayindex ) + 12 ] = 0.050;
     _k[( FluidCount * arrayindex ) + 13 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 14 ] = 0.228;
     _k[( FluidCount * arrayindex ) + 15 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 16 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 17 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 18 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 19 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 20 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 21 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 22 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 23 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 24 ] = 0.000;

     arrayindex = 10;     
     _k[( FluidCount * arrayindex ) + 0 ] = 0.010;
     _k[( FluidCount * arrayindex ) + 1 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 2 ] = 0.0031;
     _k[( FluidCount * arrayindex ) + 3 ] = 0.004;
     _k[( FluidCount * arrayindex ) + 4 ] = 0.0045;
     _k[( FluidCount * arrayindex ) + 5 ] = 0.005;
     _k[( FluidCount * arrayindex ) + 6 ] = 0.006;
     _k[( FluidCount * arrayindex ) + 7 ] = 0.007;
     _k[( FluidCount * arrayindex ) + 8 ] = 0.0085;
     _k[( FluidCount * arrayindex ) + 9 ] = 0.010;
     _k[( FluidCount * arrayindex ) + 10 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 11 ] = 0.003;
     _k[( FluidCount * arrayindex ) + 12 ] = 0.048;
     _k[( FluidCount * arrayindex ) + 13 ] = 0.045;
     _k[( FluidCount * arrayindex ) + 14 ] = 0.070;
     _k[( FluidCount * arrayindex ) + 15 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 16 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 17 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 18 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 19 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 20 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 21 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 22 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 23 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 24 ] = 0.000;

     arrayindex = 11;     
     _k[( FluidCount * arrayindex ) + 0 ] = 0.021;
     _k[( FluidCount * arrayindex ) + 1 ] = 0.003;
     _k[( FluidCount * arrayindex ) + 2 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 3 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 4 ] = 0.0031;
     _k[( FluidCount * arrayindex ) + 5 ] = 0.0045;
     _k[( FluidCount * arrayindex ) + 6 ] = 0.0045;
     _k[( FluidCount * arrayindex ) + 7 ] = 0.005;
     _k[( FluidCount * arrayindex ) + 8 ] = 0.0065;
     _k[( FluidCount * arrayindex ) + 9 ] = 0.008;
     _k[( FluidCount * arrayindex ) + 10 ] = 0.003;
     _k[( FluidCount * arrayindex ) + 11 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 12 ] = 0.045;
     _k[( FluidCount * arrayindex ) + 13 ] = 0.040;
     _k[( FluidCount * arrayindex ) + 14 ] = 0.100;
     _k[( FluidCount * arrayindex ) + 15 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 16 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 17 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 18 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 19 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 20 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 21 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 22 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 23 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 24 ] = 0.000;

     arrayindex = 12;     
     _k[( FluidCount * arrayindex ) + 0 ] = 0.050;
     _k[( FluidCount * arrayindex ) + 1 ] = 0.048;
     _k[( FluidCount * arrayindex ) + 2 ] = 0.045;
     _k[( FluidCount * arrayindex ) + 3 ] = 0.050;
     _k[( FluidCount * arrayindex ) + 4 ] = 0.050;
     _k[( FluidCount * arrayindex ) + 5 ] = 0.050;
     _k[( FluidCount * arrayindex ) + 6 ] = 0.050;
     _k[( FluidCount * arrayindex ) + 7 ] = 0.050;
     _k[( FluidCount * arrayindex ) + 8 ] = 0.050;
     _k[( FluidCount * arrayindex ) + 9 ] = 0.050;
     _k[( FluidCount * arrayindex ) + 10 ] = 0.048;
     _k[( FluidCount * arrayindex ) + 11 ] = 0.045;
     _k[( FluidCount * arrayindex ) + 12 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 13 ] = 0.035;
     _k[( FluidCount * arrayindex ) + 14 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 15 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 16 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 17 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 18 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 19 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 20 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 21 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 22 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 23 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 24 ] = 0.000;

     arrayindex = 13;     
     _k[( FluidCount * arrayindex ) + 0 ] = 0.050;
     _k[( FluidCount * arrayindex ) + 1 ] = 0.045;
     _k[( FluidCount * arrayindex ) + 2 ] = 0.040;
     _k[( FluidCount * arrayindex ) + 3 ] = 0.036;
     _k[( FluidCount * arrayindex ) + 4 ] = 0.034;
     _k[( FluidCount * arrayindex ) + 5 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 6 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 7 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 8 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 9 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 10 ] = 0.045;
     _k[( FluidCount * arrayindex ) + 11 ] = 0.040;
     _k[( FluidCount * arrayindex ) + 12 ] = 0.035;
     _k[( FluidCount * arrayindex ) + 13 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 14 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 15 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 16 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 17 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 18 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 19 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 20 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 21 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 22 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 23 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 24 ] = 0.000;

     arrayindex = 14;     
     _k[( FluidCount * arrayindex ) + 0 ] = 0.025;
     _k[( FluidCount * arrayindex ) + 1 ] = 0.070;
     _k[( FluidCount * arrayindex ) + 2 ] = 0.100;
     _k[( FluidCount * arrayindex ) + 3 ] = 0.110;
     _k[( FluidCount * arrayindex ) + 4 ] = 0.120;
     _k[( FluidCount * arrayindex ) + 5 ] = 0.134;
     _k[( FluidCount * arrayindex ) + 6 ] = 0.148;
     _k[( FluidCount * arrayindex ) + 7 ] = 0.172;
     _k[( FluidCount * arrayindex ) + 8 ] = 0.200;
     _k[( FluidCount * arrayindex ) + 9 ] = 0.228;
     _k[( FluidCount * arrayindex ) + 10 ] = 0.070;
     _k[( FluidCount * arrayindex ) + 11 ] = 0.100;
     _k[( FluidCount * arrayindex ) + 12 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 13 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 14 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 15 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 16 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 17 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 18 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 19 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 20 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 21 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 22 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 23 ] = 0.000;
     _k[( FluidCount * arrayindex ) + 24 ] = 0.000;

     for( arrayindex=15; arrayindex<FluidCount; arrayindex++)
     {
          _k[( FluidCount * arrayindex ) + 0 ] = 0.000;
          _k[( FluidCount * arrayindex ) + 1 ] = 0.000;
          _k[( FluidCount * arrayindex ) + 2 ] = 0.000;
          _k[( FluidCount * arrayindex ) + 3 ] = 0.000;
          _k[( FluidCount * arrayindex ) + 4 ] = 0.000;
          _k[( FluidCount * arrayindex ) + 5 ] = 0.000;
          _k[( FluidCount * arrayindex ) + 6 ] = 0.000;
          _k[( FluidCount * arrayindex ) + 7 ] = 0.000;
          _k[( FluidCount * arrayindex ) + 8 ] = 0.000;
          _k[( FluidCount * arrayindex ) + 9 ] = 0.000;
          _k[( FluidCount * arrayindex ) + 10 ] = 0.000;
          _k[( FluidCount * arrayindex ) + 11 ] = 0.000;
          _k[( FluidCount * arrayindex ) + 12 ] = 0.000;
          _k[( FluidCount * arrayindex ) + 13 ] = 0.000;
          _k[( FluidCount * arrayindex ) + 14 ] = 0.000;
          _k[( FluidCount * arrayindex ) + 15 ] = 0.000;
          _k[( FluidCount * arrayindex ) + 16 ] = 0.000;
          _k[( FluidCount * arrayindex ) + 17 ] = 0.000;
          _k[( FluidCount * arrayindex ) + 18 ] = 0.000;
          _k[( FluidCount * arrayindex ) + 19 ] = 0.000;
          _k[( FluidCount * arrayindex ) + 20 ] = 0.000;
          _k[( FluidCount * arrayindex ) + 21 ] = 0.000;
          _k[( FluidCount * arrayindex ) + 22 ] = 0.000;
          _k[( FluidCount * arrayindex ) + 23 ] = 0.000;
          _k[( FluidCount * arrayindex ) + 24 ] = 0.000;
     }


     /////////////////////////////////////////////////////////////////////
     // Fluid Index No. 0001
     /////////////////////////////////////////////////////////////////////
     // Methane
     /////////////////////////////////////////////////////////////////////
     arrayindex                              = ( 1 - 1 );
     strcpy_s(FluidList[arrayindex].name, "Methane");
     strcpy_s(FluidList[arrayindex].name_alt01, "-" );
     strcpy_s(FluidList[arrayindex].chemsym, "CH4");
     FluidList[arrayindex].CASNo[0]          = 8;
     FluidList[arrayindex].CASNo[1]          = 82;
     FluidList[arrayindex].CASNo[2]          = 74;
     FluidList[arrayindex].Polingetl         = 26;
     FluidList[arrayindex].molewt            = 16.043;
     FluidList[arrayindex].omega             = 0.011;
     FluidList[arrayindex].Vc_SI             = 98.60;
     FluidList[arrayindex].Tc_SI             = 190.56;
     FluidList[arrayindex].Pc_SI             = 45.99;
     FluidList[arrayindex].hfo_SI            = -7.45200 * pow( 10.0, +04.0 );
     FluidList[arrayindex].sfo_SI            = +1.86270 * pow( 10.0, +02.0 );
     FluidList[arrayindex].LHV_SI            = -802618;
     FluidList[arrayindex].HHV_SI            = -891308;
     ToUSCS( ( arrayindex + 1 ) );
     FluidList[arrayindex].Tmin_USCS         = 199.67;
     FluidList[arrayindex].Tmax_USCS         = 959.67;
     FluidList[arrayindex].Pmin_USCS         = 1.0;
     FluidList[arrayindex].Pmax_USCS         = 8000.0;
     FluidList[arrayindex].Dipole_SI         = 0.00;
     FluidList[arrayindex].Dipole_USCS       = 0.00;
     FluidList[arrayindex].Bo                = 0.723251;
     FluidList[arrayindex].Ao                = 7520.29;
     FluidList[arrayindex].Co                = 271092000;
     FluidList[arrayindex].Do                = 10773700000;
     FluidList[arrayindex].Eo                = 30112200000;
     FluidList[arrayindex].b                 = 0.925404;
     FluidList[arrayindex].a                 = 2574.89;
     FluidList[arrayindex].d                 = 47489.1;
     FluidList[arrayindex].alpha             = 0.468828;
     FluidList[arrayindex].c                 = 437222000;
     FluidList[arrayindex].gamma             = 1.48640;
     FluidList[arrayindex].A                 = -6.977000 * pow( 10.0, +0 );
     FluidList[arrayindex].B                 = +5.717000 * pow( 10.0, -1 );
     FluidList[arrayindex].C                 = -2.943120 * pow( 10.0, -4 );
     FluidList[arrayindex].D                 = +4.231570 * pow( 10.0, -7 );
     FluidList[arrayindex].E                 = -1.526740 * pow( 10.0, -10 );
     FluidList[arrayindex].F                 = +1.945261 * pow( 10.0, -14 );
     FluidList[arrayindex].G                 = -6.560000 * pow( 10.0, -1 );

     FluidList[arrayindex].HT5               = -9.326941 * pow( 10.0, -14.0 );
     FluidList[arrayindex].HT4               = +1.686788 * pow( 10.0, -09.0 );
     FluidList[arrayindex].HT3               = -1.231348 * pow( 10.0, -05.0 );
     FluidList[arrayindex].HT2               = +4.677102 * pow( 10.0, -02.0 );
     FluidList[arrayindex].HT1               = +8.720586 * pow( 10.0, +00.0 );
     FluidList[arrayindex].HT0               = -6.267704 * pow( 10.0, +03.0 );
     FluidList[arrayindex].Hn                = -4.900000 * pow( 10.0, +07.0 );

     FluidList[arrayindex].ST5               = +5.108041 * pow( 10.0, -17.0 );
     FluidList[arrayindex].ST4               = -1.004853 * pow( 10.0, -12.0 );
     FluidList[arrayindex].ST3               = +8.168600 * pow( 10.0, -09.0 );
     FluidList[arrayindex].ST2               = -3.765116 * pow( 10.0, -05.0 );
     FluidList[arrayindex].ST1               = +1.272391 * pow( 10.0, -01.0 );
     FluidList[arrayindex].ST0               = +1.510698 * pow( 10.0, +02.0 );

     /////////////////////////////////////////////////////////////////////
     // Fluid Index No. 0002
     /////////////////////////////////////////////////////////////////////
     // Ethane
     /////////////////////////////////////////////////////////////////////
     arrayindex = ( 2 - 1 );
     strcpy_s(FluidList[arrayindex].name, "Ethane");
     strcpy_s(FluidList[arrayindex].name_alt01, "-" );
     strcpy_s(FluidList[arrayindex].chemsym, "C2H6");
     FluidList[arrayindex].CASNo[0] = 0;
     FluidList[arrayindex].CASNo[1] = 84;
     FluidList[arrayindex].CASNo[2] = 74;
     FluidList[arrayindex].Polingetl = 65;
     FluidList[arrayindex].molewt = 30.070;
     FluidList[arrayindex].omega = 0.099;
     FluidList[arrayindex].Vc_SI = 145.50;
     FluidList[arrayindex].Tc_SI = 305.32;
     FluidList[arrayindex].Pc_SI = 48.72;
     FluidList[arrayindex].hfo_SI = -8.38200 * pow( 10.0, +04.0 );
     FluidList[arrayindex].sfo_SI = +2.29120 * pow( 10.0, +02.0 );
     FluidList[arrayindex].LHV_SI = -1428600;
     FluidList[arrayindex].HHV_SI = -1561806;
     ToUSCS( ( arrayindex + 1 ) );
     FluidList[arrayindex].Tmin_USCS = 219.67;
     FluidList[arrayindex].Tmax_USCS = 959.67;
     FluidList[arrayindex].Pmin_USCS = 1.0;
     FluidList[arrayindex].Pmax_USCS = 8000.0;
     FluidList[arrayindex].Dipole_SI = 0.00;
     FluidList[arrayindex].Dipole_USCS = 0.00;
     FluidList[arrayindex].Bo                = 0.826059;
     FluidList[arrayindex].Ao                = 13439.3;
     FluidList[arrayindex].Co                = 2951950000;
     FluidList[arrayindex].Do                = 2.57477E+11;
     FluidList[arrayindex].Eo                = 1.46819E+13;
     FluidList[arrayindex].b                 = 3.11206;
     FluidList[arrayindex].a                 = 22404.5;
     FluidList[arrayindex].d                 = 702189;
     FluidList[arrayindex].alpha             = 0.909681;
     FluidList[arrayindex].c                 = 6818260000;
     FluidList[arrayindex].gamma             = 2.99656;

     FluidList[arrayindex].A                 = -2.100000 * pow( 10.0, -2 );
     FluidList[arrayindex].B                 = +2.648800 * pow( 10.0, -1 );
     FluidList[arrayindex].C                 = -2.501400 * pow( 10.0, -5 );
     FluidList[arrayindex].D                 = +2.923340 * pow( 10.0, -7 );
     FluidList[arrayindex].E                 = -1.286050 * pow( 10.0, -10 );
     FluidList[arrayindex].F                 = +1.822060 * pow( 10.0, -14 );
     FluidList[arrayindex].G                 = +8.220000 * pow( 10.0, -2 );

     FluidList[arrayindex].HT5               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT4               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT3               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT2               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT1               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT0               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].Hn                = +0.000000 * pow( 10.0, +0 );

     FluidList[arrayindex].ST5               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST4               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST3               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST2               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST1               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST0               = +0.000000 * pow( 10.0, +0 );

     /////////////////////////////////////////////////////////////////////
     // Fluid Index No. 0003 
     /////////////////////////////////////////////////////////////////////
     // Propane
     /////////////////////////////////////////////////////////////////////
     arrayindex                              = ( 3 - 1 );
     strcpy_s(FluidList[arrayindex].name, "Propane");
     strcpy_s(FluidList[arrayindex].name_alt01, "-" );
     strcpy_s(FluidList[arrayindex].chemsym, "C3H8");
     FluidList[arrayindex].CASNo[0]          = 6;
     FluidList[arrayindex].CASNo[1]          = 98;
     FluidList[arrayindex].CASNo[2]          = 74;
     FluidList[arrayindex].Polingetl         = 95;
     FluidList[arrayindex].molewt            = 44.097;
     FluidList[arrayindex].omega             = 0.152;
     FluidList[arrayindex].Vc_SI             = 200.00;
     FluidList[arrayindex].Tc_SI             = 369.83;
     FluidList[arrayindex].Pc_SI             = 42.48;
     FluidList[arrayindex].hfo_SI            = -1.04680 * pow( 10.0, +05.0 );
     FluidList[arrayindex].sfo_SI            = +2.70200 * pow( 10.0, +02.0 );
     FluidList[arrayindex].LHV_SI            = -2043100;
     FluidList[arrayindex].HHV_SI            = -2219938;
     ToUSCS( ( arrayindex + 1 ) );
     FluidList[arrayindex].Tmin_USCS         = 219.67;
     FluidList[arrayindex].Tmax_USCS         = 959.67;
     FluidList[arrayindex].Pmin_USCS         = 1.0;
     FluidList[arrayindex].Pmax_USCS         = 8000.0;
     FluidList[arrayindex].Dipole_SI         = 0.00;
     FluidList[arrayindex].Dipole_USCS       = 0.00;
     FluidList[arrayindex].Bo                = 0.964762;
     FluidList[arrayindex].Ao                = 18634.7;
     FluidList[arrayindex].Co                = 7961780000;
     FluidList[arrayindex].Do                = 4.53708E+11;
     FluidList[arrayindex].Eo                = 2.56053E+13;
     FluidList[arrayindex].b                 = 5.46248;
     FluidList[arrayindex].a                 = 40066.4;
     FluidList[arrayindex].d                 = 15052000;
     FluidList[arrayindex].alpha             = 2.01402;
     FluidList[arrayindex].c                 = 27446100000;
     FluidList[arrayindex].gamma             = 4.56182;

     FluidList[arrayindex].A                 = -7.380000 * pow( 10.0, -1 );
     FluidList[arrayindex].B                 = +1.726000 * pow( 10.0, -1 );
     FluidList[arrayindex].C                 = +9.404100 * pow( 10.0, -5 );
     FluidList[arrayindex].D                 = +2.155430 * pow( 10.0, -7 );
     FluidList[arrayindex].E                 = -1.070986 * pow( 10.0, -10 );
     FluidList[arrayindex].F                 = +1.592794 * pow( 10.0, -14 );
     FluidList[arrayindex].G                 = +2.066000 * pow( 10.0, -1 );

     FluidList[arrayindex].HT5               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT4               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT3               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT2               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT1               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT0               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].Hn                = +0.000000 * pow( 10.0, +0 );

     FluidList[arrayindex].ST5               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST4               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST3               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST2               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST1               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST0               = +0.000000 * pow( 10.0, +0 );

     /////////////////////////////////////////////////////////////////////
     // Fluid Index No. 0004
     /////////////////////////////////////////////////////////////////////
     // I_Butane
     /////////////////////////////////////////////////////////////////////
     arrayindex                              = ( 4 - 1 );
     strcpy_s(FluidList[arrayindex].name, "I_Butane");
     strcpy_s(FluidList[arrayindex].name_alt01, "2-Methylpropane" );
     strcpy_s(FluidList[arrayindex].chemsym, "C4H10");
     FluidList[arrayindex].CASNo[0]          = 5;
     FluidList[arrayindex].CASNo[1]          = 28;
     FluidList[arrayindex].CASNo[2]          = 75;
     FluidList[arrayindex].Polingetl         = 128;
     FluidList[arrayindex].molewt            = 58.123;
     FluidList[arrayindex].omega             = 0.186;
     FluidList[arrayindex].Vc_SI             = 262.70;
     FluidList[arrayindex].Tc_SI             = 407.85;
     FluidList[arrayindex].Pc_SI             = 36.40;
     FluidList[arrayindex].hfo_SI            = -1.34180 * pow( 10.0, +05.0 );
     FluidList[arrayindex].sfo_SI            = +2.95390 * pow( 10.0, +02.0 );
     FluidList[arrayindex].LHV_SI            = -2649000;
     FluidList[arrayindex].HHV_SI            = -2869970;
     ToUSCS( ( arrayindex + 1 ) );
     FluidList[arrayindex].Tmin_USCS         = 319.67;
     FluidList[arrayindex].Tmax_USCS         = 939.67;
     FluidList[arrayindex].Pmin_USCS         = 1.0;
     FluidList[arrayindex].Pmax_USCS         = 8000.0;
     FluidList[arrayindex].Dipole_SI         = 0.13;
     FluidList[arrayindex].Dipole_USCS       = 0.00;
     FluidList[arrayindex].Bo                = 1.8789;
     FluidList[arrayindex].Ao                = 37264;
     FluidList[arrayindex].Co                = 10141300000;
     FluidList[arrayindex].Do                = 8.53176E+11;
     FluidList[arrayindex].Eo                = 8.4086E+13;
     FluidList[arrayindex].b                 = 8.58663;
     FluidList[arrayindex].a                 = 47990.7;
     FluidList[arrayindex].d                 = 21686300;
     FluidList[arrayindex].alpha             = 4.23987;
     FluidList[arrayindex].c                 = 40676300000;
     FluidList[arrayindex].gamma             = 7.11486;

     FluidList[arrayindex].A                 = +1.149800 * pow( 10.0, +1 );
     FluidList[arrayindex].B                 = +4.668000 * pow( 10.0, -2 );
     FluidList[arrayindex].C                 = +3.348010 * pow( 10.0, -4 );
     FluidList[arrayindex].D                 = +1.442300 * pow( 10.0, -8 );
     FluidList[arrayindex].E                 = -3.164200 * pow( 10.0, -11 );
     FluidList[arrayindex].F                 = +5.428930 * pow( 10.0, -15 );
     FluidList[arrayindex].G                 = +5.616000 * pow( 10.0, -1 );

     FluidList[arrayindex].HT5               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT4               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT3               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT2               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT1               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT0               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].Hn                = +0.000000 * pow( 10.0, +0 );
     
     FluidList[arrayindex].ST5               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST4               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST3               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST2               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST1               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST0               = +0.000000 * pow( 10.0, +0 );
     
     /////////////////////////////////////////////////////////////////////
     // Fluid Index No. 0005
     /////////////////////////////////////////////////////////////////////
     // N_Butane
     /////////////////////////////////////////////////////////////////////
     arrayindex                              = (5 - 1);
     strcpy_s(FluidList[arrayindex].name, "N_Butane");
     strcpy_s(FluidList[arrayindex].name_alt01, "-" );
     strcpy_s(FluidList[arrayindex].chemsym, "C4H10");
     FluidList[arrayindex].CASNo[0]          = 8;
     FluidList[arrayindex].CASNo[1]          = 97;
     FluidList[arrayindex].CASNo[2]          = 106;
     FluidList[arrayindex].Polingetl         = 127;
     FluidList[arrayindex].molewt            = 58.123;
     FluidList[arrayindex].omega             = 0.200;
     FluidList[arrayindex].Vc_SI             = 255.00;
     FluidList[arrayindex].Tc_SI             = 425.12;
     FluidList[arrayindex].Pc_SI             = 37.96;
     FluidList[arrayindex].hfo_SI            = -1.25790 * pow( 10.0, +05.0 );
     FluidList[arrayindex].sfo_SI            = +3.09910 * pow( 10.0, +02.0 );
     FluidList[arrayindex].LHV_SI            = -2657300;
     FluidList[arrayindex].HHV_SI            = -2878360;
     ToUSCS( ( arrayindex + 1 ) );
     FluidList[arrayindex].Tmin_USCS         = 239.67;
     FluidList[arrayindex].Tmax_USCS         = 959.67;
     FluidList[arrayindex].Pmin_USCS         = 1.0;
     FluidList[arrayindex].Pmax_USCS         = 8000.0;
     FluidList[arrayindex].Dipole_SI         = 0.00;
     FluidList[arrayindex].Dipole_USCS       = 0.00;
     FluidList[arrayindex].Bo                = 1.56588;
     FluidList[arrayindex].Ao                = 32544.7;
     FluidList[arrayindex].Co                = 13743600000;
     FluidList[arrayindex].Do                = 33159000000;
     FluidList[arrayindex].Eo                = 2.30902E+12;
     FluidList[arrayindex].b                 = 9.14066;
     FluidList[arrayindex].a                 = 71181.8;
     FluidList[arrayindex].d                 = 36423800;
     FluidList[arrayindex].alpha             = 4.00985;
     FluidList[arrayindex].c                 = 70004400000;
     FluidList[arrayindex].gamma             = 7.54122;

     FluidList[arrayindex].A                 = +7.430000 * pow( 10.0, +0 );
     FluidList[arrayindex].B                 = +9.857000 * pow( 10.0, -2 );
     FluidList[arrayindex].C                 = +2.691800 * pow( 10.0, -4 );
     FluidList[arrayindex].D                 = +5.182000 * pow( 10.0, -8 );
     FluidList[arrayindex].E                 = -4.201390 * pow( 10.0, -11 );
     FluidList[arrayindex].F                 = +6.560420 * pow( 10.0, -15 );
     FluidList[arrayindex].G                 = +3.516000 * pow( 10.0, -1 );
     
     FluidList[arrayindex].HT5               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT4               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT3               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT2               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT1               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT0               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].Hn                = +0.000000 * pow( 10.0, +0 );
     
     FluidList[arrayindex].ST5               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST4               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST3               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST2               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST1               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST0               = +0.000000 * pow( 10.0, +0 );

     /////////////////////////////////////////////////////////////////////
     // Fluid Index No. 0006
     /////////////////////////////////////////////////////////////////////
     // I_Pentane
     /////////////////////////////////////////////////////////////////////
     arrayindex                              = (6 - 1);
     strcpy_s(FluidList[arrayindex].name, "I_Pentane");
     strcpy_s(FluidList[arrayindex].name_alt01, "2-Methylbutane" );
     strcpy_s(FluidList[arrayindex].chemsym, "C5H12");
     FluidList[arrayindex].CASNo[0]          = 4;
     FluidList[arrayindex].CASNo[1]          = 78;
     FluidList[arrayindex].CASNo[2]          = 78;
     FluidList[arrayindex].Polingetl         = 167;
     FluidList[arrayindex].molewt            = 72.150;
     FluidList[arrayindex].omega             = 0.229;
     FluidList[arrayindex].Vc_SI             = 308.30;
     FluidList[arrayindex].Tc_SI             = 460.39;
     FluidList[arrayindex].Pc_SI             = 33.81;
     FluidList[arrayindex].hfo_SI            = -1.53700 * pow( 10.0, +05.0 );
     FluidList[arrayindex].sfo_SI            = +3.43740 * pow( 10.0, +02.0 );
     FluidList[arrayindex].LHV_SI            = -3239500;
     FluidList[arrayindex].HHV_SI            = -3529982;
     ToUSCS( ( arrayindex + 1 ) );
     FluidList[arrayindex].Tmin_USCS         = 359.67;
     FluidList[arrayindex].Tmax_USCS         = 1059.67;
     FluidList[arrayindex].Pmin_USCS         = 1.0;
     FluidList[arrayindex].Pmax_USCS         = 8000.0;
     FluidList[arrayindex].Dipole_SI         = 0.13;
     FluidList[arrayindex].Dipole_USCS       = 0.00;
     FluidList[arrayindex].Bo                = 1.27752;
     FluidList[arrayindex].Ao                = 35742;
     FluidList[arrayindex].Co                = 22843000000;
     FluidList[arrayindex].Do                = 1.42115E+12;
     FluidList[arrayindex].Eo                = 2.41326E+13;
     FluidList[arrayindex].b                 = 19.8384;
     FluidList[arrayindex].a                 = 204344;
     FluidList[arrayindex].d                 = 34922000;
     FluidList[arrayindex].alpha             = 6.16154;
     FluidList[arrayindex].c                 = 1.29083E+11;
     FluidList[arrayindex].gamma             = 11.73840;

     FluidList[arrayindex].A                 = +2.762300 * pow( 10.0, +1 );
     FluidList[arrayindex].B                 = -3.150000 * pow( 10.0, -2 );
     FluidList[arrayindex].C                 = +4.698840 * pow( 10.0, -4 );
     FluidList[arrayindex].D                 = -9.828200 * pow( 10.0, -8 );
     FluidList[arrayindex].E                 = +1.029850 * pow( 10.0, -11 );
     FluidList[arrayindex].F                 = -2.948500 * pow( 10.0, -16 );
     FluidList[arrayindex].G                 = +8.719000 * pow( 10.0, -1 );
     
     FluidList[arrayindex].HT5               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT4               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT3               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT2               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT1               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT0               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].Hn                = +0.000000 * pow( 10.0, +0 );
     
     FluidList[arrayindex].ST5               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST4               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST3               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST2               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST1               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST0               = +0.000000 * pow( 10.0, +0 );

     /////////////////////////////////////////////////////////////////////
     // Fluid Index No. 0007
     /////////////////////////////////////////////////////////////////////
     // N_Pentane 
     /////////////////////////////////////////////////////////////////////
     arrayindex                              = (7 - 1);
     strcpy_s(FluidList[arrayindex].name, "N_Pentane");
     strcpy_s(FluidList[arrayindex].name_alt01, "-" );
     strcpy_s(FluidList[arrayindex].chemsym, "C5H12");
     FluidList[arrayindex].CASNo[0]          = 0;
     FluidList[arrayindex].CASNo[1]          = 66;
     FluidList[arrayindex].CASNo[2]          = 109;
     FluidList[arrayindex].Polingetl         = 166;
     FluidList[arrayindex].molewt            = 72.150;
     FluidList[arrayindex].omega             = 0.252;
     FluidList[arrayindex].Vc_SI             = 311.00;
     FluidList[arrayindex].Tc_SI             = 469.70;
     FluidList[arrayindex].Pc_SI             = 33.70;
     FluidList[arrayindex].hfo_SI            = -1.46760 * pow( 10.0, +05.0 );
     FluidList[arrayindex].sfo_SI            = +3.49450 * pow( 10.0, +02.0 );
     FluidList[arrayindex].LHV_SI            = -3244900;
     FluidList[arrayindex].HHV_SI            = -3536922;
     ToUSCS( ( arrayindex + 1 ) );
     FluidList[arrayindex].Tmin_USCS         = 319.67;
     FluidList[arrayindex].Tmax_USCS         = 1059.67;
     FluidList[arrayindex].Pmin_USCS         = 1.0;
     FluidList[arrayindex].Pmax_USCS         = 8000.0;
     FluidList[arrayindex].Dipole_SI         = 0.00;
     FluidList[arrayindex].Dipole_USCS       = 0.00;
     FluidList[arrayindex].Bo                = 2.44417;
     FluidList[arrayindex].Ao                = 51108.2;
     FluidList[arrayindex].Co                = 22393100000;
     FluidList[arrayindex].Do                = 1.01769E+12;
     FluidList[arrayindex].Eo                = 3.9086E+13;
     FluidList[arrayindex].b                 = 16.607;
     FluidList[arrayindex].a                 = 162185;
     FluidList[arrayindex].d                 = 38852100;
     FluidList[arrayindex].alpha             = 7.06702;
     FluidList[arrayindex].c                 = 1.35286E+11;
     FluidList[arrayindex].gamma             = 11.85930;

     FluidList[arrayindex].A                 = +2.717200 * pow( 10.0, +1 );
     FluidList[arrayindex].B                 = -2.800000 * pow( 10.0, -3 );
     FluidList[arrayindex].C                 = +4.400730 * pow( 10.0, -4 );
     FluidList[arrayindex].D                 = -8.628700 * pow( 10.0, -8 );
     FluidList[arrayindex].E                 = +8.176400 * pow( 10.0, -12 );
     FluidList[arrayindex].F                 = -1.971500 * pow( 10.0, -16 );
     FluidList[arrayindex].G                 = +7.362000 * pow( 10.0, -1 );

     FluidList[arrayindex].HT5               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT4               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT3               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT2               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT1               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT0               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].Hn                = +0.000000 * pow( 10.0, +0 );
     
     FluidList[arrayindex].ST5               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST4               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST3               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST2               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST1               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST0               = +0.000000 * pow( 10.0, +0 );
     
     /////////////////////////////////////////////////////////////////////
     // Fluid Index No. 0008
     /////////////////////////////////////////////////////////////////////
     // N_Hexane
     /////////////////////////////////////////////////////////////////////
     arrayindex                              = (8 - 1);
     strcpy_s(FluidList[arrayindex].name, "N_Hexane");
     strcpy_s(FluidList[arrayindex].name_alt01, "-" );
     strcpy_s(FluidList[arrayindex].chemsym, "C6H14");
     FluidList[arrayindex].CASNo[0]          = 3;
     FluidList[arrayindex].CASNo[1]          = 54;
     FluidList[arrayindex].CASNo[2]          = 110;
     FluidList[arrayindex].Polingetl         = 216;
     FluidList[arrayindex].molewt            = 86.177;
     FluidList[arrayindex].omega             = 0.300;
     FluidList[arrayindex].Vc_SI             = 368.00;
     FluidList[arrayindex].Tc_SI             = 507.60;
     FluidList[arrayindex].Pc_SI             = 30.25;
     FluidList[arrayindex].hfo_SI            = -1.66940 * pow( 10.0, +05.0 );
     FluidList[arrayindex].sfo_SI            = +3.88740 * pow( 10.0, +02.0 );
     FluidList[arrayindex].LHV_SI            = -3855100;
     FluidList[arrayindex].HHV_SI            = -4196274;
     ToUSCS( ( arrayindex + 1 ) );
     FluidList[arrayindex].Tmin_USCS         = 399.67;
     FluidList[arrayindex].Tmax_USCS         = 1159.67;
     FluidList[arrayindex].Pmin_USCS         = 1.0;
     FluidList[arrayindex].Pmax_USCS         = 8000.0;
     FluidList[arrayindex].Dipole_SI         = 0.00;
     FluidList[arrayindex].Dipole_USCS       = 0.00;
     FluidList[arrayindex].Bo                = 2.66233;
     FluidList[arrayindex].Ao                = 45333.1;
     FluidList[arrayindex].Co                = 5.26067E+10;
     FluidList[arrayindex].Do                = 5.52158E+12;
     FluidList[arrayindex].Eo                = 6.26433E+14;
     FluidList[arrayindex].b                 = 29.4983;
     FluidList[arrayindex].a                 = 434517;
     FluidList[arrayindex].d                 = +3.27460E+07;
     FluidList[arrayindex].alpha             = +9.70230;
     FluidList[arrayindex].c                 = +3.18412E+11;
     FluidList[arrayindex].gamma             = +1.48720E+01;

     FluidList[arrayindex].A                 = -7.391000;
     FluidList[arrayindex].B                 = +2.291100E-01;
     FluidList[arrayindex].C                 = -8.156900E-05;
     FluidList[arrayindex].D                 = +4.527840E-07;
     FluidList[arrayindex].E                 = -2.523180E-10;
     FluidList[arrayindex].F                 = +4.748019E-14;
     FluidList[arrayindex].G                 = -4.230000E-01;

     FluidList[arrayindex].HT5               = +0.000000;
     FluidList[arrayindex].HT4               = +0.000000;
     FluidList[arrayindex].HT3               = +0.000000;
     FluidList[arrayindex].HT2               = +4.148000E-04;
     FluidList[arrayindex].HT1               = +0.240000E-03;
     FluidList[arrayindex].HT0               = -6.183800E+02;
     FluidList[arrayindex].Hn                = +0.000000;
     
     FluidList[arrayindex].ST5               = +0.000000;
     FluidList[arrayindex].ST4               = +0.000000;
     FluidList[arrayindex].ST3               = +0.000000;
     FluidList[arrayindex].ST2               = +0.000000;
     FluidList[arrayindex].ST1               = +0.000000;
     FluidList[arrayindex].ST0               = +0.000000;

     /////////////////////////////////////////////////////////////////////
     // Fluid Index No. 0009 
     /////////////////////////////////////////////////////////////////////
     // N_Heptane 
     /////////////////////////////////////////////////////////////////////
     arrayindex                              = (9 - 1);
     strcpy_s(FluidList[arrayindex].name, "N_Heptane");
     strcpy_s(FluidList[arrayindex].name_alt01, "-" );
     strcpy_s(FluidList[arrayindex].chemsym, "C7H16");
     FluidList[arrayindex].CASNo[0]          = 5;
     FluidList[arrayindex].CASNo[1]          = 82;
     FluidList[arrayindex].CASNo[2]          = 142;
     FluidList[arrayindex].Polingetl         = 259;
     FluidList[arrayindex].molewt            = 100.204;
     FluidList[arrayindex].omega             = 0.350;
     FluidList[arrayindex].Vc_SI             = 428;
     FluidList[arrayindex].Tc_SI             = 540.20;
     FluidList[arrayindex].Pc_SI             = 27.40;
     FluidList[arrayindex].hfo_SI            = -1.87650 * pow( 10.0, +05.0 );
     FluidList[arrayindex].sfo_SI            = +4.27980 * pow( 10.0, +02.0 );
     FluidList[arrayindex].LHV_SI            = -4464700;
     FluidList[arrayindex].HHV_SI            = -4855096;
     ToUSCS( ( arrayindex + 1 ) );
     FluidList[arrayindex].Tmin_USCS         = 459.67;
     FluidList[arrayindex].Tmax_USCS         = 1159.67;
     FluidList[arrayindex].Pmin_USCS         = 1.0;
     FluidList[arrayindex].Pmax_USCS         = 8000.0;
     FluidList[arrayindex].Dipole_SI         = 0.00;
     FluidList[arrayindex].Dipole_USCS       = 0.00;
     FluidList[arrayindex].Bo                = 3.60493;
     FluidList[arrayindex].Ao                = 77826.9;
     FluidList[arrayindex].Co                = 61566200000;
     FluidList[arrayindex].Do                = 7.77123E+12;
     FluidList[arrayindex].Eo                = 6.36251E+12;
     FluidList[arrayindex].b                 = 27.4415;
     FluidList[arrayindex].a                 = 359087;
     FluidList[arrayindex].d                 = 8351150;
     FluidList[arrayindex].alpha             = 21.8782;
     FluidList[arrayindex].c                 = 3.74876E+11;
     FluidList[arrayindex].gamma             = 24.76040;

     FluidList[arrayindex].A                 = -6.600000 * pow( 10.0, -2 );
     FluidList[arrayindex].B                 = +1.802100 * pow( 10.0, -1 );
     FluidList[arrayindex].C                 = +3.472900 * pow( 10.0, -5 );
     FluidList[arrayindex].D                 = +3.218790 * pow( 10.0, -7 );
     FluidList[arrayindex].E                 = -1.836600 * pow( 10.0, -10 );
     FluidList[arrayindex].F                 = +3.376939 * pow( 10.0, -14 );
     FluidList[arrayindex].G                 = -2.540000 * pow( 10.0, -1 );

     FluidList[arrayindex].HT5               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT4               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT3               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT2               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT1               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT0               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].Hn                = +0.000000 * pow( 10.0, +0 );
     
     FluidList[arrayindex].ST5               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST4               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST3               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST2               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST1               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST0               = +0.000000 * pow( 10.0, +0 );

     /////////////////////////////////////////////////////////////////////
     // Fluid Index No. 0010
     /////////////////////////////////////////////////////////////////////
     // N_Octane
     /////////////////////////////////////////////////////////////////////
     arrayindex                              = (10 - 1);
     strcpy_s(FluidList[arrayindex].name, "N_Octane");
     strcpy_s(FluidList[arrayindex].name_alt01, "-" );
     strcpy_s(FluidList[arrayindex].chemsym, "C8H18");
     FluidList[arrayindex].CASNo[0]          = 9;
     FluidList[arrayindex].CASNo[1]          = 65;
     FluidList[arrayindex].CASNo[2]          = 111;
     FluidList[arrayindex].Polingetl         = 296;
     FluidList[arrayindex].molewt            = 114.231;
     FluidList[arrayindex].omega             = 0.399;
     FluidList[arrayindex].Vc_SI             = 492.00;
     FluidList[arrayindex].Tc_SI             = 568.70;
     FluidList[arrayindex].Pc_SI             = 24.90;
     FluidList[arrayindex].hfo_SI            = -2.08750 * pow( 10.0, +05.0 );
     FluidList[arrayindex].sfo_SI            = +4.67230 * pow( 10.0, +02.0 );
     FluidList[arrayindex].LHV_SI            = -5074200;
     FluidList[arrayindex].HHV_SI            = -5513528;
     ToUSCS( ( arrayindex + 1 ) );
     FluidList[arrayindex].Tmin_USCS         = 459.67;
     FluidList[arrayindex].Tmax_USCS         = 1159.67;
     FluidList[arrayindex].Pmin_USCS         = 1.0;
     FluidList[arrayindex].Pmax_USCS         = 8000.0;
     FluidList[arrayindex].Dipole_SI         = 0.00;
     FluidList[arrayindex].Dipole_USCS       = 0.00;
     FluidList[arrayindex].Bo                = 4.86965;
     FluidList[arrayindex].Ao                = 81690.6;
     FluidList[arrayindex].Co                = 99654600000;
     FluidList[arrayindex].Do                = 7.90575E+12;
     FluidList[arrayindex].Eo                = 3.46419E+13;
     FluidList[arrayindex].b                 = 10.5907;
     FluidList[arrayindex].a                 = 131646;
     FluidList[arrayindex].d                 = 185906000;
     FluidList[arrayindex].alpha             = 34.5124;
     FluidList[arrayindex].c                 = 6.42053E+11;
     FluidList[arrayindex].gamma             = 21.98880;

     FluidList[arrayindex].A                 = +1.120000 * pow( 10.0, +0 );
     FluidList[arrayindex].B                 = +1.173080 * pow( 10.0, -1 );
     FluidList[arrayindex].C                 = +4.881000 * pow( 10.0, -5 );
     FluidList[arrayindex].D                 = +3.054010 * pow( 10.0, -7 );
     FluidList[arrayindex].E                 = -1.736549 * pow( 10.0, -10 );
     FluidList[arrayindex].F                 = +3.124831 * pow( 10.0, -14 );
     FluidList[arrayindex].G                 = -2.623000 * pow( 10.0, -1 );

     FluidList[arrayindex].HT5               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT4               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT3               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT2               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT1               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT0               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].Hn                = +0.000000 * pow( 10.0, +0 );
     
     FluidList[arrayindex].ST5               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST4               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST3               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST2               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST1               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST0               = +0.000000 * pow( 10.0, +0 );

     /////////////////////////////////////////////////////////////////////
     // Fluid Index No. 0011
     /////////////////////////////////////////////////////////////////////
     // Ethylene
     /////////////////////////////////////////////////////////////////////
     arrayindex                              = (11 - 1);
     strcpy_s(FluidList[arrayindex].name, "Ethylene");
     strcpy_s(FluidList[arrayindex].name_alt01, "Ethene" );
     strcpy_s(FluidList[arrayindex].chemsym, "C2H4");
     FluidList[arrayindex].CASNo[0]          = 1;
     FluidList[arrayindex].CASNo[1]          = 85;
     FluidList[arrayindex].CASNo[2]          = 74;
     FluidList[arrayindex].Polingetl         = 55;
     FluidList[arrayindex].molewt            = 28.054;
     FluidList[arrayindex].omega             = 0.087;
     FluidList[arrayindex].Vc_SI             = 131.10;
     FluidList[arrayindex].Tc_SI             = 282.34;
     FluidList[arrayindex].Pc_SI             = 50.41;
     FluidList[arrayindex].hfo_SI            = +5.25100 * pow( 10.0, +04.0 );
     FluidList[arrayindex].sfo_SI            = +2.19200 * pow( 10.0, +02.0 );
     FluidList[arrayindex].LHV_SI            = -1323000;
     FluidList[arrayindex].HHV_SI            = -1411574;
     ToUSCS( ( arrayindex + 1 ) );
     FluidList[arrayindex].Tmin_USCS         = 219.67;
     FluidList[arrayindex].Tmax_USCS         = 959.67;
     FluidList[arrayindex].Pmin_USCS         = 1.0;
     FluidList[arrayindex].Pmax_USCS         = 8000.0;
     FluidList[arrayindex].Dipole_SI         = 0.00;
     FluidList[arrayindex].Dipole_USCS       = 0.00;
     FluidList[arrayindex].Bo                = 0.747945;
     FluidList[arrayindex].Ao                = 12133.9;
     FluidList[arrayindex].Co                = 1632030000;
     FluidList[arrayindex].Do                = 51756300000;
     FluidList[arrayindex].Eo                = 16170600000;
     FluidList[arrayindex].b                 = 2.62914;
     FluidList[arrayindex].a                 = 15988.1;
     FluidList[arrayindex].d                 = 903550;
     FluidList[arrayindex].alpha             = 0.589158;
     FluidList[arrayindex].c                 = 4097250000;
     FluidList[arrayindex].gamma             = 2.27971;

     FluidList[arrayindex].A                 = +2.583500 * pow( 10.0, +1 );
     FluidList[arrayindex].B                 = +1.450000 * pow( 10.0, -1 );
     FluidList[arrayindex].C                 = +1.710120 * pow( 10.0, -4 );
     FluidList[arrayindex].D                 = +7.619700 * pow( 10.0, -8 );
     FluidList[arrayindex].E                 = -4.503090 * pow( 10.0, -11 );
     FluidList[arrayindex].F                 = +6.664900 * pow( 10.0, -15 );
     FluidList[arrayindex].G                 = +7.483000 * pow( 10.0, -1 );

     FluidList[arrayindex].HT5               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT4               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT3               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT2               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT1               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT0               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].Hn                = +0.000000 * pow( 10.0, +0 );
     
     FluidList[arrayindex].ST5               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST4               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST3               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST2               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST1               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST0               = +0.000000 * pow( 10.0, +0 );

     /////////////////////////////////////////////////////////////////////
     // Fluid Index No. 0012 
     /////////////////////////////////////////////////////////////////////
     // Propylene
     /////////////////////////////////////////////////////////////////////
     arrayindex                              = (12 - 1);
     strcpy_s(FluidList[11].name, "Propylene");
     strcpy_s(FluidList[arrayindex].name_alt01, "Propene" );
     strcpy_s(FluidList[11].chemsym, "C3H6");
     FluidList[arrayindex].CASNo[0]          = 1;
     FluidList[arrayindex].CASNo[1]          = 07;
     FluidList[arrayindex].CASNo[2]          = 115;
     FluidList[arrayindex].Polingetl         = 85;
     FluidList[arrayindex].molewt            = 42.081;
     FluidList[arrayindex].omega             = 0.142;
     FluidList[arrayindex].Vc_SI             = 184.60;
     FluidList[arrayindex].Tc_SI             = 364.90;
     FluidList[arrayindex].Pc_SI             = 46.00;
     FluidList[arrayindex].hfo_SI            = +1.97100 * pow( 10.0, +04.0 );
     FluidList[arrayindex].sfo_SI            = +2.66600 * pow( 10.0, +02.0 );
     FluidList[arrayindex].LHV_SI            = -1925700;
     FluidList[arrayindex].HHV_SI            = -2058306;
     ToUSCS( ( arrayindex + 1 ) );
     FluidList[arrayindex].Tmin_USCS         = 219.67;
     FluidList[arrayindex].Tmax_USCS         = 959.67;
     FluidList[arrayindex].Pmin_USCS         = 1.0;
     FluidList[arrayindex].Pmax_USCS         = 8000.0;
     FluidList[arrayindex].Dipole_SI         = 0.37;
     FluidList[arrayindex].Dipole_USCS       = 0.00;
     FluidList[arrayindex].Bo                = 0.114457;
     FluidList[arrayindex].Ao                = 6051.36;
     FluidList[arrayindex].Co                = 9747620000;
     FluidList[arrayindex].Do                = 7.05921E+11;
     FluidList[arrayindex].Eo                = 3.4125E+13;
     FluidList[arrayindex].b                 = 7.64114;
     FluidList[arrayindex].a                 = 81880.4;
     FluidList[arrayindex].d                 = 5419350;
     FluidList[arrayindex].alpha             = 1.36532;
     FluidList[arrayindex].c                 = 29414100000;
     FluidList[arrayindex].gamma             = 4.07919;

     FluidList[arrayindex].A                 = +2.853400 * pow( 10.0, +1 );
     FluidList[arrayindex].B                 = +3.080000 * pow( 10.0, -2 );
     FluidList[arrayindex].C                 = +3.512240 * pow( 10.0, -4 );
     FluidList[arrayindex].D                 = -4.946600 * pow( 10.0, -8 );
     FluidList[arrayindex].E                 = -2.261700 * pow( 10.0, -12 );
     FluidList[arrayindex].F                 = +1.125500 * pow( 10.0, -15 );
     FluidList[arrayindex].G                 = +9.653000 * pow( 10.0, -1 );
     
     FluidList[arrayindex].HT5               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT4               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT3               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT2               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT1               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT0               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].Hn                = +0.000000 * pow( 10.0, +0 );
     
     FluidList[arrayindex].ST5               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST4               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST3               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST2               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST1               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST0               = +0.000000 * pow( 10.0, +0 );

     /////////////////////////////////////////////////////////////////////
     // Fluid Index No. 0013
     /////////////////////////////////////////////////////////////////////
     // Carbon Dioxide
     /////////////////////////////////////////////////////////////////////
     arrayindex                              = (13 - 1);
     strcpy_s(FluidList[arrayindex].name, "Carbon Dioxide");
     strcpy_s(FluidList[arrayindex].name_alt01, "-" );
     strcpy_s(FluidList[arrayindex].chemsym, "CO2");
     FluidList[arrayindex].CASNo[0]          = 9;
     FluidList[arrayindex].CASNo[1]          = 38;
     FluidList[arrayindex].CASNo[2]          = 124;
     FluidList[arrayindex].Polingetl         = 31;
     FluidList[arrayindex].molewt            = 44.010;
     FluidList[arrayindex].omega             = 0.225;
     FluidList[arrayindex].Vc_SI             = 94.07;
     FluidList[arrayindex].Tc_SI             = 304.12;
     FluidList[arrayindex].Pc_SI             = 73.74;
     FluidList[arrayindex].hfo_SI            = -3.93510 * pow( 10.0, +05.0 );
     FluidList[arrayindex].sfo_SI            = +2.13680 * pow( 10.0, +02.0 );
     FluidList[arrayindex].LHV_SI            = 0.0;
     FluidList[arrayindex].HHV_SI            = 0.0;
     ToUSCS( ( arrayindex + 1 ) );
     FluidList[arrayindex].Tmin_USCS         = 399.67;
     FluidList[arrayindex].Tmax_USCS         = 959.67;
     FluidList[arrayindex].Pmin_USCS         = 1.0;
     FluidList[arrayindex].Pmax_USCS         = 8000.0;
     FluidList[arrayindex].Dipole_SI         = 0.00;
     FluidList[arrayindex].Dipole_USCS       = 0.00;
     FluidList[arrayindex].Bo                = 0.394117;
     FluidList[arrayindex].Ao                = 6592.03;
     FluidList[arrayindex].Co                = 2959020000;
     FluidList[arrayindex].Do                = 4.09151E+11;
     FluidList[arrayindex].Eo                = 10289800000;
     FluidList[arrayindex].b                 = 0.971443;
     FluidList[arrayindex].a                 = 5632.85;
     FluidList[arrayindex].d                 = 59929.7;
     FluidList[arrayindex].alpha             = 0.395525;
     FluidList[arrayindex].c                 = 2746680000;
     FluidList[arrayindex].gamma             = 1.64916;

     FluidList[arrayindex].A                 = +4.778000 * pow( 10.0, +0 );
     FluidList[arrayindex].B                 = +1.144300 * pow( 10.0, -1 );
     FluidList[arrayindex].C                 = +1.011300 * pow( 10.0, -4 );
     FluidList[arrayindex].D                 = -2.649000 * pow( 10.0, -8 );
     FluidList[arrayindex].E                 = +3.470600 * pow( 10.0, -12 );
     FluidList[arrayindex].F                 = -1.314000 * pow( 10.0, -16 );
     FluidList[arrayindex].G                 = +3.434000 * pow( 10.0, -1 );
     
     FluidList[arrayindex].HT5               = -8.069173 * pow( 10.0, -14.0 );
     FluidList[arrayindex].HT4               = +1.242033 * pow( 10.0, -09.0 );
     FluidList[arrayindex].HT3               = -7.423608 * pow( 10.0, -06.0 );
     FluidList[arrayindex].HT2               = +2.227603 * pow( 10.0, -02.0 );
     FluidList[arrayindex].HT1               = +2.721386 * pow( 10.0, +01.0 );
     FluidList[arrayindex].HT0               = -1.008585 * pow( 10.0, +04.0 );
     FluidList[arrayindex].Hn                = +9.000000 * pow( 10.0, +06.0 );
     
     FluidList[arrayindex].ST5               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST4               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST3               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST2               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST1               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST0               = +0.000000 * pow( 10.0, +0 );

     /////////////////////////////////////////////////////////////////////
     // Fluid Index No. 0014 
     /////////////////////////////////////////////////////////////////////
     // Hydrogen Sulfide 
     /////////////////////////////////////////////////////////////////////
     arrayindex                              = (14 - 1);
     strcpy_s(FluidList[arrayindex].name, "Hydrogen Sulfide");
     strcpy_s(FluidList[arrayindex].name_alt01, "-" );
     strcpy_s(FluidList[arrayindex].chemsym, "H2S");
     FluidList[arrayindex].CASNo[0]          = 4;
     FluidList[arrayindex].CASNo[1]          = 06;
     FluidList[arrayindex].CASNo[2]          = 7783;
     FluidList[arrayindex].Polingetl         = 441;
     FluidList[arrayindex].molewt            = 34.082;
     FluidList[arrayindex].omega             = 0.090;
     FluidList[arrayindex].Vc_SI             = 98.00;
     FluidList[arrayindex].Tc_SI             = 373.40;
     FluidList[arrayindex].Pc_SI             = 89.63;
     FluidList[arrayindex].hfo_SI            = -2.06300 * pow( 10.0, +04.0 );
     FluidList[arrayindex].sfo_SI            = +2.05600 * pow( 10.0, +02.0 );
     FluidList[arrayindex].LHV_SI            = -518000;
     FluidList[arrayindex].HHV_SI            = -562232;
     ToUSCS( ( arrayindex + 1 ) );
     FluidList[arrayindex].Tmin_USCS         = 339.67;
     FluidList[arrayindex].Tmax_USCS         = 1059.67;
     FluidList[arrayindex].Pmin_USCS         = 1.0;
     FluidList[arrayindex].Pmax_USCS         = 8000.0;
     FluidList[arrayindex].Dipole_SI         = 0.97;
     FluidList[arrayindex].Dipole_USCS       = 0.00;
     FluidList[arrayindex].Bo                = 0.297508;
     FluidList[arrayindex].Ao                = 10586.3;
     FluidList[arrayindex].Co                = 2114960000;
     FluidList[arrayindex].Do                = 48651800000;
     FluidList[arrayindex].Eo                = 39322600000;
     FluidList[arrayindex].b                 = 2.53315;
     FluidList[arrayindex].a                 = 20511;
     FluidList[arrayindex].d                 = 19973.1;
     FluidList[arrayindex].alpha             = 0.165961;
     FluidList[arrayindex].c                 = 4361320000;
     FluidList[arrayindex].gamma             = 1.20447;

     FluidList[arrayindex].A                 = -6.170000 * pow( 10.0, -1 );
     FluidList[arrayindex].B                 = +2.385800 * pow( 10.0, -1 );
     FluidList[arrayindex].C                 = -2.446000 * pow( 10.0, -5 );
     FluidList[arrayindex].D                 = +4.107000 * pow( 10.0, -8 );
     FluidList[arrayindex].E                 = -1.301260 * pow( 10.0, -11 );
     FluidList[arrayindex].F                 = +1.448520 * pow( 10.0, -15 );
     FluidList[arrayindex].G                 = -4.590000 * pow( 10.0, -2 );
     
     FluidList[arrayindex].HT5               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT4               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT3               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT2               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT1               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT0               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].Hn                = +0.000000 * pow( 10.0, +0 );
     
     FluidList[arrayindex].ST5               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST4               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST3               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST2               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST1               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST0               = +0.000000 * pow( 10.0, +0 );

     /////////////////////////////////////////////////////////////////////
     // Fluid Index No. 0015 
     /////////////////////////////////////////////////////////////////////
     // Nitrogen
     /////////////////////////////////////////////////////////////////////
     arrayindex                              = (15 - 1);
     strcpy_s(FluidList[arrayindex].name, "Nitrogen");
     strcpy_s(FluidList[arrayindex].name_alt01, "-" );
     strcpy_s(FluidList[arrayindex].chemsym, "N2");
     FluidList[arrayindex].CASNo[0]          = 9;
     FluidList[arrayindex].CASNo[1]          = 37;
     FluidList[arrayindex].CASNo[2]          = 7727;
     FluidList[arrayindex].Polingetl         = 455;
     FluidList[arrayindex].molewt            = 28.014;
     FluidList[arrayindex].omega             = 0.037;
     FluidList[arrayindex].Vc_SI             = 90.10;
     FluidList[arrayindex].Tc_SI             = 126.20;
     FluidList[arrayindex].Pc_SI             = 33.98;
     FluidList[arrayindex].hfo_SI            = -0.00000 * pow( 10.0, +04.0 );
     FluidList[arrayindex].sfo_SI            = +1.91500 * pow( 10.0, +02.0 );
     FluidList[arrayindex].LHV_SI            = 0.0;
     FluidList[arrayindex].HHV_SI            = 0.0;
     ToUSCS( ( arrayindex + 1 ) );
     FluidList[arrayindex].Tmin_USCS         = 159.67;
     FluidList[arrayindex].Tmax_USCS         = 939.67;
     FluidList[arrayindex].Pmin_USCS         = 1.0;
     FluidList[arrayindex].Pmax_USCS         = 8000.0;
     FluidList[arrayindex].Dipole_SI         = 0.00;
     FluidList[arrayindex].Dipole_USCS       = 0.00;
     FluidList[arrayindex].Bo                = 0.677022;
     FluidList[arrayindex].Ao                = 4185.05;
     FluidList[arrayindex].Co                = 137936000;
     FluidList[arrayindex].Do                = 19518300000;
     FluidList[arrayindex].Eo                = 1.21648E+12;
     FluidList[arrayindex].b                 = 0.83347;
     FluidList[arrayindex].a                 = 1404.59;
     FluidList[arrayindex].d                 = 31189.4;
     FluidList[arrayindex].alpha             = 0.302696;
     FluidList[arrayindex].c                 = 84431700;
     FluidList[arrayindex].gamma             = 1.10011;

     FluidList[arrayindex].A                 = -9.340000 * pow( 10.0, -1 );
     FluidList[arrayindex].B                 = +2.552000 * pow( 10.0, -1 );
     FluidList[arrayindex].C                 = -1.779000 * pow( 10.0, -5 );
     FluidList[arrayindex].D                 = +1.589000 * pow( 10.0, -8 );
     FluidList[arrayindex].E                 = -3.220300 * pow( 10.0, -12 );
     FluidList[arrayindex].F                 = +1.589300 * pow( 10.0, -16 );
     FluidList[arrayindex].G                 = +4.240000 * pow( 10.0, -2 );

     FluidList[arrayindex].HT5               = -2.264581 * pow( 10.0, -15.0 );
     FluidList[arrayindex].HT4               = +8.733881 * pow( 10.0, -11.0 );
     FluidList[arrayindex].HT3               = -9.640211 * pow( 10.0, -07.0 );
     FluidList[arrayindex].HT2               = +4.833534 * pow( 10.0, -03.0 );
     FluidList[arrayindex].HT1               = +2.559339 * pow( 10.0, +01.0 );
     FluidList[arrayindex].HT0               = -7.900000 * pow( 10.0, +03.0 );
     FluidList[arrayindex].Hn                = -3.754775 * pow( 10.0, +07.0 );
     
     FluidList[arrayindex].ST5               = +3.407212 * pow( 10.0, -16.0 );
     FluidList[arrayindex].ST4               = -4.970684 * pow( 10.0, -12.0 );
     FluidList[arrayindex].ST3               = +2.768674 * pow( 10.0, -08.0 );
     FluidList[arrayindex].ST2               = -7.575788 * pow( 10.0, -05.0 );
     FluidList[arrayindex].ST1               = +1.199519 * pow( 10.0, -01.0 );
     FluidList[arrayindex].ST0               = +1.617520 * pow( 10.0, +02.0 );

     /////////////////////////////////////////////////////////////////////
     // Fluid Index No. 0016
     /////////////////////////////////////////////////////////////////////
     // Hydrogen
     /////////////////////////////////////////////////////////////////////
     arrayindex                              = (16 - 1);
     strcpy_s(FluidList[arrayindex].name, "Hydrogen");
     strcpy_s(FluidList[arrayindex].name_alt01, "-" );
     strcpy_s(FluidList[arrayindex].chemsym, "H2");
     FluidList[arrayindex].CASNo[0]          = 5;
     FluidList[arrayindex].CASNo[1]          = 51;
     FluidList[arrayindex].CASNo[2]          = 800000;
     FluidList[arrayindex].Polingetl         = 439;
     FluidList[arrayindex].molewt            = 2.016;
     FluidList[arrayindex].omega             = -0.216;
     FluidList[arrayindex].Vc_SI             = 65.00;
     FluidList[arrayindex].Tc_SI             = 33.25;
     FluidList[arrayindex].Pc_SI             = 12.97;
     FluidList[arrayindex].hfo_SI            = -0.00000 * pow( 10.0, +04.0 );
     FluidList[arrayindex].sfo_SI            = +1.30570 * pow( 10.0, +02.0 );
     FluidList[arrayindex].LHV_SI            = -241800;
     FluidList[arrayindex].HHV_SI            = -286022;
     ToUSCS( ( arrayindex + 1 ) );
     FluidList[arrayindex].Tmin_USCS         = 219.67;
     FluidList[arrayindex].Tmax_USCS         = 959.67;
     FluidList[arrayindex].Pmin_USCS         = 1.0;
     FluidList[arrayindex].Pmax_USCS         = 8000.0;
     FluidList[arrayindex].Dipole_SI         = 0.00;
     FluidList[arrayindex].Dipole_USCS       = 0.00;
     //Hydrogen is a highly polar molecule and the BWRS EOS does
     //a poor job of describing the properties.  However, false
     //critical constants squeeze the best performance possible out
     //of the EOS.  To generate the generalized BWRS constants, we 
     //will substitute the false properties for the real ones and
     //then restore the original constants.
     dtemp01                                 = FluidList[arrayindex].omega;
     dtemp02                                 = FluidList[arrayindex].MDc;
     FluidList[arrayindex].omega             = 0.00;
     FluidList[arrayindex].MDc               = 1.2486;
     bTemp = _bCalc_Bo( arrayindex, &FluidList[arrayindex].Bo);
     bTemp = _bCalc_Ao( arrayindex, &FluidList[arrayindex].Ao);
     bTemp = _bCalc_Co( arrayindex, &FluidList[arrayindex].Co);
     bTemp = _bCalc_Do( arrayindex, &FluidList[arrayindex].Do);
     bTemp = _bCalc_Eo( arrayindex, &FluidList[arrayindex].Eo);
     bTemp = _bCalc_b( arrayindex, &FluidList[arrayindex].b);
     bTemp = _bCalc_a( arrayindex, &FluidList[arrayindex].a);
     bTemp = _bCalc_d( arrayindex, &FluidList[arrayindex].d);
     bTemp = _bCalc_alpha( arrayindex, &FluidList[arrayindex].alpha);
     bTemp = _bCalc_c( arrayindex, &FluidList[arrayindex].c);
     bTemp = _bCalc_gamma( arrayindex, &FluidList[arrayindex].gamma);
     //Restore the values
     FluidList[arrayindex].omega             = dtemp01;
     FluidList[arrayindex].MDc               = dtemp02;
     //carry on
     FluidList[arrayindex].A                 = +1.232800 * pow( 10.0, +01 );
     FluidList[arrayindex].B                 = +3.199620 * pow( 10.0, +00 );
     FluidList[arrayindex].C                 = +3.927900 * pow( 10.0, -04 );
     FluidList[arrayindex].D                 = -2.934500 * pow( 10.0, -07 );
     FluidList[arrayindex].E                 = +1.090070 * pow( 10.0, -10 );
     FluidList[arrayindex].F                 = -1.387870 * pow( 10.0, -14 );
     FluidList[arrayindex].G                 = -4.938300 * pow( 10.0, +00 );
     
     FluidList[arrayindex].HT5               = +1.898078 * pow( 10.0, -14.0 );
     FluidList[arrayindex].HT4               = -2.563904 * pow( 10.0, -10.0 );
     FluidList[arrayindex].HT3               = +1.134178 * pow( 10.0, -06.0 );
     FluidList[arrayindex].HT2               = -3.442698 * pow( 10.0, -04.0 );
     FluidList[arrayindex].HT1               = +2.863626 * pow( 10.0, +01.0 );
     FluidList[arrayindex].HT0               = -8.535139 * pow( 10.0, +03.0 );
     FluidList[arrayindex].Hn                = +0.000000 * pow( 10.0, +00.0 );
     
     FluidList[arrayindex].ST5               = +3.732955 * pow( 10.0, -16.0 );
     FluidList[arrayindex].ST4               = -5.423520 * pow( 10.0, -12.0 );
     FluidList[arrayindex].ST3               = +2.994065 * pow( 10.0, -08.0 );
     FluidList[arrayindex].ST2               = -8.018026 * pow( 10.0, -05.0 );
     FluidList[arrayindex].ST1               = +1.215131 * pow( 10.0, -01.0 );
     FluidList[arrayindex].ST0               = +1.008135 * pow( 10.0, +02.0 );

     /////////////////////////////////////////////////////////////////////
     // Fluid Index No. 0017
     /////////////////////////////////////////////////////////////////////
     // Ammonia
     /////////////////////////////////////////////////////////////////////
     arrayindex                              = (17 - 1);
     strcpy_s(FluidList[arrayindex].name, "Ammonia");
     strcpy_s(FluidList[arrayindex].name_alt01, "-" );
     strcpy_s(FluidList[arrayindex].chemsym, "H3N");
     FluidList[arrayindex].CASNo[0]          = 7;
     FluidList[arrayindex].CASNo[1]          = 41;
     FluidList[arrayindex].CASNo[2]          = 7664;
     FluidList[arrayindex].Polingetl         = 447;
     FluidList[arrayindex].molewt            = 17.031;
     FluidList[arrayindex].omega             = 0.257;
     FluidList[arrayindex].Vc_SI             = 72.47;
     FluidList[arrayindex].Tc_SI             = 405.40;
     FluidList[arrayindex].Pc_SI             = 113.53;
     FluidList[arrayindex].hfo_SI            = -4.58980 * pow( 10.0, +04.0 );
     FluidList[arrayindex].sfo_SI            = +1.92660 * pow( 10.0, +02.0 );
     FluidList[arrayindex].LHV_SI            = -316800;
     FluidList[arrayindex].HHV_SI            = -383135;
     ToUSCS( ( arrayindex + 1 ) );
     FluidList[arrayindex].Tmin_USCS         = 219.67;
     FluidList[arrayindex].Tmax_USCS         = 959.67;
     FluidList[arrayindex].Pmin_USCS         = 1.0;
     FluidList[arrayindex].Pmax_USCS         = 8000.0;
     FluidList[arrayindex].Dipole_SI         = 1.47;
     FluidList[arrayindex].Dipole_USCS       = 0.00;
     FluidList[arrayindex].Bo                = 0.548633162;
     FluidList[arrayindex].Ao                = 9661.52583;
     FluidList[arrayindex].Co                = 3879076756;
     FluidList[arrayindex].Do                = 275123362994;
     FluidList[arrayindex].Eo                = 11525495511744;
     FluidList[arrayindex].b                 = 0.830244189;
     FluidList[arrayindex].a                 = 7156.10064;
     FluidList[arrayindex].d                 = 1479323.2;
     FluidList[arrayindex].alpha             = 0.092974899;
     FluidList[arrayindex].c                 = 4798007157;
     FluidList[arrayindex].gamma             = 0.643302037;

     FluidList[arrayindex].A                 = -9.470000 * pow( 10.0, -1 );
     FluidList[arrayindex].B                 = +4.801500 * pow( 10.0, -1 );
     FluidList[arrayindex].C                 = -8.625800 * pow( 10.0, -5 );
     FluidList[arrayindex].D                 = +1.749500 * pow( 10.0, -7 );
     FluidList[arrayindex].E                 = -6.542800 * pow( 10.0, -11 );
     FluidList[arrayindex].F                 = +8.558900 * pow( 10.0, -15 );
     FluidList[arrayindex].G                 = -0.284500 * pow( 10.0, -1 );

     FluidList[arrayindex].HT5               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT4               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT3               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT2               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT1               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT0               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].Hn                = +0.000000 * pow( 10.0, +0 );
     
     FluidList[arrayindex].ST5               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST4               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST3               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST2               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST1               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST0               = +0.000000 * pow( 10.0, +0 );
     
     /////////////////////////////////////////////////////////////////////
     // Fluid Index No. 0018
     /////////////////////////////////////////////////////////////////////
     // Water
     /////////////////////////////////////////////////////////////////////
     arrayindex                              = (18 - 1);
     strcpy_s(FluidList[arrayindex].name, "Water");
     strcpy_s(FluidList[arrayindex].name_alt01, "-" );
     strcpy_s(FluidList[arrayindex].chemsym, "H2O");
     FluidList[arrayindex].CASNo[0]          = 5;
     FluidList[arrayindex].CASNo[1]          = 18;
     FluidList[arrayindex].CASNo[2]          = 7732;
     FluidList[arrayindex].Polingetl         = 440;
     FluidList[arrayindex].molewt            = 18.015;
     FluidList[arrayindex].omega             = 0.344;
     FluidList[arrayindex].Vc_SI             = 55.95;
     FluidList[arrayindex].Tc_SI             = 647.14;
     FluidList[arrayindex].Pc_SI             = 220.64;
     FluidList[arrayindex].hfo_SI            = -2.41814 * pow( 10.0, +05.0 );
     FluidList[arrayindex].sfo_SI            = +1.88720 * pow( 10.0, +02.0 );
     FluidList[arrayindex].LHV_SI            = 0.0;
     FluidList[arrayindex].HHV_SI            = 0.0;
     ToUSCS( ( arrayindex + 1 ) );
     FluidList[arrayindex].Tmin_USCS         = 459.67;
     FluidList[arrayindex].Tmax_USCS         = 2459.67;
     FluidList[arrayindex].Pmin_USCS         = 1.0;
     FluidList[arrayindex].Pmax_USCS         = 8000.0;
     FluidList[arrayindex].Dipole_SI         = 1.85;
     FluidList[arrayindex].Dipole_USCS       = 0.00;
     bTemp = _bCalc_Bo( arrayindex, &FluidList[arrayindex].Bo);
     bTemp = _bCalc_Ao( arrayindex, &FluidList[arrayindex].Ao);
     bTemp = _bCalc_Co( arrayindex, &FluidList[arrayindex].Co);
     bTemp = _bCalc_Do( arrayindex, &FluidList[arrayindex].Do);
     bTemp = _bCalc_Eo( arrayindex, &FluidList[arrayindex].Eo);
     bTemp = _bCalc_b( arrayindex, &FluidList[arrayindex].b);
     bTemp = _bCalc_a( arrayindex, &FluidList[arrayindex].a);
     bTemp = _bCalc_d( arrayindex, &FluidList[arrayindex].d);
     bTemp = _bCalc_alpha( arrayindex, &FluidList[arrayindex].alpha);
     bTemp = _bCalc_c( arrayindex, &FluidList[arrayindex].c);
     bTemp = _bCalc_gamma( arrayindex, &FluidList[arrayindex].gamma);

     FluidList[arrayindex].A                 = -2.463000 * pow( 10.0, +0 );
     FluidList[arrayindex].B                 = +4.573900 * pow( 10.0, -1 );
     FluidList[arrayindex].C                 = -5.251200 * pow( 10.0, -5 );
     FluidList[arrayindex].D                 = +6.459400 * pow( 10.0, -8 );
     FluidList[arrayindex].E                 = -2.027600 * pow( 10.0, -11 );
     FluidList[arrayindex].F                 = +2.363100 * pow( 10.0, -15 );
     FluidList[arrayindex].G                 = -3.398000 * pow( 10.0, -1 );
     
     FluidList[arrayindex].HT5               = +2.334314 * pow( 10.0, -14.0 );
     FluidList[arrayindex].HT4               = -2.285832 * pow( 10.0, -10.0 );
     FluidList[arrayindex].HT3               = +7.598178 * pow( 10.0, -08.0 );
     FluidList[arrayindex].HT2               = +6.770593 * pow( 10.0, -03.0 );
     FluidList[arrayindex].HT1               = +2.852271 * pow( 10.0, +01.0 );
     FluidList[arrayindex].HT0               = -9.019494 * pow( 10.0, +03.0 );
     FluidList[arrayindex].Hn                = -2.394421 * pow( 10.0, +07.0 );
     
     FluidList[arrayindex].ST5               = +3.910386 * pow( 10.0, -16.0 );
     FluidList[arrayindex].ST4               = -5.655803 * pow( 10.0, -12.0 );
     FluidList[arrayindex].ST3               = +3.112474 * pow( 10.0, -08.0 );
     FluidList[arrayindex].ST2               = -8.415569 * pow( 10.0, -05.0 );
     FluidList[arrayindex].ST1               = +1.376850 * pow( 10.0, -01.0 );
     FluidList[arrayindex].ST0               = +1.543180 * pow( 10.0, +02.0 );

     /////////////////////////////////////////////////////////////////////
     // Fluid Index No. 0019
     /////////////////////////////////////////////////////////////////////
     // Air    
     /////////////////////////////////////////////////////////////////////
     arrayindex                              = (19 - 1);
     strcpy_s(FluidList[arrayindex].name, "Air");
     strcpy_s(FluidList[arrayindex].name_alt01, "-" );
     strcpy_s(FluidList[arrayindex].chemsym, "-");
     FluidList[arrayindex].CASNo[0]          = 0;
     FluidList[arrayindex].CASNo[1]          = 0;
     FluidList[arrayindex].CASNo[2]          = 0;
     FluidList[arrayindex].Polingetl         = 0;
     FluidList[arrayindex].molewt            = 28.96;
     FluidList[arrayindex].omega             = 0.089;
     FluidList[arrayindex].Vc_SI             = 86.431;
     FluidList[arrayindex].MDc               = 0;
     FluidList[arrayindex].Tc_SI             = 132.406;
     FluidList[arrayindex].Pc_SI             = 37.585;
     FluidList[arrayindex].hfo_SI            = -0.00000 * pow( 10.0, +04.0 );
     FluidList[arrayindex].sfo_SI            = +1.99000 * pow( 10.0, +02.0 );
     FluidList[arrayindex].LHV_SI            = +0.00000 * pow( 10.0, +05.0 );
     FluidList[arrayindex].HHV_SI            = 0.0;
     ToUSCS( ( arrayindex + 1 ) );
     FluidList[arrayindex].Tmin_USCS         = 409.67;
     FluidList[arrayindex].Tmax_USCS         = 2459.67;
     FluidList[arrayindex].Pmin_USCS         = 1.0;
     FluidList[arrayindex].Pmax_USCS         = 8000.0;
     FluidList[arrayindex].Dipole_SI         = 0.00;
     FluidList[arrayindex].Dipole_USCS       = 0.00;
     bTemp = _bCalc_Bo( arrayindex, &FluidList[arrayindex].Bo);
     bTemp = _bCalc_Ao( arrayindex, &FluidList[arrayindex].Ao);
     bTemp = _bCalc_Co( arrayindex, &FluidList[arrayindex].Co);
     bTemp = _bCalc_Do( arrayindex, &FluidList[arrayindex].Do);
     bTemp = _bCalc_Eo( arrayindex, &FluidList[arrayindex].Eo);
     bTemp = _bCalc_b( arrayindex, &FluidList[arrayindex].b);
     bTemp = _bCalc_a( arrayindex, &FluidList[arrayindex].a);
     bTemp = _bCalc_d( arrayindex, &FluidList[arrayindex].d);
     bTemp = _bCalc_alpha( arrayindex, &FluidList[arrayindex].alpha);
     bTemp = _bCalc_c( arrayindex, &FluidList[arrayindex].c);
     bTemp = _bCalc_gamma( arrayindex, &FluidList[arrayindex].gamma);

     FluidList[arrayindex].A                 = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].B                 = +2.300056 * pow( 10.0, -1 );
     FluidList[arrayindex].C                 = +1.094992 * pow( 10.0, -5 );
     FluidList[arrayindex].D                 = -1.706333 * pow( 10.0, -19 );
     FluidList[arrayindex].E                 = +4.667500 * pow( 10.0, -23 );
     FluidList[arrayindex].F                 = -3.878000 * pow( 10.0, -27 );
     FluidList[arrayindex].G                 = +1.054000 * pow( 10.0, 0 );
     
     FluidList[arrayindex].HT5               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT4               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT3               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT2               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT1               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT0               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].Hn                = +0.000000 * pow( 10.0, +0 );
     
     FluidList[arrayindex].ST5               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST4               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST3               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST2               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST1               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST0               = +0.000000 * pow( 10.0, +0 );

     /////////////////////////////////////////////////////////////////////
     // Fluid Index No. 0020
     /////////////////////////////////////////////////////////////////////
     // Carbon Monoxide
     /////////////////////////////////////////////////////////////////////
     arrayindex                              = (20 - 1);
     strcpy_s(FluidList[arrayindex].name, "Carbon Monoxide");
     strcpy_s(FluidList[arrayindex].name_alt01, "-" );
     strcpy_s(FluidList[arrayindex].chemsym, "CO");
     FluidList[arrayindex].CASNo[0]          = 0;
     FluidList[arrayindex].CASNo[1]          = 8;
     FluidList[arrayindex].CASNo[2]          = 630;
     FluidList[arrayindex].Polingetl         = 30;
     FluidList[arrayindex].molewt            = 28.010;
     FluidList[arrayindex].omega             = 0.045;
     FluidList[arrayindex].Vc_SI             = 93.10;
     FluidList[arrayindex].Tc_SI             = 132.85;
     FluidList[arrayindex].Pc_SI             = 34.94;
     FluidList[arrayindex].hfo_SI            = -1.10530 * pow( 10.0, +05.0 );
     FluidList[arrayindex].sfo_SI            = +1.97560 * pow( 10.0, +02.0 );
     FluidList[arrayindex].LHV_SI            = -283000;
     FluidList[arrayindex].HHV_SI            = -283000;
     ToUSCS( ( arrayindex + 1 ) );
     FluidList[arrayindex].Tmin_USCS         = 259.67;
     FluidList[arrayindex].Tmax_USCS         = 959.67;
     FluidList[arrayindex].Pmin_USCS         = 1.0;
     FluidList[arrayindex].Pmax_USCS         = 8000.0;
     FluidList[arrayindex].Dipole_SI         = 0.11;
     FluidList[arrayindex].Dipole_USCS       = 0.00;
     bTemp = _bCalc_Bo( arrayindex, &FluidList[arrayindex].Bo);
     bTemp = _bCalc_Ao( arrayindex, &FluidList[arrayindex].Ao);
     bTemp = _bCalc_Co( arrayindex, &FluidList[arrayindex].Co);
     bTemp = _bCalc_Do( arrayindex, &FluidList[arrayindex].Do);
     bTemp = _bCalc_Eo( arrayindex, &FluidList[arrayindex].Eo);
     bTemp = _bCalc_b( arrayindex, &FluidList[arrayindex].b);
     bTemp = _bCalc_a( arrayindex, &FluidList[arrayindex].a);
     bTemp = _bCalc_d( arrayindex, &FluidList[arrayindex].d);
     bTemp = _bCalc_alpha( arrayindex, &FluidList[arrayindex].alpha);
     bTemp = _bCalc_c( arrayindex, &FluidList[arrayindex].c);
     bTemp = _bCalc_gamma( arrayindex, &FluidList[arrayindex].gamma);

     FluidList[arrayindex].A                 = -9.750000 * pow( 10.0, -1 );
     FluidList[arrayindex].B                 = +2.565200 * pow( 10.0, -1 );
     FluidList[arrayindex].C                 = -2.291000 * pow( 10.0, -5 );
     FluidList[arrayindex].D                 = +2.228000 * pow( 10.0, -8 );
     FluidList[arrayindex].E                 = -5.632600 * pow( 10.0, -12 );
     FluidList[arrayindex].F                 = +4.558900 * pow( 10.0, -16 );
     FluidList[arrayindex].G                 = +9.250000 * pow( 10.0, -2 );
     
     FluidList[arrayindex].HT5               = -1.004889 * pow( 10.0, -14 );
     FluidList[arrayindex].HT4               = +1.934658 * pow( 10.0, -10 );
     FluidList[arrayindex].HT3               = -1.480345 * pow( 10.0, -06 );
     FluidList[arrayindex].HT2               = +5.866349 * pow( 10.0, -03 );
     FluidList[arrayindex].HT1               = +2.515117 * pow( 10.0, +01 );
     FluidList[arrayindex].HT0               = -7.895474 * pow( 10.0, +03 );
     FluidList[arrayindex].Hn                = -2.415758 * pow( 10.0, +07 );
     
     FluidList[arrayindex].ST5               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST4               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST3               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST2               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST1               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST0               = +0.000000 * pow( 10.0, +0 );

     /////////////////////////////////////////////////////////////////////
     // Fluid Index No. 0021
     /////////////////////////////////////////////////////////////////////
     // Argon
     /////////////////////////////////////////////////////////////////////
     arrayindex                              = (21 - 1);
     strcpy_s(FluidList[arrayindex].name, "Argon");
     strcpy_s(FluidList[arrayindex].name_alt01, "-" );
     strcpy_s(FluidList[arrayindex].chemsym, "Ar");
     FluidList[arrayindex].CASNo[0]          = 7440;
     FluidList[arrayindex].CASNo[1]          = 37;
     FluidList[arrayindex].CASNo[2]          = 1;
     FluidList[arrayindex].Polingetl         = 1;
     FluidList[arrayindex].molewt            = 39.948;
     FluidList[arrayindex].omega             = -0.002;
     FluidList[arrayindex].Vc_SI             = 74.57;
     FluidList[arrayindex].Tc_SI             = 150.86;
     FluidList[arrayindex].Pc_SI             = 48.98;
     FluidList[arrayindex].hfo_SI            = -0.00000 * pow( 10.0, +04.0 );
     FluidList[arrayindex].sfo_SI            = +1.54740 * pow( 10.0, +02.0 );
     FluidList[arrayindex].LHV_SI            = 0.0;
     FluidList[arrayindex].HHV_SI            = 0.0;
     ToUSCS( ( arrayindex + 1 ) );
     FluidList[arrayindex].Tmin_USCS         = 219.67;
     FluidList[arrayindex].Tmax_USCS         = 959.67;
     FluidList[arrayindex].Pmin_USCS         = 1.0;
     FluidList[arrayindex].Pmax_USCS         = 8000.0;
     FluidList[arrayindex].Dipole_SI         = 0.00;
     FluidList[arrayindex].Dipole_USCS       = 0.00;
     bTemp = _bCalc_Bo( arrayindex, &FluidList[arrayindex].Bo);
     bTemp = _bCalc_Ao( arrayindex, &FluidList[arrayindex].Ao);
     bTemp = _bCalc_Co( arrayindex, &FluidList[arrayindex].Co);
     bTemp = _bCalc_Do( arrayindex, &FluidList[arrayindex].Do);
     bTemp = _bCalc_Eo( arrayindex, &FluidList[arrayindex].Eo);
     bTemp = _bCalc_b( arrayindex, &FluidList[arrayindex].b);
     bTemp = _bCalc_a( arrayindex, &FluidList[arrayindex].a);
     bTemp = _bCalc_d( arrayindex, &FluidList[arrayindex].d);
     bTemp = _bCalc_alpha( arrayindex, &FluidList[arrayindex].alpha);
     bTemp = _bCalc_c( arrayindex, &FluidList[arrayindex].c);
     bTemp = _bCalc_gamma( arrayindex, &FluidList[arrayindex].gamma);

     FluidList[arrayindex].A                 = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].B                 = +1.242540 * pow( 10.0, -1 );
     FluidList[arrayindex].C                 = +0.000000 * pow( 10.0, -4 );
     FluidList[arrayindex].D                 = +0.000000 * pow( 10.0, -7 );
     FluidList[arrayindex].E                 = +0.000000 * pow( 10.0, -12 );
     FluidList[arrayindex].F                 = +0.000000 * pow( 10.0, -16 );
     FluidList[arrayindex].G                 = -1.440000 * pow( 10.0, -1 );
     
     FluidList[arrayindex].HT5               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT4               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT3               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT2               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT1               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT0               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].Hn                = +0.000000 * pow( 10.0, +0 );
     
     FluidList[arrayindex].ST5               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST4               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST3               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST2               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST1               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST0               = +0.000000 * pow( 10.0, +0 );

     /////////////////////////////////////////////////////////////////////
     // Fluid Index No. 0022
     /////////////////////////////////////////////////////////////////////
     // Oxygen
     /////////////////////////////////////////////////////////////////////
     arrayindex                              = (22 - 1);
     strcpy_s(FluidList[arrayindex].name, "Oxygen");
     strcpy_s(FluidList[arrayindex].name_alt01, "-" );
     strcpy_s(FluidList[arrayindex].chemsym, "O2");
     FluidList[arrayindex].CASNo[0]          = 1;
     FluidList[arrayindex].CASNo[1]          = 44;
     FluidList[arrayindex].CASNo[2]          = 7782;
     FluidList[arrayindex].Polingetl         = 460;
     FluidList[arrayindex].molewt            = 31.999;
     FluidList[arrayindex].omega             = 0.0196;
     FluidList[arrayindex].Vc_SI             = 73.37;
     FluidList[arrayindex].Tc_SI             = 154.58;
     FluidList[arrayindex].Pc_SI             = 50.43;
     FluidList[arrayindex].hfo_SI            = -0.00000 * pow( 10.0, +04.0 );
     FluidList[arrayindex].sfo_SI            = +2.05040 * pow( 10.0, +02.0 );
     FluidList[arrayindex].LHV_SI            = 0.0;
     FluidList[arrayindex].HHV_SI            = 0.0;
     ToUSCS( ( arrayindex + 1 ) );
     FluidList[arrayindex].Tmin_USCS         = 219.67;
     FluidList[arrayindex].Tmax_USCS         = 959.67;
     FluidList[arrayindex].Pmin_USCS         = 1.0;
     FluidList[arrayindex].Pmax_USCS         = 8000.0;
     FluidList[arrayindex].Dipole_SI         = 0.00;
     FluidList[arrayindex].Dipole_USCS       = 0.00;
     bTemp = _bCalc_Bo( arrayindex, &FluidList[arrayindex].Bo);
     bTemp = _bCalc_Ao( arrayindex, &FluidList[arrayindex].Ao);
     bTemp = _bCalc_Co( arrayindex, &FluidList[arrayindex].Co);
     bTemp = _bCalc_Do( arrayindex, &FluidList[arrayindex].Do);
     bTemp = _bCalc_Eo( arrayindex, &FluidList[arrayindex].Eo);
     bTemp = _bCalc_b( arrayindex, &FluidList[arrayindex].b);
     bTemp = _bCalc_a( arrayindex, &FluidList[arrayindex].a);
     bTemp = _bCalc_d( arrayindex, &FluidList[arrayindex].d);
     bTemp = _bCalc_alpha( arrayindex, &FluidList[arrayindex].alpha);
     bTemp = _bCalc_c( arrayindex, &FluidList[arrayindex].c);
     bTemp = _bCalc_gamma( arrayindex, &FluidList[arrayindex].gamma);

     FluidList[arrayindex].A                 = -9.820000 * pow( 10.0, -1 );
     FluidList[arrayindex].B                 = +2.274900 * pow( 10.0, -1 );
     FluidList[arrayindex].C                 = -3.731000 * pow( 10.0, -5 );
     FluidList[arrayindex].D                 = +4.831000 * pow( 10.0, -8 );
     FluidList[arrayindex].E                 = -1.852430 * pow( 10.0, -11 );
     FluidList[arrayindex].F                 = +2.474880 * pow( 10.0, -15 );
     FluidList[arrayindex].G                 = +1.243000 * pow( 10.0, -1 );
     
     FluidList[arrayindex].HT5               = -2.624492 * pow( 10.0, -14 );
     FluidList[arrayindex].HT4               = +3.850169 * pow( 10.0, -10 );
     FluidList[arrayindex].HT3               = -2.204016 * pow( 10.0, -06 );
     FluidList[arrayindex].HT2               = +7.096107 * pow( 10.0, -03 );
     FluidList[arrayindex].HT1               = +2.569110 * pow( 10.0, +01 );
     FluidList[arrayindex].HT0               = -8.235468 * pow( 10.0, +03 );
     FluidList[arrayindex].Hn                = +0.000000 * pow( 10.0, +0 );
     
     FluidList[arrayindex].ST5               = +3.243312 * pow( 10.0, -16.0 );
     FluidList[arrayindex].ST4               = -4.785661 * pow( 10.0, -12.0 );
     FluidList[arrayindex].ST3               = +2.706854 * pow( 10.0, -08.0 );
     FluidList[arrayindex].ST2               = -7.555182 * pow( 10.0, -05.0 );
     FluidList[arrayindex].ST1               = +1.228538 * pow( 10.0, -01.0 );
     FluidList[arrayindex].ST0               = +1.743873 * pow( 10.0, +02.0 );

     /////////////////////////////////////////////////////////////////////
     // Fluid Index No. 0023
     /////////////////////////////////////////////////////////////////////
     // Sulfur Dioxide
     /////////////////////////////////////////////////////////////////////
     arrayindex                              = (23 - 1);
     strcpy_s(FluidList[arrayindex].name, "Sulfur Dioxide");
     strcpy_s(FluidList[arrayindex].name_alt01, "-" );
     strcpy_s(FluidList[arrayindex].chemsym, "SO2");
     FluidList[arrayindex].CASNo[0]          = 5;
     FluidList[arrayindex].CASNo[1]          = 9;
     FluidList[arrayindex].CASNo[2]          = 7446;
     FluidList[arrayindex].Polingetl         = 461;
     FluidList[arrayindex].molewt            = 64.065;
     FluidList[arrayindex].omega             = 0.2460;
     FluidList[arrayindex].Vc_SI             = 122.00;
     FluidList[arrayindex].Tc_SI             = 430.8;
     FluidList[arrayindex].Pc_SI             = 78.84;
     FluidList[arrayindex].hfo_SI            = -2.96840 * pow( 10.0, +05.0 );
     FluidList[arrayindex].sfo_SI            = +2.48100 * pow( 10.0, +02.0 );
     FluidList[arrayindex].LHV_SI            = 0.0;
     FluidList[arrayindex].HHV_SI            = 0.0;
     ToUSCS( ( arrayindex + 1 ) );
     FluidList[arrayindex].Tmin_USCS         = 219.67;
     FluidList[arrayindex].Tmax_USCS         = 959.67;
     FluidList[arrayindex].Pmin_USCS         = 1.0;
     FluidList[arrayindex].Pmax_USCS         = 8000.0;
     FluidList[arrayindex].Dipole_SI         = 1.63;
     FluidList[arrayindex].Dipole_USCS       = 0.00;
     bTemp = _bCalc_Bo( arrayindex, &FluidList[arrayindex].Bo);
     bTemp = _bCalc_Ao( arrayindex, &FluidList[arrayindex].Ao);
     bTemp = _bCalc_Co( arrayindex, &FluidList[arrayindex].Co);
     bTemp = _bCalc_Do( arrayindex, &FluidList[arrayindex].Do);
     bTemp = _bCalc_Eo( arrayindex, &FluidList[arrayindex].Eo);
     bTemp = _bCalc_b( arrayindex, &FluidList[arrayindex].b);
     bTemp = _bCalc_a( arrayindex, &FluidList[arrayindex].a);
     bTemp = _bCalc_d( arrayindex, &FluidList[arrayindex].d);
     bTemp = _bCalc_alpha( arrayindex, &FluidList[arrayindex].alpha);
     bTemp = _bCalc_c( arrayindex, &FluidList[arrayindex].c);
     bTemp = _bCalc_gamma( arrayindex, &FluidList[arrayindex].gamma);

     FluidList[arrayindex].A                 = +1.394000 * pow( 10.0, +0 );
     FluidList[arrayindex].B                 = +1.102600 * pow( 10.0, -1 );
     FluidList[arrayindex].C                 = +3.302600 * pow( 10.0, -5 );
     FluidList[arrayindex].D                 = +8.910000 * pow( 10.0, -9 );
     FluidList[arrayindex].E                 = -7.731300 * pow( 10.0, -12 );
     FluidList[arrayindex].F                 = +1.292870 * pow( 10.0, -15 );
     FluidList[arrayindex].G                 = +1.948000 * pow( 10.0, -1 );
     
     FluidList[arrayindex].HT5               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT4               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT3               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT2               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT1               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT0               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].Hn                = +0.000000 * pow( 10.0, +0 );
     
     FluidList[arrayindex].ST5               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST4               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST3               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST2               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST1               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST0               = +0.000000 * pow( 10.0, +0 );

     /////////////////////////////////////////////////////////////////////
     // Fluid Index No. 0024
     /////////////////////////////////////////////////////////////////////
     // R-134a
     /////////////////////////////////////////////////////////////////////
     arrayindex                              = (24 - 1);
     strcpy_s(FluidList[arrayindex].name, "1-1-1-2 tetrafluoroethane");
     strcpy_s(FluidList[arrayindex].name_alt01, "R-134a" );
     strcpy_s(FluidList[arrayindex].chemsym, "C2H2F4");
     FluidList[arrayindex].CASNo[0]          = 2;
     FluidList[arrayindex].CASNo[1]          = 97;
     FluidList[arrayindex].CASNo[2]          = 811;
     FluidList[arrayindex].Polingetl         = 49;
     FluidList[arrayindex].molewt            = 102.032;
     FluidList[arrayindex].omega             = 0.326;
     FluidList[arrayindex].Vc_SI             = 200.80;
     FluidList[arrayindex].Tc_SI             = 374.26;
     FluidList[arrayindex].Pc_SI             = 40.59;
     FluidList[arrayindex].hfo_SI            = +0.00000 * pow( 10.0, +04.0 );
     FluidList[arrayindex].sfo_SI            = +0.00000 * pow( 10.0, +02.0 );
     FluidList[arrayindex].LHV_SI            = 0.0;
     FluidList[arrayindex].HHV_SI            = 0.0;
     ToUSCS( ( arrayindex + 1 ) );
     FluidList[arrayindex].Tmin_USCS         = 219.67;
     FluidList[arrayindex].Tmax_USCS         = 959.67;
     FluidList[arrayindex].Pmin_USCS         = 1.0;
     FluidList[arrayindex].Pmax_USCS         = 8000.0;
     FluidList[arrayindex].Dipole_SI         = 0.00;
     FluidList[arrayindex].Dipole_USCS       = 0.00;
     bTemp = _bCalc_Bo( arrayindex, &FluidList[arrayindex].Bo);
     bTemp = _bCalc_Ao( arrayindex, &FluidList[arrayindex].Ao);
     bTemp = _bCalc_Co( arrayindex, &FluidList[arrayindex].Co);
     bTemp = _bCalc_Do( arrayindex, &FluidList[arrayindex].Do);
     bTemp = _bCalc_Eo( arrayindex, &FluidList[arrayindex].Eo);
     bTemp = _bCalc_b( arrayindex, &FluidList[arrayindex].b);
     bTemp = _bCalc_a( arrayindex, &FluidList[arrayindex].a);
     bTemp = _bCalc_d( arrayindex, &FluidList[arrayindex].d);
     bTemp = _bCalc_alpha( arrayindex, &FluidList[arrayindex].alpha);
     bTemp = _bCalc_c( arrayindex, &FluidList[arrayindex].c);
     bTemp = _bCalc_gamma( arrayindex, &FluidList[arrayindex].gamma);

     FluidList[arrayindex].A                 = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].B                 = +1.972760 * pow( 10.0, -2 );
     FluidList[arrayindex].C                 = +2.273220 * pow( 10.0, -4 );
     FluidList[arrayindex].D                 = -8.013600 * pow( 10.0, -8 );
     FluidList[arrayindex].E                 = +1.565413 * pow( 10.0, -11 );
     FluidList[arrayindex].F                 = -1.290392 * pow( 10.0, -15 );
     FluidList[arrayindex].G                 = +1.080000 * pow( 10.0, -1 );
     
     FluidList[arrayindex].HT5               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT4               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT3               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT2               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT1               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT0               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].Hn                = +0.000000 * pow( 10.0, +0 );
     
     FluidList[arrayindex].ST5               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST4               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST3               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST2               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST1               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST0               = +0.000000 * pow( 10.0, +0 );

     /////////////////////////////////////////////////////////////////////
     // Fluid Index No. 0025
     /////////////////////////////////////////////////////////////////////
     // Silane
     /////////////////////////////////////////////////////////////////////
     arrayindex                              = (25 - 1);
     strcpy_s(FluidList[arrayindex].name, "Silane");
     strcpy_s(FluidList[arrayindex].name_alt01, "Silicon Tetrahydride" );
     strcpy_s(FluidList[arrayindex].chemsym, "SiH4");
     FluidList[arrayindex].CASNo[0]          = 7803;
     FluidList[arrayindex].CASNo[1]          = 62;
     FluidList[arrayindex].CASNo[2]          = 5;
     FluidList[arrayindex].Polingetl         = 0;
     FluidList[arrayindex].molewt            = 32.117;
     FluidList[arrayindex].omega             = 0.097;
     FluidList[arrayindex].Vc_SI             = 132.7 / ( FluidList[arrayindex].molewt );
     FluidList[arrayindex].Tc_SI             = 269.7;
     FluidList[arrayindex].Pc_SI             = 48.43;
     FluidList[arrayindex].hfo_SI            = +3.430000 * pow( 10.0, +01.0 );
     FluidList[arrayindex].sfo_SI            = -7.580000 * pow( 10.0, +01.0 );
     FluidList[arrayindex].LHV_SI            = 0.0;
     FluidList[arrayindex].HHV_SI            = 0.0;
     ToUSCS( ( arrayindex + 1 ) );

     FluidList[arrayindex].Tmin_USCS         = 219.67;
     FluidList[arrayindex].Tmax_USCS         = 959.67;
     FluidList[arrayindex].Pmin_USCS         = 1.0;
     FluidList[arrayindex].Pmax_USCS         = 8000.0;
     FluidList[arrayindex].Dipole_SI         = 0.00;
     FluidList[arrayindex].Dipole_USCS       = 0.00;
     bTemp = _bCalc_Bo( arrayindex, &FluidList[arrayindex].Bo);
     bTemp = _bCalc_Ao( arrayindex, &FluidList[arrayindex].Ao);
     bTemp = _bCalc_Co( arrayindex, &FluidList[arrayindex].Co);
     bTemp = _bCalc_Do( arrayindex, &FluidList[arrayindex].Do);
     bTemp = _bCalc_Eo( arrayindex, &FluidList[arrayindex].Eo);
     bTemp = _bCalc_b( arrayindex, &FluidList[arrayindex].b);
     bTemp = _bCalc_a( arrayindex, &FluidList[arrayindex].a);
     bTemp = _bCalc_d( arrayindex, &FluidList[arrayindex].d);
     bTemp = _bCalc_alpha( arrayindex, &FluidList[arrayindex].alpha);
     bTemp = _bCalc_c( arrayindex, &FluidList[arrayindex].c);
     bTemp = _bCalc_gamma( arrayindex, &FluidList[arrayindex].gamma);

     FluidList[arrayindex].A                 = +0.00000000 * pow( 10.0, +1 );
     FluidList[arrayindex].B                 = +2.14783934 * pow( 10.0, -1 );
     FluidList[arrayindex].C                 = +3.62388974 * pow( 10.0, -5 );
     FluidList[arrayindex].D                 = +1.14122900 * pow( 10.0, -7 );
     FluidList[arrayindex].E                 = -4.99767376 * pow( 10.0, -11 );
     FluidList[arrayindex].F                 = +6.55746360 * pow( 10.0, -15 );
     FluidList[arrayindex].G                 = +1.29623632 * pow( 10.0, +00 );
     
     FluidList[arrayindex].HT5               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT4               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT3               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT2               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT1               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].HT0               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].Hn                = +0.000000 * pow( 10.0, +0 );
     
     FluidList[arrayindex].ST5               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST4               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST3               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST2               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST1               = +0.000000 * pow( 10.0, +0 );
     FluidList[arrayindex].ST0               = +0.000000 * pow( 10.0, +0 );
     
}

CEOSMODEL::~CEOSMODEL()
{
  //Free all elements in the message handler linked list
  ClearMessages();
}



/**************************************
 * Method Definition: ClearMessages *
 *------------------------------------***************
 * Empties the message handler            *
 ****************************************************/
bool CEOSMODEL::ClearMessages()
{
  //Free all elements in the message handler linked list
  while( messagelist != NULL)
    RemoveMessage( messagelist );

  //success
  return true;
}

/***********************************
 * Method Definition: GetT_HP_SI *
 *---------------------------------******************
 * Given an enthalpy H and a pressure P this    *
 * function calculates and returns the temperature  *
 * at the state defined by H and P in SI      *
 ****************************************************/
double CEOSMODEL::GetT_HP_SI( double H,
                double P )
{
  //local variables
  double        T;

  //initialize local variables
  T          = 0;

  //convert from USCS to SI
  //convert to SI equivalent
  H          = ( H * units.convert( units.J, units.BTU ) );
  H          = ( H * units.convert( units.lb, units.g ) );
  P          = ( P * units.convert( units.bar, units.PSIA ) );

  //solve for temperature
  T          = GetT_HP_USCS( H, P );
  
  //convert temperature from USCS to SI
  T          = ( T * units.convert( units.R, units.K ) );

  //return the temperature
  return T;
}

/*************************************
 * Method Definition: GetT_HP_USCS *
 *-----------------------------------****************
 * Given an enthalpy H and a pressure P this    *
 * function calculates and returns the temperature  *
 * at the state defined by H and P in USCS      *
 ****************************************************/
double CEOSMODEL::GetT_HP_USCS( double H,
                double P )
{
  
    //Local variables
  double        FirstT2;
  double        SecondT2;
  double        ThirdT2;
  double        FirstH;
  double        SecondH;
  double        ThirdH;
    double        TestValue;
  double        StepValue;
  int          itcount;
  double        slope;
    
  //Initialize local variables
  FirstT2        = 0;
  SecondT2      = 0;
  ThirdT2        = 0;
    FirstH        = 0;
  SecondH        = 0;
  ThirdH        = 0;
  TestValue      = 0;
  StepValue      = 0;
  itcount        = 0;
  slope        = 0;

  //Initialize iteration counter
  itcount        = 1;
    
  //Check data
  if( P < 0 )
  {
    AddMessage( messagelist, 1, 
            "Pressure is below zero in GetT_SP_USCS",
            "Check the pressure value" );
    return -1;
  }

  //Initial Pick for temperature
  StepValue      = 10;
  TestValue      = 1;

  //Establish initial points
  FirstT2        = ( Mixture.Tc_mx * 0.5 );
  SecondT2      = ( FirstT2 + StepValue );

  //We must have succeeded and now we calculate our first
  //guess at s
  FirstH        = GetH_TP_USCS( FirstT2, P );

  //calculate the entropy for the second guess
  SecondH        = GetH_TP_USCS( SecondT2, P );

  //begin loop here
  do
  {
  
    //Calculate the slope of the line that would pass between our
    //first two guess and the answer we are looking for
    slope        = (FirstH - SecondH) / (FirstT2 - SecondT2);  
    
    //and find the intercept
    ThirdT2        = (H - FirstH) / slope;
    ThirdT2        = ThirdT2 + FirstT2;
    
    //and calculate the entropy at temperature ThirdS
    ThirdH        = GetH_TP_USCS( ThirdT2, P );

    //Calculate the difference between the given value
    //and the value we just found
    TestValue      = ( ThirdH - H );

    //decide if we should replace the first or the second
    //guess with the third estimate
    if ( fabs( FirstH - H ) < fabs( SecondH - H ) )
    {
      //First was better, so replace the second
      SecondH      = ThirdH;
      SecondT2    = ThirdT2;
    }
    
    else
    {
      //Second was better, so replace the first
      FirstH      = ThirdH;
      FirstT2      = ThirdT2;
    }

    //increment the itcount
    itcount++;

  }while( !((fabs(TestValue) < Precision)||(itcount > MaxIterations)) );
  
  //check to see what through us out of the loop
  if( itcount > MaxIterations )
    return -1;
    
  //Return the value
    return ThirdT2;
    
}

/***********************************
 * Method Definition: GetT_SP_SI *
 *---------------------------------******************
 * Given an entropy S and a pressure P this      *
 * function calculates and returns the temperature  *
 * at the state defined by S and P in SI      *
 ****************************************************/
double CEOSMODEL::GetT_SP_SI( double S,
                double P )
{
  //local variables
  double        T;

  //initialize local variables
  T          = 0;

  //convert from USCS to SI
  //convert to SI equivalent
  S          = ( S * units.convert( units.J, units.BTU ) );
  S          = ( S * units.convert( units.lb, units.g ) );
  S          = ( S * units.convert( units.R, units.K ) );
  P          = ( P * units.convert( units.bar, units.PSIA ) );

  //solve for temperature
  T          = GetT_SP_USCS( S, P );
  
  //convert temperature from USCS to SI
  T          = ( T * units.convert( units.R, units.K ) );

  //return the temperature
  return T;
}

/////////////////////////////////////////////////////
// Method Definition: GetT_SP_USCS
/////////////////////////////////////////////////////
// Given an entropy S and a pressure P this
// function calculates and returns the temperature
// at the state defined by S and P in USCS
/////////////////////////////////////////////////////
double CEOSMODEL::GetT_SP_USCS( double S,
                double P )
{
  
     //Local variables
     double T1;
     double T2;
     double T3;
     double S1;
     double S2;
     double S3;
     double TestValue;
     double StepValue;
     double slope;
     int itcount;
    
     //Initialize local variables
     T1 = 0;
     T2 = 0;
     T3 = 0;
     S1 = 0;
     S2 = 0;
     S3 = 0;
     TestValue = 0;
     StepValue = 0;
     slope = 0;
     itcount = 1;

     //Check data
     if( S <= 0 )
     {
          AddMessage( messagelist, 1, 
               "Entropy is below zero in GetT_SP_USCS",
               "Check the entropy value" );
          return -1;
     }
     if( P <= 0 )
     {
          AddMessage( messagelist, 1, 
               "Pressure is below zero in GetT_SP_USCS",
               "Check the pressure value" );
          return -1;
     }

     //Initial Pick for temperature
     StepValue = 10;
     TestValue = 1;

     //Establish first point of guess
     T1 = ( Mixture.Tc_mx * 0.6 );

     //mixtures can have non-linear responses when temperatures get low.
     //if the temperature is less than 200 R and this is a mixture,
     //reset the initial temperature to at least 200 R
     if( T1 < 200 && FluidCount > 1 )
          T1 = 200;

     //Calculate the entropy for the initial guess
     S1 = GetS_TP_USCS( T1, P );

     //Establish second point
     T2 = ( T1 + StepValue );
  
     //Calculate the ideal and departure entropy 
     //functions for the second guess
     S2 = GetS_TP_USCS( T2, P );

     //begin loop here
     do
     {
  
          //avoid overly large slopes that can lead to overflow
          //errors
          if( fabs(S1 - S2) < dblGetTrace() )
          {
               return -1;
          }

          //Calculate the slope of the line that would pass between our
          //first two guess and the answer we are looking for
          slope = ( (T1 - T2) / (S1 - S2) );  
    
          //and find the intercept
          T3 = ( ( (S - S1) * slope ) + T1 );
    
          //validate temperature returned by estimate
          if( T3 < 100 )
          {
               T3 = ( Mixture.Tc_mx * 0.5 );
          }

          //calculate ideal and departure functions for the
          //new estimate
          S3 = GetS_TP_USCS( T3, P );

          //Check for a valid response from the method
          //To - do: rework the error trapping on this method.  Because entropy can
          //be negative, passing back -1 doesn't work.  So a perfect 0 is passed back
          //when there is an error.  This should be a try-catch arrangement
          if( S3 == 0 )
          {
               AddMessage( messagelist, 1, 
                    "Solver failed in GetT_SP_USCS",
                    "Check the pressure and entropy values" );
               return -1;
          }

          //Verify that the third value is better than either of the
          //previous values.  If not, add 2% to the largest value and
          //try again
          if( ( fabs( S1 - S ) < ( 0.5 * fabs( S3 - S ) ) ) && 
               ( fabs( S2 - S ) < ( 0.5 * fabs( S3 - S ) ) ) )
          {
               
               //
               T3 = ( ( ( S - S1 ) * slope * 0.75 ) + T1 );
               S3 = GetS_TP_USCS( T3, P );

               //Check for a valid response from the server
               //To do - Add message hanlder calls
               if( S3 < 0 )
               {
                    return -1;
               }
          }
               

          //Calculate the difference between the given value
          //and the value we just found
          TestValue = ( S3 - S );

          //decide if we should replace the first or the second
          //guess with the third estimate
          if ( fabs( S1 - S ) < fabs( S2 - S ) )
          {
               //First was better, so replace the second
               S2 = S3;
               T2 = T3;
          }
          else
          {
               //Second was better, so replace the first
               S1 = S3;
               T1 = T3;
          }

          //increment the	itcount
          itcount++;

     }while(	!((fabs(TestValue) < Precision)||(itcount > MaxIterations))	);

     //check	to see what through	us out of	the loop
     //To Do - Add message handler
     if( itcount > MaxIterations	)
     {
          return -1;
     }

     //Return the	value
     return T3;

}

/////////////////////////////////////////////////////
// Method Definition: GetT_VP_SI
/////////////////////////////////////////////////////
// Given a specific volume V and a pressure P this
// function calculates and returns the temperature
// at the state defined by V and P in SI
/////////////////////////////////////////////////////
double CEOSMODEL::GetT_VP_SI( double V,
                double P )
{

     //local variables
     double        T;

     //initialize local variables
     T          = 0;

     //convert from USCS to SI
     //convert to SI equivalent
     V          = ( V * units.convert( units.cm3, units.ft3 ) );
     V          = ( V * units.convert( units.lb, units.g ) );
     P          = ( P * units.convert( units.bar, units.PSIA ) );

     //solve for temperature
     T          = GetT_VP_USCS( V, P );
  
     //convert temperature from USCS to SI
     T          = ( T * units.convert( units.R, units.K ) );

     //return the temperature
     return T;
}

/////////////////////////////////////////////////////
// Method Definition: GetT_VP_USCS
/////////////////////////////////////////////////////
// Given a specific volume V and a pressure P this
// function calculates and returns the temperature
// at the state defined by V and P in USCS
/////////////////////////////////////////////////////
double CEOSMODEL::GetT_VP_USCS( double V,
                double P )
{

     //Local variables
     double T1;
     double T2;
     double T3;
     double P1;
     double P2;
     double P3;
     double TestValue;
     double StepValue;
     int itcount;
     double slope;
    
     //Initialize local variables
     T1 = 0;
     T2 = 0;
     T3 = 0;
     P1 = 0;
     P2 = 0;
     P3 = 0;
     TestValue = 0;
     StepValue = 0;
     itcount = 0;
     slope = 0;

     return -1;
}

/*************************************
 * Method Definition: GetSIdeal_USCS *
 *-----------------------------------****************
 * Calculate the ideal entropy for single fluid.    *
 * Entropy of an ideal gas only depends upon        *
 * temperature for a single fluid.                  *
 ****************************************************/
double CEOSMODEL::GetSIdeal_USCS( double T )
{
   //local variables
   double      Term1;
   double      Term2;
   double      Term3;
   double      Term4;
   double      Term5;
   double      Term6;

   //Initialize local variables
   Term1      = 0;
   Term2      = 0;
   Term3      = 0;
   Term4      = 0;
   Term5      = 0;
   Term6      = 0;
   
  //For the case of a single fluid, we can ignore the pressure
  //correction term
  if( !uniary )
  {
    AddMessage( messagelist, 1, 
      "Ideal gas entropy as a function of temperature alone is not valid for a mixture",
      "" );
    return -1;
  }

    //This function calculates the ideal s for a fluid
    Term1      = ( Mixture.B_mx * log( T ) );
    Term2      = ( 2 * Mixture.C_mx * T );
    Term3      = ( 1.5 * Mixture.D_mx * pow( T, 2 ) );
    Term4      = ( 1.3333 * Mixture.E_mx * pow( T, 3 ) );
    Term5      = ( 1.25 * Mixture.F_mx * pow( T, 4 ) );
    Term6      = ( Mixture.G_mx );

    //return the value
  return ( Term1 + Term2 + Term3 + Term4 + Term5 + Term6 );
    
 }

/*************************************
 * Method Definition: GetSIdeal_USCS *
 *-----------------------------------****************
 * Calculate the ideal entropy for mixture.         *
 ****************************************************/
double CEOSMODEL::GetSIdeal_USCS( double T, double P )
{
     //local variables
     double              Term1;
     double              Term2;
     double              Term3;
     double              Term4;
     double              Term5;
     double              Term6;
     double              Term7;
     double              pi;
     int                 i;

     //Initialize local variables
     Term1               = 0;
     Term2               = 0;
     Term3               = 0;
     Term4               = 0;
     Term5               = 0;
     Term6               = 0;
     Term7               = 0;
     pi                  = 0;
     i                   = 0;
   
    //This function calculates the ideal s for a fluid
    Term1                = ( Mixture.B_mx * log( T ) );
    Term2                = ( 2 * Mixture.C_mx * T );
    Term3                = ( 1.5 * Mixture.D_mx * pow( T, 2 ) );
    Term4                = ( 1.3333 * Mixture.E_mx * pow( T, 3 ) );
    Term5                = ( 1.25 * Mixture.F_mx * pow( T, 4 ) );
    Term6                = ( Mixture.G_mx );

  //and the pressure term
  for( i=0; i<FluidCount; i++ )
  {
    pi      = ( FluidList[i].percentmw * P );
          if( pi > 0 )
          {
         Term7    = ( Term7 + ( FluidList[i].percentmw * (log( ( pi / P ) ) ) ) );
  
          }
     }
  Term7      = ( -1 * r_USCS * Term7 );
  
  //return the value
  return ( Term1 + Term2 + Term3 + Term4 + Term5 + Term6 + Term7 );
    
 }

/********************************
 * Method Definition : Getso_SI *
 *------------------------------**********************
 * This function returns the entropy for an ideal    *
 * gas relative to the enthalpy at 298k.  Based on   *
 * the JANAF tables                                  *
 ****************************************************/
double CEOSMODEL::Getso_SI( double T )
{

  //local variables
  double      Term1;
  double      Term2;
  double      Term3;
  double      Term4;
  double      Term5;
  double      Term6;

  //Initialize local variables
  Term1      = 0;
  Term2      = 0;
  Term3      = 0;
  Term4      = 0;
  Term5      = 0;
  Term6      = 0;
   
  //For the case of a single mixture, we can ignore the pressure
  //correction term
  if( !uniary )
  {
    AddMessage( messagelist, 1, 
      "Molar ideal gas entropy as a function of temperature is not valid for a mixture",
      "" );
    return -1;
  }

    //This function calculates the entropy (s) for an ideal gas
    Term1      = ( Mixture.ST0_mx );
    Term2      = ( Mixture.ST1_mx * T );
    Term3      = ( Mixture.ST2_mx * pow( T, 2 ) );
    Term4      = ( Mixture.ST3_mx * pow( T, 3 ) );
    Term5      = ( Mixture.ST4_mx * pow( T, 4 ) );
    Term6      = ( Mixture.ST5_mx * pow( T, 5 ) );

    //return the value
  return ( Term1 + Term2 + Term3 + Term4 + Term5 + Term6 );
    
}

/************************************
 * Method Definition : Getso_USCS *
 *----------------------------------******************
 * This function returns the enthalpy for an ideal   *
 * gas relative to the enthalpy at 298k.  Based on   *
 * the JANAF tables  in USCS               *
 ****************************************************/
double CEOSMODEL::Getso_USCS( double T )
{

  //local variables
  double      s;
  double      T_SI;

  //Initialize local variables
  s        = 0;
  T_SI      = 0;   

  //Convert the temperature
  T_SI      = T * units.convert( units.K, units. R );

  //Get the enthalpy
  s        = Getso_SI( T_SI );
    
    //Convert from SI to USCS
  s        = ( s * units.convert( units.J, units.BTU ) );
  s        = ( s * units.convert( units.lb, units.g ) );
  s        = ( s * units.convert( units.R, units.K ) );

  //return the value
  return s;
    
}

/////////////////////////////////////////////////////////////////////
// Method Definition:  Object interface, return the value of
// dblTrace
/////////////////////////////////////////////////////////////////////
double CEOSMODEL::dblGetTrace()
{
    return _dblTrace;
}

/////////////////////////////////////////////////////////////////////
// Method Definition: This method solves for the vapor pressure of
// the mixture. 
/////////////////////////////////////////////////////////////////////
double CEOSMODEL::GetV_TP_SI( double T, double P )
{
  //local variables
  double        MD;
  double        V;

  //initialize local variables
  MD          = 0;
  V          = 0;

  //Convert the SI values to their USCS equivalents
  T          = ( T * units.convert( units.K, units.R ) );
  P          = ( P * units.convert( units.bar, units.PSIA ) );

  //call the solver
  V          = GetV_TP_USCS( T, P );

  //convert from BTU to kj
  V          = ( V * units.convert( units.ft3, units.cm3 ) );
  V          = ( V * units.convert( units.g, units.lb ) );
  
  //all done
  return V;

}

 /**********************************
 * Method Definition: GetV_TP_USCS *
 *---------------------------------*******************
 * This function solves for the vapor pressure of    *
 * the mixture.                                      *
 ****************************************************/
double CEOSMODEL::GetV_TP_USCS( double T, double P )
{
  //local variables
  double MD;
  double V;

  //initialize local variables
  MD = 0;
  V = 0;

  //validate the temperature and pressure before proceeding
  if( T < 10.0 )
  {
    AddMessage( messagelist, 1, 
            "Temperature in GetV_TP_USCS less than 10 R",
            "Check the fluid temperature and pressure" );
    return -1;
  }

  if( P < dblGetTrace() )
  {
    AddMessage( messagelist, 1, 
            "Pressure in GetV_TP_USCS less than 1E-15 psia",
            "Check the fluid temperature and pressure" );
    return -1;
  }

  //Call the solver
  MD = GetMolarDensity_TP_USCS( T, P );
  
  //check that we won't divide by zero or something very small.
  if( MD < dblGetTrace() )
  {
    AddMessage( messagelist, 1, 
            "GetV_TP_USCS failed to converge",
            "Check the fluid temperature and pressure" );
    return 0;
  }

  //invert for the volume
  V = ( 1 / MD );

  //check that the molecular weight is not very small
  if( fabs(Mixture.molewt_mx) < dblGetTrace() )
  {
    return -1;
  }

  //convert from lb-mols to lbm
  V = ( V / Mixture.molewt_mx );
  
  //all done
  return V;

}

 /************************************************
 * Method Definition: GetVaporTemperature_P_SI *
 *-----------------------------------------------*****
 * This function solves for the vapor temperature of *
 * a single fluid as a function of P.  It does not   *
 * apply to mixtures.                 *
 ****************************************************/
double CEOSMODEL::GetVaporTemperature_P_SI( double P )
{
  //local variables
  double        T;

  //initialize local variables
  T          = 0;

  //convert from SI to USCS
  P          = ( P * units.convert( units.bar, units.PSIA ) );

  //call solver
  T          = GetVaporTemperature_P_USCS( P );
  
  //convert to SI
  T          = ( T * units.convert( units.R, units.K ) );

  //return the value
  return T;
}

 /**************************************************
 * Method Definition: GetVaporTemperature_P_USCS *
 *-------------------------------------------------***
 * This function solves for the vapor temperature of *
 * a single fluid as a function of P.  It does not   *
 * apply to mixtures.                 *
 ****************************************************/
double CEOSMODEL::GetVaporTemperature_P_USCS( double P )
{
  //local variables
  double      MDl;
  double      MDv;

  //Initialize local variables
  MDl        = 0;
  MDv        = 0;

  //Return Value
  return GetVaporTemperature_P_USCS( P, &MDl, &MDv );

}

/**************************************************
* Method Definition: GetVaporTemperature_P_USCS   *
*-------------------------------------------------***
* This function solves for the vapor temperature of *
* a single fluid as a function of P.  It does not   *
* apply to mixtures.  This overloaded function      *
* returns the molar densities, so they can be used  *
* in other calculations                             *
****************************************************/
double CEOSMODEL::GetVaporTemperature_P_USCS( double P, 
                        double *MDl, 
                        double *MDv )
{
     //local variables
     double         TsFirst;
     double         TsSecond;
     double         TsThird;
     double         PFirst;
     double         PSecond;
     double         PThird;
     double         TestValue;
     double         slope;
     int            IterCount;

  //Initialize local variables
  //At equilibrium, the vapor pressure (Pv) must
  //equal the liquid pressure (Pl) ie, Ps = Pl = Pv
  //and the liquid and vapor fugacities have to be
  //equal Fl=Fv
  TsFirst      = 0;
  TsSecond    = 0;
  TsThird      = 0;
  PFirst      = 0;
  PSecond      = 0;
  PThird      = 0;
  TestValue    = 0;
  slope      = 0;
  IterCount    = 1;

  //it really doesn't make sense to get the vapor pressure of
  //a mixture.  We keep the variable uniary just for this
  //ocassion
  if( !uniary )
  {
    AddMessage( messagelist, 1, 
      "Vapor pressure calculatation is not valid for a mixture",
      "" );
    return -1;
  }

  //The temperature has to be lower than the critcal temeprature
  if( P > Mixture.Pc_mx )
  {
    AddMessage( messagelist, 1, 
      "Vapor pressure input pressure is greater than the critical pressure",
      "" );
    return -1;
  }

  //Initialize the temperature at a very low value.
  TsFirst      = ( Mixture.Tc_mx * 0.30 );
  PFirst      = GetVaporPressure_T_USCS( TsFirst, MDl, MDv );

  //if that value did not succeed, warm it up just a little
  if( PFirst <= 0 )
  {
    TsFirst      = ( Mixture.Tc_mx * 0.50 );
    PFirst      = GetVaporPressure_T_USCS( TsFirst, MDl, MDv );
  }

  TsSecond    = ( TsFirst + 20 );
  PSecond      = GetVaporPressure_T_USCS( TsSecond, MDl, MDv );

  //Begin the iteration loop
  do
  {
    
    //the vapor curve initially starts very flat, so if the
    //estimate is too low, we'll shoot right past the
    //critical temperature and get knocked out of the loop.
    do
    {
      //calculate the slope between the two points
      slope      = ( ( PFirst - PSecond ) / ( TsFirst - TsSecond ) );

      //Pretend the function is linear and use the slope
      //intercept equation to estimate the third pressure
      TsThird      = ( ( P - PSecond ) / slope );
      TsThird      = ( TsThird + TsSecond );

      //If we have to loop again, update the variables
      if( TsThird > Mixture.Tc_mx )
      {
        TsFirst      = ( TsFirst * 1.10 );
        PFirst      = GetVaporPressure_T_USCS( TsFirst );

        TsSecond    = ( TsFirst + 20 );
        PSecond      = GetVaporPressure_T_USCS( TsSecond );
      }

    }while( TsThird > Mixture.Tc_mx );

    //Calculate the estimated pressure
    PThird      = GetVaporPressure_T_USCS( TsThird, MDl, MDv );

    //Calculate the new testvalue
    TestValue    = fabs( PThird - P );

    //See which value is closest
    if ( fabs( PFirst - P ) < fabs( PSecond - P ) )
    {
      //First was better so replace second
      PSecond    = PThird;
      TsSecond  = TsThird;
    }
    else
    {
      //Second was better so replace first
      PFirst    = PThird;
      TsFirst    = TsThird;
    }

    //Increment the iteration counter
    IterCount    = IterCount++;
  }
  while ( ( IterCount < MaxIterations ) && ( TestValue > Precision ) );

  //Check to see what knocked us out of the loop
  if( IterCount > MaxIterations )
  {
    AddMessage( messagelist, 1, 
      "Vapor Pressure (P) failed to converge - Check Pressure",
      "" );
    return -1;
  }

  //Return Value
  return TsThird;

}


/////////////////////////////////////////////////////
// Method Definition: GetViscocity_USCS   *
/////////////////////////////////////////////////////
// This function calculates viscocity at a given     *
// pressure and temperature in USCS                  *
/////////////////////////////////////////////////////
double CEOSMODEL::GetViscosityGas_TP_USCS( double T, double P) 
{
    //local variables
    double T_SI,P_SI,V;

    //Convert the USCS values to their SI equivalents
    T_SI = ( T * units.convert( units.R, units.K ) );
    P_SI = ( P * units.convert( units.PSIA, units.bar ) );

    //call the solver
    V = GetViscosityGas_TP_SI( T_SI, P_SI );

    //convert from microPoise to Poise
    V = (V / 1000000);

    //convert from Poise (g/cm*s) to USCS (lb/ft*s)
    V = ( V * units.convert( units.g, units.lb ) );
    V = ( V * units.convert( units.ft, units.cm ) );

    return V;
}
/***************************************
* Method Definition: GetViscocity_SI   *
*--------------------------------------**************
* This function calculates viscocity at a given     *
* pressure and temperature in SI                    *
* To do:                                            *
*  - Optimize loop using arrays to avoid repeating  *
*    calculations                                   *
*  - Add association factor                         *
****************************************************/
double CEOSMODEL::GetViscosityGas_TP_SI( double T, double P) 
{
     //local variables
     int            i;
     int            j;
     double         DimDipole;
     double         HardRadius;
     double         Epsilon;
     double         MoleWeight;
     double         OmegaMixture;
     double         DipoleMixture;
     double         AsFacMixture;
     double         MolDens;
     double         y;
     double         a[10];
     double         b[10];
     double         c[10];
     double         d[10];
     double         G1;
     double         G2;
     double         Visc2Corr;
     double         Visc1Corr;
     double         Tstar;
     double         CollInt;
     double         Vcm;
     double         Tcm;
     double         Mrm;
     double         Fcm;
     double         temp;
     double         temp1;
     double         temp2;
     double         temp3;
     double         temp4;
     double         E[10];

     //initialize local variables
     i              = 0;
     j              = 0;
     DimDipole      = 0;
     HardRadius     = 0;
     Epsilon        = 0;
     MoleWeight     = 0;
     OmegaMixture   = 0;
     DipoleMixture  = 0;
     AsFacMixture   = 0;
     MolDens        = 0;
     y              = 0;

     a[0]           = 6.324;
     b[0]           = 50.412;
     c[0]           = -51.680;
     d[0]           = 1189.0;

     a[1]           = 0.00121;
     b[1]           = -0.001154;
     c[1]           = -0.006257;
     d[1]           = 0.03728;

     a[2]           = 5.283;
     b[2]           = 254.209;
     c[2]           = -168.48;
     d[2]           = 3898.0;

     a[3]           = 6.623;
     b[3]           = 38.096;
     c[3]           = -8.464;
     d[3]           = 31.42;

     a[4]           = 19.745;
     b[4]           = 7.630;
     c[4]           = -14.354;
     d[4]           = 31.53;

     a[5]           = -1.900;
     b[5]           = -12.537;
     c[5]           = 4.985;
     d[5]           = -18.15;

     a[6]           = 24.275;
     b[6]           = 3.450;
     c[6]           = -11.291;
     d[6]           = 69.35;

     a[7]           = 0.7972;
     b[7]           = 1.117;
     c[7]           = 0.01235;
     d[7]           = -4.117;

     a[8]           = -0.2382;
     b[8]           = 0.06770;
     c[8]           = -0.8163;
     d[8]           = 4.025;

     a[9]           = 0.06863;
     b[9]           = 0.3479;
     c[9]           = 0.5926;
     d[9]           = -0.727;

     G1             = 0;
     G2             = 0;
     Visc2Corr      = 0;
     Visc1Corr      = 0;     
     Tstar          = 0;
     CollInt        = 0;
     Vcm            = 0;
     Tcm            = 0;
     Mrm            = 0;
     Fcm            = 0;
     temp           = 0;
     temp1          = 0;
     temp2          = 0;
     temp3          = 0;
     temp4          = 0;
     for(i=0;i<10;i++)
          E[i] = 0;

     //begin calculation of mixture properties
     for(i=0;i<FluidCount;i++)
     {
          for(j=0;j<FluidCount;j++)
          {
               temp = ( FluidList[i].percentmw * FluidList[j].percentmw );
               temp1 = ( 0.809 * pow( FluidList[i].Vc_SI, 0.3333333333333 ) );
               temp2 = ( 0.809 * pow( FluidList[j].Vc_SI, 0.3333333333333 ) );
               temp4 = sqrt( temp1 * temp2 );
               temp3 = ( temp4 * temp4 * temp4 );
               HardRadius = ( HardRadius + ( temp * temp3 ) );

               temp1 = ( 0.794090955484 * sqrt( FluidList[i].Tc_SI * FluidList[j].Tc_SI ) );
               Epsilon = ( Epsilon + ( temp * temp1 * temp3 ) );

               temp2 = ( 2 * FluidList[i].molewt * FluidList[j].molewt ) / ( FluidList[i].molewt + FluidList[j].molewt );
               MoleWeight = MoleWeight + ( temp * temp1 * temp4 * temp4 * sqrt( temp2 ) );

               temp2 = ( ( FluidList[i].omega + FluidList[j].omega ) / 2 );
               OmegaMixture = ( OmegaMixture + ( temp * temp2 * temp3 ) );

               temp2 = ( FluidList[i].Dipole_SI * FluidList[i].Dipole_SI * FluidList[j].Dipole_SI * FluidList[j].Dipole_SI );
               DipoleMixture = ( DipoleMixture + ( ( temp * temp2 ) / temp3 ) );

               //the association factor mixture calculation will go here
               
          }
     }
     
     //complete mixing calculations
     Epsilon = ( Epsilon / HardRadius );
     temp = ( Epsilon * pow( HardRadius, 0.6666666667 ) );
     MoleWeight = ( MoleWeight / temp );
     MoleWeight = ( MoleWeight * MoleWeight );
     OmegaMixture = ( OmegaMixture / HardRadius );
     DipoleMixture = ( DipoleMixture * HardRadius );


     //Calculate viscosity
     DipoleMixture = pow( DipoleMixture, 0.25 );

     //calculate psuedo critical values
     Vcm = ( HardRadius / 0.529475129 );
     Tcm = ( 1.2593 * Epsilon );
     Mrm = ( ( 131.3 * DipoleMixture ) / ( sqrt( Vcm * Tcm ) ) );
     Fcm = ( 1 + ( -0.275 * OmegaMixture ) + ( 0.059035 * Mrm * Mrm * Mrm * Mrm ) + AsFacMixture );
     
     //add association here
     Tstar = ( T / Epsilon );
     CollInt = ( 1.16145 * pow( Tstar, -0.14874 ) );
     CollInt = ( CollInt + ( 0.52487 * exp( -0.77320 * Tstar ) ) );
     CollInt = ( CollInt + ( 2.16178 * exp( -2.43787 * Tstar ) ) );

     //calculate molar density
     MolDens = GetMolarDensity_TP_SI(T, P);

     //calculate y
     y = ( ( MolDens * Vcm ) / 6 );

     //calculate G1
     G1 = ( ( 1 - ( 0.5 * y ) ) / ( pow( ( 1- y ), 3 ) ) );

     //loop through and calculate the correction factor coefficients
     for(i=0;i<10;i++)
     {
          E[i] = a[i] + 
               ( b[i] * OmegaMixture ) +
               ( c[i] * Mrm * Mrm * Mrm * Mrm ) + 
               ( d[i] * AsFacMixture );
     }

     //calculate G2
     temp1 = ( ( ( 1 - exp( -1 * E[3] * y ) ) / y ) * E[0] );
     temp2 = ( E[1] * G1 * exp( E[4] * y ) );
     G2 = ( ( temp1 + temp2 + ( E[2] * G1 ) ) / ( ( E[0] * E[3] ) + E[1] + E[2] ) );

     //G2 is a divisor and can't be zero
     if( G2 == 0 )
     {
          return -1;
     }

     //calculate second viscosity correction factor
     Visc2Corr = ( E[6] * y * y * G2 * exp( E[7] + ( E[8]/Tstar ) + ( E[9] / ( Tstar * Tstar ) ) ) );
     
     //calculate the first viscosity correction factor
     Visc1Corr = ( ( ( sqrt( Tstar ) / CollInt ) * Fcm * ( ( 1.0 / G2 ) + ( E[5] * y ) ) ) + Visc2Corr );

     return ( ( Visc1Corr * 36.344 * sqrt( MoleWeight * Tcm ) ) / pow( Vcm, 0.66666666666667 ) );
}

////////////////////////////////////////////////////////////
// Method Definition: GetVaporPressure_T_SI 
////////////////////////////////////////////////////////////
//
// This function solves for the vapor pressure of
// a single fluid as a function of T in SI units.  It does not
// apply to mixtures.
//
////////////////////////////////////////////////////////////
double CEOSMODEL::GetVaporPressure_T_SI( double T )
{
  //local variables
  double       MDl;
  double       MDv;

  //Initialize local variables
  MDl          = 0;
  MDv          = 0;

  return GetVaporPressure_T_SI( T, &MDl, &MDv );

}

////////////////////////////////////////////////////////////
// Method Definition: GetVaporPressure_T_SI 
////////////////////////////////////////////////////////////
//
// This function solves for the vapor pressure of
// a single fluid as a function of T in SI units.  
// arguements include initial molar volume of the vapor and
// liquid.  It does not apply to mixtures.
//
////////////////////////////////////////////////////////////
double CEOSMODEL::GetVaporPressure_T_SI( double T,
                      double *MDl,
                      double *MDv )
{

     double    Press_USCS;
     double    MDl_SI;
     double    MDv_SI;

     
     //convert from SI to USCS   
     MDl_SI = ( *MDl * units.convert( units.cm3, units.ft3 ) );
     MDl_SI = ( *MDl * units.convert( units.lb, units.g ) );
     MDv_SI = ( *MDv * units.convert( units.cm3, units.ft3 ) );
     MDv_SI = ( *MDv * units.convert( units.lb, units.g ) );
     
     //call the solver
     Press_USCS = GetVaporPressure_T_USCS( T * units.convert( units.K, units.R),
                                             &MDl_SI, &MDv_SI );

     if( Press_USCS <= 0 )
          return -1;
     
     return (units.convert ( units.PSIA, units.bar ) * Press_USCS );

}

////////////////////////////////////////////////////////////
// Method Definition: GetVaporPressure_T_USCS
////////////////////////////////////////////////////////////
//
// This function solves for the vapor pressure of
// a single fluid as a function of T.  It does not
// apply to mixtures. 
// In this overloaded case, the return values can be 
// used to calculate entropy or enthalpy directly.
//
////////////////////////////////////////////////////////////
double CEOSMODEL::GetVaporPressure_T_USCS( double T,
                      double *MDl,
                      double *MDv )
{
  //local variables
  double      Fl;
  double      Fv;
  double      TestValue;
  double      Ps;
  double      Pl;
  double      Pv;
  double      Vs;
  double      Term1;
  int        IterCount;

  //Initialize local variables
  //At equilibrium, the vapor pressure (Pv) must
  //equal the liquid pressure (Pl) ie, Ps = Pl = Pv
  //and the liquid and vapor fugacities have to be
  //equal Fl=Fv
  Fl           = 0;
  Fv           = 0;
  TestValue    = 0;
  Ps           = 0;
  Pl           = 0;
  Pv           = 0;
  Vs           = 0;
  Term1        = 0;
  IterCount    = 1;

  //it really doesn't make sense to get the vapor pressure of
  //a mixture.  We keep the variable uniary just for this
  //ocassion
  if( !uniary )
  {
    AddMessage( messagelist, 1, 
      "Vapor pressure calculatation is not valid for a mixture",
      "" );
    return -1;
  }

  //The temperature has to be lower than the critcal temeprature
  if( T > Mixture.Tc_mx )
  {
    AddMessage( messagelist, 1, 
      "Vapor pressure input temperature is greater than the critical temperature",
      "" );
    return -1;
  }

  //We begin be estimating the volume at saturation (Vs)
  Vs        = ( 0.29056 - ( 0.08775 * Mixture.omega_mx ) );
  Term1     = pow( ( 1 - ( T / Mixture.Tc_mx ) ), 0.285714285714 );
  Vs        = ( pow( Vs,  Term1) );
  Vs        = ( Mixture.Vc_mx * Vs );

  //To be sure we don't overshoot in the solver, we'll back off the
  //molar density (increase specific volume) just a little bit.  As
  //we get closer to the critical temperature the increase in Vs needs
  //to be greater
  Vs        = ( Vs * ( 1.015 + ( T / Mixture.Tc_mx ) ) );

  //And since the units of Vs are ft/lbmol, we can invert to get
  //molar density and turn to the solver for the estimated pressure
  Ps        = EOS( T, ( 1 / Vs ) );

  //At low pressures and low temperatures, the curve can be almost
  //vertical at this point.  The small error in the estimation could
  //cause us to overshoot into negative pressures.  If we did, just
  //throttle back to 1 psia
  if( Ps <= 0 )
    Ps      = 1;
  
  //Begin the iteration loop
  do
  {
    
    //Solve for the Molar Densities
    *MDl      = GetFalsePositionMolarDensity( T, Ps, 10.0 );
    if( *MDl <= 0 )
    {
      AddMessage( messagelist, 50, 
        "Molar Density (liquid) failed to converge in GetVaporPressure_T_USCS",
        "" );
      return -1;
    }

    //we choose a low density (high specific volume) to start with.
    //if we don't converge, try a little bit bigger density (smaller
    //specific volume) and see if we can get there.  Since the pressure
    //is the result of an estimation, we could end up getting the same
    //value for MDv and MDf and we'll need to bring the pressure down
    //(To prove this, plot the data on a P-v plot.  If you're ideal
    //saturation pressure is the one you're shooting for, but you guess
    //a little high on pressure you end up in the liquid region.)
    do
    {
      *MDv      = GetFalsePositionMolarDensity( T, Ps, 0.001 );
      if( *MDv <= 0 )
      {
        AddMessage( messagelist, 50, 
          "Molar Density (vapor) failed to converge in GetVaporPressure",
          "" );
      }

      //We may have back off the pressure
      if( *MDv < 0 )
        Ps  = Ps * 0.98;

    }while( *MDv < 0 );

    //Find the fugacity of liquid
    Fl        = EOSFugacity( T, *MDl );
    if( Fl <= ( 1 * pow(10.0, -20.0) ) )
    {
      AddMessage( messagelist, 50, 
        "Fugacity (vapor) failed to converge",
        "" );
      return -1;
    }


    //Find the fugacity of vapor
    Fv        = EOSFugacity(T, *MDv );
    if( Fv <= ( 1 * pow(10.0, -20.0 ) ) )
    {
      AddMessage( messagelist, 1, 
        "Fugacity (vapor) failed to converge",
        "" );
      return -1;
    }

    //Calculate the new testvalue
    TestValue    = fabs( 1 - ( Fl / Fv ) );

    //Estimate the new vapor pressure
    Ps        = ( Ps * ( Fl / Fv ) );
    
    //Increment the iteration counter
    IterCount    = IterCount++;
  }
  while ( ( IterCount < MaxIterations ) && ( TestValue > Precision ) );

  //Check to see what knocked us out of the loop
  if( IterCount > MaxIterations )
  {
    AddMessage( messagelist, 1, 
      "Vapor Pressure failed to converge - Check Temperature",
      "" );
    return -1;
  }


  //Return Value
  return Ps;

}

////////////////////////////////////////////////////////////
// Method Definition: GetVaporPressure_T_USCS 
////////////////////////////////////////////////////////////
//
// This function solves for the vapor pressure of
// a single fluid as a function of T.  It does not
// apply to mixtures.
//
////////////////////////////////////////////////////////////
double CEOSMODEL::GetVaporPressure_T_USCS( double T )
{
  //local variables
  double       MDl;
  double       MDv;

  //Initialize local variables
  MDl          = 0;
  MDv          = 0;

  return GetVaporPressure_T_USCS( T, &MDl, &MDv );

}

/**********************************
 * Method Definition : AddMessage *
 *--------------------------------*******************
 * This method adds a message to the message        *
 * handler.                                         *
 ****************************************************/
bool CEOSMODEL::AddMessage( class CMeshan *element,
                    int priority,
                    char *maintext,
                    char *auxtext)
{
  //local variables
  class      CMeshan *currentElement;

  //initialize local variables
  currentElement  = AddMessage( element );

  //check that all went well
  if( currentElement == NULL )
    return false;
  
  //input the information
  currentElement->SetPriority( priority );
  currentElement->SetMainText( maintext );
  currentElement->SetAuxText( auxtext );

  //success
  return true;
}

/************************************
 * Method Definition : AddMessage *
 *-------------------------------------***************
 * This function adds a message to the message     *
 * handler.                       *
 ****************************************************/
class CMeshan *CEOSMODEL::AddMessage( class CMeshan *element )
{
  //local variables
  class      CMeshan *currentElement;

  //initialize local variables
  currentElement  = NULL;

  // Is this list empty? If so create first element
  if( messagelist == NULL )
  {
    messagelist = new CMeshan;
    
    return messagelist;
  }

  // Find the end of the linked list by starting at the head
  // and look for the first NULL element.  Then attach a new element.
  currentElement = messagelist;

  // There is still another element after this one?  
  // Keep going while there is
  while(currentElement->next != NULL)
    currentElement = currentElement->next;

  // Now we know currentElement->next points to NULL.  
  // currentElement must be
  // the last member...Let's add a new one on the end!
  currentElement->next = new CMeshan;

  //pass the newly created class back to
  //the calling structure
  return currentElement->next;
}

//////////////////////////////////////////////////////
// Method Definition : GetAo_USCS 
//--------------------------------------------------//
// This method returns the Ao constant 
// in USCS terms for a single component.
//////////////////////////////////////////////////////
double CEOSMODEL::GetAo_USCS( int fluidindex )
{
  //Check for valid data
  if( !ValidityCheckFluid ( fluidindex ) )
  {
    AddMessage( messagelist, 1, 
      "GetAo_USCS ValidityCheckFluid failed",
      "" );
    return NULL;
  }

  return FluidList[( fluidindex - 1 )].Ao;
}

//////////////////////////////////////////////////////
// Method Definition : GetAo_mx_USCS 
//--------------------------------------------------//
// This method returns the Ao constant 
// in USCS terms for a mixture.
//////////////////////////////////////////////////////
double CEOSMODEL::GetAo_mx_USCS()
{

  return Mixture.Ao_mx;

}

//////////////////////////////////////////////////////
// Method Definition : GetBo_USCS 
//--------------------------------------------------//
// This method returns the Bo constant 
// in USCS terms for a single component.
//////////////////////////////////////////////////////
double CEOSMODEL::GetBo_USCS( int fluidindex )
{
  //Check for valid data
  if( !ValidityCheckFluid ( fluidindex ) )
  {
    AddMessage( messagelist, 1, 
      "GetBo_USCS ValidityCheckFluid failed",
      "" );
    return NULL;
  }

  return FluidList[( fluidindex - 1 )].Bo;
}

//////////////////////////////////////////////////////
// Method Definition : GetBo_mx_USCS 
//--------------------------------------------------//
// This method returns the Bo constant 
// in USCS terms for a mixture.
//////////////////////////////////////////////////////
double CEOSMODEL::GetBo_mx_USCS()
{

  return Mixture.Bo_mx;

}

//////////////////////////////////////////////////////
// Method Definition : GetCo_USCS 
//--------------------------------------------------//
// This method returns the Co constant 
// in USCS terms for a single component.
//////////////////////////////////////////////////////
double CEOSMODEL::GetCo_USCS( int fluidindex )
{
  //Check for valid data
  if( !ValidityCheckFluid ( fluidindex ) )
  {
    AddMessage( messagelist, 1, 
      "GetCo_USCS ValidityCheckFluid failed",
      "" );
    return NULL;
  }

  return FluidList[( fluidindex - 1 )].Co;
}

//////////////////////////////////////////////////////
// Method Definition : GetCo_mx_USCS 
//--------------------------------------------------//
// This method returns the Co constant 
// in USCS terms for a mixture.
//////////////////////////////////////////////////////
double CEOSMODEL::GetCo_mx_USCS()
{

  return Mixture.Co_mx;

}

//////////////////////////////////////////////////////
// Method Definition : GetDo_USCS 
//--------------------------------------------------//
// This method returns the Do constant 
// in USCS terms for a single component.
//////////////////////////////////////////////////////
double CEOSMODEL::GetDo_USCS( int fluidindex )
{
  //Check for valid data
  if( !ValidityCheckFluid ( fluidindex ) )
  {
    AddMessage( messagelist, 1, 
      "GetDo_USCS ValidityCheckFluid failed",
      "" );
    return NULL;
  }

  return FluidList[( fluidindex - 1 )].Do;
}

//////////////////////////////////////////////////////
// Method Definition : GetDo_mx_USCS 
//--------------------------------------------------//
// This method returns the Do constant 
// in USCS terms for a mixture.
//////////////////////////////////////////////////////
double CEOSMODEL::GetDo_mx_USCS()
{

  return Mixture.Do_mx;

}

//////////////////////////////////////////////////////
// Method Definition : GetEo_USCS 
//--------------------------------------------------//
// This method returns the Do constant 
// in USCS terms for a single component.
//////////////////////////////////////////////////////
double CEOSMODEL::GetEo_USCS( int fluidindex )
{
  //Check for valid data
  if( !ValidityCheckFluid ( fluidindex ) )
  {
    AddMessage( messagelist, 1, 
      "GetEo_USCS ValidityCheckFluid failed",
      "" );
    return NULL;
  }

  return FluidList[( fluidindex - 1 )].Eo;
}

//////////////////////////////////////////////////////
// Method Definition : GetEo_mx_USCS 
//--------------------------------------------------//
// This method returns the Eo constant 
// in USCS terms for a mixture.
//////////////////////////////////////////////////////
double CEOSMODEL::GetEo_mx_USCS()
{

  return Mixture.Eo_mx;

}

//////////////////////////////////////////////////////
// Method Definition : Getb_USCS 
//--------------------------------------------------//
// This method returns the b constant 
// in USCS terms for a single component.
//////////////////////////////////////////////////////
double CEOSMODEL::Getb_USCS( int fluidindex )
{
  //Check for valid data
  if( !ValidityCheckFluid ( fluidindex ) )
  {
    AddMessage( messagelist, 1, 
      "Getb_USCS ValidityCheckFluid failed",
      "" );
    return NULL;
  }

  return FluidList[( fluidindex - 1 )].b;
}

//////////////////////////////////////////////////////
// Method Definition : Getb_mx_USCS 
//--------------------------------------------------//
// This method returns the b constant 
// in USCS terms for a mixture.
//////////////////////////////////////////////////////
double CEOSMODEL::Getb_mx_USCS()
{

  return Mixture.b_mx;

}

//////////////////////////////////////////////////////
// Method Definition : Geta_USCS 
//--------------------------------------------------//
// This method returns the a constant 
// in USCS terms for a single component.
//////////////////////////////////////////////////////
double CEOSMODEL::Geta_USCS( int fluidindex )
{
  //Check for valid data
  if( !ValidityCheckFluid ( fluidindex ) )
  {
    AddMessage( messagelist, 1, 
      "Geta_USCS ValidityCheckFluid failed",
      "" );
    return NULL;
  }

  return FluidList[( fluidindex - 1 )].a;
}

//////////////////////////////////////////////////////
// Method Definition : Geta_mx_USCS 
//--------------------------------------------------//
// This method returns the a constant 
// in USCS terms for a mixture.
//////////////////////////////////////////////////////
double CEOSMODEL::Geta_mx_USCS()
{

  return Mixture.a_mx;

}

/***************************************
 * Method Definition : GetCASNo      *
 *-------------------------------------***************
 * This function returns the a pointer to CAS no.    *
 * array for the fluid at index fluidindex           *
 ****************************************************/
int *CEOSMODEL::GetCASNo( int fluidindex )
{
  //Check for valid data
  if( !ValidityCheckFluid ( fluidindex ) )
  {
    AddMessage( messagelist, 50, 
            "GetCASNo failed",
            "" );
    return 0;
  }

  return FluidList[( fluidindex - 1 )].CASNo;
}

/////////////////////////////////////////////////////
// Method Definition: GetCp_TP_SI
/////////////////////////////////////////////////////
// This method uses the defintion of specific heat
// at constant pressure (defined to be the derivative
// of enthalpy with respect to temperature at
// constant pressure) to calculate the specific heat
// in SI units
/////////////////////////////////////////////////////
double CEOSMODEL::GetCp_TP_SI( double T, double P )
{
  //local variables
  double Cp;

  //Convert from SI to USCS
  T = ( T * units.convert( units.K, units.R ) );
  P = ( P * units.convert( units.bar, units.PSIA ) );

  //Call the Cp solver
  Cp = GetCp_TP_USCS( T, P );

  //Convert to SI equivalent
  Cp = ( Cp * units.convert( units.BTU, units.J ) );
  Cp = ( Cp * units.convert( units.g, units.lb ) );
  Cp = ( Cp * units.convert( units.K, units.R ) );

  return Cp;
}

/////////////////////////////////////////////////////
// Method Definition: GetCp_TP_USCS
/////////////////////////////////////////////////////
// This method uses the defintion of specific heat
// at constant pressure (defined to be the derivative
// of enthalpy with respect to temperature at
// constant pressure) to calculate the specific heat
// in USCS units
/////////////////////////////////////////////////////
double CEOSMODEL::GetCp_TP_USCS( double T, double P )
{
    double OffsetTemp;
    double ha;
    double hb;

    //calculate initial enthalpy
    ha = GetH_TP_USCS( T, P );
    if (ha <= 0 )
    {
        AddMessage( messagelist, 1, 
                "Enthalpy call in GetCp_TP_USCS failed to converge",
                "Check the fluid temperature and pressure" );
        return -1;
    }

    //increase the temperature by a small delta
    OffsetTemp = ( T * 1.005 );

    //calulate the second enthalpy point
    hb = GetH_TP_USCS( OffsetTemp, P );
    if (ha <= 0 )
    {
        AddMessage( messagelist, 1, 
                "Enthalpy call for offset temperature in GetCp_TP_USCS failed to converge",
                "Check the fluid temperature and pressure" );
        return -1;
    }


    return ( hb - ha ) / ( OffsetTemp - T );
}


/////////////////////////////////////////////////////
// Method Definition: GetCv_TP_SI
/////////////////////////////////////////////////////
// This method returns the specific heat at constant
// volume in SI
/////////////////////////////////////////////////////
double CEOSMODEL::GetCv_TP_SI( double T, double P )
{
  //local variables
  double Cv;

  //Convert from SI to USCS
  T = ( T * units.convert( units.K, units.R ) );
  P = ( P * units.convert( units.bar, units.PSIA ) );

  //Call the Cp solver
  Cv = GetCv_TP_USCS( T, P );

  //Convert to SI equivalent
  Cv = ( Cv * units.convert( units.BTU, units.J ) );
  Cv = ( Cv * units.convert( units.g, units.lb ) );
  Cv = ( Cv * units.convert( units.K, units.R ) );

  return Cv;
}

/////////////////////////////////////////////////////
// Method Definition: GetCv_TP_USCS
/////////////////////////////////////////////////////
// This method uses the defintion of specific heat
// at constant volume (defined to be the derivative
// of int. energy with respect to temperature at
// constant volume) to calculate the specific heat
// in USCS units
/////////////////////////////////////////////////////
double CEOSMODEL::GetCv_TP_USCS( double T, double P )
{
    double OffsetTemp;
    double ha;
    double ua;
    double v;
    double hb;
    double ub;
    double pb;

    //calculate initial enthalpy and internal energy
    ha = GetH_TP_USCS( T, P );
    v = GetV_TP_USCS( T, P );
    ua = ha - ( P * v * ( 144 / 778.16 ) );
    if (ha <= 0 )
    {
        AddMessage( messagelist, 1, 
                "Enthalpy call in GetCp_TP_USCS failed to converge",
                "Check the fluid temperature and pressure" );
        return -1;
    }

    //increase the temperature by a small delta
    OffsetTemp = ( T * 1.005 );

    //calulate the second enthalpy point
    pb = ( P * 1.0001 );
    OffsetTemp = GetT_VP_USCS( v, pb );
    hb = GetH_TP_USCS( OffsetTemp, P );
    ub = hb - ( pb * v * ( 144 / 778.16 ) );
    if (hb <= 0 )
    {
        AddMessage( messagelist, 1, 
                "Enthalpy call for offset temperature in GetCp_TP_USCS failed to converge",
                "Check the fluid temperature and pressure" );
        return -1;
    }


    return ( ub - ua ) / ( OffsetTemp - T );
}

/************************************
* Method Definition : GetDipole_SI  *
*-----------------------------------*****************
* This function returns the dipole moment           *
* in SI units for the fluid fluidindex              *
****************************************************/
double CEOSMODEL::GetDipole_SI( int fluidindex )
{
     
     //Check for valid data
     if( !ValidityCheckFluid ( fluidindex ) )
     {
          AddMessage( messagelist, 50, 
               "GetDipole_SI failed validity check",
               "" );
          return 0;
     }
     
     return FluidList[( fluidindex - 1 )].Dipole_SI;
}

/******************************
 * Method Definition : GetEcc *
 *----------------------------************************
 * This function returns the eccentricity factor     *
 * for the fluid at index fluidindex                 *
 ****************************************************/
double CEOSMODEL::GetEcc( int fluidindex )
{
  //Check for valid data
  if( !ValidityCheckFluid ( fluidindex ) )
  {
    AddMessage( messagelist, 50, 
      "GetEcc failed to return a value",
      "" );
    return 0;
  }

  return FluidList[( fluidindex - 1 )].omega;
}

/***********************************
 * Method Definition : GetEcc_mx *
 *---------------------------------*******************
 * This function returns the eccentricity factor   *
 * for the mixture                     *
 ****************************************************/
double CEOSMODEL::GetEcc_mx()
{
  return Mixture.omega_mx;
}

/***************************************
 * Method Definition : GetFluidCount *
 *-------------------------------------***************
 * This function returns the number of fluids loaded *
 * into the eos object                 *
 ****************************************************/
int CEOSMODEL::GetFluidCount()
{
  return FluidCount;
}

/***************************************
 * Method Definition : GetFormula    *
 *-------------------------------------***************
 * This function returns a pointer to the formula for*
 * the fluid at index fluidindex             *
 ****************************************************/
char *CEOSMODEL::GetFormula( int fluidindex )
{
  //Check for valid data
  if( !ValidityCheckFluid ( fluidindex ) )
  {
    AddMessage( messagelist, 1, 
            "GetFormula failed to return molecular formula",
            "" );
    return errchar;
  }

  return FluidList[( fluidindex - 1 )].chemsym;
}

/************************************
 * Method Definition : Geth_h298_SI *
 *----------------------------------******************
 * This method returns the enthalpy for an ideal     *
 * gas relative to the enthalpy at 298k.  Based on   *
 * the JANAF tables                                  *
 ****************************************************/
double CEOSMODEL::Geth_h298_SI( double T )
{

  //local variables
  double      Term1;
  double      Term2;
  double      Term3;
  double      Term4;
  double      Term5;
  double      Term6;
  double      Term7;

  //Initialize local variables
  Term1      = 0;
  Term2      = 0;
  Term3      = 0;
  Term4      = 0;
  Term5      = 0;
  Term6      = 0;
  Term7      = 0;
   
     //This function calculates the enthalpy(h) for an ideal gas
     Term1      = ( Mixture.HT0_mx );
     Term2      = ( Mixture.HT1_mx * T );
     Term3      = ( Mixture.HT2_mx * T * T );
     Term4      = ( Mixture.HT3_mx * T * T * T );
     Term5      = ( Mixture.HT4_mx * pow( T, 4 ) );
     Term6      = ( Mixture.HT5_mx * pow( T, 5 ) );
     Term7      = ( Mixture.Hn_mx * pow( T, -2.2 ) );

     //return the value
  return ( Term1 + Term2 + Term3 + Term4 + Term5 + Term6 + Term7);
    
}

/**************************************
 * Method Definition : Geth_h298_USCS *
 *------------------------------------****************
 * This function returns the enthalpy for an ideal   *
 * gas relative to the enthalpy at 298k.  Based on   *
 * the JANAF tables  in USCS                          *
 ****************************************************/
double CEOSMODEL::Geth_h298_USCS( double T )
{

  //local variables
  double      h;
  double      T_SI;

  //Initialize local variables
  h        = 0;
  T_SI                = 0;   

  //Convert the temperature
  T_SI                = ( T * units.convert( units.K, units. R ) );

  //Get the enthalpy
  h        = Geth_h298_SI( T_SI );
    
    //Convert from SI to USCS
  h        = ( h * units.convert( units.J, units.BTU ) );
  h        = ( h * units.convert( units.lb, units.g ) );

  //return the value
  return h;
    
}

/**********************************
 * Method Definition : GetH_PS_SI *
 *--------------------------------********************
 * This method returns the entropy for a fluid       *
 * mixture given the pressure and entropy in SI      *
 ****************************************************/
double CEOSMODEL::GetH_PS_SI( double P, double S )
{

  //local variables
  double        H;

  //initialize local variables
  H          = 0;

  //Convert from SI to USCS
  P          = ( P * units.convert( units.bar, units.PSIA ) );
  S          = ( S * units.convert( units.J, units.BTU ) );
  S          = ( S * units.convert( units.lb, units.g ) );
  S          = ( S * units.convert( units.R, units.K ) );


     //Call the entropy solver in USCS
  H          = GetH_PS_USCS( P, S );

  //Convert to SI equivalent
  H          = ( H * units.convert( units.BTU, units.J ) );
  H          = ( H * units.convert( units.g, units.lb ) );

  //return the enthalpy
  return H;

}

/************************************
 * Method Definition : GetH_PS_USCS *
 *----------------------------------******************
 * This method returns the entropy for a fluid       *
 * mixture given the pressire and entropy in USCS    *
 ****************************************************/
double CEOSMODEL::GetH_PS_USCS( double P, double S )
{
  //Approach is to find the temperature T at which S_TP gives S
     //Then calculate H_TP and return the H value
     
     //local variables
     int                 i;
     int                 IterCountInner;
     int                 IterCountOuter;
     double              FirstTemperature;
     double              SecondTemperature;
     double              ThirdTemperature;
     double              FirstEntropy;
     double              SecondEntropy;
     double              ThirdEntropy;
     double              CalcPrecision;
     double              Slope;

     //initialize local variables
     i                   = 0;
     IterCountInner      = 0;
     IterCountOuter      = 0;
     FirstTemperature    = 0;
     SecondTemperature   = 0;
     ThirdTemperature    = 0;
     FirstEntropy        = 0;
     SecondEntropy       = 0;
     ThirdEntropy        = 0;
     CalcPrecision       = 0;
     Slope               = 0;

    //Check for valid P and S
  if( P < 0.00001 )
  {
    AddMessage( messagelist, 1, 
            "Pressure in GETH_PS_USCS below 0.00001 PSIA",
            "Check the fluid pressure" );
    return 0;
  }

     //initialize the variables for the false position method
     FirstTemperature    = 600.00;
     SecondTemperature   = 800.00;

   //enter the loop
   do
   {
   
          FirstEntropy        = GetS_TP_USCS( FirstTemperature, P );
          SecondEntropy       = GetS_TP_USCS( SecondTemperature, P );

          //calculate the slope
          Slope               = ( ( FirstTemperature - SecondTemperature ) / ( FirstEntropy - SecondEntropy ) );
     
          //calculate the third Temperature
          ThirdTemperature    = ( FirstTemperature + ( Slope * ( S - FirstEntropy ) ) );

          //check that a valid Temperature was returned
          if( ThirdTemperature < 10 )
          {

               if( ThirdTemperature < 0 )
               {
                    ThirdTemperature    = ( -1 * ThirdTemperature );
                    if( ThirdTemperature < 10 )
                    {
                         ThirdTemperature    = 100.0;
                    }
               }
               else
               {
                    ThirdTemperature    = 100.0;
               }
          
          }

          //Calculate the entropy at the third point and swap out the
          //worst performer
          ThirdEntropy        = GetS_TP_USCS( ThirdTemperature, P );
          if( fabs( ThirdEntropy - FirstEntropy ) < fabs( ThirdEntropy - SecondEntropy ) )
          {
               SecondTemperature   = ThirdTemperature;
               CalcPrecision       = fabs( ThirdEntropy - FirstEntropy );
          }
          else
          {
               FirstTemperature    = ThirdTemperature;
               CalcPrecision       = fabs( ThirdEntropy - SecondEntropy );
          }

          //else increment the counter and try again
          IterCountOuter++;
      
      }while( ( CalcPrecision > Precision ) && ( IterCountOuter < MaxIterations ) );

     //return the desired enthalpy
  return GetH_TP_USCS( ThirdTemperature, P );

}

/************************************
 * Method Definition : GetH_TP_SI *
 *----------------------------------******************
 * This function returns the enthalpy for a fluid    *
 * mixture given the temperature and pressure in SI   *
 ****************************************************/
double CEOSMODEL::GetH_TP_SI( double T, double P )
{

  //local variables
  double        H;

  //initialize local variables
  H          = 0;

  //Convert from SI to USCS
  T          = ( T * units.convert( units.K, units.R ) );
  P          = ( P * units.convert( units.bar, units.PSIA ) );

  //Call the entropy solver
  H          = GetH_TP_USCS( T, P );

  //Convert to SI equivalent
  H          = ( H * units.convert( units.BTU, units.J ) );
  H          = ( H * units.convert( units.g, units.lb ) );

  //return the enthalpy
  return H;

}

/************************************
 * Method Definition : GetH_TP_USCS *
 *----------------------------------******************
 * This method returns the entropy for a fluid       *
 * mixture given the temperature and pressure        *
 ****************************************************/
double CEOSMODEL::GetH_TP_USCS( double T, double P )
{
  //local variables
  double        HI;
  double        HD;

  //initialize local variables
  HI          = 0;
  HD          = 0;

  //Calculate the entropy for the initial guess
  HI          = GetHIdeal_USCS( T );
  HD          = GetHDepart_TP_USCS( T, P );

  //Check for a valid response from the solver
  if( HD == 0 )
  {
    AddMessage( messagelist, 1, 
            "Enthalpy departure call in GETH_TP_USCS failed to converge",
            "Check the fluid temperature and pressure" );
    return 0;
  }

  //we must have made it
  return ( HI + HD );

}

/**********************************
 * Method Definition : GetH_TS_SI *
 *--------------------------------********************
 * This method returns the entropy for a fluid       *
 * mixture given the temperature and entropy in SI   *
 ****************************************************/
double CEOSMODEL::GetH_TS_SI( double T, double S )
{

  //local variables
  double        H;

  //initialize local variables
  H          = 0;

  //Convert from SI to USCS
  T          = ( T * units.convert( units.K, units.R ) );
  S          = ( S * units.convert( units.J, units.BTU ) );
  S          = ( S * units.convert( units.lb, units.g ) );
  S          = ( S * units.convert( units.R, units.K ) );


     //Call the entropy solver in USCS
  H          = GetH_TS_USCS( T, S );

  //Convert to SI equivalent
  H          = ( H * units.convert( units.BTU, units.J ) );
  H          = ( H * units.convert( units.g, units.lb ) );

  //return the enthalpy
  return H;

}

/************************************
 * Method Definition : GetH_TS_USCS *
 *----------------------------------******************
 * This method returns the entropy for a fluid       *
 * mixture given the temperature and entropy in USCS *
 ****************************************************/
double CEOSMODEL::GetH_TS_USCS( double T, double S )
{
    //Approach is to find the pressure P at which S_TP gives S
	//Then calculate H_TP and return the H value
	 
	//local variables
	int                           i;
	int                           IterCountInner;
	int                           IterCountOuter;
	double                        FirstPressure;
	double                        SecondPressure;
	double                        ThirdPressure;
	double                        FirstEntropy;
	double                        SecondEntropy;
	double                        ThirdEntropy;
	double                        CalcPrecision;
	double                        Slope;
#ifdef _DEBUGH_TS
    errno_t                       err;
    FILE                          *stream;
#endif

    //initialize local variables
    i                             = 0;
    IterCountInner                = 0;
    IterCountOuter                = 0;
    FirstPressure                 = 0;
    SecondPressure                = 0;
    ThirdPressure                 = 0;
    FirstEntropy                  = 0;
    SecondEntropy                 = 0;
    ThirdEntropy                  = 0;
    CalcPrecision                 = 0;
    Slope                         = 0;
#ifdef _DEBUGH_TS
    stream                        = NULL;
#endif

    //Check for valid T and S
    if( T < 10 )
    {
        AddMessage( messagelist, 1, 
                "Temperature in GETH_TS_USCS below 10 R",
                "Check the fluid temperature" );
        return 0;
    }

#ifdef _DEBUGH_TS

    //Open the log file
    err = fopen_s( &stream, "H_TS_USCSFalsePos.csv" , "w");
    if( err != 0 )
    {
        return -1;
    }
    
    //output header
    fprintf( stream, "Composition: \n" );
    for(i=0;i<GetFluidCount(); i++)
    {
        fprintf( stream, "%i, %f\n", i, GetPercentMW( ( i+1 ) ) );
    }
    fprintf( stream, "Entropy: %f\n", S );
    fprintf( stream, "Temperature: %f\n", T);
    fprintf( stream, "IterCountInner, IterCountOuter, FirstPressure, FirstEntropy, SecondPressure, SecondEntropy, SLOPE, ThirdPressure, ThirdEntropy\n" );

#endif

    //initialize the variables for the false position method
    FirstPressure  = 100.00;
    SecondPressure = 200.00;

    //enter the loop
    do
    {

        FirstEntropy   = GetS_TP_USCS( T, FirstPressure );
        SecondEntropy  = GetS_TP_USCS( T, SecondPressure );

          //calculate the slope
          Slope          = ( ( FirstPressure - SecondPressure ) / ( FirstEntropy - SecondEntropy ) );
     
          //calculate the third pressure
          ThirdPressure  = ( FirstPressure + ( Slope * ( S - FirstEntropy ) ) );

          //check that a valid pressure was returned
          if( ThirdPressure < 0 )
          {
               //Reset the iteration counter
               IterCountInner = 0;

               //check that pressure is valid
               do
               {
                    FirstPressure  = ( FirstPressure * 0.5 ) ;
                    SecondPressure = ( SecondPressure * 0.5 ) ;
                    FirstEntropy   = GetS_TP_USCS( T, FirstPressure );
                    SecondEntropy  = GetS_TP_USCS( T, SecondPressure );

                    //calculate the slope
                    Slope          = ( ( FirstPressure - SecondPressure ) / ( FirstEntropy - SecondEntropy ) );
          
                    //calculate the third pressure
                    ThirdPressure  = ( FirstPressure + ( Slope * ( S - FirstEntropy ) ) );

                    //Increment the iteration counter
                    IterCountInner++;

               }while( ( ThirdPressure < 0 ) && ( IterCountInner < MaxIterations ) );

               //check that we were able to find a third pressure
               if( ThirdPressure < 0 )
               {
              AddMessage( messagelist, 1, 
                      "GetH_TS_USCS failed to converge",
                      "Check fluid properties and mixture" );
#ifdef _DEBUGH_TS
       
           //Close the log file
           fclose( stream );

#endif
                    return 0;
     
               }

          }

#ifdef _DEBUGH_TS
  
    //Open the log file
    fprintf( stream, "%i, %i, %f, %f, %f, %f, %f, %f, %f\n",
      IterCountInner,
      IterCountOuter,
               FirstPressure,
      FirstEntropy,
      SecondPressure,
      SecondEntropy,
      Slope,
      ThirdPressure,
      ThirdEntropy);

#endif
          //Calculate the entropy at the third point and swap out the
          //worst performer
          ThirdEntropy   = GetS_TP_USCS( T, ThirdPressure );
          if( fabs( ThirdEntropy - FirstEntropy ) < fabs( ThirdEntropy - SecondEntropy ) )
          {
               SecondPressure = ThirdPressure;
               CalcPrecision  = fabs( ThirdEntropy - FirstEntropy );
          }
          else
          {
               FirstPressure  = ThirdPressure;
               CalcPrecision  = fabs( ThirdEntropy - SecondEntropy );
          }

          //else increment the counter and try again
          IterCountOuter++;
      
      }while( ( CalcPrecision > Precision ) && ( IterCountOuter < MaxIterations ) );

#ifdef _DEBUGH_TS
  
  //Close the log file
  fclose( stream );

#endif

     //return the desired enthalpy
  return GetH_TP_USCS( T, ThirdPressure );

}

/*********************************
 * Method Definition : Gethfo_SI *
 *-------------------------------********************
 * This method returns the enthalpy of formation    *
 * in SI units for the fluid fluidindex             *
 ****************************************************/
double CEOSMODEL::Gethfo_SI( int fluidindex )
{

  //Check for valid data
  if( !ValidityCheckFluid ( fluidindex ) )
  {
    AddMessage( messagelist, 50, 
      "Gethfo_SI failed validity check",
      "" );
    return 0;
  }

  return FluidList[( fluidindex - 1 )].hfo_SI;
}

/*********************************
 * Method Definition : Gethfo_SI *
 *-------------------------------*********************
 * This function returns the enthalpy of formation   *
 * in SI units for the mixture                       *
 ****************************************************/
double CEOSMODEL::Gethfo_SI()
{

  return Mixture.hfo_SI_mx;

}

/*************************************
 * Method Definition : Gethfo_USCS *
 *-----------------------------------*****************
 * This function returns the enthalpy of formation   *
 * in USCS units for the fluid fluidindex       *
 ****************************************************/
double CEOSMODEL::Gethfo_USCS( int fluidindex )
{

  //Check for valid data
  if( !ValidityCheckFluid ( fluidindex ) )
  {
    AddMessage( messagelist, 50, 
      "Gethfo_USCS failed validity check",
      "" );
    return 0;
  }

  return FluidList[( fluidindex - 1 )].hfo_USCS;
}

/***********************************
 * Method Definition : Gethfo_USCS *
 *---------------------------------*******************
 * This function returns the enthalpy of formation   *
 * in USCS units for the mixture                     *
 ****************************************************/
double CEOSMODEL::Gethfo_USCS( )
{

  return Mixture.hfo_USCS_mx;
}

///////////////////////////////////////////////////////
// Method Definition : GetHHV_mx_SI 
///////////////////////////////////////////////////////
// Returns the higher heating value in SI units for
// the mixture 
///////////////////////////////////////////////////////
double CEOSMODEL::GetHHV_mx_SI()
{

  return Mixture.HHV_SI_mx;

}

///////////////////////////////////////////////////////
// Method Definition : GetHHV_mx_USCS
///////////////////////////////////////////////////////
// Returns the higher heating value in USCS units for
// the mixture.
///////////////////////////////////////////////////////
double CEOSMODEL::GetHHV_mx_USCS()
{

  return Mixture.HHV_USCS_mx;

}

///////////////////////////////////////////////////////
// Method Definition : GetHHV_SI
///////////////////////////////////////////////////////
// This function returns the higher heating value
// in SI units for the fluid fluidindex
///////////////////////////////////////////////////////
double CEOSMODEL::GetHHV_SI( int fluidindex )
{
     
     //Check for valid data
     if( !ValidityCheckFluid ( fluidindex ) )
     {
          AddMessage( messagelist, 50, 
               "GetHHV_SI failed validity check",
               "" );
          return 0;
     }
     
     return FluidList[( fluidindex - 1 )].HHV_SI;
}

///////////////////////////////////////////////////////
// Method Definition : GetHHV_USCS
///////////////////////////////////////////////////////
// This function returns the lower heating value
// in USCS units for the fluid fluidindex
///////////////////////////////////////////////////////
double CEOSMODEL::GetHHV_USCS( int fluidindex )
{

  //Check for valid data
  if( !ValidityCheckFluid ( fluidindex ) )
  {
    AddMessage( messagelist, 50, 
      "GetHHV_USCS failed validity check",
      "" );
    return 0;
  }

  return FluidList[( fluidindex - 1 )].HHV_USCS;
}


/**************************************
 * Method Definition : GetHIdeal_SI *
 *------------------------------------****************
 * This function returns entropy of an ideal gas     *
 * misture.                       *
 ****************************************************/
double CEOSMODEL::GetHIdeal_SI( double T )
{

  //local variables
  double        H;

  //initialize local variables
  H          = 0;

  //Convert from SI to USCS
  T          = ( T * units.convert( units.K, units.R ) );

  //Call the entropy solver
  H          = GetHIdeal_USCS( T );

  //Convert to SI equivalent
  H          = ( H * units.convert( units.BTU, units.J ) );
  H          = ( H * units.convert( units.g, units.lb ) );

  //return the enthalpy
  return H;

}

/*************************************
 * Method Definition: GetHIdeal_USCS *
 *-----------------------------------****************
 * Calculate the ideal enthalpy for mixture.        *
 * Note:  The reference datum is 1 atm              *
 ****************************************************/
double CEOSMODEL::GetHIdeal_USCS( double T )
{
   //local variables
   double      Term1;
   double      Term2;
   double      Term3;
   double      Term4;
   double      Term5;
   double      Term6;

   //Initialize local variables
   Term1      = 0;
   Term2      = 0;
   Term3      = 0;
   Term4      = 0;
   Term5      = 0;
   Term6      = 0;
   
    //This function calculates the ideal enthalpy(h) for a fluid
    Term1      = ( Mixture.A_mx );
    Term2      = ( Mixture.B_mx * T );
    Term3      = ( Mixture.C_mx * T * T );
    Term4      = ( Mixture.D_mx * T * T * T );
    Term5      = ( Mixture.E_mx * pow( T, 4 ) );
    Term6      = ( Mixture.F_mx * pow( T, 5 ) );

    //return the value
  return ( Term1 + Term2 + Term3 + Term4 + Term5 + Term6 );
    
}
///////////////////////////////////////////////////////
// Method Definition: 
//   Returns the array size for the mixing parameter
//   array 'k'
///////////////////////////////////////////////////////
bool CEOSMODEL::GetkCount( int *iCount)
{
    
    *iCount = _ikCount;
    return true;

}

///////////////////////////////////////////////////////
// Method Definition: 
//   Returns the value at index iIndex from the mixing 
//   parameter array 'k'
///////////////////////////////////////////////////////
bool CEOSMODEL::GetkValue( int iIndex, double *dblValue )
{
    
    if( ( iIndex < 0 ) || ( iIndex > _ikCount ) )
    {
        return false;
    }
    *dblValue = _k[iIndex];
    return true;

}

///////////////////////////////////////////////////////
// Method Definition : GetLHV_mx_SI 
///////////////////////////////////////////////////////
// Returns the lower heating value  in SI units for
// the mixture 
///////////////////////////////////////////////////////
double CEOSMODEL::GetLHV_mx_SI()
{

  return Mixture.LHV_SI_mx;

}

///////////////////////////////////////////////////////
// Method Definition : GetLHV_mx_USCS
///////////////////////////////////////////////////////
// Returns the lower heating value in USCS units for
// the mixture.
///////////////////////////////////////////////////////
double CEOSMODEL::GetLHV_mx_USCS()
{

  return Mixture.LHV_USCS_mx;

}

///////////////////////////////////////////////////////
// Method Definition : GetLHV_SI
///////////////////////////////////////////////////////
// This function returns the lower heating value
// in SI units for the fluid fluidindex
///////////////////////////////////////////////////////
double CEOSMODEL::GetLHV_SI( int fluidindex )
{
     
     //Check for valid data
     if( !ValidityCheckFluid ( fluidindex ) )
     {
          AddMessage( messagelist, 50, 
               "GetLHV_SI failed validity check",
               "" );
          return 0;
     }
     
     return FluidList[( fluidindex - 1 )].LHV_SI;
}

 
///////////////////////////////////////////////////////
// Method Definition : GetLHV_USCS
///////////////////////////////////////////////////////
// This function returns the lower heating value
// in USCS units for the fluid fluidindex
///////////////////////////////////////////////////////
double CEOSMODEL::GetLHV_USCS( int fluidindex )
{

  //Check for valid data
  if( !ValidityCheckFluid ( fluidindex ) )
  {
    AddMessage( messagelist, 50, 
      "GetLHV_USCS failed validity check",
      "" );
    return 0;
  }

  return FluidList[( fluidindex - 1 )].LHV_USCS;
}

/***************************************
 * Method Definition : GetMessageAux *
 *-------------------------------------***************
 * This function returns a pointer to the auxillary  *
 * or secondary text of a message           *
 ****************************************************/
char *CEOSMODEL::GetMessageAux( int mesno )
{
  //local variables
  int        i;
  class      CMeshan *currentElement;

  //initialize local variables
  i        = 1;
  currentElement  = NULL;

  //locate the head of the pointer list
  currentElement  = messagelist;

  //check to see if the messagelist pointer is null
  while( currentElement != NULL)
  {
    if( i == mesno )
      return currentElement->GetAuxText();
    currentElement  = currentElement->next;
    i++;
  }
  
  //we didn't find the requested message
  return NULL;
}

/*****************************************
 * Method Definition : GetMessageCount *
 *---------------------------------------*************
 * This function returns the number of messages in   *
 * the handler                     *
 ****************************************************/
int CEOSMODEL::GetMessageCount()
{
  //local variables
  int        i;
  class      CMeshan *currentElement;

  //initialize local variables
  i        = 0;
  currentElement  = NULL;

  //locate the head of the pointer list
  currentElement  = messagelist;

  //check to see if the messagelist pointer is null
   while( currentElement != NULL)
  {
    currentElement = currentElement->next;
    i++;
  }
  
  //return the total messages found
  return i;
}

/**************************************
 * Method Definition : GetMessageMain *
 *------------------------------------****************
 * This method returns a pointer to the main text    *
 * of a message                                      *
 ****************************************************/
char *CEOSMODEL::GetMessageMain( int mesno )
{
  //local variables
  int        i;
  class      CMeshan *currentElement;

  //initialize local variables
  i        = 1;
  currentElement  = NULL;

  //locate the head of the pointer list
  currentElement  = messagelist;

  if (currentElement != NULL)
  {
     //check to see if the messagelist pointer is null
     while( currentElement != NULL)
     {
     if( i == mesno )
          return currentElement->GetMainText();
     currentElement  = currentElement->next;
     i++;
     }
  }

  //we didn't find the requested message
  return NULL;
}

/******************************************
 * Method Definition : GetMessagePriority *
 *----------------------------------------************
 * This method returns a the priority of a message   *
 * at index mesno                                    *
 ****************************************************/
int CEOSMODEL::GetMessagePriority( int mesno )
{
  //local variables
  int        i;
  class      CMeshan *currentElement;

  //initialize local variables
  i        = 1;
  currentElement  = NULL;

  //locate the head of the pointer list
  currentElement  = messagelist;

  //check to see if the messagelist pointer is null
  while( currentElement != NULL)
  {
    if( i == mesno )
      return currentElement->GetPriority();
    currentElement  = currentElement->next;
    i++;
  }
  
  //we didn't find the requested message
  return -1;
}

/******************************************
 * Method Definition : GetMessageStatus *
 *----------------------------------------************
 * This function returns true if messages exist and  *
 * false if there are no messages           *
 ****************************************************/
bool CEOSMODEL::GetMessageStatus()
{
  //check to see if the messagelist pointer is null
  if( messagelist == NULL )
    return false;
  
  //we must have some messages
  return true;
}

 /***************************************
 * Method Definition : GetMolWeight    *
 *-------------------------------------***************
 * This function returns the molecular weight for    *
 * the fluid at index fluidindex                     *
 ****************************************************/
double CEOSMODEL::GetMolWeight( int fluidindex )
{

     //Check for valid data
     if( !ValidityCheckFluid ( fluidindex ) )
     {
          AddMessage( messagelist, 50, 
               "GetMolWeight failed to return a value",
               "" );
          return 0;
     }
     
     return FluidList[( fluidindex - 1 )].molewt;
}

/***************************************
 * Method Definition : GetMolWeight  *
 *-------------------------------------***************
 * This function returns the molecular weight for    *
 * the mixture fluid                 *
 ****************************************************/
double CEOSMODEL::GetMolWeight_mx()
{

  //Pull the value from the object and return it
  return Mixture.molewt_mx;

}

/*********************************
 * Method Definition : GetName *
 *-------------------------------*********************
 * This function returns a pointer to the name for   *
 * the fluid at index fluidindex             *
 ****************************************************/
char *CEOSMODEL::GetName( int fluidindex )
{
  //Check for valid data
  if( !ValidityCheckFluid ( fluidindex ) )
  {
    AddMessage( messagelist, 1, 
            "GetName failed ValidityCheck on fluid index",
            "" );
    messagelist->SetPriority( 1 );
    return errchar;
  }

  return FluidList[( fluidindex - 1 )].name;
}

/***************************************
 * Method Definition : GetName_Alt01 *
 *-------------------------------------***************
 * This function returns a pointer to the alternate  *
 * secondary name for the fluid at index fluidindex   *
 ****************************************************/
char *CEOSMODEL::GetName_Alt01( int fluidindex )
{
  //Check for valid data
  if( !ValidityCheckFluid ( fluidindex ) )
  {
    AddMessage( messagelist, 50, 
            "GetName_Alt01 failed ValidityCheck on fluid index",
            "" );
    return errchar;
  }

  return FluidList[( fluidindex - 1 )].name_alt01;
}

/***************************************
 * Method Definition : GetPercentMW  *
 *-------------------------------------***************
 * This function returns the molecular weight for    *
 * the fluid at index fluidindex             *
 ****************************************************/
double CEOSMODEL::GetPercentMW( int fluidindex )
{

  //Check for valid data
  if( !ValidityCheckFluid ( fluidindex ) )
  {
    AddMessage( messagelist, 50, 
            "GetPercentMW failed ValidityCheck on fluidindex",
            "" );
    return 0;
  }

  return FluidList[( fluidindex - 1 )].percentmw;
}

/************************************
 * Method Definition : GetPmax_SI *
 *----------------------------------******************
 * This function returns the maximum pressure the    *
 * EOS engine correlation matches the measured data  *
 * for the fluid at fluindex in SI units       *
 ****************************************************/
double CEOSMODEL::GetPmax_SI( int fluidindex )
{
  //local variables
  class        CMeshan *currentElement;
  double        P;

  //Initialize local variables
  currentElement    = NULL;
  P          = 0;

  //Check for valid data
  if( !ValidityCheckFluid ( fluidindex ) )
  {
    AddMessage( messagelist, 50, 
            "GetPmax_SI ValidityCheckFluid failed",
            "" );
    return NULL;
  }

  //retrieve the USCS value
  P          = FluidList[( fluidindex - 1 )].Pmax_USCS;

  //convert to SI
  P          = ( P * units.convert( units.PSIA, units.bar ) );

  //return the value
  return P;
}

/**************************************
 * Method Definition : GetPmax_USCS *
 *------------------------------------****************
 * This function returns the maximum pressure the    *
 * EOS engine correlation matches the measured data  *
 * for the fluid at fluindex in USCS units       *
 ****************************************************/
double CEOSMODEL::GetPmax_USCS( int fluidindex )
{
  //local variables
  class        CMeshan *currentElement;
  double        P;

  //Initialize local variables
  currentElement    = NULL;
  P          = 0;

  //Check for valid data
  if( !ValidityCheckFluid ( fluidindex ) )
  {
    AddMessage( messagelist, 50, 
            "GetPmax_USCS ValidityCheckFluid failed",
            "" );
    return NULL;
  }

  //retrieve the USCS value
  P          = FluidList[( fluidindex - 1 )].Pmax_USCS;

  //return the value
  return P;
}

/************************************
 * Method Definition : GetPmin_SI *
 *----------------------------------******************
 * This function returns the minimum pressure the    *
 * EOS engine correlation matches the measured data  *
 * for the fluid at fluindex in SI units       *
 ****************************************************/
double CEOSMODEL::GetPmin_SI( int fluidindex )
{
  //local variables
  class        CMeshan *currentElement;
  double        P;

  //Initialize local variables
  currentElement    = NULL;
  P          = 0;

  //Check for valid data
  if( !ValidityCheckFluid ( fluidindex ) )
  {
    AddMessage( messagelist, 50, 
            "GetPmin_SI ValidityCheckFluid failed",
            "" );
    return NULL;
  }

  //retrieve the USCS value
  P          = FluidList[( fluidindex - 1 )].Pmin_USCS;

  //convert to SI
  P          = ( P * units.convert( units.PSIA, units.bar ) );

  //return the value
  return P;
}

/**************************************
 * Method Definition : GetPmin_USCS *
 *------------------------------------****************
 * This function returns the minimum pressure the    *
 * EOS engine correlation matches the measured data  *
 * for the fluid at fluindex in USCS units       *
 ****************************************************/
double CEOSMODEL::GetPmin_USCS( int fluidindex )
{
  //local variables
  class        CMeshan *currentElement;
  double        P;

  //Initialize local variables
  currentElement    = NULL;
  P          = 0;

  //Check for valid data
  if( !ValidityCheckFluid ( fluidindex ) )
  {
    AddMessage( messagelist, 50, 
            "GetPmin_USCS ValidityCheckFluid failed",
            "" );
    return NULL;
  }

  //retrieve the USCS value
  P          = FluidList[( fluidindex - 1 )].Pmin_USCS;

  //return the value
  return P;
}

/////////////////////////////////////////////////////
// Method Definition: GetP_MT_USCS 
/////////////////////////////////////////////////////
// Given a molar density MD and a temperature T this
// function calculates and returns the pressure
// at the state defined by MD and T in USCS
/////////////////////////////////////////////////////
double CEOSMODEL::GetP_MT_USCS( double MD,
                double T )
{
  //call the EOS
  return EOS( T, MD );
}
  
/////////////////////////////////////////////////////
// Method Definition: GetP_VS_SI
/////////////////////////////////////////////////////
// Given a molar density V and a entropy S this
// function calculates and returns the pressure     
// at the state defined by V and S in SI           
/////////////////////////////////////////////////////
double CEOSMODEL::GetP_VS_SI( double V,
                double S )
{
  
    //local variables
    double        pres;

    //initialize local variables
    pres        = 0;

    //convert to SI
    V          = ( V * units.convert( units.cm3, units.ft3 ) );
    V          = ( V * units.convert( units.lb, units.g ) );
    S          = ( S * units.convert( units.J, units.BTU ) );
    S          = ( S * units.convert( units.lb, units.g ) );
    S          = ( S * units.convert( units.R, units.K ) );

    //calculate the pressure
    pres        = GetP_VS_USCS( V, S );

    //convert the pressure to SI (bar)
    pres        = (pres * units.convert( units.PSIA, units.bar ) );
  
    //return the value
    return pres;

}

/////////////////////////////////////////////////////
// Method Definition: GetP_VS_USCS
/////////////////////////////////////////////////////
// Given a molar density V and a entropy S this
// function calculates and returns the pressure     
// at the state defined by V and S in USCS         
/////////////////////////////////////////////////////
double CEOSMODEL::GetP_VS_USCS( double V, double S )
{
     //Local variables
     double    P1;
     double    P2;
     double    P3;
     double    T1;
     double    T2;
     double    T3;
     double    V1;
     double    V2;
     double    V3;
     double    TestValue;
     double    slope;
     int itcount;
    
     //Initialize local variables
     P1        = 0;
     P2        = 0;
     P3        = 0;
     T1        = 0;
     T2        = 0;
     T3        = 0;
     V1        = 0;
     V2        = 0;
     V3        = 0;
     TestValue = 0;
     slope     = 0;
     itcount   = 1;

     //Check data
     if( S <= 0 )
     {
     AddMessage( messagelist, 1, 
            "Entropy is below zero in GetP_VS_USCS",
            "Check the entropy value" );
     return -1;
     }
     if( V <= 0 )
     {
     AddMessage( messagelist, 1, 
            "Specific Volume is below zero in GetP_VS_USCS",
            "Check the pressure value" );
     return -1;
     }

     //Establish first point of guess
     P1 = 10;
     T1 = GetT_SP_USCS( S, P1);
     
     //below 150 rankine is not likely....if it comes in that low,
     //try a little higher
     if( T1 < 150 )
     {
          itcount = 1;
          do
          {
               //reset the message handler
               ClearMessages();
               P1 = ( P1 * 1.50 );
               T1 = GetT_SP_USCS( S, P1);
               itcount++;
          }while( ( T1 < 150 ) && ( itcount < 10 ) );
     }

     //it is possible that the lines of constant entropy, S
     //do not cross P1 - increase P1 and try again
     if( itcount >= 9 )
     {
          AddMessage( messagelist, 1, 
               "Trial temperature was not found in GetP_VS_USCS",
               "Check the pressure and entropy values" );
          return -1;
     }

     V1 = GetV_TP_USCS( T1, P1 );
     P2 = ( P1 * 1.05 );
     T2 = GetT_SP_USCS( S, P2);
     V2 = GetV_TP_USCS( T2, P2 );
     
     //begin loop here
     itcount = 1;
     do
     {
  
          //calculate and validate slope of line
          if( fabs( V2 - V1 ) < dblGetTrace() )
          {
               return -1;
          }
          slope = ( ( P2 - P1 ) / ( V2  - V1 ) );
 
          //Estimate next pressure, P3
          P3 = ( P1 + ( slope * ( V - V1 ) ) ) ;

          //check that didn't estimate a negative pressure
          if( P3 <= 0 )
          {
            P3 = 0.90;
          }
          
          //calculate the new V3
          try
          {
               T3 = GetT_SP_USCS( S, P3);
          }
          catch ( ... )
          {
               return -1;
          }

          try
          {

               V3 = GetV_TP_USCS( T3, P3 );
          }
          catch ( ... )
          {
               return -1;
          }

          //replace the worst approximiation with this most recent
          if( fabs( V - V1 ) < fabs( V - V2 ) )
          {
               P2 = P3;
               V2 = V3;
          }
          else
          {
               P1 = P3;
               V1 = V3;
          }

          //Check to see if pressures are close to each other
          TestValue = fabs( P2 - P1);

          //increment the itcount
          itcount++;

     }while( !((TestValue < Precision )||(itcount > MaxIterations)) );
  
     //check to see what through us out of the loop
     if( itcount > MaxIterations )
     {
          return -1;
     }
    
     //Return the value
     return P3;
}

/////////////////////////////////////////////////////
// Method Definition: GetP_VT_SI
/////////////////////////////////////////////////////
// Given a specific volume V and a temperature T this
// function calculates and returns the pressure     
// at the state defined by V and T in SI           
/////////////////////////////////////////////////////
double CEOSMODEL::GetP_VT_SI( double V,
                double T )
{
  //local variables
  double        pres;

  //initialize local variables
  pres        = 0;

  //convert to SI
  T          = ( T * units.convert( units.K, units.R ) );
  V          = ( V * units.convert( units.cm3, units.ft3 ) );
  V          = ( V * units.convert( units.lb, units.g ) );

  //calculate the pressure
  pres        = GetP_VT_USCS( V, T );

  //convert the pressure to SI (bar)
  pres        = (pres * units.convert( units.PSIA, units.bar ) );
  
  //return the value
  return pres;
}
  
/////////////////////////////////////////////////////
// Method Definition: GetPres__VT_USCS
/////////////////////////////////////////////////////
// Given a specific volume V and a temperature T this
// function calculates and returns the pressure     
// at the state defined by V and T in SI           
/////////////////////////////////////////////////////
double CEOSMODEL::GetP_VT_USCS( double V,
                double T )
{
  //local variables
  double dblMD;
  class CMeshan *currentElement;

  //initialize local variables
  dblMD = 0;
  currentElement = NULL;

  //Ensure we don't divide by zero
  if( V == 0 )
  {
    AddMessage( messagelist, 1, 
            "GetPres_VT_USCS volume (V) was zero",
            "Check for a valid V" );
    return NULL;
  }
    
  //Since density is equal to the inverse of the
  //specific volume
  dblMD = ( 1 / V );
  
  //Also check to make sure the MW is not zero
  if( Mixture.molewt_mx == 0 )
  {
    AddMessage( messagelist, 1, 
      "Mixture mole weight was zero in GetPres_VT_USCS",
      "Check percent composision" );
    return -1;
  }

  //We need to convert the units from ft^3/lbm
  //to mol/ft^3
  dblMD = ( dblMD / Mixture.molewt_mx );

  //call the EOS
  return EOS( T, dblMD );
}
  
/////////////////////////////////////////////////////
// Method Definition : GetPresCrit_mx_SI
/////////////////////////////////////////////////////
// This function returns the psuedo critical
// temperature (in SI) for the mixture 
/////////////////////////////////////////////////////
double CEOSMODEL::GetPresCrit_mx_SI()
{

  return ( Mixture.Pc_mx * units.convert( units.PSIA, units.bar ) );

}

/*******************************************
 * Method Definition : GetPresCrit_mx_USCS *
 *-----------------------------------------***********
 * Returns the psuedo critical temperature (in USCS) *
 * for the mixture                                   *
 ****************************************************/
double CEOSMODEL::GetPresCrit_mx_USCS()
{

  return Mixture.Pc_mx;

}

/**************************************
 * Method Definition : GetPresCrit_SI *
 *------------------------------------****************
 * Returns the psuedo critical temperature (in SI)   *
 * for the mixture                                   *
 ****************************************************/
double CEOSMODEL::GetPresCrit_SI( int fluidindex )
{
  //Check for valid data
  if( !ValidityCheckFluid ( fluidindex ) )
  {
    AddMessage( messagelist, 1, 
            "GetPresCrit_SI ValidityCheckFluid failed",
            "" );
    return NULL;
  }

  return FluidList[( fluidindex - 1 )].Pc_SI;
}

/********************************************
 * Method Definition : GetPresCrit_USCS   *
 *------------------------------------------**********
 * This function returns the critical         *
 * temperature (in USCS) for the fluid at index      *
 * fluidindex                       *
 ****************************************************/
double CEOSMODEL::GetPresCrit_USCS( int fluidindex )
{
  //Check for valid data
  if( !ValidityCheckFluid ( fluidindex ) )
  {
    AddMessage( messagelist, 1, 
      "GetPresCrit_USCS ValidityCheckFluid failed",
      "" );
    return NULL;
  }

  return FluidList[( fluidindex - 1 )].Pc;
}

/***********************************
 * Method Definition : GetR_USCS *
 *---------------------------------*******************
 * This function returns the gas specific constant   *
 * in USCS terms                   *
 ****************************************************/
double CEOSMODEL::GetR_USCS( int fluidindex )
{
  //Check for valid data
  if( !ValidityCheckFluid ( fluidindex ) )
  {
    AddMessage( messagelist, 1, 
      "GetR_USCS ValidityCheckFluid failed",
      "" );
    return NULL;
  }

  return FluidList[( fluidindex - 1 )].R_USCS;
}

/************************************
 * Method Definition : GetS_TP_SI *
 *----------------------------------******************
 * This function returns the entropy for a fluid     *
 * mixture given the temperature and pressure in SI   *
 ****************************************************/
double CEOSMODEL::GetS_TP_SI( double T, double P )
{

  //local variables
  double        S;

  //initialize local variables
  S          = 0;

  //Convert from SI to USCS
  T          = ( T * units.convert( units.K, units.R ) );
  P          = ( P * units.convert( units.bar, units.PSIA ) );

  //Call the entropy solver
  S          = GetS_TP_USCS( T, P );

  //Convert to SI equivalent
  S          = ( S * units.convert( units.BTU, units.J ) );
  S          = ( S * units.convert( units.g, units.lb ) );
  S          = ( S * units.convert( units.K, units.R ) );

  return S;

}

/**************************************
 * Method Definition : GetS_TP_USCS   *
 *------------------------------------****************
 * This function returns the entropy for a fluid     *
 * mixture given the temperature and pressure        *
 ****************************************************/
double CEOSMODEL::GetS_TP_USCS( double T, double P )
{
  //local variables
  double        SI;
  double        SD;

  //initialize local variables
  SI          = 0;
  SD          = 0;

  //Check for valid T and P
  if( T < 10 )
  {
    AddMessage( messagelist, 1, 
            "Temperature in GETS_TP_USCS below 10 R",
            "Check the fluid temperature" );
    return 0;
  }

  //Calculate the entropy for the initial guess
  SI = GetSIdeal_USCS( T, P );
  SD = GetSDepart_TP_USCS( T, P );

  //Check for a valid response from the solver
  if(SD == 0)
  {
    AddMessage( messagelist, 1, 
            "Entropy departure call in GETS_TP_USCS failed to converge",
            "Check the fluid temperature and pressure" );
    return 0;
  }

  //we must have made it
  return ( SI + SD );

}

 /***************************************
 * Method Definition: GetSatLiqH_P_SI *
 *--------------------------------------**************
 * This function solves for the enthalpy of       *
 * a single fluid at saturated liquid conditions.    *
 * It does not apply to mixtures.           *
 ****************************************************/
double CEOSMODEL::GetSatLiqH_P_SI( double P )
{
  //local variables
  double        H;

  //initialize local variables
  H          = 0;

  //convert from SI to USCS
  P          = ( P * units.convert( units.bar, units.PSIA ) );

  //call solver
  H          = GetSatLiqH_P_USCS( P );

  //convert from USCS to SI
  H          = ( H * units.convert( units.BTU, units.J ) );
  H          = ( H * units.convert( units.g, units.lb ) );

  //return the value
  return H;
}

 /*****************************************
 * Method Definition: GetSatLiqH_P_USCS *
 *----------------------------------------************
 * This function solves for the enthalpy of       *
 * a single fluid at saturated liquid conditions.    *
 * Given pressure. It does not apply to mixtures.   *
 ****************************************************/
double CEOSMODEL::GetSatLiqH_P_USCS( double P )
{
  //local variables
  double      MDl;
  double      MDv;
  double      Ts;
  double      HI;
  double      HD;

  //Initialize local variables
  MDl        = 0;
  MDv        = 0;
  Ts        = 0;
  HI        = 0;
  HD        = 0;

  //it really doesn't make sense to get the saturated liquid enthalpy for
  //a mixture.  We keep the variable uniary just for this
  //ocassion
  if( !uniary )
  {
    AddMessage( messagelist, 1, 
      "Saturated liquid enthalpy calculatation is not valid for a mixture",
      "" );
    return -1;
  }

  //Retrieve the saturation pressure
  Ts        = GetVaporTemperature_P_USCS( P, &MDl, &MDv );

  //Check to see if we succeeded.
  if( Ts <= 0 )
    return -1;

  //Calculate the enthalpy
  //Calculate the entropy for the initial guess
  HI          = GetHIdeal_USCS( Ts );
  HD          = GetHDepart_MT_USCS( MDl, Ts );

  //we must have made it
  return ( HI + HD );

}

 /***************************************
 * Method Definition: GetSatLiqH_T_SI *
 *--------------------------------------**************
 * This function solves for the enthalpy of       *
 * a single fluid at saturated liquid conditions in  *
 * SI units. It does not apply to mixtures.       *
 ****************************************************/
double CEOSMODEL::GetSatLiqH_T_SI( double T )
{

  //local variables
  double        H;

  //initialize local variables
  H          = 0;

  //Convert from SI to USCS
  T          = ( T * units.convert( units.K, units.R ) );

  //Call the entropy solver
  H          = GetSatLiqH_T_USCS( T );

  //Convert to SI equivalent
  H          = ( H * units.convert( units.BTU, units.J ) );
  H          = ( H * units.convert( units.g, units.lb ) );

  //return the enthalpy
  return H;

}

 /*****************************************
 * Method Definition: GetSatLiqH_T_USCS *
 *----------------------------------------************
 * This function solves for the enthalpy of       *
 * a single fluid at saturated liquid conditions.    *
 * It does not apply to mixtures.           *
 ****************************************************/
double CEOSMODEL::GetSatLiqH_T_USCS( double T )
{
  //local variables
  double      MDl;
  double      MDv;
  double      Ps;
  double      HI;
  double      HD;

  //Initialize local variables
  MDl        = 0;
  MDv        = 0;
  Ps        = 0;
  HI        = 0;
  HD        = 0;

  //it really doesn't make sense to get the saturated liquid enthalpy for
  //a mixture.  We keep the variable uniary just for this
  //ocassion
  if( !uniary )
  {
    AddMessage( messagelist, 1, 
      "Saturated liquid enthalpy calculatation is not valid for a mixture",
      "" );
    return -1;
  }

  //Retrieve the saturation pressure
  Ps        = GetVaporPressure_T_USCS( T, &MDl, &MDv );

  //Check to see if we succeeded.
  if( Ps <= 0 )
    return -1;

  //Calculate the enthalpy
  //Calculate the entropy for the initial guess
  HI          = GetHIdeal_USCS( T );
  HD          = GetHDepart_MT_USCS( MDl, T );

  //we must have made it
  return ( HI + HD );

}

/***************************************
 * Method Definition: GetSatLiqS_P_SI *
 *--------------------------------------**************
 * This function solves for the entropy of       *
 * a single fluid at saturated liquid conditions.    *
 * It does not apply to mixtures.           *
 ****************************************************/
double CEOSMODEL::GetSatLiqS_P_SI( double P )
{
  //local variables
  double        S;

  //initialize local variables
  S          = 0;

  //convert from SI to USCS
  P          = ( P * units.convert( units.bar, units.PSIA ) );

  //call solver
  S          = GetSatLiqS_P_USCS( P );

  //convert from USCS to SI
  S          = ( S * units.convert( units.BTU, units.J ) );
  S          = ( S * units.convert( units.g, units.lb ) );
  S          = ( S * units.convert( units.K, units.R ) );

  //return the value
  return S;
}

 /*****************************************
 * Method Definition: GetSatLiqS_P_USCS *
 *----------------------------------------************
 * This function solves for the entropy of       *
 * a single fluid at saturated liquid conditions   *
 * given pressure.  It does not apply to mixtures.   *
 ****************************************************/
double CEOSMODEL::GetSatLiqS_P_USCS( double P )
{
  //local variables
  double      MDl;
  double      MDv;
  double      Ts;
  double      SI;
  double      SD;

  //Initialize local variables
  MDl        = 0;
  MDv        = 0;
  Ts        = 0;
  SI        = 0;
  SD        = 0;

  //it really doesn't make sense to get the saturated liquid enthalpy for
  //a mixture.  We keep the variable uniary just for this
  //ocassion
  if( !uniary )
  {
    AddMessage( messagelist, 1, 
      "Saturated liquid entropy calculatation is not valid for a mixture",
      "" );
    return -1;
  }

  //Retrieve the saturation pressure
  Ts        = GetVaporTemperature_P_USCS( P, &MDl, &MDv );

  //Check to see if we succeeded.
  if( Ts <= 0 )
    return -1;

  //Calculate the entropy
  SI          = GetSIdeal_USCS( Ts );
  SD          = GetSDepart_MT_USCS( MDl, Ts );

  //we must have made it
  return ( SI + SD );

}

 /*****************************************
 * Method Definition: GetSatLiqS_T_USCS *
 *----------------------------------------************
 * This function solves for the entropy of       *
 * a single fluid at saturated liquid conditions.    *
 * It does not apply to mixtures.           *
 ****************************************************/
double CEOSMODEL::GetSatLiqS_T_USCS( double T )
{
  //local variables
  double      MDl;
  double      MDv;
  double      Ps;
  double      SI;
  double      SD;

  //Initialize local variables
  MDl        = 0;
  MDv        = 0;
  Ps        = 0;
  SI        = 0;
  SD        = 0;

  //it really doesn't make sense to get the saturated liquid enthalpy for
  //a mixture.  We keep the variable uniary just for this
  //ocassion
  if( !uniary )
  {
    AddMessage( messagelist, 1, 
      "Saturated liquid entropy calculatation is not valid for a mixture",
      "" );
    return -1;
  }

  //Retrieve the saturation pressure
  Ps        = GetVaporPressure_T_USCS( T, &MDl, &MDv );

  //Check to see if we succeeded.
  if( Ps <= 0 )
    return -1;

  //Calculate the entropy
  SI          = GetSIdeal_USCS( T );
  SD          = GetSDepart_MT_USCS( MDl, T );

  //we must have made it
  return ( SI + SD );

}

 /***************************************
 * Method Definition: GetSatLiqV_P_SI *
 *--------------------------------------**************
 * This function solves for the specific volume of   *
 * a single fluid at saturated liquid conditions.    *
 * It does not apply to mixtures.           *
 ****************************************************/
double CEOSMODEL::GetSatLiqV_P_SI( double P )
{
  //local variables
  double        V;

  //initialize local variables
  V          = 0;

  //convert from SI to USCS
  P          = ( P * units.convert( units.bar, units.PSIA ) );

  //call solver
  V          = GetSatLiqV_P_USCS( P );

  //convert from USCS to SI
  V          = ( V * units.convert( units.ft3, units.cm3 ) );
  V          = ( V * units.convert( units.g, units.lb ) );

  //return the value
  return V;
}

 /*****************************************
 * Method Definition: GetSatLiqV_P_USCS *
 *----------------------------------------************
 * This function solves for the specific volume of   *
 * a single fluid at saturated liquid conditions.    *
 * It does not apply to mixtures.           *
 ****************************************************/
double CEOSMODEL::GetSatLiqV_P_USCS( double P )
{
  //local variables
  double      MDl;
  double      MDv;
  double      Ts;
  double      V;

  //Initialize local variables
  MDl        = 0;
  MDv        = 0;
  Ts        = 0;
  V        = 0;

  //it really doesn't make sense to get the vapor pressure of
  //a mixture.  We keep the variable uniary just for this
  //ocassion
  if( !uniary )
  {
    AddMessage( messagelist, 1, 
      "Saturated liquid specific volume calculatation is not valid for a mixture",
      "" );
    return -1;
  }

  //Retrieve the saturation pressure
  Ts        = GetVaporTemperature_P_USCS( P, &MDl, &MDv );

  //Check to see if we succeeded.
  if( Ts <= 0 )
    return -1;

  //invert for the volume
  V        = ( 1/ MDl );

  //check that the molecular weight is not zero
  if( Mixture.molewt_mx == 0 )
    return -1;

  //convert from lb-mols to lbm
  V        = ( V / Mixture.molewt_mx );
  
  //all done
  return V;

}

 /***************************************
 * Method Definition: GetSatLiqV_T_SI *
 *--------------------------------------**************
 * This function solves for the specific volume of   *
 * a single fluid at saturated liquid conditions in  *
 * SI units. It does not apply to mixtures.       *
 ****************************************************/
double CEOSMODEL::GetSatLiqV_T_SI( double T )
{

  //local variables
  double        v;

  //initialize local variables
  v          = 0;

  //Convert from SI to USCS
  T          = ( T * units.convert( units.K, units.R ) );

  //Call the entropy solver
  v          = GetSatLiqV_T_USCS( T );

  //Convert to SI equivalent
  v          = ( v * units.convert( units.ft3, units.cm3 ) );
  v          = ( v * units.convert( units.g, units.lb ) );

  //return the specific volume
  return v;

}

 /*****************************************
 * Method Definition: GetSatLiqV_T_USCS *
 *----------------------------------------************
 * This function solves for the specific volume of   *
 * a single fluid at saturated liquid conditions.    *
 * It does not apply to mixtures.           *
 ****************************************************/
double CEOSMODEL::GetSatLiqV_T_USCS( double T )
{
  //local variables
  double      MDl;
  double      MDv;
  double      Ps;
  double      V;

  //Initialize local variables
  MDl        = 0;
  MDv        = 0;
  Ps        = 0;
  V        = 0;

  //it really doesn't make sense to get the vapor pressure of
  //a mixture.  We keep the variable uniary just for this
  //ocassion
  if( !uniary )
  {
    AddMessage( messagelist, 1, 
      "Saturated liquid specific volume calculatation is not valid for a mixture",
      "" );
    return -1;
  }

  //Retrieve the saturation pressure
  Ps        = GetVaporPressure_T_USCS( T, &MDl, &MDv );

  //Check to see if we succeeded.
  if( Ps <= 0 )
    return -1;

  //invert for the volume
  V        = ( 1/ MDl );

  //check that the molecular weight is not zero
  if( Mixture.molewt_mx == 0 )
    return -1;

  //convert from lb-mols to lbm
  V        = ( V / Mixture.molewt_mx );
  
  //all done
  return V;

}

 /***************************************
 * Method Definition: GetSatVapH_P_SI *
 *--------------------------------------**************
 * This function solves for the enthalpy of       *
 * a single fluid at saturated vapor conditions.     *
 * It does not apply to mixtures.           *
 ****************************************************/
double CEOSMODEL::GetSatVapH_P_SI( double P )
{
  //local variables
  double        H;

  //initialize local variables
  H          = 0;

  //convert from SI to USCS
  P          = ( P * units.convert( units.bar, units.PSIA ) );

  //call solver
  H          = GetSatVapH_P_USCS( P );

  //convert from USCS to SI
  H          = ( H * units.convert( units.BTU, units.J ) );
  H          = ( H * units.convert( units.g, units.lb ) );

  //return the value
  return H;
}

 /*****************************************
 * Method Definition: GetSatVapH_P_USCS *
 *----------------------------------------************
 * This function solves for the enthalpy of       *
 * a single fluid at saturated vapor conditions     *
 * Given pressure.  It does not apply to mixtures.   *
 ****************************************************/
double CEOSMODEL::GetSatVapH_P_USCS( double P )
{
  //local variables
  double      MDl;
  double      MDv;
  double      Ts;
  double      HI;
  double      HD;

  //Initialize local variables
  MDl        = 0;
  MDv        = 0;
  Ts        = 0;
  HI        = 0;
  HD        = 0;

  //it really doesn't make sense to get the saturated liquid enthalpy for
  //a mixture.  We keep the variable uniary just for this
  //ocassion
  if( !uniary )
  {
    AddMessage( messagelist, 1, 
      "Saturated vapor enthaply calculatation is not valid for a mixture",
      "" );
    return -1;
  }

  //Retrieve the saturation pressure
  Ts        = GetVaporTemperature_P_USCS( P, &MDl, &MDv );

  //Check to see if we succeeded.
  if( Ts <= 0 )
    return -1;

  //Calculate the enthalpy
  //Calculate the entropy for the initial guess
  HI          = GetHIdeal_USCS( Ts );
  HD          = GetHDepart_MT_USCS( MDv, Ts );

  //we must have made it
  return ( HI + HD );

}

 /***************************************
 * Method Definition: GetSatVapH_T_SI *
 *--------------------------------------**************
 * This function solves for the enthalpy of       *
 * a single fluid at saturated liquid conditions in  *
 * SI units. It does not apply to mixtures.       *
 ****************************************************/
double CEOSMODEL::GetSatVapH_T_SI( double T )
{

  //local variables
  double        H;

  //initialize local variables
  H          = 0;

  //Convert from SI to USCS
  T          = ( T * units.convert( units.K, units.R ) );

  //Call the entropy solver
  H          = GetSatVapH_T_USCS( T );

  //Convert to SI equivalent
  H          = ( H * units.convert( units.BTU, units.J ) );
  H          = ( H * units.convert( units.g, units.lb ) );

  //return the enthalpy
  return H;

}

 /*****************************************
 * Method Definition: GetSatVapH_T_USCS *
 *----------------------------------------************
 * This function solves for the enthalpy of       *
 * a single fluid at saturated vapor conditions.   *
 * It does not apply to mixtures.           *
 ****************************************************/
double CEOSMODEL::GetSatVapH_T_USCS( double T )
{
  //local variables
  double      MDl;
  double      MDv;
  double      Ps;
  double      HI;
  double      HD;

  //Initialize local variables
  MDl        = 0;
  MDv        = 0;
  Ps        = 0;
  HI        = 0;
  HD        = 0;

  //it really doesn't make sense to get the saturated liquid enthalpy for
  //a mixture.  We keep the variable uniary just for this
  //ocassion
  if( !uniary )
  {
    AddMessage( messagelist, 1, 
      "Saturated vapor enthaply calculatation is not valid for a mixture",
      "" );
    return -1;
  }

  //Retrieve the saturation pressure
  Ps        = GetVaporPressure_T_USCS( T, &MDl, &MDv );

  //Check to see if we succeeded.
  if( Ps <= 0 )
    return -1;

  //Calculate the enthalpy
  //Calculate the entropy for the initial guess
  HI          = GetHIdeal_USCS( T );
  HD          = GetHDepart_MT_USCS( MDv, T );

  //Check for a valid response from the solver
  if( HD == 0 )
  {
    AddMessage( messagelist, 1, 
            "Enthalpy departure call in GetSatVapH_T_USCS failed to converge",
            "Check the fluid temperature and pressure" );
    return 0;
  }

  //we must have made it
  return ( HI + HD );

}

/***************************************
 * Method Definition: GetSatVapS_P_SI *
 *--------------------------------------**************
 * This function solves for the entropy of       *
 * a single fluid at saturated vapor conditions.     *
 * It does not apply to mixtures.           *
 ****************************************************/
double CEOSMODEL::GetSatVapS_P_SI( double P )
{
  //local variables
  double        S;

  //initialize local variables
  S          = 0;

  //convert from SI to USCS
  P          = ( P * units.convert( units.bar, units.PSIA ) );

  //call solver
  S          = GetSatVapS_P_USCS( P );

  //convert from USCS to SI
  S          = ( S * units.convert( units.BTU, units.J ) );
  S          = ( S * units.convert( units.g, units.lb ) );
  S          = ( S * units.convert( units.K, units.R ) );

  //return the value
  return S;
}

 /*****************************************
 * Method Definition: GetSatVapS_P_USCS *
 *----------------------------------------************
 * This function solves for the entropy         *
 * a single fluid at saturated vapor conditions     *
 * given pressure.  It does not apply to mixtures.   *
 ****************************************************/
double CEOSMODEL::GetSatVapS_P_USCS( double P )
{
  //local variables
  double      MDl;
  double      MDv;
  double      Ts;
  double      SI;
  double      SD;

  //Initialize local variables
  MDl        = 0;
  MDv        = 0;
  Ts        = 0;
  SI        = 0;
  SD        = 0;

  //it really doesn't make sense to get the saturated liquid enthalpy for
  //a mixture.  We keep the variable uniary just for this
  //ocassion
  if( !uniary )
  {
    AddMessage( messagelist, 1, 
      "Saturated vapor entropy calculatation is not valid for a mixture",
      "" );
    return -1;
  }

  //Retrieve the saturation pressure
  Ts        = GetVaporTemperature_P_USCS( P, &MDl, &MDv );

  //Check to see if we succeeded.
  if( Ts <= 0 )
    return -1;

  //Calculate the entropy
  SI          = GetSIdeal_USCS( Ts );
  SD          = GetSDepart_MT_USCS( MDv, Ts );

  //we must have made it
  return ( SI + SD );

}

 /*****************************************
 * Method Definition: GetSatVapS_T_USCS *
 *----------------------------------------************
 * This function solves for the entropy         *
 * a single fluid at saturated vapor conditions.   *
 * It does not apply to mixtures.           *
 ****************************************************/
double CEOSMODEL::GetSatVapS_T_USCS( double T )
{
  //local variables
  double      MDl;
  double      MDv;
  double      Ps;
  double      SI;
  double      SD;

  //Initialize local variables
  MDl        = 0;
  MDv        = 0;
  Ps        = 0;
  SI        = 0;
  SD        = 0;

  //it really doesn't make sense to get the saturated liquid enthalpy for
  //a mixture.  We keep the variable uniary just for this
  //ocassion
  if( !uniary )
  {
    AddMessage( messagelist, 1, 
      "Saturated vapor entropy calculatation is not valid for a mixture",
      "" );
    return -1;
  }

  //Retrieve the saturation pressure
  Ps        = GetVaporPressure_T_USCS( T, &MDl, &MDv );

  //Check to see if we succeeded.
  if( Ps <= 0 )
    return -1;

  //Calculate the entropy
  SI          = GetSIdeal_USCS( T );
  SD          = GetSDepart_MT_USCS( MDv, T );

  //we must have made it
  return ( SI + SD );

}

 /***************************************
 * Method Definition: GetSatVapV_P_SI *
 *--------------------------------------**************
 * This function solves for the specific volume of   *
 * a single fluid at saturated vapor conditions.     *
 * It does not apply to mixtures.           *
 ****************************************************/
double CEOSMODEL::GetSatVapV_P_SI( double P )
{
  //local variables
  double        V;

  //initialize local variables
  V          = 0;

  //convert from SI to USCS
  P          = ( P * units.convert( units.bar, units.PSIA ) );

  //call solver
  V          = GetSatVapV_P_USCS( P );

  //convert from USCS to SI
  V          = ( V * units.convert( units.ft3, units.cm3 ) );
  V          = ( V * units.convert( units.g, units. lb ) );

  //return the value
  return V;
}

 /*****************************************
 * Method Definition: GetSatVapV_P_USCS *
 *----------------------------------------************
 * This function solves for the specific volume of   *
 * a single fluid at saturated vapor conditions.     *
 * It does not applty to mixtures.           *
 ****************************************************/
double CEOSMODEL::GetSatVapV_P_USCS( double P )
{
  //local variables
  double      MDl;
  double      MDv;
  double      Ts;
  double      V;

  //Initialize local variables
  MDl        = 0;
  MDv        = 0;
  Ts        = 0;
  V        = 0;

  //it really doesn't make sense to get the vapor pressure of
  //a mixture.  We keep the variable uniary just for this
  //ocassion
  if( !uniary )
  {
    AddMessage( messagelist, 1, 
      "Saturated Liquid Specific volume calculatation is not valid for a mixture",
      "" );
    return -1;
  }

  //Retrieve the saturation pressure
  Ts        = GetVaporTemperature_P_USCS( P, &MDl, &MDv );

  //Check to see if we succeeded.
  if( Ts <= 0 )
    return -1;

  //invert for the volume
  V        = ( 1/ MDv );

  //check that the molecular weight is not zero
  if( Mixture.molewt_mx == 0 )
    return -1;

  //convert from lb-mols to lbm
  V        = ( V / Mixture.molewt_mx );
  
  //all done
  return V;

}

 /***************************************
 * Method Definition: GetSatVapV_T_SI *
 *--------------------------------------**************
 * This function solves for the specific volume of   *
 * a single fluid at saturated vapor conditions in   *
 * SI units. It does not apply to mixtures.       *
 ****************************************************/
double CEOSMODEL::GetSatVapV_T_SI( double T )
{

  //local variables
  double        v;

  //initialize local variables
  v          = 0;

  //Convert from SI to USCS
  T          = ( T * units.convert( units.K, units.R ) );

  //Call the entropy solver
  v          = GetSatVapV_T_USCS( T );

  //Convert to SI equivalent
  v          = ( v * units.convert( units.ft3, units.cm3 ) );
  v          = ( v * units.convert( units.g, units.lb ) );

  //return the enthalpy
  return v;

}

 /*****************************************
 * Method Definition: GetSatVapV_T_USCS *
 *----------------------------------------************
 * This function solves for the specific volume of   *
 * a single fluid at saturated vapor conditions.     *
 * It does not applty to mixtures.           *
 ****************************************************/
double CEOSMODEL::GetSatVapV_T_USCS( double T )
{
  //local variables
  double      MDl;
  double      MDv;
  double      Ps;
  double      V;

  //Initialize local variables
  MDl        = 0;
  MDv        = 0;
  Ps        = 0;
  V        = 0;

  //it really doesn't make sense to get the vapor pressure of
  //a mixture.  We keep the variable uniary just for this
  //ocassion
  if( !uniary )
  {
    AddMessage( messagelist, 1, 
      "Saturated Liquid Specific volume calculatation is not valid for a mixture",
      "" );
    return -1;
  }

  //Retrieve the saturation pressure
  Ps        = GetVaporPressure_T_USCS( T, &MDl, &MDv );

  //Check to see if we succeeded.
  if( Ps <= 0 )
    return -1;

  //invert for the volume
  V        = ( 1/ MDv );

  //check that the molecular weight is not zero
  if( Mixture.molewt_mx == 0 )
    return -1;

  //convert from lb-mols to lbm
  V        = ( V / Mixture.molewt_mx );
  
  //all done
  return V;


}

/***********************************
 * Method Definition : Getsfo_SI *
 *---------------------------------*******************
 * This function returns the entropy of formation    *
 * in SI units for the fluid fluidindex         *
 ****************************************************/
double CEOSMODEL::Getsfo_SI( int fluidindex )
{

  //Check for valid data
  if( !ValidityCheckFluid ( fluidindex ) )
  {
    AddMessage( messagelist, 50, 
      "Getsfo_SI failed validity check",
      "" );
    return 0;
  }

  return FluidList[( fluidindex - 1 )].sfo_SI;
}

/***********************************
 * Method Definition : Getsfo_SI *
 *---------------------------------*******************
 * This function returns the entropy of formation    *
 * in SI units for the mixture             *
 ****************************************************/
double CEOSMODEL::Getsfo_SI( )
{

  return Mixture.sfo_SI_mx;

}

/*************************************
 * Method Definition : Getsfo_USCS *
 *-----------------------------------*****************
 * This function returns the entropy of formation    *
 * in USCS units for the fluid fluidindex       *
 ****************************************************/
double CEOSMODEL::Getsfo_USCS( int fluidindex )
{

  //Check for valid data
  if( !ValidityCheckFluid ( fluidindex ) )
  {
    AddMessage( messagelist, 50, 
      "Getsfo_USCS failed validity check",
      "" );
    return 0;
  }

  return FluidList[( fluidindex - 1 )].sfo_USCS;
}

/*************************************
 * Method Definition : Getsfo_USCS *
 *-----------------------------------*****************
 * This function returns the entropy of formation    *
 * in USCS units for the mixture           *
 ****************************************************/
double CEOSMODEL::Getsfo_USCS( )
{

  return Mixture.sfo_USCS_mx;

}

/*******************************************
 * Method Definition : GetTempCrit_mx_SI *
 *------------------------------------------**********
 * This function returns the psuedo critical     *
 * temperature (in SI) for the mixture         *
 ****************************************************/
double CEOSMODEL::GetTempCrit_mx_SI()
{

  //all work in the object is done is USCS and coverted
  //at the output
  return Mixture.Tc_mx * units.convert(units.R, units.K);

}

/*********************************************
 * Method Definition : GetTempCrit_mx_USCS *
 *-------------------------------------------*********
 * This function returns the critical         *
 * temperature (in USCS) for the fluid at index      *
 * fluidindex                       *
 ****************************************************/
double CEOSMODEL::GetTempCrit_mx_USCS()
{

  return Mixture.Tc_mx;

}

/***************************************
 * Method Definition : GetTempCrit_SI*
 *-------------------------------------***************
 * This function returns the critical         *
 * temperature (in SI) for the fluid at index      *
 * fluidindex                       *
 ****************************************************/
double CEOSMODEL::GetTempCrit_SI( int fluidindex )
{
  //Check for valid data
  if( !ValidityCheckFluid ( fluidindex ) )
  {
    AddMessage( messagelist, 1, 
            "GetTempCrit_SI ValidityCheckFluid failed",
            "" );
    return NULL;
  }

  return FluidList[( fluidindex - 1 )].Tc_SI;
}

/********************************************
 * Method Definition : GetTempCrit_USCS   *
 *------------------------------------------**********
 * This function returns the critical         *
 * temperature (in USCS) for the fluid at index      *
 * fluidindex                       *
 ****************************************************/
double CEOSMODEL::GetTempCrit_USCS( int fluidindex )
{
  //local variables
  class        CMeshan *currentElement;

  //Initialize local variables
  currentElement    = NULL;

  //Check for valid data
  if( !ValidityCheckFluid ( fluidindex ) )
  {
    AddMessage( messagelist, 50, 
            "GetTempCrit_USCS ValidityCheckFluid failed",
            "" );
    return NULL;
  }

  return FluidList[( fluidindex - 1 )].Tc;
}

/************************************
 * Method Definition : GetTmax_SI *
 *----------------------------------******************
 * This function returns the maximum temperature the *
 * EOS engine correlation matches the measured data  *
 * for the fluid at fluindex in SI units       *
 ****************************************************/
double CEOSMODEL::GetTmax_SI( int fluidindex )
{
  //local variables
  class        CMeshan *currentElement;
  double        T;

  //Initialize local variables
  currentElement    = NULL;
  T          = 0;

  //Check for valid data
  if( !ValidityCheckFluid ( fluidindex ) )
  {
    AddMessage( messagelist, 50, 
            "GetTmax_SI ValidityCheckFluid failed",
            "" );
    return NULL;
  }

  //retrieve the USCS value
  T          = FluidList[( fluidindex - 1 )].Tmax_USCS;

  //convert to SI
  T          = ( T * units.convert( units.R, units.K ) );

  //return the value
  return T;
}

/**************************************
 * Method Definition : GetTmax_USCS *
 *------------------------------------****************
 * This function returns the maximum temperature the *
 * EOS engine correlation matches the measured data  *
 * for the fluid at fluindex in USCS units.       *
 ****************************************************/
double CEOSMODEL::GetTmax_USCS( int fluidindex )
{
  //local variables
  class        CMeshan *currentElement;
  double        T;

  //Initialize local variables
  currentElement    = NULL;
  T          = 0;

  //Check for valid data
  if( !ValidityCheckFluid ( fluidindex ) )
  {
    AddMessage( messagelist, 50, 
            "GetTmax_USCS ValidityCheckFluid failed",
            "" );
    return NULL;
  }

  //retrieve the USCS value
  T          = FluidList[( fluidindex - 1 )].Tmax_USCS;

  //return the value
  return T;
}

/************************************
 * Method Definition : GetTmin_SI *
 *----------------------------------******************
 * This function returns the minimum temperature the *
 * EOS engine correlation matches the measured data  *
 * for the fluid at fluindex in SI units       *
 ****************************************************/
double CEOSMODEL::GetTmin_SI( int fluidindex )
{
  //local variables
  class        CMeshan *currentElement;
  double        T;

  //Initialize local variables
  currentElement    = NULL;
  T          = 0;

  //Check for valid data
  if( !ValidityCheckFluid ( fluidindex ) )
  {
    AddMessage( messagelist, 50, 
            "GetTmin_SI ValidityCheckFluid failed",
            "" );
    return NULL;
  }

  //retrieve the USCS value
  T          = FluidList[( fluidindex - 1 )].Tmin_USCS;

  //convert to SI
  T          = ( T * units.convert( units.R, units.K ) );

  //return the value
  return T;
}

/**************************************
 * Method Definition : GetTmin_USCS *
 *------------------------------------****************
 * This function returns the minimum temperature the *
 * EOS engine correlation matches the measured data  *
 * for the fluid at fluindex in USCS units.       *
 ****************************************************/
double CEOSMODEL::GetTmin_USCS( int fluidindex )
{
  //local variables
  class        CMeshan *currentElement;
  double        T;

  //Initialize local variables
  currentElement    = NULL;
  T          = 0;

  //Check for valid data
  if( !ValidityCheckFluid ( fluidindex ) )
  {
    AddMessage( messagelist, 50, 
            "GetTmin_USCS ValidityCheckFluid failed",
            "" );
    return NULL;
  }

  //retrieve the USCS value
  T          = FluidList[( fluidindex - 1 )].Tmin_USCS;

  //return the value
  return T;
}

/***************************************
 * Method Definition : GetTPGLIndex  *
 *-------------------------------------***************
 * This function returns the 'Properties of Gases and*
 * Liquids' (TPGL) index number             *
 ****************************************************/
int CEOSMODEL::GetTPGLIndex( int fluidindex )
{
  //Check for valid data
  if( !ValidityCheckFluid ( fluidindex ) )
  {
    AddMessage( messagelist, 50, 
            "GetTPGLIndex ValidityCheckFluid failed",
            "" );
    return -1;
  }

  return FluidList[( fluidindex - 1 )].Polingetl;
}

/******************************************
 * Method Definition : GetVolCrit_mx_SI *
 *----------------------------------------************
 * This function returns the psuedo critical     *
 * volume (in SI) for the mixture           *
 ****************************************************/
double CEOSMODEL::GetVolCrit_mx_SI()
{

  return ( Mixture.Vc_mx * units.convert( units.cm3, units.ft3 ) );

}

/********************************************
 * Method Definition : GetVolCrit_mx_USCS *
 *------------------------------------------**********
 * This function returns the psuedo critical     *
 * volume (in SI) for the mixture           *
 ****************************************************/
double CEOSMODEL::GetVolCrit_mx_USCS()
{

  return Mixture.Vc_mx;

}

/***************************************
 * Method Definition : GetVolCrit_SI *
 *-------------------------------------***************
 * This function returns the critical         *
 * volume (in SI) for the fluid at index       *
 * fluidindex                       *
 ****************************************************/
double CEOSMODEL::GetVolCrit_SI( int fluidindex )
{
  //Check for valid data
  if( !ValidityCheckFluid ( fluidindex ) )
  {
    AddMessage( messagelist, 50, 
            "GetVolCrit_SI ValidityCheckFluid failed",
            "" );
    return NULL;
  }

  return FluidList[( fluidindex - 1 )].Vc_SI;
}

/********************************************
 * Method Definition : GetVolCrit_USCS    *
 *------------------------------------------**********
 * This function returns the critical         *
 * volume (in USCS) for the fluid at index       *
 * fluidindex                       *
 ****************************************************/
double CEOSMODEL::GetVolCrit_USCS( int fluidindex )
{

  //Check for valid data
  if( !ValidityCheckFluid ( fluidindex ) )
  {
    AddMessage( messagelist, 50,
            "GetVolCrit_USCS ValidityCheckFluid failed",
            "" );
    return NULL;
  }

  return FluidList[( fluidindex - 1 )].Vc;
}

 /****************************************
 * Method Definition: SetMaxIterations *
 *---------------------------------------*************
 * This function loads the maximum number of     *
 * attempts or iterations the solver has to work   *
 ****************************************************/
bool CEOSMODEL::SetMaxIterations( int maxiters )
{
  //check the data
  if( maxiters < 10 )
  {
    AddMessage( messagelist, 1,
            "Maximum iterations was set too low (less than 10)",
            "Recommended maximum iteration count is 200" );
    return false;
  }

  //load the data
  MaxIterations    = maxiters;

  //all done
  return true;
}

/**************************************
* Method Definition: Set Mixture Data *
*-------------------------------------***************
* This function loads the mixture variables into    *
* each fluid's object.  The MixArray has percentages*
* (expressed as a decimal) for each of the fluids   *
****************************************************/
bool CEOSMODEL::SetMixtureData( double *MixArray )
{

     //Local variables
     int i;
     int j;
     double temp1;

     //Initialize local variables
     i = 0;
     j = 0;

     //initialize object variables
     uniary    = false;
     
     //Make sure the data is good
     if( !_bValidityCheckMixture( MixArray ) )
     {
          AddMessage( messagelist, 50, 
               "SetMixtureData ValidityCheckMixture failed",
               "" );
          return false;
     }
  
     //Transfer the mixture percentages to the fluid
     for(i=0; i<FluidCount; i++)
     {
          FluidList[i].percentmw  = MixArray[i];
          if( FluidList[i].percentmw  == 1.00 )
               uniary  = true;
     }

     //Calculate mixture mole weight
     Mixture.molewt_mx = 0;
     for(i=0; i<FluidCount; i++)
     {
          temp1 = ( FluidList[i].percentmw * FluidList[i].molewt );
          Mixture.molewt_mx = ( temp1 + Mixture.molewt_mx );
     }  

     //Calculate the mixture coefficient for each fluid.
     //Start with the Bo coefficient
     Mixture.Bo_mx = 0;
     for(i=0; i<FluidCount; i++)
     {
          temp1 = ( FluidList[i].percentmw * FluidList[i].Bo );
          Mixture.Bo_mx = ( Mixture.Bo_mx + temp1 );
     }

     //Now for the Ao coefficient
     Mixture.Ao_mx = 0;
     for(i=0; i<FluidCount; i++)
     {
          for( j=0; j<FluidCount; j++)
          {
               temp1 = ( FluidList[i].percentmw * FluidList[j].percentmw );
               temp1 = ( temp1 * pow(FluidList[i].Ao, 0.5) * pow(FluidList[j].Ao, 0.5 ) );
               temp1 = ( temp1 * (1 - _k[(i*FluidCount)+j]) );
               Mixture.Ao_mx = ( Mixture.Ao_mx + temp1 );
          }
     }

     //And the Co coefficient
     Mixture.Co_mx = 0;
     for(i=0; i<FluidCount; i++)
     {
          for( j=0; j<FluidCount; j++ )
          {
               temp1 = ( FluidList[i].percentmw * FluidList[j].percentmw );
               temp1 = ( temp1 * pow( FluidList[i].Co, 0.5 ) * pow( FluidList[j].Co, 0.5 ) );
               temp1 = ( temp1 * (1 - _k[(i*FluidCount)+j]) * (1 - _k[(i*FluidCount)+j]) * (1 - _k[(i*FluidCount)+j]) );
               Mixture.Co_mx = ( Mixture.Co_mx + temp1 );

          }
     }

     //And the Do coefficient
     Mixture.Do_mx = 0;
     for(i=0; i<FluidCount; i++)
     {
          
          for( j=0; j<FluidCount; j++ )
          {
               temp1 = ( FluidList[i].percentmw * FluidList[j].percentmw );
               temp1 = ( temp1 * pow( FluidList[i].Do, 0.5 ) * pow( FluidList[j].Do, 0.5 ) );
               temp1 = ( temp1 * pow( (1 - _k[(i*FluidCount)+j]), 4 ) );
               Mixture.Do_mx = ( Mixture.Do_mx + temp1 );

          }

     }

     //And the Eo coefficient
     Mixture.Eo_mx = 0;
     for(i=0; i<FluidCount; i++)
     {
          for( j=0; j<FluidCount; j++ )
          {
               temp1 = ( FluidList[i].percentmw * FluidList[j].percentmw );
               temp1 = ( temp1 * pow( FluidList[i].Eo, 0.5 ) * pow( FluidList[j].Eo, 0.5 ) );
               temp1 = ( temp1 * pow( (1 - _k[(i*FluidCount)+j]), 5 ) );
               Mixture.Eo_mx = ( Mixture.Eo_mx + temp1 );

          }
     }

     //And the b coefficient
     Mixture.b_mx = 0;
     for(i=0; i<FluidCount; i++)
     {
          temp1 = pow( FluidList[i].b, 0.3333333333333 );
          temp1 = ( temp1 * FluidList[i].percentmw );
          Mixture.b_mx = ( Mixture.b_mx + temp1 );
     }
     Mixture.b_mx = ( Mixture.b_mx * Mixture.b_mx * Mixture.b_mx );

     //And the a coefficient
     Mixture.a_mx = 0;
     for(i=0; i<FluidCount; i++)
     {
          temp1 = pow( FluidList[i].a, 0.333333333333333 );
          temp1 = ( temp1 * FluidList[i].percentmw );
          Mixture.a_mx = ( Mixture.a_mx + temp1 );
     }
     Mixture.a_mx = ( Mixture.a_mx * Mixture.a_mx * Mixture.a_mx );

     //And the d coefficient
     Mixture.d_mx = 0;
     for(i=0; i<FluidCount; i++)
     {
          temp1 = pow( FluidList[i].d, 0.333333333333333333 );
          temp1 = ( temp1 * FluidList[i].percentmw );
          Mixture.d_mx = ( Mixture.d_mx + temp1 );
     }
     Mixture.d_mx = ( Mixture.d_mx * Mixture.d_mx * Mixture.d_mx );

     //And the alpha coefficient
     Mixture.alpha_mx = 0;
     for(i=0; i<FluidCount; i++)
     {
          temp1 = pow( FluidList[i].alpha, 0.33333333333333333 );
          temp1 = ( temp1 * FluidList[i].percentmw );
          Mixture.alpha_mx = ( Mixture.alpha_mx + temp1 );
     }
     Mixture.alpha_mx = ( Mixture.alpha_mx * Mixture.alpha_mx * Mixture.alpha_mx);

     //And the c coefficient
     Mixture.c_mx = 0;
     for(i=0; i<FluidCount; i++)
     {
          temp1 = pow( FluidList[i].c, 0.3333333333333333333 );
          temp1 = ( temp1 * FluidList[i].percentmw );
          Mixture.c_mx = ( temp1 + Mixture.c_mx );
     }
     Mixture.c_mx = ( Mixture.c_mx * Mixture.c_mx * Mixture.c_mx );

     //And the gamma coefficient
     Mixture.gamma_mx = 0;
     for(i=0; i<FluidCount; i++)
     {
          temp1 = pow( FluidList[i].gamma, 0.5 );
          temp1 = ( FluidList[i].percentmw * temp1 );
          Mixture.gamma_mx = ( Mixture.gamma_mx + temp1 );
     }
     Mixture.gamma_mx = ( Mixture.gamma_mx * Mixture.gamma_mx );

     //and the A coefficient
     Mixture.A_mx = 0;
     for(i=0; i<FluidCount; i++)
     {
          FluidList[i].A_mx = FluidList[i].percentmw * FluidList[i].A;
          Mixture.A_mx = Mixture.A_mx + FluidList[i].A_mx;
     }  

     //and the B coefficient
     Mixture.B_mx = 0;
     for(i=0; i<FluidCount; i++)
     {
          FluidList[i].B_mx = FluidList[i].percentmw * FluidList[i].B;
          Mixture.B_mx = Mixture.B_mx + FluidList[i].B_mx;
     }  

     //and the C coefficient
     Mixture.C_mx = 0;
     for(i=0; i<FluidCount; i++)
     {
          FluidList[i].C_mx = FluidList[i].percentmw * FluidList[i].C;
          Mixture.C_mx = Mixture.C_mx + FluidList[i].C_mx;
     }  

     //and the D coefficient
     Mixture.D_mx = 0;
     for(i=0; i<FluidCount; i++)
     {
          FluidList[i].D_mx = FluidList[i].percentmw * FluidList[i].D;
          Mixture.D_mx = Mixture.D_mx + FluidList[i].D_mx;
     }  
     
     //and the E coefficient
     Mixture.E_mx = 0;
     for(i=0; i<FluidCount; i++)
     {
          FluidList[i].E_mx = FluidList[i].percentmw * FluidList[i].E;
          Mixture.E_mx = Mixture.E_mx + FluidList[i].E_mx;
     }  

     //and the F coefficient
     Mixture.F_mx = 0;
     for(i=0; i<FluidCount; i++)
     {
          FluidList[i].F_mx = FluidList[i].percentmw * FluidList[i].F;
          Mixture.F_mx = Mixture.F_mx + FluidList[i].F_mx;
     }  

     //and the G coefficient
     Mixture.G_mx = 0;
     for(i=0; i<FluidCount; i++)
     {
          FluidList[i].G_mx    = FluidList[i].percentmw * FluidList[i].G;
          Mixture.G_mx      = Mixture.G_mx + FluidList[i].G_mx;
     }  
     
     //Begin with critical volume
     Mixture.Vc_mx = 0;
     for(i=0; i<FluidCount; i++)
     {
          FluidList[i].Vc_mx    = FluidList[i].percentmw * FluidList[i].Vc;
          Mixture.Vc_mx      = Mixture.Vc_mx + FluidList[i].Vc_mx;
     }  
     
     //Begin with critical temperature
     Mixture.Tc_mx = 0;
     for(i=0; i<FluidCount; i++)
     {
          //calculate the percent mole weight contribution
          FluidList[i].Tc_mx    = FluidList[i].percentmw * FluidList[i].Tc;
          Mixture.Tc_mx      = Mixture.Tc_mx + FluidList[i].Tc_mx;
     }  
     
     //Begin with critical pressure
     Mixture.Pc_mx = 0;
     for(i=0; i<FluidCount; i++)
     {
          FluidList[i].Pc_mx    = FluidList[i].percentmw * FluidList[i].Pc;
          Mixture.Pc_mx      = Mixture.Pc_mx + FluidList[i].Pc_mx;
     }  
     
     //and now omega
     Mixture.omega_mx = 0;
     for(i=0; i<FluidCount; i++)
     {
          FluidList[i].omega_mx  = FluidList[i].percentmw * FluidList[i].omega;
          Mixture.omega_mx    = Mixture.omega_mx + FluidList[i].omega_mx;
     }  
     
     //and now the gas constant
     Mixture.R_USCS_mx = 0;
     for(i=0; i<FluidCount; i++)
     {
          FluidList[i].R_USCS_mx  = FluidList[i].percentmw * FluidList[i].R_USCS;
          Mixture.R_USCS_mx    = Mixture.R_USCS_mx + FluidList[i].R_USCS_mx;
     }  
     
     //the ideal gas enthalp of formation (SI Units)
     Mixture.hfo_SI_mx      = 0;
     for(i=0; i<FluidCount; i++)
     {
          FluidList[i].hfo_SI_mx  = FluidList[i].percentmw * FluidList[i].hfo_SI;
          Mixture.hfo_SI_mx    = Mixture.hfo_SI_mx + FluidList[i].hfo_SI_mx;
     }  
     
     //the ideal gas enthalpy of formation (USCS Units)
     Mixture.hfo_USCS_mx      = 0;
     for(i=0; i<FluidCount; i++)
     {
          FluidList[i].hfo_USCS_mx= FluidList[i].percentmw * FluidList[i].hfo_USCS;
          Mixture.hfo_USCS_mx    = Mixture.hfo_USCS_mx + FluidList[i].hfo_USCS_mx;
     }  
     
     //the ideal gas entropy of formation (SI Units)
     Mixture.sfo_SI_mx      = 0;
     for(i=0; i<FluidCount; i++)
     {
          FluidList[i].sfo_SI_mx  = FluidList[i].percentmw * FluidList[i].sfo_SI;
          Mixture.sfo_SI_mx    = Mixture.sfo_SI_mx + FluidList[i].sfo_SI_mx;
     }  
     
     //the ideal gas entropy of formation (USCS Units)
     Mixture.sfo_USCS_mx      = 0;
     for(i=0; i<FluidCount; i++)
     {
          FluidList[i].sfo_USCS_mx= FluidList[i].percentmw * FluidList[i].sfo_USCS;
          Mixture.sfo_USCS_mx    = Mixture.sfo_USCS_mx + FluidList[i].sfo_USCS_mx;
     }  
     
     //Mixture data for JANAF enthalpy coefficents
     //T5 coefficient
     Mixture.HT5_mx        = 0;
     for(i=0; i<FluidCount; i++)
     {
          FluidList[i].HT5_mx    = FluidList[i].percentmw * FluidList[i].HT5;
          Mixture.HT5_mx      = Mixture.HT5_mx + FluidList[i].HT5_mx;
     }  
     
     //T4 coefficient
     Mixture.HT4_mx        = 0;
     for(i=0; i<FluidCount; i++)
     {
          FluidList[i].HT4_mx    = FluidList[i].percentmw * FluidList[i].HT4;
          Mixture.HT4_mx      = Mixture.HT4_mx + FluidList[i].HT4_mx;
     }  
     
     //T3 coefficient
     Mixture.HT3_mx        = 0;
     for(i=0; i<FluidCount; i++)
     {
          FluidList[i].HT3_mx    = FluidList[i].percentmw * FluidList[i].HT3;
          Mixture.HT3_mx      = Mixture.HT3_mx + FluidList[i].HT3_mx;
     }  
     
     //T2 coefficient
     Mixture.HT2_mx        = 0;
     for(i=0; i<FluidCount; i++)
     {
          FluidList[i].HT2_mx    = FluidList[i].percentmw * FluidList[i].HT2;
          Mixture.HT2_mx      = Mixture.HT2_mx + FluidList[i].HT2_mx;
     }  
     
     //T1 coefficient
     Mixture.HT1_mx        = 0;
     for(i=0; i<FluidCount; i++)
     {
          FluidList[i].HT1_mx    = FluidList[i].percentmw * FluidList[i].HT1;
          Mixture.HT1_mx      = Mixture.HT1_mx + FluidList[i].HT1_mx;
     }  
     
     //T0 coefficient
     Mixture.HT0_mx        = 0;
     for(i=0; i<FluidCount; i++)
     {
          FluidList[i].HT0_mx    = FluidList[i].percentmw * FluidList[i].HT0;
          Mixture.HT0_mx      = Mixture.HT0_mx + FluidList[i].HT0_mx;
     }  
     
     //Hn coefficient
     Mixture.Hn_mx        = 0;
     for(i=0; i<FluidCount; i++)
     {
          FluidList[i].Hn_mx    = FluidList[i].percentmw * FluidList[i].Hn;
          Mixture.Hn_mx      = Mixture.Hn_mx + FluidList[i].Hn_mx;
     }  
     
     //Mixture data for JANAF entropy coefficents
     //T5 coefficient
     Mixture.ST5_mx        = 0;
     for(i=0; i<FluidCount; i++)
     {
          FluidList[i].ST5_mx    = FluidList[i].percentmw * FluidList[i].ST5;
          Mixture.ST5_mx      = Mixture.ST5_mx + FluidList[i].ST5_mx;
     }  
     
     //T4 coefficient
     Mixture.ST4_mx        = 0;
     for(i=0; i<FluidCount; i++)
     {
          FluidList[i].ST4_mx    = FluidList[i].percentmw * FluidList[i].ST4;
          Mixture.ST4_mx      = Mixture.ST4_mx + FluidList[i].ST4_mx;
     }  
     
     //T3 coefficient
     Mixture.ST3_mx        = 0;
     for(i=0; i<FluidCount; i++)
     {
          FluidList[i].ST3_mx    = FluidList[i].percentmw * FluidList[i].ST3;
          Mixture.ST3_mx      = Mixture.ST3_mx + FluidList[i].ST3_mx;
     }  
     
     //T2 coefficient
     Mixture.ST2_mx        = 0;
     for(i=0; i<FluidCount; i++)
     {
          FluidList[i].ST2_mx    = FluidList[i].percentmw * FluidList[i].ST2;
          Mixture.ST2_mx      = Mixture.ST2_mx + FluidList[i].ST2_mx;
     }  
     
     //T1 coefficient
     Mixture.ST1_mx        = 0;
     for(i=0; i<FluidCount; i++)
     {
          FluidList[i].ST1_mx    = FluidList[i].percentmw * FluidList[i].ST1;
          Mixture.ST1_mx      = Mixture.ST1_mx + FluidList[i].ST1_mx;
     }  
     
     //T0 coefficient
     Mixture.ST0_mx        = 0;
     for(i=0; i<FluidCount; i++)
     {
          FluidList[i].ST0_mx    = FluidList[i].percentmw * FluidList[i].ST0;
          Mixture.ST0_mx      = Mixture.ST0_mx + FluidList[i].ST0_mx;
     }  
     
     //the lower heating value (SI Units)
     Mixture.LHV_SI_mx      = 0;
     for(i=0; i<FluidCount; i++)
     {
          FluidList[i].LHV_SI_mx  = FluidList[i].percentmw * FluidList[i].LHV_SI;
          Mixture.LHV_SI_mx    = Mixture.LHV_SI_mx + FluidList[i].LHV_SI_mx;
     }  
     
     //the lower heating value (USCS Units)
     Mixture.LHV_USCS_mx      = 0;
     for(i=0; i<FluidCount; i++)
     {
          FluidList[i].LHV_USCS_mx= FluidList[i].percentmw * FluidList[i].LHV_USCS;
          Mixture.LHV_USCS_mx    = Mixture.LHV_USCS_mx + FluidList[i].LHV_USCS_mx;
     }
     
     //the higher heating value (SI Units)
     Mixture.HHV_SI_mx      = 0;
     for(i=0; i<FluidCount; i++)
     {
          FluidList[i].HHV_SI_mx  = FluidList[i].percentmw * FluidList[i].HHV_SI;
          Mixture.HHV_SI_mx    = Mixture.LHV_SI_mx + FluidList[i].HHV_SI_mx;
     }  
     
     //the higher heating value (USCS Units)
     Mixture.HHV_USCS_mx      = 0;
     for(i=0; i<FluidCount; i++)
     {
          FluidList[i].HHV_USCS_mx= FluidList[i].percentmw * FluidList[i].HHV_USCS;
          Mixture.HHV_USCS_mx    = Mixture.HHV_USCS_mx + FluidList[i].HHV_USCS_mx;
     }

     //Success!
     return true;
}

 /************************************
 * Method Definition: SetPrecision *
 *-----------------------------------*****************
 * Once the two guesses in the solver are closer     *
 * than prec, solver is done
 ****************************************************/
bool CEOSMODEL::SetPrecision( double prec )
{
  //check the data
  if( prec <= 0 )
  {
    AddMessage( messagelist, 50,
            "Solver argument was too low (less than or equal to 0)",
            "Recommend setting is 0.000001" );
    return false;
  }

  //load the data
  Precision      = prec;

  //all done
  return true;
}

/**************************************
 * Method Definition: RemoveMessage *
 *------------------------------------****************
 * Remove a message from the message handler     *
 ****************************************************/
void CEOSMODEL::RemoveMessage(class CMeshan *element)
{
  //local variables
  class CMeshan *deleteElement, *currentElement;

  //initialize local variables
  deleteElement    = NULL;
  currentElement    = NULL;

  // Is the ELEMENT passed in the first element of the list?
  if(element == messagelist)
  {
    // Remember first element address to delete in a second
    deleteElement = messagelist;

    //Check to see if there is a second member
    if(element->next != NULL)
      // Set the head of the list to be the second member
      messagelist = messagelist->next;
    else
      messagelist = NULL;

    // See ya delete element!
    delete deleteElement;
    return;
  }

  // Find the element passed in by looking 
  // through the WHOLE list until we find it,
  // and checking to make aure we haven't hit 
  // the end of the list in case the element
  // passed in is not in the list.

  // Start at the head
  currentElement = messagelist;

  // Keep looking as long as the next value isn't the end of the list
  // and it isn't the one we want
  while(currentElement->next != NULL && currentElement->next != element)
    currentElement = currentElement->next;

  // Return if we didn't find it...
  if(currentElement->next == NULL)
    return;

  //...otherwise remember it for later...
  deleteElement = currentElement->next;

  //...point around it to it's next...
  currentElement->next = currentElement->next->next;

  // DELETE IT!
  delete currentElement;

}

/***********************
 * Method Definition *
 *---------------------*******************************
 * To avoid nasty errors, check the data before     *
 * devoting computing time to solving the EOS     *
 ****************************************************/
 bool CEOSMODEL::ValidityCheck()
 {

  //Bad data should not have gotten this far,
  //but just in case check to see that temperature
  //and pressure values are valid
  if( FluidCount < 25)
  {
    AddMessage( messagelist, 1,
            "The fluid count is too low",
            "" );
    return false;
  }
  if( Precision <= 0 )
  {
    AddMessage( messagelist, 1, 
            "The solver precision was set found to be less than or equal to 0",
            "" );
    return false;
  }
  if( Precision > 100 )
  {
    AddMessage( messagelist, 1,
            "The solver precision was set very high (greater than 100)",
            "Recommend setting is 0.00001" );
    return false;
  }
  if( MaxIterations < 1 )
  {
    AddMessage( messagelist, 1,
            "Maximum iterations were found to be less than 1",
            "Recommend setting is 200" );
    return false;
  }
  if( MaxIterations > 250000 )
  {
    AddMessage( messagelist, 1,
            "Maximum solver iterations were found to be high (more than 250000)",
            "Recommended maximum iterations setting is 200" );
    return false;
  }
  if( Mixture.Tc_mx <= 0 )
  {
    AddMessage( messagelist, 1,
            "Critical temperature of the mixture is below 0",
            "Check the fluid percent mole weights and fluid properties" );
    return false;
  }
  if( Mixture.Vc_mx <= 0 )
  {
    AddMessage( messagelist, 1,
            "Critical volume of the mixture is below 0",
            "Check the fluid percent mole weights and fluid properties" );
    return false;
  }
  if( Mixture.Pc_mx <= 0 )
  {
    AddMessage( messagelist, 1,
            "Critical pressure of the mixture is below 0",
            "Check the fluid percent mole weights and fluid properties" );
    return false;
  }
  
  //success
  return true;
 }


/***********************
 * Method Definition *
 *---------------------*******************************
 * To avoid nasty errors, check the fluid index     *
 ****************************************************/
bool CEOSMODEL::ValidityCheckFluid( int fluidindex )
{
  //Check for valid data
  if( fluidindex < 1 )
  {
    AddMessage( messagelist, 1, 
            "Fluid index was less than zero",
            "Check the fluid index number" );
    return false;
  }

  if( fluidindex > FluidCount )
  {
    AddMessage( messagelist, 1, 
            "Fluid index was greater than number of fluids in the EOS engine",
            "Check the fluid index number" );
    return false;
  }
  
  //We made it!
  return true;
}

/////////////////////////////////////////////////////////////////////
// Method Definition:  
//   Calculate the Ao parameter from fluid properties.
// 
// Source:
//   Starling, Kenneth E. Fluid Thermodynamic Properties 
//   for Light Petroleum Systems.  Houston: Gulf Pub., 1973. Print.
//
// Location:
//   Chapter 17, page 221, equation (14)
//
/////////////////////////////////////////////////////////////////////
bool CEOSMODEL::_bCalc_Ao( int iArrayIndex, double *Ao)
{

    if( fabs( FluidList[iArrayIndex].MDc ) < dblGetTrace() )
    {
        *Ao = 0.0;
        return false;
    }
    *Ao = ( A[2] + ( B[2] * FluidList[iArrayIndex].omega ) ) * ( ( r_USCS * FluidList[iArrayIndex].Tc ) / FluidList[iArrayIndex].MDc );

    return true;
}

/////////////////////////////////////////////////////////////////////
// Method Definition:  
//   Calculate the Bo parameter from fluid properties.
// 
// Source:
//   Starling, Kenneth E. Fluid Thermodynamic Properties 
//   for Light Petroleum Systems.  Houston: Gulf Pub., 1973. Print.
//
// Location:
//   Chapter 17, page 221, equation (13)
//
/////////////////////////////////////////////////////////////////////
bool CEOSMODEL::_bCalc_Bo( int iArrayIndex, double *Bo)
{
    if( fabs( FluidList[iArrayIndex].MDc ) < dblGetTrace() )
    {
        *Bo = 0.0;
        return false;
    }
    *Bo = ( A[1] + ( B[1] * FluidList[iArrayIndex].omega ) ) / FluidList[iArrayIndex].MDc;

    return true;
}

/////////////////////////////////////////////////////////////////////
// Method Definition:  
//   Calculate the Co parameter from fluid properties.
// 
// Source:
//   Starling, Kenneth E. Fluid Thermodynamic Properties 
//   for Light Petroleum Systems.  Houston: Gulf Pub., 1973. Print.
//
// Location:
//   Chapter 17, page 221, equation (15)
//
/////////////////////////////////////////////////////////////////////
bool CEOSMODEL::_bCalc_Co( int iArrayIndex, double *Co)
{
    if( fabs( FluidList[iArrayIndex].MDc ) < dblGetTrace() )
    {
        *Co = 0.0;
        return false;
    }
    *Co =  ( A[3] + ( B[3] * FluidList[iArrayIndex].omega ) ) *
          ( ( r_USCS * pow( FluidList[iArrayIndex].Tc, 3 ) ) / FluidList[iArrayIndex].MDc );

    return true;
}

/////////////////////////////////////////////////////////////////////
// Method Definition:  
//   Calculate the Do parameter from fluid properties.
// 
// Source:
//   Starling, Kenneth E. Fluid Thermodynamic Properties 
//   for Light Petroleum Systems.  Houston: Gulf Pub., 1973. Print.
//
// Location:
//   Chapter 17, page 221, equation (21)
//
/////////////////////////////////////////////////////////////////////
bool CEOSMODEL::_bCalc_Do( int iArrayIndex, double *Do)
{
    if( fabs( FluidList[iArrayIndex].MDc ) < dblGetTrace() )
    {
        *Do = 0.0;
        return false;
    }
    *Do = ( A[9] + ( B[9] * FluidList[iArrayIndex].omega ) ) * 
          ( ( r_USCS * pow( FluidList[iArrayIndex].Tc, 4 ) ) / FluidList[iArrayIndex].MDc );

    return true;
}

/////////////////////////////////////////////////////////////////////
// Method Definition:  
//   Calculate the Eo parameter from fluid properties.
// 
// Source:
//   Starling, Kenneth E. Fluid Thermodynamic Properties 
//   for Light Petroleum Systems.  Houston: Gulf Pub., 1973. Print.
//
// Location:
//   Chapter 17, page 221, equation (23)
//
/////////////////////////////////////////////////////////////////////
bool CEOSMODEL::_bCalc_Eo( int iArrayIndex, double *Eo)
{
    if( fabs( FluidList[iArrayIndex].MDc ) < dblGetTrace() )
    {
        *Eo = 0.0;
        return false;
    }
    *Eo = ( A[11] + ( B[11] * FluidList[iArrayIndex].omega * exp( -3.8 * FluidList[iArrayIndex].omega ) ) ) *
          ( ( r_USCS * pow( FluidList[iArrayIndex].Tc, 5 ) ) / FluidList[iArrayIndex].MDc );

    return true;
}

/////////////////////////////////////////////////////////////////////
// Method Definition:  
//   Calculate the b parameter from fluid properties.
// 
// Source:
//   Starling, Kenneth E. Fluid Thermodynamic Properties 
//   for Light Petroleum Systems.  Houston: Gulf Pub., 1973. Print.
//
// Location:
//   Chapter 17, page 221, equation (17)
/////////////////////////////////////////////////////////////////////
bool CEOSMODEL::_bCalc_b( int iArrayIndex, double *b)
{
    if( fabs( FluidList[iArrayIndex].MDc ) < dblGetTrace() )
    {
        *b = 0.0;
        return false;
    }
    *b = ( A[5] + ( B[5] * FluidList[iArrayIndex].omega ) ) / pow( FluidList[iArrayIndex].MDc, 2 );

    return true;
}

/////////////////////////////////////////////////////////////////////
// Method Definition:  
//   Calculate the a parameter from fluid properties.
// 
// Source:
//   Starling, Kenneth E. Fluid Thermodynamic Properties 
//   for Light Petroleum Systems.  Houston: Gulf Pub., 1973. Print.
//
// Location:
//   Chapter 17, page 221, equation (18)
/////////////////////////////////////////////////////////////////////
bool CEOSMODEL::_bCalc_a( int iArrayIndex, double *a)
{
    if( fabs( FluidList[iArrayIndex].MDc ) < dblGetTrace() )
    {
        *a = 0.0;
        return false;
    }
    *a = ( A[6] + ( B[6] * FluidList[iArrayIndex].omega ) ) *
          (( r_USCS * FluidList[iArrayIndex].Tc ) / pow( FluidList[iArrayIndex].MDc, 2 ) );

    return true;
}

/////////////////////////////////////////////////////////////////////
// Method Definition:  
//   Calculate the d parameter from fluid properties.
// 
// Source:
//   Starling, Kenneth E. Fluid Thermodynamic Properties 
//   for Light Petroleum Systems.  Houston: Gulf Pub., 1973. Print.
//
// Location:
//   Chapter 17, page 221, equation (22)
/////////////////////////////////////////////////////////////////////
bool CEOSMODEL::_bCalc_d( int iArrayIndex, double *d)
{
    if( fabs( FluidList[iArrayIndex].MDc ) < dblGetTrace() )
    {
        *d = 0.0;
        return false;
    }
    *d = ( A[10] + ( B[10] * FluidList[iArrayIndex].omega ) ) *
                    ( ( r_USCS * pow( FluidList[iArrayIndex].Tc, 2 ) ) / pow( FluidList[iArrayIndex].MDc, 2 ) );

    return true;
}

/////////////////////////////////////////////////////////////////////
// Method Definition:  
//   Calculate the alpha parameter from fluid properties.
// 
// Source:
//   Starling, Kenneth E. Fluid Thermodynamic Properties 
//   for Light Petroleum Systems.  Houston: Gulf Pub., 1973. Print.
//
// Location:
//   Chapter 17, page 221, equation (19)
/////////////////////////////////////////////////////////////////////
bool CEOSMODEL::_bCalc_alpha( int iArrayIndex, double *alpha)
{
    if( fabs( FluidList[iArrayIndex].MDc ) < dblGetTrace() )
    {
        *alpha = 0.0;
        return false;
    }
    *alpha = ( A[7] + ( B[7] * FluidList[iArrayIndex].omega ) ) / pow( FluidList[iArrayIndex].MDc, 3 );

    return true;
}

/////////////////////////////////////////////////////////////////////
// Method Definition:  
//   Calculate the c parameter from fluid properties.
// 
// Source:
//   Starling, Kenneth E. Fluid Thermodynamic Properties 
//   for Light Petroleum Systems.  Houston: Gulf Pub., 1973. Print.
//
// Location:
//   Chapter 17, page 221, equation (20)
/////////////////////////////////////////////////////////////////////
bool CEOSMODEL::_bCalc_c( int iArrayIndex, double *c)
{
    if( fabs( FluidList[iArrayIndex].MDc ) < dblGetTrace() )
    {
        *c = 0.0;
        return false;
    }
    *c = ( A[8] + ( B[8] * FluidList[iArrayIndex].omega ) ) *
          ( ( r_USCS * pow( FluidList[iArrayIndex].Tc, 3 ) ) / pow( FluidList[iArrayIndex].MDc, 2 ) );

    return true;
}

/////////////////////////////////////////////////////////////////////
// Method Definition:  
//   Calculate the gamma parameter from fluid properties.
// 
// Source:
//   Starling, Kenneth E. Fluid Thermodynamic Properties 
//   for Light Petroleum Systems.  Houston: Gulf Pub., 1973. Print.
//
// Location:
//   Chapter 17, page 221, equation (16)
/////////////////////////////////////////////////////////////////////
bool CEOSMODEL::_bCalc_gamma( int iArrayIndex, double *gamma)
{
    if( fabs( FluidList[iArrayIndex].MDc ) < dblGetTrace() )
    {
        *gamma = 0.0;
        return false;
    }
    *gamma = ( A[4] + ( B[4] * FluidList[iArrayIndex].omega ) ) / pow ( FluidList[iArrayIndex].MDc, 2 );

    return true;
}

/////////////////////////////////////////////////////////////////////
// Method Definition: ValidityCheckMixture:  To avoid nasty errors, 
// check the fluid index
/////////////////////////////////////////////////////////////////////
bool CEOSMODEL::_bValidityCheckMixture( double *mixarray )
{
  //local variables
  int i;
  double mixtotal;

  //Initialize local variables
  i = 0;
  mixtotal = 0;

  //Check for valid data
  for( i=0; i<FluidCount; i++ )
  {
    mixtotal = mixtotal + mixarray[i];
  }

  //We are pretty strict about the data we allow into
  //the engine.  There are two cases broken out like this
  //because in the future, I may want to automatically correct
  //the mixture readings if they are close.  We'll see.
  // Revision - 5 Jul 2013
  // It is hard to get a sum of doubles to agree to 5 nines so
  // I backed off of the tolerance.
  if( mixtotal < 0.99 )
  {
    AddMessage( messagelist, 1, 
            "Mixture Total was Less Than 100%",
            "Check the sum of the percent mole weights" );
    return false;
  }

  if( mixtotal > 1.01 )
  {
    AddMessage( messagelist, 1, 
            "Mixture Total Exceeded 100%",
            "Check the sum of the percent mole weights" );
    return false;
  }

  //We made it!
  return true;
}

/////////////////////////////////////////////////////////////////////
// Method Definition: 
//   Converts all stored USCS variables to their SI equivalents.
//
/////////////////////////////////////////////////////////////////////
bool CEOSMODEL::ToUSCS( )
{
  //Local variables
  int          i;

  //Initialize local variables
  i          = 0;

  //Loop through the pure fluid array
  for( i = 0; i<FluidCount; i++ )
    if( !ToUSCS( i ) )
      return false;

  //and the mixture arrray
  Mixture.Vc        = Mixture.Vc_SI * units.convert( units.cm3, units.ft3 );
  Mixture.Tc        = Mixture.Tc_SI * units.convert( units.K, units.R );
  Mixture.Pc        = FluidList[i].Pc_SI * units.convert( units.bar, units.PSIA );
    
  if( Mixture.Vc != 0 )
    Mixture.MDc      = 1.0 /( Mixture.Vc );
  else
    Mixture.MDc      = -1;

  //Done
  return true;
}

/////////////////////////////////////////////////////////////////////
// Method Definition: 
//   Converts all stored USCS variables to their SI equivalents for 
//   a single fluid.
//
// Revision:  
//   26 Nov 2011    
//   B. Howard
//   Revised to include error checking on the units conversioin
//   library.
//
/////////////////////////////////////////////////////////////////////
bool CEOSMODEL::ToUSCS( int fluidindex )
{

    // locals
    double dTemp;

    //Check the index
    if( !ValidityCheckFluid( fluidindex ))
    {
        AddMessage( messagelist, 50, 
                "ToUSCS failed because ValidityCheckFluid failed",
                "" );
        return false;
    }

    //Make the conversion
    dTemp =  units.convert( units.cm3, units.ft3 );
    if( dTemp < 0.0 )
    {
        AddMessage( messagelist, 50, 
                "Unit conversion from cm3 to ft3 failed in ToUSCS",
                "" );
        return false;
    }
    FluidList[( fluidindex - 1)].Vc = FluidList[( fluidindex - 1)].Vc_SI * dTemp;

    dTemp = units.convert( units.lb, units.g );
    if( dTemp < 0.0 )
    {
        AddMessage( messagelist, 50, 
                "Unit conversion from lb to g failed in ToUSCS",
                "" );
        return false;
    }
    FluidList[( fluidindex - 1)].Vc = FluidList[( fluidindex - 1)].Vc * dTemp;

    dTemp = units.convert( units.K, units.R );
    if( dTemp < 0.0 )
    {
        AddMessage( messagelist, 50, 
                "Unit conversion from K to R failed in ToUSCS",
                "" );
        return false;
    }
    FluidList[( fluidindex - 1)].Tc = FluidList[( fluidindex - 1)].Tc_SI * dTemp;
    FluidList[( fluidindex - 1)].Pc = FluidList[( fluidindex - 1)].Pc_SI * units.convert( units.bar, units.PSIA );
  
  //Calculate the molar density
  if( FluidList[( fluidindex - 1)].Vc != 0 )
    FluidList[( fluidindex - 1)].MDc  = 1.0 /( FluidList[( fluidindex - 1)].Vc );
  else
    FluidList[( fluidindex - 1)].MDc  = -1;
  
  //Calculate the gas specific constant
  FluidList[( fluidindex - 1)].R_USCS  = r_USCS * ( units.convert( units.lbf_ft, units.BTU ) / units.convert( units.in2, units.ft2 ) );
  FluidList[( fluidindex - 1)].R_USCS = ( FluidList[( fluidindex - 1)].R_USCS / FluidList[( fluidindex - 1)].molewt );

  //Calculate the entropy/enthalpy of formations
  FluidList[( fluidindex - 1)].hfo_USCS= FluidList[( fluidindex - 1)].hfo_SI * units.convert( units.J, units.BTU );
  FluidList[( fluidindex - 1)].hfo_USCS= FluidList[( fluidindex - 1)].hfo_USCS * units.convert( units.lb, units.g );

  FluidList[( fluidindex - 1)].sfo_USCS= FluidList[( fluidindex - 1)].sfo_SI * units.convert( units.J, units.BTU );
  FluidList[( fluidindex - 1)].sfo_USCS= FluidList[( fluidindex - 1)].sfo_USCS * units.convert( units.lb, units.g );
  FluidList[( fluidindex - 1)].sfo_USCS= FluidList[( fluidindex - 1)].sfo_USCS * units.convert( units.R, units.K );

  //Convert the heat of formation
  FluidList[( fluidindex - 1)].LHV_USCS= FluidList[( fluidindex - 1)].LHV_SI * units.convert( units.J, units.BTU );
  FluidList[( fluidindex - 1)].LHV_USCS= FluidList[( fluidindex - 1)].LHV_USCS * units.convert( units.lb, units.g );
  FluidList[( fluidindex - 1)].HHV_USCS= FluidList[( fluidindex - 1)].HHV_SI * units.convert( units.J, units.BTU );
  FluidList[( fluidindex - 1)].HHV_USCS= FluidList[( fluidindex - 1)].HHV_USCS * units.convert( units.lb, units.g );

  //Done
  return true;
}
