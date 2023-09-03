
////////////////////////////////////////////////////////////////////////
// This program serves as a test harness for 
// the Lee-Kesler equation of state (EOS) model
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
// Written by:	Brian Howard
// Date:       11 Jan 2002
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
//
// Revision History, 05 Sep 2004
//
// Revised test strategy for simple and complex fluids
// B. Howard
// 
////////////////////////////////////////////////////////////////////////
//
// Revision, 3 Sep 2023
//
// Updated to Visual Studio 2022; more use of standard library
//
// B. Howard
//
////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include "leekes.h"
#include "filtercdf.h"

// Revision 3 Sep 2023, from CLEEKES *eos to std::unique_ptr<CLEEKES>& eos
bool displaymembers(std::unique_ptr<CLEEKES>& eos, FILE* stream)
{

	//local variables
	char outputline[2048];
	int i;

	//initialize local variables
	strcpy_s(outputline, sizeof(outputline), "");
	i = 0;

	//Begin by outputting total number of fluids in the model
	printf("Total Fluid Count: %i\n", eos->GetFluidCount());
	fprintf(stream, "Total Fluid Count: %i\n", eos->GetFluidCount());

	//Output column headings - line 1
	strcpy_s(outputline, sizeof(outputline), ",,,Secondary,,,Crit. Temp.,Crit. Temp., Crit. Pres., Crit. Pres.,Crit. Vol.,Crit. Vol.,Omega\n");
	printf(outputline);
	fprintf(stream, outputline);

	//Output column headings - line 2
	strcpy_s(outputline, sizeof(outputline), "TPGL Index,Formula,Name,Name,CAS #,Mo. Wt.,(R),(K),(psia),(bar),(ft3/mol),(cm3/mol),-\n");
	printf(outputline);
	fprintf(stream, outputline);

	//Output members.
	for (i = 1; i <= eos->GetFluidCount(); i++)
	{
		printf("%i,%s,%s,%s,%i-%i-%i,%f,%f,%f,%f,%f,%f,%f,%f\n", eos->GetTPGLIndex(i),
			eos->GetFormula(i),
			eos->GetName(i),
			eos->GetName_Alt01(i),
			eos->GetCASNo(i)[2],
			eos->GetCASNo(i)[1],
			eos->GetCASNo(i)[0],
			eos->GetMolWeight(i),
			eos->GetTempCrit_USCS(i),
			eos->GetTempCrit_SI(i),
			eos->GetPresCrit_USCS(i),
			eos->GetPresCrit_SI(i),
			eos->GetVolCrit_USCS(i),
			eos->GetVolCrit_SI(i),
			eos->GetEcc(i));
		fprintf(stream, "%i,%s,%s,%s,%i-%i-%i,%f,%f,%f,%f,%f,%f,%f,%f\n", eos->GetTPGLIndex(i),
			eos->GetFormula(i),
			eos->GetName(i),
			eos->GetName_Alt01(i),
			eos->GetCASNo(i)[2],
			eos->GetCASNo(i)[1],
			eos->GetCASNo(i)[0],
			eos->GetMolWeight(i),
			eos->GetTempCrit_USCS(i),
			eos->GetTempCrit_SI(i),
			eos->GetPresCrit_USCS(i),
			eos->GetPresCrit_SI(i),
			eos->GetVolCrit_USCS(i),
			eos->GetVolCrit_SI(i),
			eos->GetEcc(i));
	}

	//success
	return true;
}

