// FluidProp.h: interface for the CEOSMODEL class.
//
// This structure defines those members required for
// fluid property calculations
//
// Brian Howard
//////////////////////////////////////////////////////////////////////
#ifndef _FluidProp
#define _FluidProp

struct FluidProp
{
	//Generic constants associated with the fluid
	//and not any specific EOS model.
	char        name[256];
	char        name_alt01[256];
	char        chemsym[256];
	double	    percentmw;
	double		molewt;
	double		molewt_mx;
	double		omega;
	double		omega_mx;
	double		Vc;
	double		Vc_mx;
	double		Pc;
	double		Pc_mx;
	double		Tc;
	double		Tc_mx;
	double		MDc;
	double		MDc_mx;
	double		Vc_SI;
	double		Pc_SI;
	double		Tc_SI;
	double		R_USCS;
	double		R_USCS_mx;
	double		R_SI;
	double		R_SI_mx;
    double      Dipole_SI;
    double      Dipole_USCS;

	//These variables have to do with identification
	//Three integers make up the CAS number for a fluid
	int			CASNo[3];

	//This is the index number used in the book
	//Properties of Gases and Liquids by Poling, etc.
	int			Polingetl;

	//These are the operating limits for the EOS
	double		Tmin_USCS;
	double		Tmax_USCS;
	double		Pmin_USCS;
	double		Pmax_USCS;
	
	//BWRS fluid property constants
	double		Bo;
	double		Ao;
	double		Co;
	double		Do;
	double		Eo;
	double		b;
	double		a;
	double		d;
	double		alpha;
	double		c;
	double		gamma;

	//BWRS fluid mixture constants
	double		Bo_mx;
	double		Ao_mx;
	double		Co_mx;
	double		Do_mx;
	double		Eo_mx;
	double		b_mx;
	double		a_mx;
	double		d_mx;
	double		alpha_mx;
	double		c_mx;
	double		gamma_mx;

	//Entropy calculation constants
	double		A;
	double		A_mx;
	double		B;
	double		B_mx;
	double		C;
	double		C_mx;
	double		D;
	double		D_mx;
	double		E;
	double		E_mx;
	double		F;
	double		F_mx;
	double		G;
	double		G_mx;

	//Enthalpy/entropy of formation constants
	double		hfo_USCS;
	double		hfo_USCS_mx;
	double		hfo_SI;
	double		hfo_SI_mx;
	double		sfo_USCS;
	double		sfo_USCS_mx;
	double		sfo_SI;
	double		sfo_SI_mx;

	//JANAF ideal gas enthalpy constants
	double		HT5;
	double		HT5_mx;
	double		HT4;
	double		HT4_mx;
	double		HT3;
	double		HT3_mx;
	double		HT2;
	double		HT2_mx;
	double		HT1;
	double		HT1_mx;
	double		HT0;
	double		HT0_mx;
	double		Hn;
	double		Hn_mx;

	//JANAF ideal gas entropy constants
	double		ST5;
	double		ST5_mx;
	double		ST4;
	double		ST4_mx;
	double		ST3;
	double		ST3_mx;
	double		ST2;
	double		ST2_mx;
	double		ST1;
	double		ST1_mx;
	double		ST0;
	double		ST0_mx;

	//Heating values
	double		LHV_SI;
	double		LHV_SI_mx;
	double		LHV_USCS;
	double		LHV_USCS_mx;
	double		HHV_SI;
	double		HHV_SI_mx;
	double		HHV_USCS;
	double		HHV_USCS_mx;

};

#endif

