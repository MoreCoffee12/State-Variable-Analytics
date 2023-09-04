#include "gasanalysisfunctions.h"
#include "windows.h"
#include "oleauto.h"
#include <memory>

/***********************************
 * Function Definition : ShowBuild *
 *---------------------------------******************
 * This function returns an integer with the build  *
 * number                                           *
 ****************************************************/
int ShowBuild()
{
	return 998;
}

/*************************************
 * Function Definition : ShowVersion *
 *-----------------------------------****************
 * This function returns an integer with the build  *
 * number                                           *
 ****************************************************/
int	ShowVersion()
{
	return 0;
}

/**************************************
 * Function Definition : ShowVersioin *
 *------------------------------------***************
 * This function is included for legacy support only*
 ****************************************************/
bool CheckMixture(double* MixtureArray, int FluidCount)
{
	//success
	return true;
}

/****************************************
 * Function Definition : ShowFluidCount *
 *--------------------------------------*************
 * This function returns the total number of fluids *
 * in the EOS engine								*
 ****************************************************/
int ShowFluidCount()
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();

	return (short)bwrs->GetFluidCount();
}

/**********************************
 * Function Definition : ShowName *
 *--------------------------------*******************
 * This function returns the name of the requested	*
 * fluid.											*
 ****************************************************/
int ShowName(int* fluidindex, char* textline,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	char* peosline;
	char* pmerrline;
	int	errs;
	int	i;

	//initialize localvariables
	errs = 0;
	peosline = bwrs->GetName(*fluidindex);
	pmerrline = NULL;
	i = 0;

	// transfer internal value the to the function argument
	strcpy_s(textline, strlen(textline), peosline);

	//Check to see if the action generated any errors
	errs = bwrs->GetMessageCount();
	if( errs > 0 )
	{
		pmerrline = bwrs->GetMessageMain( 1 );
		strcpy_s( mainerrline01, strlen(mainerrline01), pmerrline );
		*priority01	= bwrs->GetMessagePriority( 1 );
	}

	//return errs;
	return errs;
}

/*****************************************
 * Function Definition : ShowName_Aalt01 *
 *---------------------------------------************
 * This function returns the alternate or secondary *
 * name of the requested fluid.						*
 ****************************************************/
int ShowName_Alt01(int* fluidindex, char* textline,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	char* peosline;
	char* pmerrline;
	int	errs;
	int	i;

	//initialize localvariables
	errs = 0;
	peosline = bwrs->GetName_Alt01(*fluidindex);
	i = 0;
	pmerrline = NULL;

	//initialize local variables
	strcpy_s(textline, strlen(textline), peosline);

	//Check to see if the action generated any errors
	errs = bwrs->GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs->GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs->GetMessagePriority(1);
	}

	return errs;
}

/*************************************
 * Function Definition : ShowFormula *
 *-----------------------------------****************
 * This function returns the chemical formula for a *
 * fluid at fluidindex								*
 ****************************************************/