// Revision 3 Sep 2023, from CLEEKES *eos to std::unique_ptr<CLEEKES>& eos
bool displayLEEKESParams(std::unique_ptr<CLEEKES>& eos, FILE* stream)
{

	//local variables
	char outputline[2048];
	int i;

	//initialize local variables
	strcpy_s(outputline, sizeof(outputline), "");
	i = 0;

	//Output data description
	strcpy_s(outputline, sizeof(outputline), "Lee-Kesler Specific Constants\n");
	printf(outputline);
	fprintf(stream, outputline);

	//Output column headings - line 1
	strcpy_s(outputline, sizeof(outputline), "Fluid Formula, Name, Bo,Ao,Co,Do,Eo,b,a,d,alpha,c,gamma\n");
	printf(outputline);
	fprintf(stream, outputline);

	//Output members.
	for (i = 1; i <= eos->GetFluidCount(); i++)
	{
		printf("%s,%s,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n", eos->GetFormula(i),
			eos->GetName(i),
			eos->GetBo(i),
			eos->GetAo(i),
			eos->GetCo(i),
			eos->GetDo(i),
			eos->GetEo(i),
			eos->Getb(i),
			eos->Geta(i),
			eos->Getd(i),
			eos->GetAlpha(i),
			eos->Getc(i),
			eos->GetGamma(i));
		fprintf(stream, "%s,%s,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n", eos->GetFormula(i),
			eos->GetName(i),
			eos->GetBo(i),
			eos->GetAo(i),
			eos->GetCo(i),
			eos->GetDo(i),
			eos->GetEo(i),
			eos->Getb(i),
			eos->Geta(i),
			eos->Getd(i),
			eos->GetAlpha(i),
			eos->Getc(i),
			eos->GetGamma(i));
	}

	//success
	return true;
}

