#include "gasanalysisfunctions.h"
#include "windows.h"
#include "oleauto.h"
#include <memory>

/// <summary>
/// Retrieves the the C++ dll version number.
/// </summary>
/// <file>GasAnalysisFunctions.cpp</file>
/// <author>Brian Howard</author>
/// <date>2001</date>
/// <returns>An integer representing the build number.</returns>
int ShowBuild()
{
	return 998;
}

/// <summary>
/// Retrieves the the C++ dll version number.
/// </summary>
/// <file>GasAnalysisFunctions.cpp</file>
/// <author>Brian Howard</author>
/// <date>2001</date>
/// <returns>An integer representing the version number.</returns>
int	ShowVersion()
{
	return 0;
}

// Revision, 4 Sep 2023, deprecated this function
///**************************************
// * Function Definition : CheckMixture *
// *------------------------------------***************
// * This function is included for legacy support only*
// ****************************************************/
//bool CheckMixture(double* MixtureArray, int FluidCount)
//{
//	//success
//	return true;
//}

/// <summary>
/// Retrieves the total number of fluids in the C++ dll.
/// </summary>
/// <file>GasAnalysisFunctions.cpp</file>
/// <author>Brian Howard</author>
/// <date>2001</date>
/// <revision>Revision, 4 Sep 2023: Used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety.</revision>
/// <returns>An integer representing the build number.</returns>
int ShowFluidCount()
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();

	return (int)bwrs->GetFluidCount();
}

/// <summary>
/// Retrieves the primary name and error information for a given fluid index using the CBWRS class.
/// </summary>
/// <file>GasAnalysisFunctions.cpp</file>
/// <author>Brian Howard</author>
/// <date>2001</date>
/// <revision>Revision, 3 Sep 2023: Used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <param name="fluidindex">Pointer to an integer representing the fluid index. [in, out]</param>
/// <param name="textline">Pointer to a char array to hold the name of the fluid. [out]</param>
/// <param name="priority01">Pointer to a double representing the error priority. [out]</param>
/// <param name="mainerrline01">Pointer to a char array to hold the main error line. [out]</param>
/// <returns>An integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// <note>This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.</note>
/// <warning>Make sure that the char arrays (textline, mainerrline01) are allocated with sufficient space before calling this function.</warning>
/// </remarks>
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

	// Initialize local variables
	errs = 0;
	peosline = bwrs->GetName(*fluidindex);
	pmerrline = NULL;
	i = 0;

	// transfer internal value the to the function argument
	// Revision, 3 Sep 2023, changed strcpy to strcpy_s
	strcpy_s(textline, strlen(textline), peosline);

	//Check to see if the action generated any errors
	errs = bwrs->GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs->GetMessageMain(1);
		// Revision, 3 Sep 2023, changed strcpy to strcpy_s
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs->GetMessagePriority(1);
	}

	//return errs;
	return errs;
}

/// <summary>
/// Retrieves alternate or secondary name and error information for a given fluid index using the CBWRS class.
/// </summary>
/// <file>GasAnalysisFunctions.cpp</file>
/// <author>Brian Howard</author>
/// <date>2001</date>
/// <revision>Revision, 3 Sep 2023: Used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <param name="fluidindex">Pointer to an integer representing the fluid index. [in, out]</param>
/// <param name="textline">Pointer to a char array to hold the name of the fluid. [out]</param>
/// <param name="priority01">Pointer to a double representing the error priority. [out]</param>
/// <param name="mainerrline01">Pointer to a char array to hold the main error line. [out]</param>
/// <returns>An integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// <note>This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.</note>
/// <warning>Make sure that the char arrays (textline, mainerrline01) are allocated with sufficient space before calling this function.</warning>
/// </remarks>
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

	// Initialize local variables
	errs = 0;
	peosline = bwrs->GetName_Alt01(*fluidindex);
	i = 0;
	pmerrline = NULL;

	// Initialize local variables
	// Revision, 3 Sep 2023, changed strcpy to strcpy_s
	strcpy_s(textline, strlen(textline), peosline);

	//Check to see if the action generated any errors
	errs = bwrs->GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs->GetMessageMain(1);
		// Revision, 3 Sep 2023, changed strcpy to strcpy_s
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs->GetMessagePriority(1);
	}

	return errs;
}