int ShowFormula(double fluidindex, char* textline,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	char* peosline;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	peosline = bwrs.GetFormula((int)fluidindex);
	i = 0;
	pmerrline = NULL;

	//initialize local variables
	strcpy_s(textline, sizeof textline, peosline);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/***************************************
 * Function Definition : ShowH_H298_SI *
 *-------------------------------------**************
 * This function returns the enthalpy (based on the *
 * JANAF tables) SI									*
 ****************************************************/
int ShowH_H298_SI(double eosset,
	double temp,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* h,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*h = bwrs.Geth_h298_SI(temp);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/*****************************************
 * Function Definition : ShowH_H298_USCS *
 *---------------------------------------************
 * This function returns the enthalpy (based on the *
 * JANAF tables) USCS								*
 ****************************************************/
int ShowH_H298_USCS(double eosset,
	double temp,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* h,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*h = bwrs.Geth_h298_USCS(temp);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/***************************************
 * Function Definition : ShowHIdeal_SI *
 *-------------------------------------**************
 * This function returns the ideal gas entropy based*
 * on the passut-danner correlation.				*
 ****************************************************/
int ShowHIdeal_SI(double eosset,
	double temp,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* h,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*h = bwrs.GetHIdeal_SI(temp);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/*****************************************
 * Function Definition : ShowHIdeal_USCS *
 *-------------------------------------**************
 * This function returns the ideal gas entropy based*
 * on the passut-danner correlation in USCS units.	*
 ****************************************************/
int ShowHIdeal_USCS(double eosset,
	double temp,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* h,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*h = bwrs.GetHIdeal_USCS(temp);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/***********************************
 * Function Definition : ShowSo_SI *
 *---------------------------------******************
 * This function returns the entropy (based on the  *
 * JANAF tables) SI									*
 ****************************************************/
int ShowSo_SI(double eosset,
	double temp,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* s,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*s = bwrs.Getso_SI(temp);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/*************************************
 * Function Definition : ShowSo_USCS *
 *-----------------------------------****************
 * This function returns the entropy (based on the  *
 * JANAF tables) USCS								*
 ****************************************************/
int ShowSo_USCS(double eosset,
	double temp,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* s,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*s = bwrs.Getso_USCS(temp);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/***************************************
 * Function Definition : ShowMolWeight *
 *-------------------------------------**************
 * This function returns the chemical formula for a *
 * fluid at fluidindex								*
 ****************************************************/
int ShowMolWeight(double fluidindex, double* molweight,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	*molweight = bwrs.GetMolWeight((int)fluidindex);
	i = 0;
	pmerrline = NULL;

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/***************************************
 * Function Definition : ShowMolWeight *
 *-------------------------------------**************
 * This function returns the chemical formula for a *
 * mixture.											*
 ****************************************************/
int ShowMolWeight_mx(double* molweight,
	double* priority01,
	char* mainerrline01,
	double* MixtureArray)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	*molweight = NULL;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Retreive the mixture molecular weight
	*molweight = bwrs.GetMolWeight_mx();

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/*******************************************
 * Function Definition : ShowCritTemp_USCS *
 *-----------------------------------------**********
 * This function returns the critical temperature in*
 * USCS												*
 ****************************************************/
int ShowCritTemp_USCS(double fluidindex, double* crittemp,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	*crittemp = bwrs.GetTempCrit_USCS((int)fluidindex);
	i = 0;
	pmerrline = NULL;

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/*****************************************
 * Function Definition : ShowCritTemp_SI *
 *---------------------------------------************
 * This function returns the critical temperature in*
 * USCS												*
 ****************************************************/
int ShowCritTemp_SI(double fluidindex, double* crittemp,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	*crittemp = bwrs.GetTempCrit_SI((int)fluidindex);
	i = 0;
	pmerrline = NULL;

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/*******************************************
 * Function Definition : ShowCritPres_USCS *
 *-----------------------------------------**********
 * This function returns the critical pressure in   *
 * USCS												*
 ****************************************************/
int ShowCritPres_USCS(double fluidindex, double* crittemp,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	*crittemp = bwrs.GetPresCrit_USCS((int)fluidindex);
	i = 0;
	pmerrline = NULL;

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/*****************************************
 * Function Definition : ShowCritPres_SI *
 *---------------------------------------************
 * This function returns the critical pressure in   *
 * SI												*
 ****************************************************/
int ShowCritPres_SI(double fluidindex, double* crittemp,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	*crittemp = bwrs.GetPresCrit_SI((int)fluidindex);
	i = 0;
	pmerrline = NULL;

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/*******************************************
 * Function Definition : ShowCritVol_USCS *
 *-----------------------------------------**********
 * This function returns the critical Volume in     *
 * USCS												*
 ****************************************************/
int ShowCritVol_USCS(double fluidindex, double* crittemp,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	*crittemp = bwrs.GetVolCrit_USCS((int)fluidindex);
	i = 0;
	pmerrline = NULL;

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/****************************************
 * Function Definition : ShowCritVol_SI *
 *--------------------------------------*************
 * This function returns the critical Volume in     *
 * SI												*
 ****************************************************/
int ShowCritVol_SI(double fluidindex, double* crittemp,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	*crittemp = bwrs.GetVolCrit_SI((int)fluidindex);
	i = 0;
	pmerrline = NULL;

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/*********************************
 * Function Definition : ShowEcc *
 *-------------------------------********************
 * This function returns the eccentricity factor    *
 ****************************************************/
int ShowEcc(double fluidindex, double* ecc,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize local variables
	errs = 0;
	*ecc = bwrs.GetEcc((int)fluidindex);
	i = 0;
	pmerrline = NULL;

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/************************************
 * Function Definition : ShowR_USCS *
 *----------------------------------*****************
 * This function returns the gas constant			*
 ****************************************************/
int ShowR_USCS(double fluidindex, double* r,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize local variables
	errs = 0;
	*r = bwrs.GetR_USCS((int)fluidindex);
	i = 0;
	pmerrline = NULL;

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/************************************
 * Function Definition : ShowHfo_SI *
 *----------------------------------*****************
 * This function returns the enthalpy of formation  *
 * for the fluid at fluidindex						*
 ****************************************************/
int ShowHfo_SI(double fluidindex, double* h,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize local variables
	errs = 0;
	*h = bwrs.Gethfo_SI((int)fluidindex);
	i = 0;
	pmerrline = NULL;

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/**************************************
 * Function Definition : ShowHfo_USCS *
 *------------------------------------***************
 * This function returns the enthalpy of formation  *
 * for the fluid at fluidindex						*
 ****************************************************/
int ShowHfo_USCS(double fluidindex, double* h,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize local variables
	errs = 0;
	*h = bwrs.Gethfo_USCS((int)fluidindex);
	i = 0;
	pmerrline = NULL;

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/***************************************
 * Function Definition : ShowHfo_mx_SI *
 *-------------------------------------**************
 * This function returns the ideal gas enthalpy		*
 * formation for the mixture in SI units			*
 ****************************************************/
int ShowHfo_mx_SI(double eosset,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* h,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the enthalpy
	*h = bwrs.Gethfo_SI();

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/*****************************************
 * Function Definition : ShowHfo_mx_USCS *
 *---------------------------------------************
 * This function returns the ideal gas enthalpy		*
 * formation for the mixture in USCS units			*
 ****************************************************/
int ShowHfo_mx_USCS(double eosset,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* h,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*h = bwrs.Gethfo_USCS();

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/************************************
 * Function Definition : ShowSfo_SI *
 *----------------------------------*****************
 * This function returns the entropy of formation   *
 * for the fluid at fluidindex						*
 ****************************************************/
int ShowSfo_SI(double fluidindex, double* s,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize local variables
	errs = 0;
	*s = bwrs.Getsfo_SI((int)fluidindex);
	i = 0;
	pmerrline = NULL;

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/**************************************
 * Function Definition : ShowSfo_USCS *
 *------------------------------------***************
 * This function returns the entropy of formation   *
 * for the fluid at fluidindex						*
 ****************************************************/
int ShowSfo_USCS(double fluidindex, double* s,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize local variables
	errs = 0;
	*s = bwrs.Getsfo_USCS((int)fluidindex);
	i = 0;
	pmerrline = NULL;

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/***************************************
 * Function Definition : ShowSfo_mx_SI *
 *-------------------------------------**************
 * This function returns the ideal gas entropy of   *
 * formation for the mixture						*
 ****************************************************/
int ShowSfo_mx_SI(double eosset,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* s,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*s = bwrs.Getsfo_SI();

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/*****************************************
 * Function Definition : ShowSfo_mx_USCS *
 *---------------------------------------************
 * This function returns the ideal gas entropy of   *
 * formation for the mixture in USCS units			*
 ****************************************************/
int ShowSfo_mx_USCS(double eosset,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* s,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*s = bwrs.Getsfo_USCS();

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

////////////////////////////////////////////////////////////
// Function Definition : ShowLHV_USCS
////////////////////////////////////////////////////////////
// This function returns the lower heating value
// for the fluid at fluidindex in USCS units
////////////////////////////////////////////////////////////
int ShowLHV_USCS(double fluidindex, double* lhv,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize local variables
	errs = 0;
	*lhv = bwrs.GetLHV_USCS((int)fluidindex);
	i = 0;
	pmerrline = NULL;

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

////////////////////////////////////////////////////////////
// Function Definition : ShowLHV_SI
////////////////////////////////////////////////////////////
// This function returns the lower heating value
// for the fluid at fluidindex.
////////////////////////////////////////////////////////////
int ShowLHV_SI(double fluidindex, double* lhv,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize local variables
	errs = 0;
	*lhv = bwrs.GetLHV_SI((int)fluidindex);
	i = 0;
	pmerrline = NULL;

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/*****************************************
 * Function Definition : ShowDipole_USCS *
 *---------------------------------------**********
 * This function returns the dipole moment        *
 * for the fluid at fluidindex in USCS units		*
 **************************************************/
int ShowDipole_USCS(short fluidindex, double* dipole,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS               bwrs;
	int                 errs;
	int                 i;
	char* pmerrline;

	//initialize local variables
	errs = 0;
	*dipole = bwrs.GetDipole_USCS((int)fluidindex);
	i = 0;
	pmerrline = NULL;

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/***************************************
 * Function Definition : ShowDipole_SI *
 *-------------------------------------************
 * This function returns the dipole moment        *
 * for the fluid at fluidindex in USCS units		*
 **************************************************/
int ShowDipole_SI(short fluidindex, double* dipole,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS               bwrs;
	int                 errs;
	int                 i;
	char* pmerrline;

	//initialize local variables
	errs = 0;
	*dipole = bwrs.GetDipole_SI((int)fluidindex);
	i = 0;
	pmerrline = NULL;

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

////////////////////////////////////////////////////////////
// Function Definition : ShowLHV_mx_USCS
////////////////////////////////////////////////////////////
// This function returns the lower heating value
// for the mixture in USCS units.
////////////////////////////////////////////////////////////
int ShowLHV_mx_USCS(double eosset,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* lhv,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the enthalpy
	*lhv = bwrs.GetLHV_mx_USCS();

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

////////////////////////////////////////////////////////////
// Function Definition : ShowLHV_mx_SI
////////////////////////////////////////////////////////////
// This function returns the lower heating value
// for the mixture in SI units.
////////////////////////////////////////////////////////////
int ShowLHV_mx_SI(double eosset,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* lhv,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the enthalpy
	*lhv = bwrs.GetLHV_mx_SI();

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/******************************************
 * Function Definition : ShowPres_MT_USCS *
 *----------------------------------------***********
 * This function returns the pressure given a molar *
 * density and temperature							*
 ****************************************************/
int ShowP_MT_USCS(double eosset,
	double md,
	double temp,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* pres,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))

		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
		return errs;
	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*pres = bwrs.GetP_MT_USCS(md, temp);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/////////////////////////////////////////////////////
// Function Definition : ShowPres_VS_SI
/////////////////////////////////////////////////////
// This function returns the pressure given a
// specific volume and temperature
/////////////////////////////////////////////////////
short ShowP_VS_SI(double eosset,
	double v,
	double s,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* pres,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS bwrs;
	int errs;
	int i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*pres = bwrs.GetP_VS_SI(v, s);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/////////////////////////////////////////////////////
// Function Definition : ShowPres_VS_USCS
/////////////////////////////////////////////////////
// This function returns the pressure given a
// specific volume and temperature
/////////////////////////////////////////////////////
short ShowP_VS_USCS(double eosset,
	double v,
	double s,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* pres,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS bwrs;
	int errs;
	int i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*pres = bwrs.GetP_VS_USCS(v, s);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/****************************************
 * Function Definition : ShowPres_VT_SI *
 *--------------------------------------*************
 * This function returns the pressure given a       *
 * specific volume and temperature					*
 ****************************************************/
int ShowP_VT_SI(double eosset,
	double sv,
	double temp,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* pres,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*pres = bwrs.GetP_VT_SI(sv, temp);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/******************************************
 * Function Definition : ShowPres_VT_USCS *
 *----------------------------------------***********
 * This function returns the pressure given a       *
 * specific volume and temperature					*
 ****************************************************/
int ShowP_VT_USCS(double eosset,
	double sv,
	double temp,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* pres,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*pres = bwrs.GetP_VT_USCS(sv, temp);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/*************************************
 * Function Definition : ShowV_TP_SI *
 *-----------------------------------****************
 * This function returns the pressure in SI given a *
 * specific volume and temperature					*
 ****************************************************/
int ShowV_TP_SI(double eosset,
	double temp,
	double pres,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* v,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*v = bwrs.GetV_TP_SI(temp, pres);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

////////////////////////////////////////////////////////////
// Function Definition : ShowV_TP_USCS
////////////////////////////////////////////////////////////
// This function returns the pressure given a
// specific volume and temperature
////////////////////////////////////////////////////////////
short ShowV_TP_USCS(short eosset,
	double temp,
	double pres,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* v,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS bwrs;
	int	errs;
	int	i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//See what EOS was selected
	switch (eosset)
	{
	case 1:
		//...And load the mixture data into the BWRS object
		if (!bwrs.SetMixtureData(MixtureArray))
		{
			//Check to see if the action generated any errors
			errs = bwrs.GetMessageCount();
			if (errs > 0)
			{
				pmerrline = bwrs.GetMessageMain(1);
				strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
				*priority01 = bwrs.GetMessagePriority(1);
				return errs;
			}

		}

		//Now load the solver configuration
		bwrs.SetPrecision(Precision);
		bwrs.SetMaxIterations((int)MaxIterations);

		//and get the pressure
		*v = bwrs.GetV_TP_USCS(temp, pres);

		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();

		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
		}

		break;
	case 2:
		//...And load the mixture data into the Lee-Kesler object
		errs = 0;
		break;
	default:
		errs = 0;
	}

	return errs;
}

/*************************************
 * Function Definition : ShowS_TP_SI *
 *-----------------------------------****************
 * This function returns the entropy given a        *
 * specific temperature	and pressure in SI			*
 ****************************************************/
int ShowS_TP_SI(double eosset,
	double temp,
	double pres,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* s,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*s = bwrs.GetS_TP_SI(temp, pres);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/***************************************
 * Function Definition : ShowS_TP_USCS *
 *-------------------------------------**************
 * This function returns the entropy given a        *
 * specific temperature	and pressure               *
 ****************************************************/
int ShowS_TP_USCS(double eosset,
	double temp,
	double pres,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* s,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*s = bwrs.GetS_TP_USCS(temp, pres);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/*************************************
 * Function Definition : ShowH_TP_SI *
 *-----------------------------------****************
 * This function returns the enthalpy given a       *
 * specific temperature	and pressure in SI         *
 ****************************************************/
int ShowH_TP_SI(double eosset,
	double temp,
	double pres,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* h,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*h = bwrs.GetH_TP_SI(temp, pres);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/***************************************
 * Function Definition : ShowH_TP_USCS *
 *-------------------------------------**************
 * This function returns the enthalpy given a       *
 * specific temperature	and pressure               *
 ****************************************************/
int ShowH_TP_USCS(double eosset,
	double temp,
	double pres,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* h,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*h = bwrs.GetH_TP_USCS(temp, pres);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/*************************************
 * Function Definition : ShowH_TS_SI *
 *-----------------------------------****************
 * This function returns the enthalpy given a       *
 * specific temperature and entropy                 *
 ****************************************************/
int ShowH_TS_SI(double eosset,
	double temp,
	double entr,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* h,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS               bwrs;
	int                 errs;
	int                 i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*h = bwrs.GetH_TS_SI(temp, entr);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/***************************************
 * Function Definition : ShowH_TS_USCS *
 *-------------------------------------**************
 * This function returns the enthalpy given a       *
 * specific temperature and entropy                 *
 ****************************************************/
int ShowH_TS_USCS(double eosset,
	double temp,
	double entr,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* h,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS               bwrs;
	int                 errs;
	int                 i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*h = bwrs.GetH_TS_USCS(temp, entr);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/*************************************
 * Function Definition : ShowH_PS_SI *
 *-----------------------------------****************
 * This function returns the enthalpy given a       *
 * specific pressure and entropy                    *
 ****************************************************/
int ShowH_PS_SI(double eosset,
	double pres,
	double entr,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* h,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS               bwrs;
	int                 errs;
	int                 i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*h = bwrs.GetH_PS_SI(pres, entr);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/***************************************
 * Function Definition : ShowH_PS_USCS *
 *-------------------------------------**************
 * This function returns the enthalpy given a       *
 * specific pressure and entropy                    *
 ****************************************************/
int ShowH_PS_USCS(double eosset,
	double pres,
	double entr,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* h,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS               bwrs;
	int                 errs;
	int                 i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*h = bwrs.GetH_PS_USCS(pres, entr);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/***************************************
 * Function Definition : ShowF_TP_USCS *
 *-------------------------------------**************
 * This function returns the fugacity given a       *
 * specific temperature	and pressure				*
 ****************************************************/
int ShowF_TP_USCS(double eosset,
	double temp,
	double pres,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* f,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the fugacity
	*f = bwrs.GetFugacity_TP_USCS(temp, pres);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/********************************************
 * Function Definition : ShowVapPres_T_USCS *
 *------------------------------------------*********
 * This function returns the vapor pressure for a   *
 * fluid.											*
 ****************************************************/
int ShowVapPres_T_USCS(double eosset,
	double temp,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* ps,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*ps = bwrs.GetVaporPressure_T_USCS(temp);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

//////////////////////////////////////////////////////////////
// Function Definition : ShowVapPres_T_SI
//////////////////////////////////////////////////////////////
// This function returns the vapor pressure for a fluid.											*
//////////////////////////////////////////////////////////////
int ShowVapPres_T_SI(double eosset,
	double temp,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* ps,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*ps = bwrs.GetVaporPressure_T_SI(temp);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/********************************************
 * Function Definition : ShowSatVapV_T_USCS *
 *------------------------------------------*********
 * This function returns the specific volume for a  *
 * fluid.											*
 ****************************************************/
int ShowSatVapV_T_USCS(double eosset,
	double temp,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* v,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the specific volume
	*v = bwrs.GetSatVapV_T_USCS(temp);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/////////////////////////////////////////////////////
// Function Definition : ShowSatVapV_T_SI
/////////////////////////////////////////////////////
// This function returns the specific volume for a
// fluid.
/////////////////////////////////////////////////////
int ShowSatVapV_T_SI(double eosset,
	double temp,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* v,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS bwrs;
	int errs;
	int i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the specific volume
	*v = bwrs.GetSatVapV_T_SI(temp);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/********************************************
 * Function Definition : ShowSatLiqV_T_USCS *
 *------------------------------------------*********
 * This function returns the specific volume for a  *
 * fluid.											*
 ****************************************************/
int ShowSatLiqV_T_USCS(double eosset,
	double temp,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* v,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*v = bwrs.GetSatLiqV_T_USCS(temp);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

//////////////////////////////////////////////////////
//Function Definition : ShowSatLiqV_T_SI
//////////////////////////////////////////////////////
//This function returns the specific volume for a
//fluid.
//////////////////////////////////////////////////////
int ShowSatLiqV_T_SI(double eosset,
	double temp,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* v,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS bwrs;
	int errs;
	int i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*v = bwrs.GetSatLiqV_T_SI(temp);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/******************************************
 * Function Definition : ShowSatVapH_T_SI *
 *----------------------------------------***********
 * This function returns the ethalpy for a fluid at *
 * saturated vapor conditions in SI units.			*											*
 ****************************************************/
int ShowSatVapH_T_SI(double eosset,
	double temp,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* h,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*h = bwrs.GetSatVapH_T_SI(temp);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/********************************************
 * Function Definition : ShowSatVapH_T_USCS *
 *------------------------------------------*********
 * This function returns the ethalpy for a fluid.	*											*
 ****************************************************/
int ShowSatVapH_T_USCS(double eosset,
	double temp,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* h,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*h = bwrs.GetSatVapH_T_USCS(temp);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/******************************************
 * Function Definition : ShowSatLiqH_T_SI *
 *----------------------------------------***********
 * This function returns the ehtalpy for a fluid at *
 * saturated liquid conditions in SI units.			*
 ****************************************************/
int ShowSatLiqH_T_SI(double eosset,
	double temp,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* h,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*h = bwrs.GetSatLiqH_T_SI(temp);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/********************************************
 * Function Definition : ShowSatLiqH_T_USCS *
 *------------------------------------------*********
 * This function returns the ehtalpy for a fluid at *
 * saturated liquid conditions.						*
 ****************************************************/
int ShowSatLiqH_T_USCS(double eosset,
	double temp,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* h,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*h = bwrs.GetSatLiqH_T_USCS(temp);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/********************************************
 * Function Definition : ShowSatVapS_T_USCS *
 *------------------------------------------*********
 * This function returns the ethalpy for a fluid.	*											*
 ****************************************************/
int ShowSatVapS_T_USCS(double eosset,
	double temp,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* s,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*s = bwrs.GetSatVapS_T_USCS(temp);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/********************************************
 * Function Definition : ShowSatLiqS_T_USCS *
 *------------------------------------------*********
 * This function returns the ehtalpy for a fluid at *
 * saturated liquid conditions.						*
 ****************************************************/
int ShowSatLiqS_T_USCS(double eosset,
	double temp,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* s,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*s = bwrs.GetSatLiqS_T_USCS(temp);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/********************************************
 * Function Definition : ShowVapTemp_P_USCS *
 *------------------------------------------*********
 * This function returns the vapor temperature for a*
 * fluid.											*
 ****************************************************/
int ShowVapTemp_P_USCS(double eosset,
	double temp,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* ts,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*ts = bwrs.GetVaporTemperature_P_USCS(temp);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;

}

/******************************************
 * Function Definition : ShowVapTemp_P_SI *
 *----------------------------------------***********
 * This function returns the vapor temperature for a*
 * fluid in SI units.								*
 ****************************************************/
int ShowVapTemp_P_SI(double eosset,
	double temp,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* ts,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*ts = bwrs.GetVaporTemperature_P_SI(temp);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/********************************************
 * Function Definition : ShowSatLiqV_P_USCS *
 *------------------------------------------*********
 * This function returns the specific volume for a  *
 * fluid.											*
 ****************************************************/
int ShowSatLiqV_P_USCS(double eosset,
	double pres,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* v,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*v = bwrs.GetSatLiqV_P_USCS(pres);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/******************************************
 * Function Definition : ShowSatLiqV_P_SI *
 *----------------------------------------***********
 * This function returns the specific volume for a  *
 * fluid.											*
 ****************************************************/
int ShowSatLiqV_P_SI(double eosset,
	double pres,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* v,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*v = bwrs.GetSatLiqV_P_SI(pres);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/********************************************
 * Function Definition : ShowSatVapV_P_USCS *
 *------------------------------------------*********
 * This function returns the specific volume for a  *
 * fluid at saturation given the pressure.			*
 ****************************************************/
int ShowSatVapV_P_USCS(double eosset,
	double pres,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* v,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*v = bwrs.GetSatVapV_P_USCS(pres);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/******************************************
 * Function Definition : ShowSatVapV_P_SI *
 *----------------------------------------***********
 * This function returns the specific volume for a  *
 * fluid at saturation given the pressure in SI		*
 ****************************************************/
int ShowSatVapV_P_SI(double eosset,
	double pres,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* v,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*v = bwrs.GetSatVapV_P_SI(pres);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/*************************************
 * Function Definition : ShowT_HP_SI *
 *-----------------------------------****************
 * This function returns the temperature given the  *
 * enthalpy and pressure of a fluid in SI units.	*
 ****************************************************/
int ShowT_HP_SI(double eosset,
	double h,
	double p,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* t,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the temperature
	*t = bwrs.GetT_HP_SI(h, p);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/***************************************
 * Function Definition : ShowT_HP_USCS *
 *-------------------------------------**************
 * This function returns the temperature given the  *
 * enthalpy and pressure of a fluid.				*
 ****************************************************/
int ShowT_HP_USCS(double eosset,
	double h,
	double p,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* t,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the temperature
	*t = bwrs.GetT_HP_USCS(h, p);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/*************************************
 * Function Definition : ShowT_SP_SI *
 *-----------------------------------****************
 * This function returns the temperature given the  *
 * entropy and pressure of a fluid in SI units		*
 ****************************************************/
int ShowT_SP_SI(double eosset,
	double s,
	double p,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* t,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the temperature
	*t = bwrs.GetT_SP_SI(s, p);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/***************************************
 * Function Definition : ShowT_SP_USCS *
 *-------------------------------------**************
 * This function returns the temperature given the  *
 * entropy and pressure of a fluid.					*
 ****************************************************/
int ShowT_SP_USCS(double eosset,
	double s,
	double p,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* t,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the temperature
	*t = bwrs.GetT_SP_USCS(s, p);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/********************************************
 * Function Definition : ShowSatLiqH_P_USCS *
 *------------------------------------------*********
 * This function returns the specific volume for a  *
 * saturated fluid given pressure.					*
 ****************************************************/
int ShowSatLiqH_P_USCS(double eosset,
	double pres,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* h,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the enthalpy
	*h = bwrs.GetSatLiqH_P_USCS(pres);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/******************************************
 * Function Definition : ShowSatLiqH_P_SI *
 *----------------------------------------***********
 * This function returns the specific volume for a  *
 * saturated fluid given pressure in SI.			*
 ****************************************************/
int ShowSatLiqH_P_SI(double eosset,
	double pres,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* h,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the enthalpy
	*h = bwrs.GetSatLiqH_P_SI(pres);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/********************************************
 * Function Definition : ShowSatVapH_P_USCS *
 *------------------------------------------*********
 * This function returns the specific volume for a  *
 * fluid at saturation given the pressure.			*
 ****************************************************/
int ShowSatVapH_P_USCS(double eosset,
	double pres,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* h,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the enthalpy
	*h = bwrs.GetSatVapH_P_USCS(pres);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/******************************************
 * Function Definition : ShowSatVapH_P_SI *
 *----------------------------------------***********
 * This function returns the specific volume for a  *
 * fluid at saturation given the pressure in SI.	*
 ****************************************************/
int ShowSatVapH_P_SI(double eosset,
	double pres,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* h,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the enthalpy
	*h = bwrs.GetSatVapH_P_SI(pres);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/********************************************
 * Function Definition : ShowSatLiqS_P_USCS *
 *------------------------------------------*********
 * This function returns the entropy for a			*
 * saturated fluid given pressure.					*
 ****************************************************/
int ShowSatLiqS_P_USCS(double eosset,
	double pres,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* s,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the entropy
	*s = bwrs.GetSatLiqS_P_USCS(pres);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/******************************************
 * Function Definition : ShowSatLiqS_P_SI *
 *----------------------------------------***********
 * This function returns the entropy for a			*
 * saturated fluid given pressure.					*
 ****************************************************/
int ShowSatLiqS_P_SI(double eosset,
	double pres,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* s,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the entropy
	*s = bwrs.GetSatLiqS_P_SI(pres);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/********************************************
 * Function Definition : ShowSatVapS_P_USCS *
 *------------------------------------------*********
 * This function returns the entropy for a			*
 * fluid at saturation given the pressure.			*
 ****************************************************/
int ShowSatVapS_P_USCS(double eosset,
	double pres,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* s,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the entropy
	*s = bwrs.GetSatVapS_P_USCS(pres);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/******************************************
 * Function Definition : ShowSatVapS_P_SI *
 *----------------------------------------***********
 * This function returns the entropy for a			*
 * fluid at saturation given the pressure in SI.	*
 ****************************************************/
int ShowSatVapS_P_SI(double eosset,
	double pres,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* s,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the entropy
	*s = bwrs.GetSatVapS_P_SI(pres);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/***********************************************
 * Function Definition : ShowViscosityGas_USCS *
 *---------------------------------------------******
 * This function returns the viscosity for a fluid  *
 * at a given the pressure and temperature          *
 ****************************************************/
int ShowViscosityGas_TP_USCS(double eosset,
	double temp,
	double pres,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* v,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the entropy
	*v = bwrs.GetViscosityGas_TP_USCS(temp, pres);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

/////////////////////////////////////////////////////
// Function Definition : ShowViscosityGas_TP_SI
/////////////////////////////////////////////////////
// This function returns the viscosity for a fluid
// at a given the pressure and temperature
/////////////////////////////////////////////////////
int ShowViscosityGas_TP_SI(double eosset,
	double temp,
	double pres,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* v,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the entropy
	*v = bwrs.GetViscosityGas_TP_SI(temp, pres);

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

////////////////////////////////////////////////////////////
// Function Definition : ShowHHV_SI
////////////////////////////////////////////////////////////
// This function returns the lower heating value
// for the fluid at fluidindex.
////////////////////////////////////////////////////////////
int ShowHHV_SI(double fluidindex, double* hhv,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS bwrs;
	int errs;
	int i;
	char* pmerrline;

	//initialize local variables
	errs = 0;
	*hhv = bwrs.GetHHV_SI((int)fluidindex);
	i = 0;
	pmerrline = NULL;

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

////////////////////////////////////////////////////////////
// Function Definition : ShowHHV_USCS
////////////////////////////////////////////////////////////
// This function returns the higher heating value
// for the fluid at fluidindex in USCS units
////////////////////////////////////////////////////////////
int ShowHHV_USCS(double fluidindex, double* hhv,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS bwrs;
	int errs;
	int i;
	char* pmerrline;

	//initialize local variables
	errs = 0;
	*hhv = bwrs.GetHHV_USCS((int)fluidindex);
	i = 0;
	pmerrline = NULL;

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

////////////////////////////////////////////////////////////
// Function Definition : ShowHHV_mx_SI
////////////////////////////////////////////////////////////
// This function returns the higher heating value
// for the mixture in SI units.
////////////////////////////////////////////////////////////
int ShowHHV_mx_SI(double eosset,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* hhv,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS bwrs;
	int errs;
	int i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the higher heating value
	*hhv = bwrs.GetHHV_mx_SI();

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

////////////////////////////////////////////////////////////
// Function Definition : ShowHHV_mx_USCS
////////////////////////////////////////////////////////////
// This function returns the higher heating value
// for the mixture in USCS units.
////////////////////////////////////////////////////////////
int ShowHHV_mx_USCS(double eosset,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* hhv,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	CBWRS bwrs;
	int errs;
	int i;
	char* pmerrline;

	//initialize localvariables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs.GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs.GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs.GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs.SetPrecision(Precision);
	bwrs.SetMaxIterations((int)MaxIterations);

	//and get the higher heating value
	*hhv = bwrs.GetLHV_mx_USCS();

	//Check to see if the action generated any errors
	errs = bwrs.GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs.GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs.GetMessagePriority(1);
	}

	return errs;
}