//main subroutine.  returns nothing to user
void main()
{
	//local variables
	int i;
	int j;
	FILE* stream;
	// Revision, 3 Sep 2023, moved from stack to heap
	// class CLEEKES* eos;
	std::unique_ptr<CLEEKES> eos = std::make_unique<CLEEKES>();
	// Revision 3 Sep 2023, was class CFilterCDF filterCDF;
	// Was: class CFilterCDF* filterCDF;
	auto filterCDF = std::make_unique<CFilterCDF>();
	char outputline[2048];
	// Revision, 3 Sep 2023, defined filename as variable
	const std::string str_filename = "CLEEKESHarness.csv";
	double testvalue;
	double dtemp;
	double dtemp2;
	double dtemp3;
	double Vr;
	double T;
	double Tr;
	double P;
	double Pr;
	double Z;
	// Revision, 3 Sep 2023, moved from stack to heap
	std::vector<double> xarray(1024);
	std::vector<double> yarray(1024);
	double r;
	double fluidarray[25];

	//initialize local variables
	i = 0;
	j = 0;
	stream = NULL;
	strcpy_s(outputline, sizeof(outputline), "");
	testvalue = 3.14159;
	dtemp = 0;
	dtemp2 = 0;
	dtemp3 = 0;
	Vr = 0;
	T = 0;
	Tr = 0;
	P = 0;
	Pr = 0;
	Z = 0;
	for (i = 0; i < 1024; i++)
	{
		xarray[i] = 0;
		yarray[i] = 0;
	}
	r = 0;

	////////////////////////////////////////////////////////////////////////
	// The remainder of this program serves as a container and harness	
	// to test the object members and methods.  Tests also				
	// include exception handling
	////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////
	// Test Sequence
	//	- Verify class instantiation
	//	- Verify member initialization
	//	- Verify method performance	
	////////////////////////////////////////////////////////////////////////

	// Open the log file
	// Revision, d Sep 2023, changed to fopen_s, added error handling
	errno_t err = fopen_s(&stream, str_filename.c_str(), "w");
	if (err != 0 || stream == nullptr)
	{
		strcpy_s(outputline, sizeof(outputline), "Failed to open the log file.\n");
		printf(outputline);
		return;
	}

	//Intro line
	strcpy_s(outputline, sizeof(outputline), "CEOSModel Test through CLEEKES Test Harness.\n");
	printf(outputline);
	fprintf(stream, outputline);

	// Revision 3 Sep 2023, moved to heap
	// Create the instance
	// eos = new CLEEKES();

	//Document instantiation of class
	if (eos == NULL)
	{
		strcpy_s(outputline, sizeof(outputline), "CLEEKES Failed to Instantiate.\n");
		printf(outputline);
		fprintf(stream, outputline);
		return;
	}
	else
	{
		strcpy_s(outputline, sizeof(outputline), "CLEEKES Instantiated.\n");
		printf(outputline);
		fprintf(stream, outputline);
	}

	////////////////////////////////////////////////////////////////////////
	// Documemt member values as found on class instatiation				*
	////////////////////////////////////////////////////////////////////////
	if (!displaymembers(eos, stream))
	{
		strcpy_s(outputline, sizeof(outputline), "Member output failed.\n");
		printf(outputline);
		fprintf(stream, outputline);
		return;
	}

	strcpy_s(outputline, sizeof(outputline), "\n");
	printf(outputline);
	fprintf(stream, outputline);

	////////////////////////////////////////////////////////////////////////
	// Now we'll display the LEEKES specific information					*
	////////////////////////////////////////////////////////////////////////
	if (!displayLEEKESParams(eos, stream))
	{
		strcpy_s(outputline, sizeof(outputline), "Member output failed.\n");
		printf(outputline);
		fprintf(stream, outputline);
		return;
	}

	////////////////////////////////////////////////////////////////////////
	// For the Lee-Kesler using the Pitzer 3-parameter it is possible
	// to generate a generic Pr, Tr Z(0) and Z(ref) tables.  Begin the
	// test procedure by validating these tables
	// Process is as follows:
	// - Test a specific point
	// - Test point set from a file
	// - generate the complete table
	////////////////////////////////////////////////////////////////////////
	//Clear the component array
	for (i = 0; i < eos->GetFluidCount(); i++)
		fluidarray[i] = 0;

	//Set the fluid for methane
	fluidarray[j] = 1.00;

	//Set the variables in the EOS object
	eos->SetMixtureData(fluidarray);
	eos->SetMaxIterations(200);
	eos->SetPrecision(0.0000001);

	//These tables are usually printed as a function of reduced
	 //pressure and temperature.  We will pass temperature and pressure
	 //in as a function of reduced values
	dtemp = eos->GetTempCrit_mx_USCS();
	dtemp2 = eos->GetPresCrit_mx_USCS();


	////////////////////////////////////////////////
	//
	// Simple Fluid Test Program Begins
	//
	////////////////////////////////////////////////
	strcpy_s(outputline, sizeof(outputline), "\n");
	printf(outputline);
	fprintf(stream, outputline);

	//define test point
	Tr = 0.55;
	Pr = 0.10;
	//output results
	strcpy_s(outputline, sizeof(outputline), "Simple Fluid Test Point\n");
	printf(outputline);
	fprintf(stream, outputline);
	fprintf(stream, "Tr, Pr, Z(0) Calc\n");
	eos->CalcSimple((dtemp * Tr), (dtemp2 * Pr), &Vr);
	fprintf(stream, "%f,%f,%f\n", Tr, Pr, ((Pr * Vr) / Tr));
	strcpy_s(outputline, sizeof(outputline), "\n");
	printf(outputline);
	fprintf(stream, outputline);

	//begin test point set check
	strcpy_s(outputline, sizeof(outputline), "Simple Test Point Set\n");
	printf(outputline);
	fprintf(stream, outputline);

	strcpy_s(outputline, sizeof(outputline), "Tr, Pr, Z(0) Pub, Z(0) Calc\n");
	printf(outputline);
	fprintf(stream, outputline);

	// Open the test point set input file
	// Revision 3 Sep 2023, access via pointer instead of the object
	i = filterCDF->GetCols("simple.tst");
	j = filterCDF->GetRows("simple.tst", (int)i);
	filterCDF->LoadData("simple.tst", xarray.data(), (i * j));

	//evaluate each test point
	for (i = 0; i < j; i++)
	{

		Tr = (xarray[(i * 3)]);
		T = (dtemp * Tr);
		Pr = (xarray[(1 + (i * 3))]);
		P = (dtemp2 * Pr);
		dtemp3 = xarray[(2 + (i * 3))];

		eos->CalcSimple(T, P, &Vr);
		Z = ((Pr * Vr) / Tr);

		fprintf(stream, "%f,%f,%f,%f\n",
			Tr,
			Pr,
			dtemp3,
			Z);

		if (fabs(Z - dtemp3) > 0.0002)
		{
			strcpy_s(outputline, sizeof(outputline), "Test Point Set Failed\n");
			printf(outputline);
			fprintf(stream, outputline);
			fclose(stream);
			return;

		}

	}

	strcpy_s(outputline, sizeof(outputline), "Simple Test Point Set Succeeded\n");
	printf(outputline);
	fprintf(stream, outputline);

	strcpy_s(outputline, sizeof(outputline), "Value of Z(0)\n");
	printf(outputline);
	fprintf(stream, outputline);

	for (Tr = 0.30; Tr < 4; Tr = Tr + 0.050)
	{
		if (Tr == 0.300)
		{
			Pr = 0.010;
			fprintf(stream, ",");
			fprintf(stream, "%4.4f,", Pr);
			for (Pr = 0.050; Pr < 10.1; Pr = Pr + 0.050)
			{
				fprintf(stream, "%4.4f,", Pr);
			}
			fprintf(stream, "\n");
		}
		fprintf(stream, "%4.4f,", Tr);
		T = (Tr * eos->GetTempCrit_mx_USCS());

		Pr = 0.010;
		P = (Pr * eos->GetPresCrit_mx_USCS());
		if (eos->CalcSimple(T, P, &Vr))
		{
			fprintf(stream, "%4.4f,", ((Pr * Vr) / Tr));
		}
		else
		{
			fprintf(stream, "CalcSimpleVapor Failed,");
		}

		for (Pr = 0.050; Pr < 10.1; Pr = Pr + 0.050)
		{
			P = (Pr * eos->GetPresCrit_mx_USCS());
			if (eos->CalcSimple(T, P, &Vr))
			{
				fprintf(stream, "%4.4f,", ((Pr * Vr) / Tr));
			}
			else
			{
				fprintf(stream, "CalcSimpleVapor Failed,");
			}
		}
		fprintf(stream, "\n");
	}
	////////////////////////////////////////////////
	//
	// Simple Fluid Test Program Ends
	//
	////////////////////////////////////////////////

	////////////////////////////////////////////////
	//
	// Deviation Test Program Begins
	//
	////////////////////////////////////////////////
	strcpy_s(outputline, sizeof(outputline), "\n");
	printf(outputline);
	fprintf(stream, outputline);

	//define test point
	Tr = 0.55;
	Pr = 0.010;
	//output results
	strcpy_s(outputline, sizeof(outputline), "Deviation Test Point\n");
	printf(outputline);
	fprintf(stream, outputline);
	fprintf(stream, "Tr, Pr, Z(1) Calc\n");
	eos->CalcDeviation((dtemp * Tr), (dtemp2 * Pr), &Vr);
	fprintf(stream, "%f,%f,%f\n", Tr, Pr, ((Pr * Vr) / Tr));
	strcpy_s(outputline, sizeof(outputline), "\n");
	printf(outputline);
	fprintf(stream, outputline);

	//begin test point set check
	strcpy_s(outputline, sizeof(outputline), "Deviation Test Point Set\n");
	printf(outputline);
	fprintf(stream, outputline);

	//write headers
	fprintf(stream, "Tr, Pr, Z(r) Pub, Z(r) Calc\n");

	//Open the test point set input file
	i = filterCDF->GetCols("deviation.tst");
	j = filterCDF->GetRows("deviation.tst", (int)i);
	filterCDF->LoadData("deviation.tst", xarray.data(), (i * j));

	//evaluate each test point
	for (i = 0; i < j; i++)
	{

		Tr = (xarray[(i * 3)]);
		T = (dtemp * Tr);
		Pr = (xarray[(1 + (i * 3))]);
		P = (dtemp2 * Pr);
		dtemp3 = xarray[(2 + (i * 3))];

		eos->CalcDeviation(T, P, &Vr);
		Z = ((Pr * Vr) / Tr);

		fprintf(stream, "%f,%f,%f,%f\n",
			Tr,
			Pr,
			dtemp3,
			Z);

		if (fabs(Z - dtemp3) > 0.0002)
		{
			strcpy_s(outputline, sizeof(outputline), "Test Point Set Failed\n");
			printf(outputline);
			fprintf(stream, outputline);
			fclose(stream);
			return;

		}

	}

	strcpy_s(outputline, sizeof(outputline), "Deviation Test Point Set Succeeded\n");
	printf(outputline);
	fprintf(stream, outputline);

	strcpy_s(outputline, sizeof(outputline), "Value of Z(r)\n");
	printf(outputline);
	fprintf(stream, outputline);

	for (Tr = 0.30; Tr < 4; Tr = Tr + 0.050)
	{
		if (Tr == 0.300)
		{
			Pr = 0.010;
			fprintf(stream, ",");
			fprintf(stream, "%4.4f,", Pr);
			for (Pr = 0.050; Pr < 10.1; Pr = Pr + 0.050)
			{
				fprintf(stream, "%4.4f,", Pr);
			}
			fprintf(stream, "\n");
		}
		fprintf(stream, "%4.4f,", Tr);
		T = (Tr * eos->GetTempCrit_mx_USCS());

		Pr = 0.010;
		P = (Pr * eos->GetPresCrit_mx_USCS());
		if (eos->CalcDeviation(T, P, &Vr))
		{
			fprintf(stream, "%4.4f,", ((Pr * Vr) / Tr));
		}
		else
		{
			fprintf(stream, "CalcDeviationVapor Failed,");
		}

		for (Pr = 0.050; Pr < 10.1; Pr = Pr + 0.050)
		{
			P = (Pr * eos->GetPresCrit_mx_USCS());
			if (eos->CalcDeviation(T, P, &Vr))
			{
				fprintf(stream, "%4.4f,", ((Pr * Vr) / Tr));
			}
			else
			{
				fprintf(stream, "CalcSimpleVapor Failed,");
			}
		}
		fprintf(stream, "\n");
	}

	////////////////////////////////////////////////
	//
	// Deviation Test Program Ends
	//
	////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////
	// Methane test
	////////////////////////////////////////////////////////////////////////
	//Clear the array
	for (j = 0; j < eos->GetFluidCount(); j++)
	{
		//Clear the array
		for (i = 0; i < eos->GetFluidCount(); i++)
			fluidarray[i] = 0;

		//Set the fluid for methane
		fluidarray[j] = 1.00;

		//Set the variables in the EOS object
		eos->SetMixtureData(fluidarray);
		eos->SetMaxIterations(200);
		eos->SetPrecision(0.000001);

		//Set the headers
		printf("Name:,%s\n", eos->GetName(j + 1));
		fprintf(stream, "Name:,%s\n", eos->GetName(j + 1));

		strcpy_s(outputline, sizeof(outputline), "Pressure,Temperature,Spec. Vol.\n");
		printf(outputline);
		fprintf(stream, outputline);

		for (i = 600; i < 1620; i = i + 100)
		{
			dtemp = eos->GetP_VT_USCS(0.33795, (double)i);

			printf("%f,%f,%f\n",
				dtemp,
				(double)i,
				0.33795);
			fprintf(stream, "%f,%f,%f\n",
				dtemp,
				(double)i,
				0.33795);
			if (dtemp <= 0)
			{
				strcpy_s(outputline, sizeof(outputline), "Test Point Set for Methane Failed\n");
				printf(outputline);
				fprintf(stream, outputline);
				fclose(stream);
				return;
			}
		}
	}

	///********************************************************************
	//* The next series of tests checks the EOS							*
	//* solver component by component										*
	//*********************************************************************/
	////Clear the array
	//for (j = 0; j < eos->GetFluidCount(); j++)
	//{
	//	//Clear the array
	//	for (i = 0; i < eos->GetFluidCount(); i++)
	//		fluidarray[i] = 0;

	//	//Set the fluid for methane
	//	fluidarray[j] = 1.00;

	//	//Set the variables in the EOS object
	//	eos->SetMixtureData(fluidarray);
	//	eos->SetMaxIterations(200);
	//	eos->SetPrecision(0.000001);

	//	//Set the headers
	//	printf("Name:,%s\n", eos->GetName(j + 1));
	//	fprintf(stream, "Name:,%s\n", eos->GetName(j + 1));

	//	strcpy(outputline, "Pressure,Temperature,SpecificVolume\n");
	//	printf(outputline);
	//	fprintf(stream, outputline);

	//	for (i = 320; i < 1020; i = i + 20)
	//	{
	//		printf("%f,%f,%f\n",
	//			(double)1,
	//			(double)i,
	//			eos->GetV_TP_USCS((double)i, 1));
	//		fprintf(stream, "%f,%f,%f\n",
	//			(double)1,
	//			(double)i,
	//			eos->GetV_TP_USCS((double)i, 1));
	//	}
	//}

	///********************************************************************
	//* The next series of tests checks the EOS entropy					*
	//* solver component by component										*
	//*********************************************************************/
	////Clear the array
	//for (i = 0; i < eos->GetFluidCount(); i++)
	//	fluidarray[i] = 0;

	////Set the fluid for water
	//fluidarray[17] = 1.00;

	////Set the variables in the EOS object
	//eos->SetMixtureData(fluidarray);
	//eos->SetMaxIterations(200);
	//eos->SetPrecision(0.000001);

	////Set the headers
	//printf("Name:,%s\n", eos->GetName(j + 1));
	//fprintf(stream, "Name:,%s\n", eos->GetName(j + 1));

	//strcpy(outputline, "Pressure,Temperature,entropy\n");
	//printf(outputline);
	//fprintf(stream, outputline);

	//for (i = 560; i < 1020; i = i + 100)
	//{
	//	printf("%f,%f,%f\n",
	//		(double)0.2,
	//		(double)i,
	//		eos->GetS_TP_USCS((double)i, 0.2));
	//	fprintf(stream, "%f,%f,%f\n",
	//		(double)0.2,
	//		(double)i,
	//		eos->GetS_TP_USCS((double)i, 0.2));
	//}

	///********************************************************************
	//* The next series of tests checks the EOS enthalpy departure		*
	//* function for methane												*
	//*********************************************************************/
	////Clear the array
	//for (i = 0; i < eos->GetFluidCount(); i++)
	//	fluidarray[i] = 0;

	////Set the fluid for methane
	//fluidarray[17] = 1.00;

	////Set the variables in the EOS object
	//eos->SetMixtureData(fluidarray);
	//eos->SetMaxIterations(200);
	//eos->SetPrecision(0.000001);

	////Set the headers
	//strcpy(outputline, "Pressure,Temperature,enthalpy departure\n");
	//printf(outputline);
	//fprintf(stream, outputline);

	//printf("%f,%f,%f\n",
	//	(double)200,
	//	(double)519.67,
	//	eos->GetHDepart_TP_USCS(519.67, 200));
	//fprintf(stream, "%f,%f,%f\n",
	//	(double)200,
	//	(double)519.67,
	//	eos->GetHDepart_TP_USCS(519.67, 200));

	///********************************************************************
	//* The next series of tests checks the EOS enthalpy departure		*
	//* function for methan												*
	//*********************************************************************/
	////Clear the array
	//for (i = 0; i < eos->GetFluidCount(); i++)
	//	fluidarray[i] = 0;

	////Set the fluid for 
	//fluidarray[17] = 1.00;

	////Set the variables in the EOS object
	//eos->SetMixtureData(fluidarray);
	//eos->SetMaxIterations(200);
	//eos->SetPrecision(0.000001);

	////Set the headers
	//strcpy(outputline, "Temperature,VaporPressure\n");
	//printf(outputline);
	//fprintf(stream, outputline);

	////iterate throug the temperatures and find the 
	////vapor pressures
	//for (i = 0; i < 200; i++)
	//{

	//	dtemp = ((459.67 + 32.018) + (4 * (double)i));
	//	printf("%f,%f\n",
	//		dtemp,
	//		eos->GetVaporPressure_T_USCS(dtemp));
	//	fprintf(stream, "%f,%f\n",
	//		dtemp,
	//		eos->GetVaporPressure_T_USCS(dtemp));
	//}

	/********************************************************************
	* End-of-program housekeeping functions								*
	*********************************************************************/

	// Document test completion
	printf("File output to %s\n", str_filename.c_str());
	printf("CLEEKES Test Harness Completed Successfully.\n");
	fprintf(stream, "CLEEKES Test Harness Completed Successfully.\n");

	// close the output file
	fclose(stream);

	// All done
	return;
}