/// <summary>
/// Retrieves chemical formula information for a given fluid index using the CBWRS class.
/// </summary>
/// <file>GasAnalysisFunctions.cpp</file>
/// <author>Brian Howard</author>
/// <date>2001</date>
/// <revision>Revision, 3 Sep 2023: Used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <param name="fluidindex">Pointer to an integer representing the fluid index. [in, out]</param>
/// <param name="textline">Pointer to a char array to hold the fluid chemical formula. [out]</param>
/// <param name="priority01">Pointer to a double representing the error priority. [out]</param>
/// <param name="mainerrline01">Pointer to a char array to hold the main error line. [out]</param>
/// <returns>An integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// <note>This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.</note>
/// <warning>Make sure that the char arrays (textline, mainerrline01) are allocated with sufficient space before calling this function.</warning>
/// </remarks>
int ShowFormula(int* fluidindex, char* textline,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	char* peosline;
	int	errs;
	int	i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	peosline = bwrs->GetFormula( *fluidindex);
	i = 0;
	pmerrline = NULL;

	// Initialize local variables
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

 /// <summary>
 /// Calculates and returns the enthalpy (H) of an ideal gas mixture in SI units based on the JANAF tables, relative to 298 Kelvin.
 /// </summary>
/// <file>GasAnalysisFunctions.cpp</file>
/// <author>Brian Howard</author>
/// <date>2001</date>
/// <revision>Revision, 4 Sep 2023: Used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <param name="eosset">Pointer to a set of equation of state constants. Not currently used but reserved for future use.</param>
/// <param name="temp">Pointer to the temperature, in Kelvin, of the gas mixture.</param>
/// <param name="MixtureArray">Pointer to the array representing the gas mixture composition.</param>
/// <param name="Precision">The precision to be used in calculations.</param>
/// <param name="MaxIterations">Maximum number of iterations for the solver.</param>
/// <param name="h">Pointer to store the calculated enthalpy.</param>
/// <param name="priority01">Pointer to store the priority of any generated message.</param>
/// <param name="mainerrline01">Pointer to store any generated error message.</param>
/// <returns>An integer representing the number of errors. If greater than zero, an error has occurred and additional information can be found in 'priority01' and 'mainerrline01'.</returns>
int ShowH_H298_SI(double* eosset,
	double* temp,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* h,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 4 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int	errs;
	int	i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//The eosset variable is not used in this function, but is included
	//for future compatibility with the C++ version of the BWRS class
	//bwrs->set_eosset(*eosset);

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration. Begin by setting the solver precision.
	bwrs->SetPrecision(Precision);

	//...And the maximum number of iterations
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the enthalpy by passing the temperature into the BWRS object
	*h = bwrs->Geth_h298_SI(*temp);

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

/// <summary>
/// Calculates and returns the enthalpy (H) of an ideal gas mixture in USCS units based on the JANAF tables, relative to 298 Kelvin.
/// </summary>
/// <file>GasAnalysisFunctions.cpp</file>
/// <author>Brian Howard</author>
/// <date>2001</date>
/// <revision>Revision, 4 Sep 2023: Used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <param name="eosset">Pointer to a set of equation of state constants. Not currently used but reserved for future use.</param>
/// <param name="temp">Pointer to the temperature, in Rankine, of the gas mixture.</param>
/// <param name="MixtureArray">Pointer to the array representing the gas mixture composition.</param>
/// <param name="Precision">The precision to be used in calculations.</param>
/// <param name="MaxIterations">Maximum number of iterations for the solver.</param>
/// <param name="h">Pointer to store the calculated enthalpy.</param>
/// <param name="priority01">Pointer to store the priority of any generated message.</param>
/// <param name="mainerrline01">Pointer to store any generated error message.</param>
/// <returns>An integer representing the number of errors. If greater than zero, an error has occurred and additional information can be found in 'priority01' and 'mainerrline01'.</returns>
int ShowH_H298_USCS(double* eosset,
	double* temp,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* h,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int	errs;
	int	i;
	char* pmerrline;

	//The eosset variable is not used in this function, but is included
	//for future compatibility with the C++ version of the BWRS class
	//bwrs->set_eosset(*eosset);

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*h = bwrs->Geth_h298_USCS(*temp);

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

/// <summary>
/// Returns the ideal gas entropy in SI units based on the Passut-Danner correlation.
/// </summary>
/// <param name="eosset">Pointer to a double representing the EOS set. Currently not used but included for future compatibility.</param>
/// <param name="temp">Pointer to a double representing the temperature in kelvin.</param>
/// <param name="MixtureArray">Pointer to a double array representing the mixture.</param>
/// <param name="Precision">Double representing the solver's precision.</param>
/// <param name="MaxIterations">Double representing the maximum number of iterations for the solver.</param>
/// <param name="h">Pointer to a double where the calculated entropy will be stored.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to hold the main error line.</param>
/// <returns>An integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
int ShowHIdeal_SI(double* eosset,
	double* temp,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* h,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int	errs;
	int	i;
	char* pmerrline;

	//The eosset variable is not used in this function, but is included
	//for future compatibility with the C++ version of the BWRS class
	//bwrs->set_eosset(*eosset);

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the ideal entropy
	*h = bwrs->GetHIdeal_SI(*temp);

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

/// <summary>
/// Returns the ideal gas entropy in USCS units based on the Passut-Danner correlation.
/// </summary>
/// <param name="eosset">Pointer to a double representing the EOS set. Currently not used but included for future compatibility.</param>
/// <param name="temp">Pointer to a double representing the temperature in Rankine.</param>
/// <param name="MixtureArray">Pointer to a double array representing the mixture.</param>
/// <param name="Precision">Double representing the solver's precision.</param>
/// <param name="MaxIterations">Double representing the maximum number of iterations for the solver.</param>
/// <param name="h">Pointer to a double where the calculated entropy will be stored.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to hold the main error line.</param>
/// <returns>An integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
int ShowHIdeal_USCS(double* eosset,
	double* temp,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* h,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int	errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*h = bwrs->GetHIdeal_USCS(*temp);

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

/***********************************
 * Function Definition : ShowSo_SI *
 *---------------------------------******************
 * This function returns the entropy (based on the  *
 * JANAF tables) SI									*
 ****************************************************/
int ShowSo_SI(double* eosset,
	double* temp,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* s,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*s = bwrs->Getso_SI(*temp);

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
 * Function Definition : ShowSo_USCS *
 *-----------------------------------****************
 * This function returns the entropy (based on the  *
 * JANAF tables) USCS								*
 ****************************************************/
int ShowSo_USCS(double* eosset,
	double* temp,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* s,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*s = bwrs->Getso_USCS(*temp);

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

/// <summary>
/// Retrieves the mole weight and error information for a given fluid index using the CBWRS class.
/// </summary>
/// <param name="fluidindex">Pointer to an integer representing the fluid index.</param>
/// <param name="molweight">Pointer to a double to hold the fluid's mole weight.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to hold the main error line.</param>
/// <returns>An integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.
/// </remarks>
/// <warning>
/// Make sure that the char array (mainerrline01) is allocated with sufficient space before calling this function.
/// </warning>
/// <author>Brian Howard</author>
/// <date>2001</date>
/// <revision>Revision, 3 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
int ShowMolWeight(int* fluidindex, double* molweight,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int	i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	*molweight = bwrs->GetMolWeight(*fluidindex);
	i = 0;
	i = 0;
	pmerrline = NULL;

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

/// <summary>
/// Retrieves the mixture mole weight and error information for a given fluid index using the CBWRS class.
/// </summary>
/// <param name="molweight">Pointer to a double to hold the mixture's mole weight.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to hold the main error line.</param>
/// <param name="MixtureArray">Pointer to a double array containing the mixture percent mole weights.</param>
/// <returns>An integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.
/// </remarks>
/// <warning>
/// Make sure that the char array (mainerrline01) is allocated with sufficient space before calling this function.
/// </warning>
/// <author>Brian Howard</author>
/// <date>2001</date>
/// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
int ShowMolWeight_mx(double* molweight,
	double* priority01,
	char* mainerrline01,
	double* MixtureArray)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int	errs;
	int	i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	*molweight = NULL;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Retreive the mixture molecular weight
	*molweight = bwrs->GetMolWeight_mx();

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

/// <summary>
/// Retrieves the critical temperature in USCS units and error information for a given fluid index using the CBWRS class.
/// </summary>
/// <param name="fluidindex">Pointer to an integer representing the fluid index.</param>
/// <param name="crittemp">Pointer to a double to hold the fluid critical temperature in USCS units, Rankine.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to hold the main error line.</param>
/// <returns>An integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <author>Brian Howard</author>
/// <date>2001</date>
/// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
int ShowCritTemp_USCS(int* fluidindex, double* crittemp,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int	i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	*crittemp = bwrs->GetTempCrit_USCS(*fluidindex);
	i = 0;
	pmerrline = NULL;

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

/// <summary>
/// Retrieves the critical temperature in SI units, kelvin, and error information for a given fluid index using the CBWRS class.
/// </summary>
/// <param name="fluidindex">Pointer to an integer representing the fluid index.</param>
/// <param name="crittemp">Pointer to a double to hold the fluid critical temperature in SI units, kelvin.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to hold the main error line.</param>
/// <returns>An integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <author>Brian Howard</author>
/// <date>2001</date>
/// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
int ShowCritTemp_SI(int* fluidindex, double* crittemp,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	*crittemp = bwrs->GetTempCrit_SI(*fluidindex);
	i = 0;
	pmerrline = NULL;

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

/// <summary>
/// Retrieves the critical temperature in USCS units, Rankine, and error information for a given fluid index using the CBWRS class.
/// </summary>
/// <param name="fluidindex">Pointer to an integer representing the fluid index.</param>
/// <param name="crittemp">Pointer to a double to hold the fluid critical temperature in USCS units, Rankine.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to hold the main error line.</param>
/// <returns>An integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <author>Brian Howard</author>
/// <date>2001</date>
/// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
int ShowCritPres_USCS(int* fluidindex, double* crittemp,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	*crittemp = bwrs->GetPresCrit_USCS(*fluidindex);
	i = 0;
	pmerrline = NULL;

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

/// <summary>
/// Retrieves the critical pressure in SI units, bar(a), and error information for a given fluid index using the CBWRS class.
/// </summary>
/// <param name="fluidindex">Pointer to an integer representing the fluid index.</param>
/// <param name="critpres">Pointer to a double to hold the fluid critical pressure in SI units, bar(a).</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to hold the main error line.</param>
/// <returns>An integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <author>Brian Howard</author>
/// <date>2001</date>
/// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
int ShowCritPres_SI(int* fluidindex, double* critpres,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	*critpres = bwrs->GetPresCrit_SI(*fluidindex);
	i = 0;
	pmerrline = NULL;

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

/// <summary>
/// Retrieves the critical volume in USCS units, ft3/lbmol, and error information for a given fluid index using the CBWRS class.
/// </summary>
/// <param name="fluidindex">Pointer to an integer representing the fluid index.</param>
/// <param name="critvol">Pointer to a double to hold the fluid critical pressure in USCS units, ft3/lbmol.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to hold the main error line.</param>
/// <returns>An integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <author>Brian Howard</author>
/// <date>2001</date>
/// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
int ShowCritVol_USCS(int* fluidindex, double* critvol,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	*critvol = bwrs->GetVolCrit_USCS(*fluidindex);
	i = 0;
	pmerrline = NULL;

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

/// <summary>
/// Retrieves the critical volume in SI units, cm3/gmol, and error information for a given fluid index using the CBWRS class.
/// </summary>
/// <param name="fluidindex">Pointer to an integer representing the fluid index.</param>
/// <param name="critvol">Pointer to a double to hold the fluid critical pressure in SI units, cm3/gmol.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to hold the main error line.</param>
/// <returns>An integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <author>Brian Howard</author>
/// <date>2001</date>
/// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
int ShowCritVol_SI(int* fluidindex, double* crittemp,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	*crittemp = bwrs->GetVolCrit_SI(*fluidindex);
	i = 0;
	pmerrline = NULL;

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

/// <summary>
/// Retrieves the ecentricity value, unitless, and error information for a given fluid index using the CBWRS class.
/// </summary>
/// <param name="fluidindex">Pointer to an integer representing the fluid index.</param>
/// <param name="ecc">Pointer to a double to hold the fluid eccentricity value, unitless.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to hold the main error line.</param>
/// <returns>An integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <author>Brian Howard</author>
/// <date>2001</date>
/// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
int ShowEcc(int* fluidindex, double* ecc,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	*ecc = bwrs->GetEcc(*fluidindex);
	i = 0;
	pmerrline = NULL;

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

/// <summary>
/// Retrieves the gas constant in USCS units, (psia-ft3)/(lbmol-R)  , and error information for a given fluid index using the CBWRS class.
/// </summary>
/// <param name="fluidindex">Pointer to an integer representing the fluid index.</param>
/// <param name="r">Pointer to a double to hold the fluid gas constant in USCS, (psia-ft3)/(lbmol-R).</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to hold the main error line.</param>
/// <returns>An integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <author>Brian Howard</author>
/// <date>2001</date>
/// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
int ShowR_USCS(int* fluidindex, double* r,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	*r = bwrs->GetR_USCS(*fluidindex);
	i = 0;
	pmerrline = NULL;

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

/// <summary>
/// Retrieves the ethalpy of formation in SI units, J/(gmol-K), and error information for a given fluid index using the CBWRS class.
/// </summary>
/// <param name="fluidindex">Pointer to an integer representing the fluid index.</param>
/// <param name="h">Pointer to a double to hold the fluid gas constant in  SI units, J/(gmol-K).</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to hold the main error line.</param>
/// <returns>An integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <author>Brian Howard</author>
/// <date>2001</date>
/// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
int ShowHfo_SI(int* fluidindex, double* h,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	*h = bwrs->Gethfo_SI(*fluidindex);
	i = 0;
	pmerrline = NULL;

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

/// <summary>
/// Retrieves the ethalpy of formation in USCS units, BTU/(lbmol-R) , and error information for a given fluid index using the CBWRS class.
/// </summary>
/// <param name="fluidindex">Pointer to an integer representing the fluid index.</param>
/// <param name="h">Pointer to a double to hold the fluid enthalpy of formation in USCS units, BTU/(lbmol-R) .</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to hold the main error line.</param>
/// <returns>An integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <author>Brian Howard</author>
/// <date>2001</date>
/// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
int ShowHfo_USCS(int* fluidindex, double* h,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	*h = bwrs->Gethfo_USCS(*fluidindex);
	i = 0;
	pmerrline = NULL;

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

/***************************************
 * Function Definition : ShowHfo_mx_SI *
 *-------------------------------------**************
 * This function returns the ideal gas enthalpy		*
 * formation for the mixture in SI units			*
 ****************************************************/
int ShowHfo_mx_SI(double* eosset,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* h,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the enthalpy
	*h = bwrs->Gethfo_SI();

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

/*****************************************
 * Function Definition : ShowHfo_mx_USCS *
 *---------------------------------------************
 * This function returns the ideal gas enthalpy		*
 * formation for the mixture in USCS units			*
 ****************************************************/
int ShowHfo_mx_USCS(double* eosset,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* h,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*h = bwrs->Gethfo_USCS();

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


 /// <summary>
 /// Retrieves the entropy of formation in SI units, BTU/(lbmol-R), and error information for a given fluid index using the CBWRS class.
 /// </summary>
 /// <param name="fluidindex">Pointer to an integer representing the fluid index.</param>
 /// <param name="s">Pointer to a double to hold the fluid entropy of formation in SI units, BTU/(lbmol-R).</param>
 /// <param name="priority01">Pointer to a double representing the error priority.</param>
 /// <param name="mainerrline01">Pointer to a char array to hold the main error line.</param>
 /// <returns>An integer representing the number of errors (0 for no errors).</returns>
 /// <remarks>
 /// This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.
 /// </remarks>
 /// <warning>
 /// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
 /// </warning>
 /// <author>Brian Howard</author>
 /// <date>2001</date>
 /// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
int ShowSfo_SI(int* fluidindex, double* s,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	*s = bwrs->Getsfo_SI(*fluidindex);
	i = 0;
	pmerrline = NULL;

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

/// <summary>
/// Retrieves the entropy of formation in USCS units,J/(gmol-K), and error information for a given fluid index using the CBWRS class.
/// </summary>
/// <param name="fluidindex">Pointer to an integer representing the fluid index.</param>
/// <param name="s">Pointer to a double to hold the fluid entropy of formation in  USCS units,J/(gmol-K).</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to hold the main error line.</param>
/// <returns>An integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <author>Brian Howard</author>
/// <date>2001</date>
/// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
int ShowSfo_USCS(int* fluidindex, double* s,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	*s = bwrs->Getsfo_USCS(*fluidindex);
	i = 0;
	pmerrline = NULL;

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

/***************************************
 * Function Definition : ShowSfo_mx_SI *
 *-------------------------------------**************
 * This function returns the ideal gas entropy of   *
 * formation for the mixture						*
 ****************************************************/
int ShowSfo_mx_SI(double* eosset,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* s,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*s = bwrs->Getsfo_SI();

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

/*****************************************
 * Function Definition : ShowSfo_mx_USCS *
 *---------------------------------------************
 * This function returns the ideal gas entropy of   *
 * formation for the mixture in USCS units			*
 ****************************************************/
int ShowSfo_mx_USCS(double* eosset,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* s,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	// Now load the solver configuration, starting with required precision
	bwrs->SetPrecision(Precision);
	// ...and maximum iterations
	bwrs->SetMaxIterations((int)MaxIterations);

	// ...and get the ideal gas entropy
	*s = bwrs->Getsfo_USCS();

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

/// <summary>
/// Retrieves the lower heating value in USCS units, (BTU/lbmol), and error information for a given fluid index using the CBWRS class.
/// </summary>
/// <param name="fluidindex">Pointer to an integer representing the fluid index.</param>
/// <param name="lhv">Pointer to a double to hold the fluid enthalpy of formation in USCS units, (BTU/lbmol).</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to hold the main error line.</param>
/// <returns>An integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <author>Brian Howard</author>
/// <date>2001</date>
/// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
int ShowLHV_USCS(int* fluidindex, double* lhv,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	*lhv = bwrs->GetLHV_USCS(*fluidindex);
	i = 0;
	pmerrline = NULL;

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

/// <summary>
/// Retrieves the lower heating value in SI units, (J/gmol), and error information for a given fluid index using the CBWRS class.
/// </summary>
/// <param name="fluidindex">Pointer to an integer representing the fluid index.</param>
/// <param name="lhv">Pointer to a double to hold the fluid enthalpy of formation in  SI units, (J/gmol).</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to hold the main error line.</param>
/// <returns>An integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <author>Brian Howard</author>
/// <date>2001</date>
/// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
int ShowLHV_SI(int* fluidindex, double* lhv,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	*lhv = bwrs->GetLHV_SI(*fluidindex);
	i = 0;
	pmerrline = NULL;

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

/// <summary>
/// Retrieves the dipole moment in USCS units, (-), and error information for a given fluid index using the CBWRS class.
/// </summary>
/// <param name="fluidindex">Pointer to an integer representing the fluid index.</param>
/// <param name="dipole">Pointer to a double to hold the fluid enthalpy of formation in USCS units, (-).</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to hold the main error line.</param>
/// <returns>An integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <author>Brian Howard</author>
/// <date>2001</date>
/// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
int ShowDipole_USCS(int* fluidindex, double* dipole,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;
	
	// Initialize local variables
	errs = 0;
	*dipole = bwrs->GetDipole_USCS(*fluidindex);
	i = 0;
	pmerrline = NULL;
	
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

 /// <summary>
 /// Retrieves the dipole moment in SI units, (debye), and error information for a given fluid index using the CBWRS class.
 /// </summary>
 /// <param name="fluidindex">Pointer to an integer representing the fluid index.</param>
 /// <param name="dipole">Pointer to a double to hold the fluid enthalpy of formation in SI units, (debye).</param>
 /// <param name="priority01">Pointer to a double representing the error priority.</param>
 /// <param name="mainerrline01">Pointer to a char array to hold the main error line.</param>
 /// <returns>An integer representing the number of errors (0 for no errors).</returns>
 /// <remarks>
 /// This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.
 /// </remarks>
 /// <warning>
 /// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
 /// </warning>
 /// <author>Brian Howard</author>
 /// <date>2001</date>
 /// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
int ShowDipole_SI(int* fluidindex, double* dipole,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int                 errs;
	int                 i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	*dipole = bwrs->GetDipole_SI(*fluidindex);
	i = 0;
	pmerrline = NULL;

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

////////////////////////////////////////////////////////////
// Function Definition : ShowLHV_mx_USCS
////////////////////////////////////////////////////////////
// This function returns the lower heating value
// for the mixture in USCS units.
////////////////////////////////////////////////////////////
int ShowLHV_mx_USCS(double* eosset,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* lhv,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	// Now load the solver configuration, starting with precision
	bwrs->SetPrecision(Precision);
	//	...and the maximum number of iterations
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the enthalpy
	*lhv = bwrs->GetLHV_mx_USCS();

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

////////////////////////////////////////////////////////////
// Function Definition : ShowLHV_mx_SI
////////////////////////////////////////////////////////////
// This function returns the lower heating value
// for the mixture in SI units.
////////////////////////////////////////////////////////////
int ShowLHV_mx_SI(double* eosset,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* lhv,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the enthalpy
	*lhv = bwrs->GetLHV_mx_SI();

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

/******************************************
 * Function Definition : ShowPres_MT_USCS *
 *----------------------------------------***********
 * This function returns the pressure given a molar *
 * density and temperature							*
 ****************************************************/
int ShowP_MT_USCS(double* eosset,
	double md,
	double* temp,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* pres,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))

		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
	if (errs > 0)
	{
		pmerrline = bwrs->GetMessageMain(1);
		strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
		*priority01 = bwrs->GetMessagePriority(1);
		return errs;
	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*pres = bwrs->GetP_MT_USCS(md, *temp);

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

/////////////////////////////////////////////////////
// Function Definition : ShowPres_VS_SI
/////////////////////////////////////////////////////
// This function returns the pressure given a
// specific volume and temperature
/////////////////////////////////////////////////////
short ShowP_VS_SI(double* eosset,
	double v,
	double s,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* pres,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*pres = bwrs->GetP_VS_SI(v, s);

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

/////////////////////////////////////////////////////
// Function Definition : ShowPres_VS_USCS
/////////////////////////////////////////////////////
// This function returns the pressure given a
// specific volume and temperature
/////////////////////////////////////////////////////
short ShowP_VS_USCS(double* eosset,
	double v,
	double s,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* pres,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*pres = bwrs->GetP_VS_USCS(v, s);

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

/****************************************
 * Function Definition : ShowPres_VT_SI *
 *--------------------------------------*************
 * This function returns the pressure given a       *
 * specific volume and temperature					*
 ****************************************************/
int ShowP_VT_SI(double* eosset,
	double sv,
	double* temp,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* pres,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*pres = bwrs->GetP_VT_SI(sv, *temp);

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

/******************************************
 * Function Definition : ShowPres_VT_USCS *
 *----------------------------------------***********
 * This function returns the pressure given a       *
 * specific volume and temperature					*
 ****************************************************/
int ShowP_VT_USCS(double* eosset,
	double sv,
	double* temp,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* pres,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*pres = bwrs->GetP_VT_USCS(sv, *temp);

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

/// <summary>
/// Retrieves the specific volume in SI units, cm3/g, for a given temperature and pressure using the CBWRS class.
/// </summary>
/// <param name="eosset">Pointer to a double representing the equation of state set. Currently unused, but reserved for future compatibility.</param>
/// <param name="temp">Pointer to a double representing the temperature in SI units, kelvin.</param>
/// <param name="pres">Double representing the pressure in SI units, bar(a).</param>
/// <param name="MixtureArray">Pointer to a double array containing the mole percentages of the mixture.</param>
/// <param name="Precision">Double representing the solver precision.</param>
/// <param name="MaxIterations">Double representing the maximum number of solver iterations.</param>
/// <param name="v">Pointer to a double to hold the calculated specific volume in SI units.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to hold the main error line.</param>
/// <returns>An integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <author>Brian Howard</author>
/// <date>2001</date>
/// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
int ShowV_TP_SI(double* eosset,
	double* temp,
	double pres,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* v,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*v = bwrs->GetV_TP_SI(*temp, pres);

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

/// <summary>
/// Retrieves the specific volume in USCS units, (ft3/lbm), for a given temperature and pressure using the CBWRS class.
/// </summary>
/// <param name="eosset">Pointer to a double representing the equation of state set. Currently unused, but reserved for future compatibility.</param>
/// <param name="temp">Pointer to a double representing the temperature in USCS units, Rankind.</param>
/// <param name="pres">Double representing the pressure in USCS units, PSIA.</param>
/// <param name="MixtureArray">Pointer to a double array containing the mole percentages of the mixture.</param>
/// <param name="Precision">Double representing the solver precision.</param>
/// <param name="MaxIterations">Double representing the maximum number of solver iterations.</param>
/// <param name="v">Pointer to a double to hold the calculated specific volume in SI units.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to hold the main error line.</param>
/// <returns>An integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <author>Brian Howard</author>
/// <date>2001</date>
/// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
short ShowV_TP_USCS(short eosset,
	double* temp,
	double pres,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* v,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int	errs;
	int	i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//See what EOS was selected
	switch (eosset)
	{
	case 1:
		//...And load the mixture data into the BWRS object
		if (!bwrs->SetMixtureData(MixtureArray))
		{
			//Check to see if the action generated any errors
			errs = bwrs->GetMessageCount();
			if (errs > 0)
			{
				pmerrline = bwrs->GetMessageMain(1);
				strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
				*priority01 = bwrs->GetMessagePriority(1);
				return errs;
			}

		}

		//Now load the solver configuration
		bwrs->SetPrecision(Precision);
		bwrs->SetMaxIterations((int)MaxIterations);

		//and get the pressure
		*v = bwrs->GetV_TP_USCS(*temp, pres);

		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();

		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
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
int ShowS_TP_SI(double* eosset,
	double* temp,
	double pres,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* s,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*s = bwrs->GetS_TP_SI(*temp, pres);

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

/***************************************
 * Function Definition : ShowS_TP_USCS *
 *-------------------------------------**************
 * This function returns the entropy given a        *
 * specific temperature	and pressure               *
 ****************************************************/
int ShowS_TP_USCS(double* eosset,
	double* temp,
	double pres,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* s,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*s = bwrs->GetS_TP_USCS(*temp, pres);

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
 * Function Definition : ShowH_TP_SI *
 *-----------------------------------****************
 * This function returns the enthalpy given a       *
 * specific temperature	and pressure in SI         *
 ****************************************************/
int ShowH_TP_SI(double* eosset,
	double* temp,
	double pres,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* h,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*h = bwrs->GetH_TP_SI(*temp, pres);

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

/***************************************
 * Function Definition : ShowH_TP_USCS *
 *-------------------------------------**************
 * This function returns the enthalpy given a       *
 * specific temperature	and pressure               *
 ****************************************************/
int ShowH_TP_USCS(double* eosset,
	double* temp,
	double pres,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* h,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*h = bwrs->GetH_TP_USCS(*temp, pres);

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
 * Function Definition : ShowH_TS_SI *
 *-----------------------------------****************
 * This function returns the enthalpy given a       *
 * specific temperature and entropy                 *
 ****************************************************/
int ShowH_TS_SI(double* eosset,
	double* temp,
	double entr,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* h,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int                 errs;
	int                 i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*h = bwrs->GetH_TS_SI(*temp, entr);

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

/***************************************
 * Function Definition : ShowH_TS_USCS *
 *-------------------------------------**************
 * This function returns the enthalpy given a       *
 * specific temperature and entropy                 *
 ****************************************************/
int ShowH_TS_USCS(double* eosset,
	double* temp,
	double entr,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* h,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int                 errs;
	int                 i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*h = bwrs->GetH_TS_USCS(*temp, entr);

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
 * Function Definition : ShowH_PS_SI *
 *-----------------------------------****************
 * This function returns the enthalpy given a       *
 * specific pressure and entropy                    *
 ****************************************************/
int ShowH_PS_SI(double* eosset,
	double pres,
	double entr,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* h,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int                 errs;
	int                 i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*h = bwrs->GetH_PS_SI(pres, entr);

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

/***************************************
 * Function Definition : ShowH_PS_USCS *
 *-------------------------------------**************
 * This function returns the enthalpy given a       *
 * specific pressure and entropy                    *
 ****************************************************/
int ShowH_PS_USCS(double* eosset,
	double pres,
	double entr,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* h,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int                 errs;
	int                 i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*h = bwrs->GetH_PS_USCS(pres, entr);

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

/***************************************
 * Function Definition : ShowF_TP_USCS *
 *-------------------------------------**************
 * This function returns the fugacity given a       *
 * specific temperature	and pressure				*
 ****************************************************/
int ShowF_TP_USCS(double* eosset,
	double* temp,
	double pres,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* f,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the fugacity
	*f = bwrs->GetFugacity_TP_USCS(*temp, pres);

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

/********************************************
 * Function Definition : ShowVapPres_T_USCS *
 *------------------------------------------*********
 * This function returns the vapor pressure for a   *
 * fluid.											*
 ****************************************************/
int ShowVapPres_T_USCS(double* eosset,
	double* temp,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* ps,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*ps = bwrs->GetVaporPressure_T_USCS(*temp);

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

//////////////////////////////////////////////////////////////
// Function Definition : ShowVapPres_T_SI
//////////////////////////////////////////////////////////////
// This function returns the vapor pressure for a fluid.											*
//////////////////////////////////////////////////////////////
int ShowVapPres_T_SI(double* eosset,
	double* temp,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* ps,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*ps = bwrs->GetVaporPressure_T_SI(*temp);

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

/********************************************
 * Function Definition : ShowSatVapV_T_USCS *
 *------------------------------------------*********
 * This function returns the specific volume for a  *
 * fluid.											*
 ****************************************************/
int ShowSatVapV_T_USCS(double* eosset,
	double* temp,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* v,
	double* priority01,
	char* mainerrline01)
{
	//local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the specific volume
	*v = bwrs->GetSatVapV_T_USCS(*temp);

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

/////////////////////////////////////////////////////
// Function Definition : ShowSatVapV_T_SI
/////////////////////////////////////////////////////
// This function returns the specific volume for a
// fluid.
/////////////////////////////////////////////////////
int ShowSatVapV_T_SI(double* eosset,
	double* temp,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* v,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the specific volume
	*v = bwrs->GetSatVapV_T_SI(*temp);

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

/********************************************
 * Function Definition : ShowSatLiqV_T_USCS *
 *------------------------------------------*********
 * This function returns the specific volume for a  *
 * fluid.											*
 ****************************************************/
int ShowSatLiqV_T_USCS(double* eosset,
	double* temp,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* v,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*v = bwrs->GetSatLiqV_T_USCS(*temp);

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

//////////////////////////////////////////////////////
//Function Definition : ShowSatLiqV_T_SI
//////////////////////////////////////////////////////
//This function returns the specific volume for a
//fluid.
//////////////////////////////////////////////////////
int ShowSatLiqV_T_SI(double* eosset,
	double* temp,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* v,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*v = bwrs->GetSatLiqV_T_SI(*temp);

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

/******************************************
 * Function Definition : ShowSatVapH_T_SI *
 *----------------------------------------***********
 * This function returns the ethalpy for a fluid at *
 * saturated vapor conditions in SI units.			*											*
 ****************************************************/
int ShowSatVapH_T_SI(double* eosset,
	double* temp,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* h,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*h = bwrs->GetSatVapH_T_SI(*temp);

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

/********************************************
 * Function Definition : ShowSatVapH_T_USCS *
 *------------------------------------------*********
 * This function returns the ethalpy for a fluid.	*											*
 ****************************************************/
int ShowSatVapH_T_USCS(double* eosset,
	double* temp,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* h,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*h = bwrs->GetSatVapH_T_USCS(*temp);

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

/******************************************
 * Function Definition : ShowSatLiqH_T_SI *
 *----------------------------------------***********
 * This function returns the ehtalpy for a fluid at *
 * saturated liquid conditions in SI units.			*
 ****************************************************/
int ShowSatLiqH_T_SI(double* eosset,
	double* temp,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* h,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*h = bwrs->GetSatLiqH_T_SI(*temp);

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

/********************************************
 * Function Definition : ShowSatLiqH_T_USCS *
 *------------------------------------------*********
 * This function returns the ehtalpy for a fluid at *
 * saturated liquid conditions.						*
 ****************************************************/
int ShowSatLiqH_T_USCS(double* eosset,
	double* temp,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* h,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*h = bwrs->GetSatLiqH_T_USCS(*temp);

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

/********************************************
 * Function Definition : ShowSatVapS_T_USCS *
 *------------------------------------------*********
 * This function returns the ethalpy for a fluid.	*											*
 ****************************************************/
int ShowSatVapS_T_USCS(double* eosset,
	double* temp,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* s,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*s = bwrs->GetSatVapS_T_USCS(*temp);

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

/********************************************
 * Function Definition : ShowSatLiqS_T_USCS *
 *------------------------------------------*********
 * This function returns the ehtalpy for a fluid at *
 * saturated liquid conditions.						*
 ****************************************************/
int ShowSatLiqS_T_USCS(double* eosset,
	double* temp,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* s,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*s = bwrs->GetSatLiqS_T_USCS(*temp);

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

/********************************************
 * Function Definition : ShowVapTemp_P_USCS *
 *------------------------------------------*********
 * This function returns the vapor temperature for a*
 * fluid.											*
 ****************************************************/
int ShowVapTemp_P_USCS(double* eosset,
	double* temp,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* ts,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*ts = bwrs->GetVaporTemperature_P_USCS(*temp);

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

/******************************************
 * Function Definition : ShowVapTemp_P_SI *
 *----------------------------------------***********
 * This function returns the vapor temperature for a*
 * fluid in SI units.								*
 ****************************************************/
int ShowVapTemp_P_SI(double* eosset,
	double* temp,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* ts,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*ts = bwrs->GetVaporTemperature_P_SI(*temp);

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

/********************************************
 * Function Definition : ShowSatLiqV_P_USCS *
 *------------------------------------------*********
 * This function returns the specific volume for a  *
 * fluid.											*
 ****************************************************/
int ShowSatLiqV_P_USCS(double* eosset,
	double pres,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* v,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*v = bwrs->GetSatLiqV_P_USCS(pres);

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

/******************************************
 * Function Definition : ShowSatLiqV_P_SI *
 *----------------------------------------***********
 * This function returns the specific volume for a  *
 * fluid.											*
 ****************************************************/
int ShowSatLiqV_P_SI(double* eosset,
	double pres,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* v,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*v = bwrs->GetSatLiqV_P_SI(pres);

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

/********************************************
 * Function Definition : ShowSatVapV_P_USCS *
 *------------------------------------------*********
 * This function returns the specific volume for a  *
 * fluid at saturation given the pressure.			*
 ****************************************************/
int ShowSatVapV_P_USCS(double* eosset,
	double pres,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* v,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*v = bwrs->GetSatVapV_P_USCS(pres);

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

/******************************************
 * Function Definition : ShowSatVapV_P_SI *
 *----------------------------------------***********
 * This function returns the specific volume for a  *
 * fluid at saturation given the pressure in SI		*
 ****************************************************/
int ShowSatVapV_P_SI(double* eosset,
	double pres,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* v,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the pressure
	*v = bwrs->GetSatVapV_P_SI(pres);

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
 * Function Definition : ShowT_HP_SI *
 *-----------------------------------****************
 * This function returns the temperature given the  *
 * enthalpy and pressure of a fluid in SI units.	*
 ****************************************************/
int ShowT_HP_SI(double* eosset,
	double h,
	double p,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* t,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the temperature
	*t = bwrs->GetT_HP_SI(h, p);

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

/***************************************
 * Function Definition : ShowT_HP_USCS *
 *-------------------------------------**************
 * This function returns the temperature given the  *
 * enthalpy and pressure of a fluid.				*
 ****************************************************/
int ShowT_HP_USCS(double* eosset,
	double h,
	double p,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* t,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the temperature
	*t = bwrs->GetT_HP_USCS(h, p);

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
 * Function Definition : ShowT_SP_SI *
 *-----------------------------------****************
 * This function returns the temperature given the  *
 * entropy and pressure of a fluid in SI units		*
 ****************************************************/
int ShowT_SP_SI(double* eosset,
	double s,
	double p,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* t,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the temperature
	*t = bwrs->GetT_SP_SI(s, p);

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

/***************************************
 * Function Definition : ShowT_SP_USCS *
 *-------------------------------------**************
 * This function returns the temperature given the  *
 * entropy and pressure of a fluid.					*
 ****************************************************/
int ShowT_SP_USCS(double* eosset,
	double s,
	double p,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* t,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the temperature
	*t = bwrs->GetT_SP_USCS(s, p);

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

/********************************************
 * Function Definition : ShowSatLiqH_P_USCS *
 *------------------------------------------*********
 * This function returns the specific volume for a  *
 * saturated fluid given pressure.					*
 ****************************************************/
int ShowSatLiqH_P_USCS(double* eosset,
	double pres,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* h,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the enthalpy
	*h = bwrs->GetSatLiqH_P_USCS(pres);

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

/******************************************
 * Function Definition : ShowSatLiqH_P_SI *
 *----------------------------------------***********
 * This function returns the specific volume for a  *
 * saturated fluid given pressure in SI.			*
 ****************************************************/
int ShowSatLiqH_P_SI(double* eosset,
	double pres,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* h,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the enthalpy
	*h = bwrs->GetSatLiqH_P_SI(pres);

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

/********************************************
 * Function Definition : ShowSatVapH_P_USCS *
 *------------------------------------------*********
 * This function returns the specific volume for a  *
 * fluid at saturation given the pressure.			*
 ****************************************************/
int ShowSatVapH_P_USCS(double* eosset,
	double pres,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* h,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the enthalpy
	*h = bwrs->GetSatVapH_P_USCS(pres);

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

/******************************************
 * Function Definition : ShowSatVapH_P_SI *
 *----------------------------------------***********
 * This function returns the specific volume for a  *
 * fluid at saturation given the pressure in SI.	*
 ****************************************************/
int ShowSatVapH_P_SI(double* eosset,
	double pres,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* h,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the enthalpy
	*h = bwrs->GetSatVapH_P_SI(pres);

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

/********************************************
 * Function Definition : ShowSatLiqS_P_USCS *
 *------------------------------------------*********
 * This function returns the entropy for a			*
 * saturated fluid given pressure.					*
 ****************************************************/
int ShowSatLiqS_P_USCS(double* eosset,
	double pres,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* s,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the entropy
	*s = bwrs->GetSatLiqS_P_USCS(pres);

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

/******************************************
 * Function Definition : ShowSatLiqS_P_SI *
 *----------------------------------------***********
 * This function returns the entropy for a			*
 * saturated fluid given pressure.					*
 ****************************************************/
int ShowSatLiqS_P_SI(double* eosset,
	double pres,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* s,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the entropy
	*s = bwrs->GetSatLiqS_P_SI(pres);

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

/********************************************
 * Function Definition : ShowSatVapS_P_USCS *
 *------------------------------------------*********
 * This function returns the entropy for a			*
 * fluid at saturation given the pressure.			*
 ****************************************************/
int ShowSatVapS_P_USCS(double* eosset,
	double pres,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* s,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the entropy
	*s = bwrs->GetSatVapS_P_USCS(pres);

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

/******************************************
 * Function Definition : ShowSatVapS_P_SI *
 *----------------------------------------***********
 * This function returns the entropy for a			*
 * fluid at saturation given the pressure in SI.	*
 ****************************************************/
int ShowSatVapS_P_SI(double* eosset,
	double pres,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* s,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the entropy
	*s = bwrs->GetSatVapS_P_SI(pres);

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

/***********************************************
 * Function Definition : ShowViscosityGas_USCS *
 *---------------------------------------------******
 * This function returns the viscosity for a fluid  *
 * at a given the pressure and temperature          *
 ****************************************************/
int ShowViscosityGas_TP_USCS(double* eosset,
	double* temp,
	double pres,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* v,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the entropy
	*v = bwrs->GetViscosityGas_TP_USCS(*temp, pres);

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

/////////////////////////////////////////////////////
// Function Definition : ShowViscosityGas_TP_SI
/////////////////////////////////////////////////////
// This function returns the viscosity for a fluid
// at a given the pressure and temperature
/////////////////////////////////////////////////////
int ShowViscosityGas_TP_SI(double* eosset,
	double* temp,
	double pres,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* v,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the entropy
	*v = bwrs->GetViscosityGas_TP_SI(*temp, pres);

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

/// <summary>
/// Retrieves the higher heating value (HHV) in SI units, (J/gmol), and error information for a given fluid index using the CBWRS class.
/// </summary>
/// <param name="fluidindex">Pointer to an integer representing the fluid index.</param>
/// <param name="hhv">Pointer to a double to hold the fluid higher heating value in SI units, (J/gmol).</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to hold the main error line.</param>
/// <returns>An integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <author>Brian Howard</author>
/// <date>2001</date>
/// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
int ShowHHV_SI(int* fluidindex, double* hhv,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	*hhv = bwrs->GetHHV_SI(*fluidindex);
	i = 0;
	pmerrline = NULL;

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

/// <summary>
/// Retrieves the higher heating value (HHV) in USCS units, (BTU/lbmol), and error information for a given fluid index using the CBWRS class.
/// </summary>
/// <param name="fluidindex">Pointer to an integer representing the fluid index.</param>
/// <param name="hhv">Pointer to a double to hold the fluid higher heating value in USCS units, (BTU/lbmol).</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to hold the main error line.</param>
/// <returns>An integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <author>Brian Howard</author>
/// <date>2001</date>
/// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
int ShowHHV_USCS(int* fluidindex, double* hhv,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	*hhv = bwrs->GetHHV_USCS(*fluidindex);
	i = 0;
	pmerrline = NULL;

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

////////////////////////////////////////////////////////////
// Function Definition : ShowHHV_mx_SI
////////////////////////////////////////////////////////////
// This function returns the higher heating value
// for the mixture in SI units.
////////////////////////////////////////////////////////////
int ShowHHV_mx_SI(double* eosset,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* hhv,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the higher heating value
	*hhv = bwrs->GetHHV_mx_SI();

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

////////////////////////////////////////////////////////////
// Function Definition : ShowHHV_mx_USCS
////////////////////////////////////////////////////////////
// This function returns the higher heating value
// for the mixture in USCS units.
////////////////////////////////////////////////////////////
int ShowHHV_mx_USCS(double* eosset,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* hhv,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 3 Sep 2023, used heap memory to avoid stack overflow
	// and more standard library functions. 
	// Was: CBWRS bwrs;
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	i = 0;
	pmerrline = NULL;

	//...And load the mixture data into the BWRS object
	if (!bwrs->SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs = bwrs->GetMessageCount();
		if (errs > 0)
		{
			pmerrline = bwrs->GetMessageMain(1);
			strcpy_s(mainerrline01, strlen(mainerrline01), pmerrline);
			*priority01 = bwrs->GetMessagePriority(1);
			return errs;
		}

	}

	//Now load the solver configuration
	bwrs->SetPrecision(Precision);
	bwrs->SetMaxIterations((int)MaxIterations);

	//and get the higher heating value
	*hhv = bwrs->GetLHV_mx_USCS();

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

