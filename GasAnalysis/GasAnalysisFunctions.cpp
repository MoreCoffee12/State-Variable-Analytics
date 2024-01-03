#include <cstdint>
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
	return 2;
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
	return 1;
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
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
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
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
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
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
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
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowH_H298_SI(int* eosset,
	double* temp,
	double* MixtureArray,
	double* Precision,
	double* MaxIterations,
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
	bwrs->SetPrecision(*Precision);

	//...And the maximum number of iterations
	bwrs->SetMaxIterations((int)(*MaxIterations));

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
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowH_H298_USCS(int* eosset,
	double* temp,
	double* MixtureArray,
	double* Precision,
	double* MaxIterations,
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
	bwrs->SetPrecision(*Precision);
	bwrs->SetMaxIterations((int)(*MaxIterations));

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
/// Returns the ideal gas enthalpy in SI units based on the Passut-Danner correlation.
/// </summary>
/// <param name="eosset">Pointer to a double representing the EOS set. Currently not used but included for future compatibility.</param>
/// <param name="temp">Pointer to a double representing the temperature in kelvin.</param>
/// <param name="MixtureArray">Pointer to a double array representing the mixture.</param>
/// <param name="Precision">Pointer to a double representing the solver's precision.</param>
/// <param name="MaxIterations">Pointer to a double representing the maximum number of iterations for the solver.</param>
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
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowHIdeal_SI(int* eosset,
	double* temp,
	double* MixtureArray,
	double* Precision,
	double* MaxIterations,
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
	bwrs->SetPrecision(*Precision);
	bwrs->SetMaxIterations((int)(*MaxIterations));

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
/// <param name="Precision">Pointer to a double representing the solver's precision.</param>
/// <param name="MaxIterations">Pointer to a double representing the maximum number of iterations for the solver.</param>
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
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowHIdeal_USCS(int* eosset,
	double* temp,
	double* MixtureArray,
	double* Precision,
	double* MaxIterations,
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
	bwrs->SetPrecision(*Precision);
	bwrs->SetMaxIterations((int)(*MaxIterations));

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

 /// <summary>
 /// Retrieves the entropy (based on the JANAF tables) in SI, kJ/(Kg-K), for a given fluid index using the CBWRS class.
 /// </summary>
 /// <param name="eosset">Pointer to a short representing the equation of state set. Currently unused, but reserved for future compatibility.</param>
 /// <param name="temp">Double representing the temperature in SI units, kelvin.</param>
 /// <param name="MixtureArray">Pointer to a double array containing the mole percentages of the mixture.</param>
 /// <param name="Precision">Double representing the solver precision.</param>
 /// <param name="MaxIterations">Double representing the maximum number of solver iterations.</param>
 /// <param name="s">Pointer to a double to hold the calculated entorpy USCS units.</param>
 /// <param name="priority01">Pointer to a double representing the error priority.</param>
 // <param name="mainerrline01">Pointer to a char array to hold the main error line.</param>
 /// <returns>An integer representing the number of errors (0 for no errors).</returns>
 /// <remarks>
 /// This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.
 /// </remarks>
 /// <warning>
 /// Make sure that the char array (mainerrline01) is allocated with sufficient space before calling this function.
 /// </warning>
 /// <revision>Revision, 10 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowSo_SI(int* eosset,
	double temp,
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
	*s = bwrs->Getso_SI(temp);

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
 /// Retrieves the entropy (based on the JANAF tables) in USCS, BTU/(lb-R), for a given fluid index using the CBWRS class.
 /// </summary>
 /// <param name="eosset">Pointer to a short representing the equation of state set. Currently unused, but reserved for future compatibility.</param>
 /// <param name="temp">Double representing the temperature in USCS units, Rankine.</param>
 /// <param name="MixtureArray">Pointer to a double array containing the mole percentages of the mixture.</param>
 /// <param name="Precision">Double representing the solver precision.</param>
 /// <param name="MaxIterations">Double representing the maximum number of solver iterations.</param>
 /// <param name="s">Pointer to a double to hold the calculated entorpy USCS units.</param>
 /// <param name="priority01">Pointer to a double representing the error priority.</param>
 // <param name="mainerrline01">Pointer to a char array to hold the main error line.</param>
 /// <returns>An integer representing the number of errors (0 for no errors).</returns>
 /// <remarks>
 /// This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.
 /// </remarks>
 /// <warning>
 /// Make sure that the char array (mainerrline01) is allocated with sufficient space before calling this function.
 /// </warning>
 /// <revision>Revision, 10 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowSo_USCS(int* eosset,
	double temp,
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
	*s = bwrs->Getso_USCS(temp);

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
/// <revision>Revision, 3 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
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
/// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
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
/// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
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
/// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
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
/// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
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
/// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
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
/// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
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
/// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
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
/// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
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
/// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
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
/// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
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
/// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
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

 /// <summary>
 /// Retrieves the ideal gas enthalpy of formation for the mixture in SI, J/gmol, using the CBWRS class.
 /// </summary>
 /// <param name="eosset">Pointer to a short representing the equation of state set. Currently unused, but reserved for future compatibility.</param>
 /// <param name="MixtureArray">Pointer to a double array containing the mole percentages of the mixture.</param>
 /// <param name="Precision">Pointer to a double representing the solver precision.</param>
 /// <param name="MaxIterations">Pointer to a double representing the maximum number of solver iterations.</param>
 /// <param name="h">Pointer to a double to hold the calculated ethalpy in SI units.</param>
 /// <param name="priority01">Pointer to a double representing the error priority.</param>
 // <param name="mainerrline01">Pointer to a char array to hold the main error line.</param>
 /// <returns>An integer representing the number of errors (0 for no errors).</returns>
 /// <remarks>
 /// This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.
 /// </remarks>
 /// <warning>
 /// Make sure that the char array (mainerrline01) is allocated with sufficient space before calling this function.
 /// </warning>
 /// <revision>Revision, 10 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowHfo_mx_SI(int* eosset,
	double* MixtureArray,
	double* Precision,
	double* MaxIterations,
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
	bwrs->SetPrecision(*Precision);
	bwrs->SetMaxIterations((int)(*MaxIterations));

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

/// <summary>
/// Retrieves the ideal gas enthalpy of formation for the mixture in USCS, BTU/lbmol, using the CBWRS class.
/// </summary>
/// <param name="eosset">Pointer to a short representing the equation of state set. Currently unused, but reserved for future compatibility.</param>
/// <param name="MixtureArray">Pointer to a double array containing the mole percentages of the mixture.</param>
/// <param name="Precision">Pointer to a double representing the solver precision.</param>
/// <param name="MaxIterations">Pointer  to a couble representing the maximum number of solver iterations.</param>
/// <param name="h">Pointer to a double to hold the calculated ethalpy in USCS units.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
// <param name="mainerrline01">Pointer to a char array to hold the main error line.</param>
/// <returns>An integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.
/// </remarks>
/// <warning>
/// Make sure that the char array (mainerrline01) is allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 10 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowHfo_mx_USCS(int* eosset,
	double* MixtureArray,
	double* Precision,
	double* MaxIterations,
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
	bwrs->SetPrecision(*Precision);
	bwrs->SetMaxIterations((int)(*MaxIterations));

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
/// Retrieves the ethalpy of vaporization in SI units, kJ/kmol, and error information for a given fluid index using the CBWRS class.
/// </summary>
/// <param name="fluidindex">Pointer to an integer representing the fluid index.</param>
/// <param name="dT">Pointer to a double with fluid temperature in SI units, kelvin.</param>
/// <param name="dh">Pointer to a double to hold the fluid enthalpy of formation in SI units, kJ/kmol.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to hold the main error line.</param>
/// <returns>An integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowHvap_SI(int* fluidindex, double* dT, double* dh,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	*dh = bwrs->GetHvap_SI(*fluidindex, *dT);
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
/// Retrieves the ethalpy of vaporization in USCS units, BTU/lbmol, and error information for a given fluid index using the CBWRS class.
/// </summary>
/// <param name="fluidindex">Pointer to an integer representing the fluid index.</param>
/// <param name="dT">Pointer to a double with fluid temperature in USCS units, Rankine.</param>
/// <param name="dh">Pointer to a double to hold the fluid enthalpy of formation in USCS units, BTU/lbmol.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to hold the main error line.</param>
/// <returns>An integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowHvap_USCS(int* fluidindex, double* dT, double* dh,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	std::unique_ptr<CBWRS> bwrs = std::make_unique<CBWRS>();
	int errs;
	int i;
	char* pmerrline;

	// Initialize local variables
	errs = 0;
	*dh = bwrs->GetHvap_USCS(*fluidindex, *dT);
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
 /// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
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
/// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
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

 /// <summary>
 /// Retrieves the ideal gas entropy of formation for the mixture in SI, kJ/(Kg-K), using the CBWRS class.
 /// </summary>
 /// <param name="eosset">Pointer to a short representing the equation of state set. Currently unused, but reserved for future compatibility.</param>
 /// <param name="MixtureArray">Pointer to a double array containing the mole percentages of the mixture.</param>
 /// <param name="Precision">Double representing the solver precision.</param>
 /// <param name="MaxIterations">Double representing the maximum number of solver iterations.</param>
 /// <param name="s">Pointer to a double to hold the calculated entropy in SI units.</param>
 /// <param name="priority01">Pointer to a double representing the error priority.</param>
 // <param name="mainerrline01">Pointer to a char array to hold the main error line.</param>
 /// <returns>An integer representing the number of errors (0 for no errors).</returns>
 /// <remarks>
 /// This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.
 /// </remarks>
 /// <warning>
 /// Make sure that the char array (mainerrline01) is allocated with sufficient space before calling this function.
 /// </warning>
 /// <revision>Revision, 10 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowSfo_mx_SI(int* eosset,
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

/// <summary>
/// Retrieves the ideal gas entropy of formation for the mixture in USCS, BTU/(lb-R), using the CBWRS class.
/// </summary>
/// <param name="eosset">Pointer to a short representing the equation of state set. Currently unused, but reserved for future compatibility.</param>
/// <param name="MixtureArray">Pointer to a double array containing the mole percentages of the mixture.</param>
/// <param name="Precision">Double representing the solver precision.</param>
/// <param name="MaxIterations">Double representing the maximum number of solver iterations.</param>
/// <param name="s">Pointer to a double to hold the calculated entropy in SI units.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
// <param name="mainerrline01">Pointer to a char array to hold the main error line.</param>
/// <returns>An integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.
/// </remarks>
/// <warning>
/// Make sure that the char array (mainerrline01) is allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 10 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowSfo_mx_USCS(int* eosset,
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
/// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
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
/// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
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
/// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
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
/// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
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

/// <summary>
/// Retrieves the lower heating value in USCS units, (BTU/lbmol), and error information for a mixture using the CBWRS class.
/// </summary>
/// <param name="eosset">Pointer to a short representing the equation of state set. Currently unused, but reserved for future compatibility.</param>
/// <param name="MixtureArray">Pointer to a double array containing the mole percentages of the mixture.</param>
/// <param name="Precision">Pointer to a double representing the solver precision.</param>
/// <param name="MaxIterations">Pointer to a double representing the maximum number of solver iterations.</param>
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
/// <revision>Revision, 10 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Add validation and test harness.
/// </todo>
int ShowLHV_mx_USCS(int* eosset,
	double* MixtureArray,
	double* Precision,
	double* MaxIterations,
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

	// Check for null pointers
	if (eosset == nullptr)
	{
		char* errptr = "eosset is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (MixtureArray == nullptr)
	{
		char* errptr = "MixtureArray is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (Precision == nullptr)
	{
		char* errptr = "Precision is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (MaxIterations == nullptr)
	{
		char* errptr = "MaxIterations is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (lhv == nullptr)
	{
		char* errptr = "lhv is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (priority01 == nullptr)
	{
		char* errptr = "priority01 is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (mainerrline01 == nullptr)
	{
		return 1;
	}

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
	bwrs->SetPrecision(*Precision);
	//	...and the maximum number of iterations
	bwrs->SetMaxIterations((int)(*MaxIterations));

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

/// <summary>
/// Retrieves the lower heating value in SI units, (J/gmol), and error information for a mixture using the CBWRS class.
/// </summary>
/// <param name="eosset">Pointer to a short representing the equation of state set. Currently unused, but reserved for future compatibility.</param>
/// <param name="MixtureArray">Pointer to a double array containing the mole percentages of the mixture.</param>
/// <param name="Precision">Pointer to a double representing the solver precision.</param>
/// <param name="MaxIterations">Pointer to a double representing the maximum number of solver iterations.</param>
/// <param name="lhv">Pointer to a double to hold the fluid enthalpy of formation in SI units, (J/gmol).</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to hold the main error line.</param>
/// <returns>An integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 10 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Add validation and test harness.
/// </todo>
int ShowLHV_mx_SI(int* eosset,
	double* MixtureArray,
	double* Precision,
	double* MaxIterations,
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

	// Check for null pointers
	if (eosset == nullptr)
	{
		char* errptr = "eosset is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (MixtureArray == nullptr)
	{
		char* errptr = "MixtureArray is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (Precision == nullptr)
	{
		char* errptr = "Precision is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (MaxIterations == nullptr)
	{
		char* errptr = "MaxIterations is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (lhv == nullptr)
	{
		char* errptr = "lhv is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (priority01 == nullptr)
	{
		char* errptr = "priority01 is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (mainerrline01 == nullptr)
	{
		return 1;
	}

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
	bwrs->SetPrecision(*Precision);
	bwrs->SetMaxIterations((int)(*MaxIterations));

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

 /// <summary>
 /// Retrieves the pressure in USCS units, PSIA, and error information for a mixture using the CBWRS class, given the molar density and pressure.
 /// </summary>
 /// <param name="eosset">Pointer to a short representing the equation of state set. Currently unused, but reserved for future compatibility.</param>
 /// <param name="md">Double holding the molar density in USCS units, .</param>
 /// <param name="temp">Double holding the temperature in USCS units, Rankine.</param>
 /// <param name="MixtureArray">Pointer to a double array containing the mole percentages of the mixture.</param>
 /// <param name="Precision">Double representing the solver precision.</param>
 /// <param name="MaxIterations">Double representing the maximum number of solver iterations.</param>
 /// <param name="pres">Pointer to a double to hold the pressure in USCS units, PSIA.</param>
 /// <param name="priority01">Pointer to a double representing the error priority.</param>
 /// <param name="mainerrline01">Pointer to a char array to hold the main error line.</param>
 /// <returns>An integer representing the number of errors (0 for no errors).</returns>
 /// <remarks>
 /// This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.
 /// </remarks>
 /// <warning>
 /// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
 /// </warning>
 /// <revision>Revision, 10 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowP_MT_USCS(int* eosset,
	double md,
	double temp,
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
	*pres = bwrs->GetP_MT_USCS(md, temp);

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
/// Retrieves the pressure in SI units, bar(a), and error information for a mixture using the CBWRS class, given the specific volume and entropy.
/// </summary>
/// <param name="eosset">Pointer to a short representing the equation of state set. Currently unused, but reserved for future compatibility.</param>
/// <param name="v">Double holding the specific volume SI units, cm3/g.</param>
/// <param name="s">Double holding the entropy in SI units, kJ/(Kg-K).</param>
/// <param name="MixtureArray">Pointer to a double array containing the mole percentages of the mixture.</param>
/// <param name="Precision">Double representing the solver precision.</param>
/// <param name="MaxIterations">Double representing the maximum number of solver iterations.</param>
/// <param name="pres">Pointer to a double to hold the pressure in SI units, bar(a).</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to hold the main error line.</param>
/// <returns>An integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 10 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowP_VS_SI(int* eosset,
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

/// <summary>
/// Retrieves the pressure in USCS units, PSIA, and error information for a mixture using the CBWRS class, given the specific volume and entropy.
/// </summary>
/// <param name="eosset">Pointer to a short representing the equation of state set. Currently unused, but reserved for future compatibility.</param>
/// <param name="v">Double holding the specific volume USCS units, ft3/lb.</param>
/// <param name="s">Double holding the entropy in USCS units, BTU/(lb-R).</param>
/// <param name="MixtureArray">Pointer to a double array containing the mole percentages of the mixture.</param>
/// <param name="Precision">Double representing the solver precision.</param>
/// <param name="MaxIterations">Double representing the maximum number of solver iterations.</param>
/// <param name="pres">Pointer to a double to hold the pressure in USCS units, PSIA.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to hold the main error line.</param>
/// <returns>An integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 10 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
short ShowP_VS_USCS(int* eosset,
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

/// <summary>
/// Retrieves the pressure in SI units, bar(a), for a given specific volume and temperature using the CBWRS class.
/// </summary>
/// <param name="eosset">Pointer to a short representing the equation of state set. Currently unused, but reserved for future compatibility.</param>
/// <param name="sv">Double representing the specific volume in SI units, cm3/g.</param>
/// <param name="temp">Double representing the temperature in SI units, kelvin.</param>
/// <param name="MixtureArray">Pointer to a double array containing the mole percentages of the mixture.</param>
/// <param name="Precision">Double representing the solver precision.</param>
/// <param name="MaxIterations">Double representing the maximum number of solver iterations.</param>
/// <param name="v">Pointer to a double to hold the calculated specific volume in USCS units.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to hold the main error line.</param>
/// <returns>An integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowP_VT_SI(int* eosset,
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
	*pres = bwrs->GetP_VT_SI(sv, temp);

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
/// Retrieves the pressure in USCS units, PSIA, for a given specific volume and temperature using the CBWRS class.
/// </summary>
/// <param name="eosset">Pointer to a short representing the equation of state set. Currently unused, but reserved for future compatibility.</param>
/// <param name="sv">Double representing the specific volume in USCS units, ft3/lb.</param>
/// <param name="temp">Double representing the temperature in USCS units, Rankine.</param>
/// <param name="MixtureArray">Pointer to a double array containing the mole percentages of the mixture.</param>
/// <param name="Precision">Double representing the solver precision.</param>
/// <param name="MaxIterations">Double representing the maximum number of solver iterations.</param>
/// <param name="v">Pointer to a double to hold the calculated specific volume in USCS units.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to hold the main error line.</param>
/// <returns>An integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowP_VT_USCS(int* eosset,
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
	*pres = bwrs->GetP_VT_USCS(sv, temp);

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
/// <param name="eosset">Pointer to a short representing the equation of state set. Currently unused, but reserved for future compatibility.</param>
/// <param name="temp">Pointer to a double representing the temperature in SI units, kelvin.</param>
/// <param name="pres">Pointer to a double representing the pressure in SI units, bar(a).</param>
/// <param name="MixtureArray">Pointer to a double array containing the mole percentages of the mixture.</param>
/// <param name="Precision">Pointer to a double representing the solver precision.</param>
/// <param name="MaxIterations">Pointer to a double representing the maximum number of solver iterations.</param>
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
/// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Add validation and test harness.
/// </todo>
int ShowV_TP_SI(int* eosset,
	double* temp,
	double* pres,
	double* MixtureArray,
	double* Precision,
	double* MaxIterations,
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

	// Check for null pointers
	if (eosset == nullptr)
	{
		char* errptr = "eosset is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (temp == nullptr)
	{
		char* errptr = "temp is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (pres == nullptr)
	{
		char* errptr = "pres is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (MixtureArray == nullptr)
	{
		char* errptr = "MixtureArray is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (Precision == nullptr)
	{
		char* errptr = "Precision is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (MaxIterations == nullptr)
	{
		char* errptr = "MaxIterations is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (v == nullptr)
	{
		char* errptr = "v is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (priority01 == nullptr)
	{
		char* errptr = "priority01 is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (mainerrline01 == nullptr)
	{
		return 1;
	}

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
	bwrs->SetPrecision(*Precision);
	bwrs->SetMaxIterations((int)(*MaxIterations));

	//and get the pressure
	*v = bwrs->GetV_TP_SI(*temp, *pres);

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
/// <param name="eosset">Pointer to a short representing the equation of state set. Currently unused, but reserved for future compatibility.</param>
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
/// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <revision>Revision, 16 Sep 2023: added null pointer ; all pointers for input arguments</revision>
/// <todo>
/// Next-time-open items:
/// 1. Add validation and test harness.
/// </todo>
int32_t ShowV_TP_USCS(int32_t* eosset,
	double* temp,
	double* pres,
	double* MixtureArray,
	double* Precision,
	double* MaxIterations,
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

	// Check for null pointers
	if (eosset == nullptr)
	{
		char *errptr = "eosset is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (temp == nullptr)
	{
		char* errptr = "temp is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (pres == nullptr)
	{
		char* errptr = "pres is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (MixtureArray == nullptr)
	{
		char* errptr = "MixtureArray is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (Precision == nullptr)
	{
		char* errptr = "Precision is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (MaxIterations == nullptr)
	{
		char* errptr = "MaxIterations is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (v == nullptr)
	{
		char* errptr = "v is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (priority01 == nullptr)
	{
		char* errptr = "priority01 is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (mainerrline01 == nullptr)
	{
		return 1;
	}

	//See what EOS was selected
	switch (*eosset)
	{
	//If it was the BWRS EOS...
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
		bwrs->SetPrecision(*Precision);
		bwrs->SetMaxIterations((int)(*MaxIterations));

		//and get the pressure
		*v = bwrs->GetV_TP_USCS(*temp, *pres);

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

/// <summary>
/// Retrieves the mixtrue entropy in SI units, kJ/(Kg-K), for a given temperature and pressure using the CBWRS class.
/// </summary>
/// <param name="eosset">Pointer to a short representing the equation of state set. Currently unused, but reserved for future compatibility.</param>
/// <param name="temp">Pointer to a double representing the temperature in SI units, kelvin.</param>
/// <param name="pres">Pointer to a double representing the pressure in SI units, bar(a).</param>
/// <param name="MixtureArray">Pointer to a double array containing the mole percentages of the mixture.</param>
/// <param name="Precision">Pointer to a double representing the solver precision.</param>
/// <param name="MaxIterations">Pointer to a double representing the maximum number of solver iterations.</param>
/// <param name="s">Pointer to a double to hold the calculated entropy in SI units, kJ/(Kg-K).</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to hold the main error line.</param>
/// <returns>An integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowS_TP_SI(int* eosset,
	double* temp,
	double* pres,
	double* MixtureArray,
	double* Precision,
	double* MaxIterations,
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
	bwrs->SetPrecision(*Precision);
	bwrs->SetMaxIterations((int)(*MaxIterations));

	//and get the pressure
	*s = bwrs->GetS_TP_SI(*temp, *pres);

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
/// Retrieves the entropy in USCS units, (BTU/lb-R), for a given temperature and pressure using the CBWRS class.
/// </summary>
/// <param name="eosset">Pointer to a short representing the equation of state set. Currently unused, but reserved for future compatibility.</param>
/// <param name="temp">Pointer to a double representing the temperature in USCS units, Rankine.</param>
/// <param name="pres">Double representing the pressure in USCS units, PSIA.</param>
/// <param name="MixtureArray">Pointer to a double array containing the mole percentages of the mixture.</param>
/// <param name="Precision">Double representing the solver precision.</param>
/// <param name="MaxIterations">Double representing the maximum number of solver iterations.</param>
/// <param name="s">Pointer to a double to hold the calculated entropy in USCS units, kJ/(Kg-K)</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to hold the main error line.</param>
/// <returns>An integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <revision>Revision, 16 Sep 2023: added null pointer ; all pointers for input arguments</revision>
/// <todo>
/// Next-time-open items:
/// 1. Add validation and test harness.
/// </todo>
int ShowS_TP_USCS(int* eosset,
	double* temp,
	double* pres,
	double* MixtureArray,
	double* Precision,
	double* MaxIterations,
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

	// Check for null pointers
	if (eosset == nullptr)
	{
		char* errptr = "eosset is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (temp == nullptr)
	{
		char* errptr = "temp is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (pres == nullptr)
	{
		char* errptr = "pres is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (MixtureArray == nullptr)
	{
		char* errptr = "MixtureArray is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (Precision == nullptr)
	{
		char* errptr = "Precision is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (MaxIterations == nullptr)
	{
		char* errptr = "MaxIterations is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (s == nullptr)
	{
		char* errptr = "s is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (priority01 == nullptr)
	{
		char* errptr = "priority01 is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (mainerrline01 == nullptr)
	{
		return 1;
	}

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
	bwrs->SetPrecision(*Precision);
	bwrs->SetMaxIterations((int)(*MaxIterations));

	//and get the pressure
	*s = bwrs->GetS_TP_USCS(*temp, *pres);

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
/// Retrieves the enthalpy in SI units, (kJ/kg), for a given temperature and pressure using the CBWRS class.
/// </summary>
/// <param name="eosset">Pointer to a short representing the equation of state set. Currently unused, but reserved for future compatibility.</param>
/// <param name="temp">Pointer to a double representing the temperature in SI units, kelvin.</param>
/// <param name="pres">Double representing the pressure in SI units, bar(a).</param>
/// <param name="MixtureArray">Pointer to a double array containing the mole percentages of the mixture.</param>
/// <param name="Precision">Double representing the solver precision.</param>
/// <param name="MaxIterations">Double representing the maximum number of solver iterations.</param>
/// <param name="h">Pointer to a double to hold the calculated enthalpy in SI units.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to hold the main error line.</param>
/// <returns>An integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <revision>Revision, 17 Sep 2023: handled null pointers, switched inputs to pointers.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Add validation and test harness.
/// </todo>
int ShowH_TP_SI(int* eosset,
	double* temp,
	double* pres,
	double* MixtureArray,
	double* Precision,
	double* MaxIterations,
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

	// Check for null pointers
	if (eosset == nullptr)
	{
		char* errptr = "eosset is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (temp == nullptr)
	{
		char* errptr = "temp is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (pres == nullptr)
	{
		char* errptr = "pres is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (MixtureArray == nullptr)
	{
		char* errptr = "MixtureArray is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (Precision == nullptr)
	{
		char* errptr = "Precision is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (MaxIterations == nullptr)
	{
		char* errptr = "MaxIterations is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (h == nullptr)
	{
		char* errptr = "h is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (priority01 == nullptr)
	{
		char* errptr = "priority01 is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (mainerrline01 == nullptr)
	{
		return 1;
	}

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
	bwrs->SetPrecision(*Precision);
	bwrs->SetMaxIterations((int)(*MaxIterations));

	//and get the pressure
	*h = bwrs->GetH_TP_SI(*temp, *pres);

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
/// Retrieves the enthalpy in USCS units, (BTU/lb), for a given temperature and pressure using the CBWRS class.
/// </summary>
/// <param name="eosset">Pointer to a short representing the equation of state set. Currently unused, but reserved for future compatibility.</param>
/// <param name="temp">Double representing the temperature in USCS units, Rankine.</param>
/// <param name="pres">Double representing the pressure in USCS units, PSIA.</param>
/// <param name="MixtureArray">Pointer to a double array containing the mole percentages of the mixture.</param>
/// <param name="Precision">Double representing the solver precision.</param>
/// <param name="MaxIterations">Double representing the maximum number of solver iterations.</param>
/// <param name="v">Pointer to a double to hold the calculated enthalpy in USCS units.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to hold the main error line.</param>
/// <returns>An integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <revision>Revision, 17 Sep 2023: handled null pointers, switched inputs to pointers.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Add validation and test harness.
/// </todo>
int ShowH_TP_USCS(int* eosset,
	double* temp,
	double* pres,
	double* MixtureArray,
	double* Precision,
	double* MaxIterations,
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

	// Check for null pointers
	if (eosset == nullptr)
	{
		char* errptr = "eosset is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (temp == nullptr)
	{
		char* errptr = "temp is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (pres == nullptr)
	{
		char* errptr = "pres is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (MixtureArray == nullptr)
	{
		char* errptr = "MixtureArray is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (Precision == nullptr)
	{
		char* errptr = "Precision is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (MaxIterations == nullptr)
	{
		char* errptr = "MaxIterations is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (h == nullptr)
	{
		char* errptr = "h is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (priority01 == nullptr)
	{
		char* errptr = "priority01 is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (mainerrline01 == nullptr)
	{
		return 1;
	}

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
	bwrs->SetPrecision(*Precision);
	bwrs->SetMaxIterations((int)(*MaxIterations));

	//and get the pressure
	*h = bwrs->GetH_TP_USCS(*temp, *pres);

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
/// Retrieves the mixture enthalpy in SI units, kJ/kg, for a given temperature and entropy using the CBWRS class.
/// </summary>
/// <param name="eosset">Pointer to a short representing the equation of state set. Currently unused, but reserved for future compatibility.</param>
/// <param name="temp">Double representing the temperature in SI units, kelvin.</param>
/// <param name="entr">Double representing the entropy in SI units, kJ/(Kg-K).</param>
/// <param name="MixtureArray">Pointer to a double array containing the mole percentages of the mixture.</param>
/// <param name="Precision">Double representing the solver precision.</param>
/// <param name="MaxIterations">Double representing the maximum number of solver iterations.</param>
/// <param name="h">Pointer to a double to hold the calculated enthalpy in SI units, kJ/kg.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to hold the main error line.</param>
/// <returns>An integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 10 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowH_TS_SI(int* eosset,
	double temp,
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
	*h = bwrs->GetH_TS_SI(temp, entr);

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
/// Retrieves the mixture enthalpy in USCS units, BTU/lb, for a given temperature and entropy using the CBWRS class.
/// </summary>
/// <param name="eosset">Pointer to a short representing the equation of state set. Currently unused, but reserved for future compatibility.</param>
/// <param name="temp">Double representing the temperature in USCS units, Rankine.</param>
/// <param name="entr">Double representing the entropy in USCS units, BTU/(lb-R).</param>
/// <param name="MixtureArray">Pointer to a double array containing the mole percentages of the mixture.</param>
/// <param name="Precision">Double representing the solver precision.</param>
/// <param name="MaxIterations">Double representing the maximum number of solver iterations.</param>
/// <param name="h">Pointer to a double to hold the calculated enthalpy in USCS units, BTU/lb.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to hold the main error line.</param>
/// <returns>An integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 10 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowH_TS_USCS(int* eosset,
	double temp,
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
	*h = bwrs->GetH_TS_USCS(temp, entr);

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
/// Retrieves the mixture enthalpy in SI units, kJ/kg, for a given pressure and entropy using the CBWRS class.
/// </summary>
/// <param name="eosset">Pointer to a short representing the equation of state set. Currently unused, but reserved for future compatibility.</param>
/// <param name="pres">Double representing the pressure in SI units, bar(a).</param>
/// <param name="entr">Double representing the entropy in SI units, kJ/(Kg-K).</param>
/// <param name="MixtureArray">Pointer to a double array containing the mole percentages of the mixture.</param>
/// <param name="Precision">Double representing the solver precision.</param>
/// <param name="MaxIterations">Double representing the maximum number of solver iterations.</param>
/// <param name="h">Pointer to a double to hold the calculated enthalpy in SI units, kJ/kg.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to hold the main error line.</param>
/// <returns>An integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 10 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowH_PS_SI(int* eosset,
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

/// <summary>
/// Retrieves the mixture enthalpy in USCS units, BTU/lb, for a given pressure and entropy using the CBWRS class.
/// </summary>
/// <param name="eosset">Pointer to a short representing the equation of state set. Currently unused, but reserved for future compatibility.</param>
/// <param name="temp">Double representing the pressure in USCS units, PSIA.</param>
/// <param name="entr">Double representing the entropy in USCS units, BTU/(lb-R).</param>
/// <param name="MixtureArray">Pointer to a double array containing the mole percentages of the mixture.</param>
/// <param name="Precision">Double representing the solver precision.</param>
/// <param name="MaxIterations">Double representing the maximum number of solver iterations.</param>
/// <param name="h">Pointer to a double to hold the calculated enthalpy in USCS units, BTU/lb.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to hold the main error line.</param>
/// <returns>An integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 10 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowH_PS_USCS(int* eosset,
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

/// <summary>
/// Retrieves the mixture fugacity in USCS units, PSIA, for a given pressure and temperature using the CBWRS class.
/// </summary>
/// <param name="eosset">Pointer to a short representing the equation of state set. Currently unused, but reserved for future compatibility.</param>
/// <param name="temp">Double representing the pressure in USCS units, PSIA.</param>
/// <param name="pres">Double representing the pressure in USCS units, PSIA.</param>
/// <param name="MixtureArray">Pointer to a double array containing the mole percentages of the mixture.</param>
/// <param name="Precision">Double representing the solver precision.</param>
/// <param name="MaxIterations">Double representing the maximum number of solver iterations.</param>
/// <param name="f">Pointer to a double to hold the calculated fugacity in USCS units, PSIA.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to hold the main error line.</param>
/// <returns>An integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 10 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowF_TP_USCS(int* eosset,
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
	*f = bwrs->GetFugacity_TP_USCS(temp, pres);

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
/// Returns the vapor pressure in USCS units, PSIA, given the fluid temperature.
/// </summary>
/// <param name="eosset">Pointer to an integer representing the Equation of State set.</param>
/// <param name="temp">Double representing the temperature of the fluid in USCS, Rankine.</param>
/// <param name="MixtureArray">Pointer to a double array representing the fluid mixture.</param>
/// <param name="Precision">Double representing the solver precision.</param>
/// <param name="MaxIterations">Double representing the maximum number of solver iterations.</param>
/// <param name="ps">Pointer to a double where the calculated vapor pressure will be stored.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to store the main error line.</param>
/// <returns>Integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// <para>This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.</para>
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 7 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowVapPres_T_USCS(int* eosset,
	double temp,
	double* MixtureArray,
	double Precision,
	double MaxIterations,
	double* ps,
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
	*ps = bwrs->GetVaporPressure_T_USCS(temp);

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
/// Returns the vapor pressure in SI units, bar(a), given the fluid temperature.
/// </summary>
/// <param name="eosset">Pointer to an integer representing the Equation of State set.</param>
/// <param name="temp">Double representing the temperature of the fluid in SI, kelvin.</param>
/// <param name="MixtureArray">Pointer to a double array representing the fluid mixture.</param>
/// <param name="Precision">Double representing the solver precision.</param>
/// <param name="MaxIterations">Double representing the maximum number of solver iterations.</param>
/// <param name="ps">Pointer to a double where the calculated vapor pressure will be stored.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to store the main error line.</param>
/// <returns>Integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// <para>This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.</para>
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 7 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowVapPres_T_SI(int* eosset,
	double temp,
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
	*ps = bwrs->GetVaporPressure_T_SI(temp);

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
/// Returns the specific volume at saturated vapor conditions in USCS units, ft3/lb, given the fluid temperature.
/// </summary>
/// <param name="eosset">Pointer to an integer representing the Equation of State set.</param>
/// <param name="temp">Double representing the temperature of the fluid in USCS, Rankine.</param>
/// <param name="MixtureArray">Pointer to a double array representing the fluid mixture.</param>
/// <param name="Precision">Double representing the solver precision.</param>
/// <param name="MaxIterations">Double representing the maximum number of solver iterations.</param>
/// <param name="v">Pointer to a double where the calculated specific volume will be stored.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to store the main error line.</param>
/// <returns>Integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// <para>This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.</para>
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 10 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowSatVapV_T_USCS(int* eosset,
	double temp,
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
	*v = bwrs->GetSatVapV_T_USCS(temp);

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
/// Returns the specific volume at saturated vapor conditions in SI units, cm3/g, given the fluid temperature.
/// </summary>
/// <param name="eosset">Pointer to an integer representing the Equation of State set.</param>
/// <param name="temp">Double representing the temperature of the fluid in SI, kelvin.</param>
/// <param name="MixtureArray">Pointer to a double array representing the fluid mixture.</param>
/// <param name="Precision">Double representing the solver precision.</param>
/// <param name="MaxIterations">Double representing the maximum number of solver iterations.</param>
/// <param name="v">Pointer to a double where the calculated specific volume will be stored.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to store the main error line.</param>
/// <returns>Integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// <para>This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.</para>
/// <para>If MixtureArray is a nullptr then the function returns errs = 1, priority01 = 1, and places the error message "MixtureArray pointer is null" in mainerrline01.</para>
/// <para>If v is a nullptr then the function returns errs = 1, priority01 = 1, and places the error message "v pointer is null" in mainerrline01.</para>
/// <para>If priority01 is a nullptr then the function returns errs = 1 and places the error message "v pointer is null" in mainerrline01.</para>
/// <para>If mainerrline01 is a nullptr then the function returns errs = 1 and priority01 = 1.</para>
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 10 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Add validation and test harness.
/// </todo>
int ShowSatVapV_T_SI(int* eosset,
	double temp,
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

	// Handle null pointers
	if (MixtureArray == nullptr)
	{
		errs = 1;
		strcpy_s(mainerrline01, strlen(mainerrline01), "MixtureArray pointer is null");
		*priority01 = 1;
		return errs;
	}
	if (v == nullptr)
	{
		errs = 1;
		strcpy_s(mainerrline01, strlen(mainerrline01), "v pointer is null");
		*priority01 = 1;
		return errs;
	}
	if (priority01 == nullptr)
	{
		errs = 1;
		strcpy_s(mainerrline01, strlen(mainerrline01), "priority01 pointer is null");
		return errs;
	}
	if (mainerrline01 == nullptr)
	{
		errs = 1;
		*priority01 = 1;
		return errs;
	}

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
	*v = bwrs->GetSatVapV_T_SI(temp);

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
/// Returns the specific volume in USCS units, FT3/lb, at saturated liquid conditions. Useful for generating a pressure-enthalphy diagram.
/// </summary>
/// <param name="eosset">Pointer to an integer representing the Equation of State set.</param>
/// <param name="temp">Double representing the temperature of the fluid in USCS, Rankine.</param>
/// <param name="MixtureArray">Pointer to a double array representing the fluid mixture.</param>
/// <param name="Precision">Double representing the solver precision.</param>
/// <param name="MaxIterations">Double representing the maximum number of solver iterations.</param>
/// <param name="v">Pointer to a double where the calculated specific volume will be stored.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to store the main error line.</param>
/// <returns>Integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// <para>This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.</para>
/// <para>If MixtureArray is a nullptr then the function returns errs = 1, priority01 = 1, and places the error message "MixtureArray pointer is null" in mainerrline01.</para>
/// <para>If v is a nullptr then the function returns errs = 1, priority01 = 1, and places the error message "v pointer is null" in mainerrline01.</para>
/// <para>If priority01 is a nullptr then the function returns errs = 1 and places the error message "v pointer is null" in mainerrline01.</para>
/// <para>If mainerrline01 is a nullptr then the function returns errs = 1 and priority01 = 1.</para>
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 9 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Add validation and test harness.
/// </todo>
int ShowSatLiqV_T_USCS(int* eosset,
	double temp,
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

	// Handle null pointers
	if (MixtureArray == nullptr)
	{
		errs = 1;
		strcpy_s(mainerrline01, strlen(mainerrline01), "MixtureArray pointer is null");
		*priority01 = 1;
		return errs;
	}
	if (v == nullptr)
	{
		errs = 1;
		strcpy_s(mainerrline01, strlen(mainerrline01), "v pointer is null");
		*priority01 = 1;
		return errs;
	}
	if (priority01 == nullptr)
	{
		errs = 1;
		strcpy_s(mainerrline01, strlen(mainerrline01), "priority01 pointer is null");
		return errs;
	}
	if (mainerrline01 == nullptr)
	{
		errs = 1;
		*priority01 = 1;
		return errs;
	}

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
	*v = bwrs->GetSatLiqV_T_USCS(temp);

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
/// Returns the specific volume in SI units, cm3/g, at saturated liquid conditions. Useful for generating a pressure-enthalphy diagram.
/// </summary>
/// <param name="eosset">Pointer to an integer representing the Equation of State set.</param>
/// <param name="temp">Double representing the temperature of the fluid in SI, kelvin.</param>
/// <param name="MixtureArray">Pointer to a double array representing the fluid mixture.</param>
/// <param name="Precision">Double representing the solver precision.</param>
/// <param name="MaxIterations">Double representing the maximum number of solver iterations.</param>
/// <param name="v">Pointer to a double where the calculated specific volume will be stored.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to store the main error line.</param>
/// <returns>Integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// <para>This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.</para>
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 9 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
	int ShowSatLiqV_T_SI(int* eosset,
	double temp,
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
	*v = bwrs->GetSatLiqV_T_SI(temp);

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
/// Returns the enthalpy in SI units, kJ/kg, at saturated vapor conditions. Useful for generating a pressure-enthalphy diagram.
/// </summary>
/// <param name="eosset">Pointer to an integer representing the Equation of State set.</param>
/// <param name="temp">Double representing the temperature of the fluid in SI, kelvin.</param>
/// <param name="MixtureArray">Pointer to a double array representing the fluid mixture.</param>
/// <param name="Precision">Double representing the solver precision.</param>
/// <param name="MaxIterations">Double representing the maximum number of solver iterations.</param>
/// <param name="h">Pointer to a double where the calculated enthalpy will be stored.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to store the main error line.</param>
/// <returns>Integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// <para>This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.</para>
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 9 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowSatVapH_T_SI(int* eosset,
	double temp,
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
	*h = bwrs->GetSatVapH_T_SI(temp);

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
/// Returns the enthalpy in USCS units, BTU/lb, at saturated vapor conditions. Useful for generating a pressure-enthalphy diagram.
/// </summary>
/// <param name="eosset">Pointer to an integer representing the Equation of State set.</param>
/// <param name="temp">Double representing the temperature of the fluid in USCS, Rankine.</param>
/// <param name="MixtureArray">Pointer to a double array representing the fluid mixture.</param>
/// <param name="Precision">Double representing the solver precision.</param>
/// <param name="MaxIterations">Double representing the maximum number of solver iterations.</param>
/// <param name="h">Pointer to a double where the calculated enthalpy will be stored.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to store the main error line.</param>
/// <returns>Integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// <para>This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.</para>
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 9 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowSatVapH_T_USCS(int* eosset,
	double temp,
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
	*h = bwrs->GetSatVapH_T_USCS(temp);

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
/// Returns the enthalpy in SI units, kJ/kg, at saturated liquid conditions. Useful for generating a pressure-enthalphy diagram.
/// </summary>
/// <param name="eosset">Pointer to an integer representing the Equation of State set.</param>
/// <param name="temp">Double representing the temperature of the fluid in SI, kelvin.</param>
/// <param name="MixtureArray">Pointer to a double array representing the fluid mixture.</param>
/// <param name="Precision">Double representing the solver precision.</param>
/// <param name="MaxIterations">Double representing the maximum number of solver iterations.</param>
/// <param name="h">Pointer to a double where the calculated enthalpy will be stored.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to store the main error line.</param>
/// <returns>Integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// <para>This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.</para>
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 9 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowSatLiqH_T_SI(int* eosset,
	double temp,
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
	*h = bwrs->GetSatLiqH_T_SI(temp);

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
/// Returns the enthalpy in USCS units, BTU/lb, at saturated liquid conditions. Useful for generating a pressure-enthalphy diagram.
/// </summary>
/// <param name="eosset">Pointer to an integer representing the Equation of State set.</param>
/// <param name="temp">Double representing the temperature of the fluid in USCS, Rankine.</param>
/// <param name="MixtureArray">Pointer to a double array representing the fluid mixture.</param>
/// <param name="Precision">Double representing the solver precision.</param>
/// <param name="MaxIterations">Double representing the maximum number of solver iterations.</param>
/// <param name="h">Pointer to a double where the calculated enthalpy will be stored.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to store the main error line.</param>
/// <returns>Integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// <para>This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.</para>
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 9 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowSatLiqH_T_USCS(int* eosset,
	double temp,
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
	*h = bwrs->GetSatLiqH_T_USCS(temp);

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
/// Returns the entropy in USCS units, BTU/(lb-R), at saturated vapor conditions, given temperature. Useful for generating a pressure-enthalphy diagram.
/// </summary>
/// <param name="eosset">Pointer to an integer representing the Equation of State set.</param>
/// <param name="temp">Double representing the temperature of the fluid in USCS, Rankine.</param>
/// <param name="MixtureArray">Pointer to a double array representing the fluid mixture.</param>
/// <param name="Precision">Double representing the solver precision.</param>
/// <param name="MaxIterations">Double representing the maximum number of solver iterations.</param>
/// <param name="s">Pointer to a double where the calculated entropy will be stored.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to store the main error line.</param>
/// <returns>Integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// <para>This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.</para>
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 9 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowSatVapS_T_USCS(int* eosset,
	double temp,
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
	*s = bwrs->GetSatVapS_T_USCS(temp);

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
/// Returns the entropy in USCS units, BTU/(lb-R), at saturated liquid conditions, given temperature. Useful for generating a pressure-enthalphy diagram.
/// </summary>
/// <param name="eosset">Pointer to an integer representing the Equation of State set.</param>
/// <param name="temp">Double representing the temperature of the fluid in USCS, Rankine.</param>
/// <param name="MixtureArray">Pointer to a double array representing the fluid mixture.</param>
/// <param name="Precision">Double representing the solver precision.</param>
/// <param name="MaxIterations">Double representing the maximum number of solver iterations.</param>
/// <param name="s">Pointer to a double where the calculated entropy will be stored.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to store the main error line.</param>
/// <returns>Integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// <para>This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.</para>
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 9 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowSatLiqS_T_USCS(int* eosset,
	double temp,
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
	*s = bwrs->GetSatLiqS_T_USCS(temp);

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
/// Returns the vapor temperature in USCS units, Rankine, given pressure.
/// </summary>
/// <param name="eosset">Pointer to an integer representing the Equation of State set.</param>
/// <param name="pres">Pointer to a double representing the pressure of the fluid in USCS, PSIA.</param>
/// <param name="MixtureArray">Pointer to a double array representing the fluid mixture.</param>
/// <param name="Precision">Pointer to a double representing the solver precision.</param>
/// <param name="MaxIterations">Pointer to a double representing the maximum number of solver iterations.</param>
/// <param name="ts">Pointer to a double where the calculated temperature will be stored.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to store the main error line.</param>
/// <returns>Integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// <para>This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.</para>
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 10 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowVapTemp_P_USCS(int* eosset,
	double* pres,
	double* MixtureArray,
	double* Precision,
	double* MaxIterations,
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

	// Check for null pointers
	if (eosset == nullptr)
	{
		char* errptr = "eosset is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (pres == nullptr)
	{
		char* errptr = "pres is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (MixtureArray == nullptr)
	{
		char* errptr = "MixtureArray is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (Precision == nullptr)
	{
		char* errptr = "Precision is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (MaxIterations == nullptr)
	{
		char* errptr = "MaxIterations is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (ts == nullptr)
	{
		char* errptr = "ts is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (priority01 == nullptr)
	{
		char* errptr = "priority01 is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (mainerrline01 == nullptr)
	{
		return 1;
	}

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
	bwrs->SetPrecision(*Precision);
	bwrs->SetMaxIterations((int)(*MaxIterations));

	//and get the pressure
	*ts = bwrs->GetVaporTemperature_P_USCS(*pres);

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
/// Returns the vapor temperature in SI units, kelvin, given pressure.
/// </summary>
/// <param name="eosset">Pointer to an integer representing the Equation of State set.</param>
/// <param name="pres">Pointer to a double representing the pressure of the fluid in SI, bar.</param>
/// <param name="MixtureArray">Pointer to a double array representing the fluid mixture.</param>
/// <param name="Precision">Pointer to a double representing the solver precision.</param>
/// <param name="MaxIterations">Pointer to a double representing the maximum number of solver iterations.</param>
/// <param name="ts">Pointer to a double where the calculated temperature will be stored.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to store the main error line.</param>
/// <returns>Integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// <para>This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.</para>
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 10 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Add validation and test harness.
/// </todo>
int ShowVapTemp_P_SI(int* eosset,
	double* pres,
	double* MixtureArray,
	double* Precision,
	double* MaxIterations,
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

	// Check for null pointers
	if (eosset == nullptr)
	{
		char* errptr = "eosset is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (pres == nullptr)
	{
		char* errptr = "pres is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (MixtureArray == nullptr)
	{
		char* errptr = "MixtureArray is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (Precision == nullptr)
	{
		char* errptr = "Precision is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (MaxIterations == nullptr)
	{
		char* errptr = "MaxIterations is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (ts == nullptr)
	{
		char* errptr = "ts is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (priority01 == nullptr)
	{
		char* errptr = "priority01 is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (mainerrline01 == nullptr)
	{
		return 1;
	}

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
	bwrs->SetPrecision(*Precision);
	bwrs->SetMaxIterations((int)(*MaxIterations));

	//and get the pressure
	*ts = bwrs->GetVaporTemperature_P_SI(*pres);

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
/// Returns the specific volume at saturated liquid conditions in USCS units, ft3/lb, given pressure.
/// </summary>
/// <param name="eosset">Pointer to an integer representing the Equation of State set.</param>
/// <param name="pres">Double representing the pressure of the fluid in USCS, PSIA.</param>
/// <param name="MixtureArray">Pointer to a double array representing the fluid mixture.</param>
/// <param name="Precision">Double representing the solver precision.</param>
/// <param name="MaxIterations">Double representing the maximum number of solver iterations.</param>
/// <param name="v">Pointer to a double where the calculated specific volume will be stored.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to store the main error line.</param>
/// <returns>Integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// <para>This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.</para>
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 10 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowSatLiqV_P_USCS(int* eosset,
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

/// <summary>
/// Returns the specific volume at saturated liquid conditions in SI units, cm3/g, given pressure.
/// </summary>
/// <param name="eosset">Pointer to an integer representing the Equation of State set.</param>
/// <param name="pres">Double representing the pressure of the fluid in SI, bar(a).</param>
/// <param name="MixtureArray">Pointer to a double array representing the fluid mixture.</param>
/// <param name="Precision">Double representing the solver precision.</param>
/// <param name="MaxIterations">Double representing the maximum number of solver iterations.</param>
/// <param name="v">Pointer to a double where the calculated specific volume will be stored.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to store the main error line.</param>
/// <returns>Integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// <para>This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.</para>
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 10 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowSatLiqV_P_SI(int* eosset,
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

/// <summary>
/// Returns the specific volume at saturated vapor conditions in USCS units, ft3/lb, given pressure.
/// </summary>
/// <param name="eosset">Pointer to an integer representing the Equation of State set.</param>
/// <param name="pres">Double representing the pressure of the fluid in USCS, PSIA.</param>
/// <param name="MixtureArray">Pointer to a double array representing the fluid mixture.</param>
/// <param name="Precision">Double representing the solver precision.</param>
/// <param name="MaxIterations">Double representing the maximum number of solver iterations.</param>
/// <param name="v">Pointer to a double where the calculated specific volume will be stored.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to store the main error line.</param>
/// <returns>Integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// <para>This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.</para>
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 10 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowSatVapV_P_USCS(int* eosset,
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

/// <summary>
/// Returns the specific volume at saturated vapor conditions in SI units, cm3/g, given pressure.
/// </summary>
/// <param name="eosset">Pointer to an integer representing the Equation of State set.</param>
/// <param name="pres">Double representing the pressure of the fluid in SI, bar(a).</param>
/// <param name="MixtureArray">Pointer to a double array representing the fluid mixture.</param>
/// <param name="Precision">Double representing the solver precision.</param>
/// <param name="MaxIterations">Double representing the maximum number of solver iterations.</param>
/// <param name="v">Pointer to a double where the calculated specific volume will be stored.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to store the main error line.</param>
/// <returns>Integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// <para>This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.</para>
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 10 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowSatVapV_P_SI(int* eosset,
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

/// <summary>
/// Returns the temperature in USCS units, Rankine, given enthalpy and pressure.
/// </summary>
/// <param name="eosset">Pointer to an integer representing the Equation of State set.</param>
/// <param name="h">Pointer to a double representing the enthalpy of the fluid in USCS, BTU/lb.</param>
/// <param name="p">Pointer to a double representing the pressure of the fluid in USCS, PSIA.</param>
/// <param name="MixtureArray">Pointer to a double array representing the fluid mixture.</param>
/// <param name="Precision">Pointer to a double representing the solver precision.</param>
/// <param name="MaxIterations">Pointer to a double representing the maximum number of solver iterations.</param>
/// <param name="t">Pointer to a double where the calculated temperature will be stored.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to store the main error line.</param>
/// <returns>Integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// <para>This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.</para>
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 10 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowT_HP_SI(int* eosset,
	double* h,
	double* p,
	double* MixtureArray,
	double* Precision,
	double* MaxIterations,
	double* t,
	double* priority01,
	char* mainerrline01)
{
	// Local variables
	// Revision, 10 Sep 2023, used heap memory to avoid stack overflow
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
	bwrs->SetPrecision(*Precision);
	bwrs->SetMaxIterations((int)(*MaxIterations));

	//and get the temperature
	*t = bwrs->GetT_HP_SI(*h, *p);

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
/// Returns the temperature in SI units, kelvin, given the enthalpy and pressure.
/// </summary>
/// <param name="eosset">Pointer to an integer representing the Equation of State set.</param>
/// <param name="h">Double representing the enthalpy of the fluid in SI, kJ/kg.</param>
/// <param name="p">Double representing the pressure of the fluid in SI, bar(a).</param>
/// <param name="MixtureArray">Pointer to a double array representing the fluid mixture.</param>
/// <param name="Precision">Double representing the solver precision.</param>
/// <param name="MaxIterations">Double representing the maximum number of solver iterations.</param>
/// <param name="t">Pointer to a double where the calculated temperature will be stored.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to store the main error line.</param>
/// <returns>Integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// <para>This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.</para>
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 10 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowT_HP_USCS(int* eosset,
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

/// <summary>
/// Returns the temperature in SI units, kelvin, given the entropy and pressure of a fluid.
/// </summary>
/// <param name="eosset">Pointer to an integer representing the Equation of State set.</param>
/// <param name="s">Pointer to a double representing the entropy of the fluid in SI,  kJ/(Kg-K).</param>
/// <param name="p">Pointer to a double representing the pressure of the fluid in SI, bar(a).</param>
/// <param name="MixtureArray">Pointer to a double array representing the fluid mixture.</param>
/// <param name="Precision">Pointer to a double representing the solver precision.</param>
/// <param name="MaxIterations">Pointer to a double representing the maximum number of solver iterations.</param>
/// <param name="t">Pointer to a double where the calculated temperature will be stored.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to store the main error line.</param>
/// <returns>Integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// <para>This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.</para>
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 5 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowT_SP_SI(int* eosset,
	double* s,
	double* p,
	double* MixtureArray,
	double*  Precision,
	double* MaxIterations,
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
	bwrs->SetPrecision(*Precision);
	bwrs->SetMaxIterations((int)(*MaxIterations));

	//and get the temperature
	*t = bwrs->GetT_SP_SI(*s, *p);

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
/// Returns the temperature in USCS units, Rankine, given the entropy and pressure of a fluid.
/// </summary>
/// <param name="eosset">Pointer to an integer representing the Equation of State set.</param>
/// <param name="s">Double representing the entropy of the fluid in USCS, BTU/lb-R.</param>
/// <param name="p">Double representing the pressure of the fluid in USCS, PSIA.</param>
/// <param name="MixtureArray">Pointer to a double array representing the fluid mixture.</param>
/// <param name="Precision">Double representing the solver precision.</param>
/// <param name="MaxIterations">Double representing the maximum number of solver iterations.</param>
/// <param name="t">Pointer to a double where the calculated temperature will be stored.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to store the main error line.</param>
/// <returns>Integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// <para>This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.</para>
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 5 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <revision>Revision, 16 Sep 2023: added null pointer ; all pointers for input arguments</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowT_SP_USCS(int* eosset,
	double* s,
	double* p,
	double* MixtureArray,
	double* Precision,
	double* MaxIterations,
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

	// Check for null pointers
	if (eosset == nullptr)
	{
		char* errptr = "eosset is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (s == nullptr)
	{
		char* errptr = "s is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (p == nullptr)
	{
		char* errptr = "p is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (MixtureArray == nullptr)
	{
		char* errptr = "MixtureArray is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (Precision == nullptr)
	{
		char* errptr = "Precision is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (MaxIterations == nullptr)
	{
		char* errptr = "MaxIterations is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (t == nullptr)
	{
		char* errptr = "t is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (priority01 == nullptr)
	{
		char* errptr = "priority01 is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (mainerrline01 == nullptr)
	{
		return 1;
	}

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
	bwrs->SetPrecision(*Precision);
	bwrs->SetMaxIterations((int)(*MaxIterations));

	//and get the temperature
	*t = bwrs->GetT_SP_USCS(*s, *p);

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
/// Returns the enthalpy at saturated liquid conditions in USCS units, BTU/lb, given the mixture pressure.
/// </summary>
/// <param name="eosset">Pointer to an integer representing the Equation of State set.</param>
/// <param name="pres">Double representing the pressure of the fluid in USCS, PSIA.</param>
/// <param name="MixtureArray">Pointer to a double array representing the fluid mixture.</param>
/// <param name="Precision">Double representing the solver precision.</param>
/// <param name="MaxIterations">Double representing the maximum number of solver iterations.</param>
/// <param name="h">Pointer to a double where the calculated enthalpy will be stored.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to store the main error line.</param>
/// <returns>Integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// <para>This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.</para>
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 10 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowSatLiqH_P_USCS(int* eosset,
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

/// <summary>
/// Returns the enthalpy at saturated liquid conditions in SI units, KJ/kg, given the mixture pressure.
/// </summary>
/// <param name="eosset">Pointer to an integer representing the Equation of State set.</param>
/// <param name="pres">Double representing the pressure of the fluid in SI, bar(a).</param>
/// <param name="MixtureArray">Pointer to a double array representing the fluid mixture.</param>
/// <param name="Precision">Double representing the solver precision.</param>
/// <param name="MaxIterations">Double representing the maximum number of solver iterations.</param>
/// <param name="h">Pointer to a double where the calculated enthalpy will be stored.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to store the main error line.</param>
/// <returns>Integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// <para>This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.</para>
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 10 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowSatLiqH_P_SI(int* eosset,
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

/// <summary>
/// Returns the enthalpy at saturated vapor conditions in USCS units, BTU/lb, given the mixture pressure.
/// </summary>
/// <param name="eosset">Pointer to an integer representing the Equation of State set.</param>
/// <param name="pres">Pointer to a double representing the pressure of the fluid in USCS, PSIA.</param>
/// <param name="MixtureArray">Pointer to a double array representing the fluid mixture.</param>
/// <param name="Precision">Pointer to a a double representing the solver precision.</param>
/// <param name="MaxIterations">Pointer to a double representing the maximum number of solver iterations.</param>
/// <param name="h">Pointer to a double where the calculated enthalpy will be stored.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to store the main error line.</param>
/// <returns>Integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// <para>This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.</para>
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 10 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowSatVapH_P_USCS(int* eosset,
	double* pres,
	double* MixtureArray,
	double* Precision,
	double* MaxIterations,
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

	// Check for null pointers
	if (eosset == nullptr)
	{
		char* errptr = "eosset is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (pres == nullptr)
	{
		char* errptr = "pres is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (MixtureArray == nullptr)
	{
		char* errptr = "MixtureArray is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (Precision == nullptr)
	{
		char* errptr = "Precision is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (MaxIterations == nullptr)
	{
		char* errptr = "MaxIterations is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (h == nullptr)
	{
		char* errptr = "h is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (priority01 == nullptr)
	{
		char* errptr = "priority01 is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (mainerrline01 == nullptr)
	{
		return 1;
	}

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
	bwrs->SetPrecision(*Precision);
	bwrs->SetMaxIterations((int)(*MaxIterations));

	//and get the enthalpy
	*h = bwrs->GetSatVapH_P_USCS(*pres);

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
/// Returns the enthalpy at saturated vapor conditions in SI units, KJ/kg, given the mixture pressure.
/// </summary>
/// <param name="eosset">Pointer to an integer representing the Equation of State set.</param>
/// <param name="pres">Pointer to a double representing the pressure of the fluid in SI, bar(a).</param>
/// <param name="MixtureArray">Pointer to a double array representing the fluid mixture.</param>
/// <param name="Precision">Pointer to a double representing the solver precision.</param>
/// <param name="MaxIterations">Pointer to a double representing the maximum number of solver iterations.</param>
/// <param name="h">Pointer to a double where the calculated enthalpy will be stored.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to store the main error line.</param>
/// <returns>Integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// <para>This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.</para>
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 10 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Add validation and test harness.
/// </todo>
int ShowSatVapH_P_SI(int* eosset,
	double* pres,
	double* MixtureArray,
	double* Precision,
	double* MaxIterations,
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

	// Check for null pointers
	if (eosset == nullptr)
	{
		char* errptr = "eosset is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (pres == nullptr)
	{
		char* errptr = "pres is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (MixtureArray == nullptr)
	{
		char* errptr = "MixtureArray is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (Precision == nullptr)
	{
		char* errptr = "Precision is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (MaxIterations == nullptr)
	{
		char* errptr = "MaxIterations is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (h == nullptr)
	{
		char* errptr = "h is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (priority01 == nullptr)
	{
		char* errptr = "priority01 is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (mainerrline01 == nullptr)
	{
		return 1;
	}

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
	bwrs->SetPrecision(*Precision);
	bwrs->SetMaxIterations((int)(*MaxIterations));

	//and get the enthalpy
	*h = bwrs->GetSatVapH_P_SI(*pres);

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
/// Returns the entropy at saturated liquid conditions in USCS units, BTU/(lb-R), given the mixture pressure.
/// </summary>
/// <param name="eosset">Pointer to an integer representing the Equation of State set.</param>
/// <param name="pres">Double representing the pressure of the fluid in USCS, PSIA.</param>
/// <param name="MixtureArray">Pointer to a double array representing the fluid mixture.</param>
/// <param name="Precision">Double representing the solver precision.</param>
/// <param name="MaxIterations">Double representing the maximum number of solver iterations.</param>
/// <param name="s">Pointer to a double where the calculated entropy will be stored.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to store the main error line.</param>
/// <returns>Integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// <para>This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.</para>
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 10 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowSatLiqS_P_USCS(int* eosset,
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

/// <summary>
/// Returns the entropy at saturated liquid conditions in SI units, KJ/(kg-K), given the mixture pressure.
/// </summary>
/// <param name="eosset">Pointer to an integer representing the Equation of State set.</param>
/// <param name="pres">Double representing the pressure of the fluid in SI, bar(a).</param>
/// <param name="MixtureArray">Pointer to a double array representing the fluid mixture.</param>
/// <param name="Precision">Double representing the solver precision.</param>
/// <param name="MaxIterations">Double representing the maximum number of solver iterations.</param>
/// <param name="s">Pointer to a double where the calculated entropy will be stored.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to store the main error line.</param>
/// <returns>Integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// <para>This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.</para>
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 10 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowSatLiqS_P_SI(int* eosset,
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

/// <summary>
/// Returns the entropy at saturated vapor conditions in USCS units, BTU/(lb-R), given the mixture pressure.
/// </summary>
/// <param name="eosset">Pointer to an integer representing the Equation of State set.</param>
/// <param name="pres">Pointer to a double representing the pressure of the fluid in USCS, PSIA.</param>
/// <param name="MixtureArray">Pointer to a double array representing the fluid mixture.</param>
/// <param name="Precision">Pointer to a double representing the solver precision.</param>
/// <param name="MaxIterations">Pointer to a double representing the maximum number of solver iterations.</param>
/// <param name="s">Pointer to a double where the calculated entropy will be stored.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to store the main error line.</param>
/// <returns>Integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// <para>This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.</para>
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 10 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Add validation and test harness.
/// </todo>
int ShowSatVapS_P_USCS(int* eosset,
	double* pres,
	double* MixtureArray,
	double* Precision,
	double* MaxIterations,
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

	// Check for null pointers
	if (eosset == nullptr)
	{
		char* errptr = "eosset is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (pres == nullptr)
	{
		char* errptr = "pres is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (MixtureArray == nullptr)
	{
		char* errptr = "MixtureArray is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (Precision == nullptr)
	{
		char* errptr = "Precision is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (MaxIterations == nullptr)
	{
		char* errptr = "MaxIterations is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (s == nullptr)
	{
		char* errptr = "s is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (priority01 == nullptr)
	{
		char* errptr = "priority01 is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (mainerrline01 == nullptr)
	{
		return 1;
	}

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
	bwrs->SetPrecision(*Precision);
	bwrs->SetMaxIterations((int)(*MaxIterations));

	//and get the entropy
	*s = bwrs->GetSatVapS_P_USCS(*pres);

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
/// Returns the entropy at saturated vapor conditions in SI units, KJ/(kg-K), given the mixture pressure.
/// </summary>
/// <param name="eosset">Pointer to an integer representing the Equation of State set.</param>
/// <param name="pres">Pointer to a double representing the pressure of the fluid in SI, bar(a).</param>
/// <param name="MixtureArray">Pointer to a double array representing the fluid mixture.</param>
/// <param name="Precision">Pointer to a double representing the solver precision.</param>
/// <param name="MaxIterations">Pointer to a double representing the maximum number of solver iterations.</param>
/// <param name="s">Pointer to a double where the calculated entropy will be stored.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to store the main error line.</param>
/// <returns>Integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// <para>This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.</para>
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 10 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Add validation and test harness.
/// </todo>
int ShowSatVapS_P_SI(int* eosset,
	double* pres,
	double* MixtureArray,
	double* Precision,
	double* MaxIterations,
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

	// Check for null pointers
	if (eosset == nullptr)
	{
		char* errptr = "eosset is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (pres == nullptr)
	{
		char* errptr = "pres is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (MixtureArray == nullptr)
	{
		char* errptr = "MixtureArray is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (Precision == nullptr)
	{
		char* errptr = "Precision is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (MaxIterations == nullptr)
	{
		char* errptr = "MaxIterations is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (s == nullptr)
	{
		char* errptr = "s is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (priority01 == nullptr)
	{
		char* errptr = "priority01 is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (mainerrline01 == nullptr)
	{
		return 1;
	}

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
	bwrs->SetPrecision(*Precision);
	bwrs->SetMaxIterations((int)(*MaxIterations));

	//and get the entropy
	*s = bwrs->GetSatVapS_P_SI(*pres);

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
/// Returns the gas absolute viscosity at in USCS units, lb/ft-sec, given the mixture temperture and pressure
/// </summary>
/// <param name="eosset">Pointer to an integer representing the Equation of State set.</param>
/// <param name="temp">Pointer to a double representing the temperature of the fluid in USCS, Rankine.</param>
/// <param name="pres">Pointer to a double representing the pressure of the fluid in USCS, PSIA.</param>
/// <param name="MixtureArray">Pointer to a double array representing the fluid mixture.</param>
/// <param name="Precision">Pointer to a double representing the solver precision.</param>
/// <param name="MaxIterations">Pointer to a double representing the maximum number of solver iterations.</param>
/// <param name="v">Pointer to a double where the calculated viscosity will be stored.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to store the main error line.</param>
/// <returns>Integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// <para>This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.</para>
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 10 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowViscosityGas_TP_USCS(int* eosset,
	double* temp,
	double* pres,
	double* MixtureArray,
	double* Precision,
	double* MaxIterations,
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

	// Check for null pointers
	if (eosset == nullptr)
	{
		char* errptr = "eosset is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (pres == nullptr)
	{
		char* errptr = "pres is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (temp == nullptr)
	{
		char* errptr = "temp is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (MixtureArray == nullptr)
	{
		char* errptr = "MixtureArray is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (Precision == nullptr)
	{
		char* errptr = "Precision is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (MaxIterations == nullptr)
	{
		char* errptr = "MaxIterations is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (v == nullptr)
	{
		char* errptr = "v is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (priority01 == nullptr)
	{
		char* errptr = "priority01 is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (mainerrline01 == nullptr)
	{
		return 1;
	}

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
	bwrs->SetPrecision(*Precision);
	bwrs->SetMaxIterations((int)(*MaxIterations));

	//and get the entropy
	*v = bwrs->GetViscosityGas_TP_USCS(*temp, *pres);

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
/// Returns the gas absolute viscosity at in SI units, kg/m-sec, given the mixture temperture and pressure
/// </summary>
/// <param name="eosset">Pointer to an integer representing the Equation of State set.</param>
/// <param name="temp">Pointer to a double representing the temperature of the fluid in SI, kelvin.</param>
/// <param name="pres">Pointer to a double representing the pressure of the fluid in SI, bar(a).</param>
/// <param name="MixtureArray">Pointer to a double array representing the fluid mixture.</param>
/// <param name="Precision">Pointer to a double representing the solver precision.</param>
/// <param name="MaxIterations">Pointer to a double representing the maximum number of solver iterations.</param>
/// <param name="v">Pointer to a double where the calculated viscosity, in units of uPoise, will be stored.</param>
/// <param name="priority01">Pointer to a double representing the error priority.</param>
/// <param name="mainerrline01">Pointer to a char array to store the main error line.</param>
/// <returns>Integer representing the number of errors (0 for no errors).</returns>
/// <remarks>
/// <para>This function uses std::unique_ptr for better memory management and to avoid stack overflow issues.</para>
/// </remarks>
/// <warning>
/// Make sure that the char arrays (mainerrline01) are allocated with sufficient space before calling this function.
/// </warning>
/// <revision>Revision, 10 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowViscosityGas_TP_SI(int* eosset,
	double* temp,
	double* pres,
	double* MixtureArray,
	double* Precision,
	double* MaxIterations,
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

	// Check for null pointers
	if (eosset == nullptr)
	{
		char* errptr = "eosset is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (pres == nullptr)
	{
		char* errptr = "pres is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (temp == nullptr)
	{
		char* errptr = "temp is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (MixtureArray == nullptr)
	{
		char* errptr = "MixtureArray is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (Precision == nullptr)
	{
		char* errptr = "Precision is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (MaxIterations == nullptr)
	{
		char* errptr = "MaxIterations is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (v == nullptr)
	{
		char* errptr = "v is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (priority01 == nullptr)
	{
		char* errptr = "priority01 is null";
		strcpy_s(mainerrline01, strlen(mainerrline01), errptr);
		return 1;
	}
	if (mainerrline01 == nullptr)
	{
		return 1;
	}

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
	bwrs->SetPrecision(*Precision);
	bwrs->SetMaxIterations((int)(*MaxIterations));

	//and get the entropy
	*v = bwrs->GetViscosityGas_TP_SI(*temp, *pres);

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
/// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
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
/// <revision>Revision, 4 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
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

/// <summary>
/// Retrieves the higher heating value in USCS units, (BTU/lbmol), and error information for a mixture using the CBWRS class.
/// </summary>
/// <param name="eosset">Pointer to a short representing the equation of state set. Currently unused, but reserved for future compatibility.</param>
/// <param name="MixtureArray">Pointer to a double array containing the mole percentages of the mixture.</param>
/// <param name="Precision">Double representing the solver precision.</param>
/// <param name="MaxIterations">Double representing the maximum number of solver iterations.</param>
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
/// <revision>Revision, 10 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowHHV_mx_SI(int* eosset,
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

/// <summary>
/// Retrieves the higher heating value in USCS units, (BTU/lbmol), and error information for a mixture using the CBWRS class.
/// </summary>
/// <param name="eosset">Pointer to a short representing the equation of state set. Currently unused, but reserved for future compatibility.</param>
/// <param name="MixtureArray">Pointer to a double array containing the mole percentages of the mixture.</param>
/// <param name="Precision">Double representing the solver precision.</param>
/// <param name="MaxIterations">Double representing the maximum number of solver iterations.</param>
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
/// <revision>Revision, 10 Sep 2023: used heap memory via std::unique_ptr and more standard library functions to improve efficiency and safety. Update strcpy to strcpy_s.</revision>
/// <todo>
/// Next-time-open items:
/// 1. Handle nullptr values.
/// 2. Add validation and test harness.
/// </todo>
int ShowHHV_mx_USCS(int* eosset,
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

