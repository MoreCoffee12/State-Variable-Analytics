// As the name of the file crashtestdummy infers, these abstracted
// class exists only to test the parent class.  None of the functions
// return valid responses
//
// Brian Howard
//
// 11 Jan 2002
//
//////////////////////////////////////////////////////////////////////

#include "crashtestdummy.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCrashTestDummy::CCrashTestDummy()
{
	//Initialize object variables
}

CCrashTestDummy::~CCrashTestDummy()
{

	//Clean up
	//delete debugLog;


}
  
/***********************
 * Function Definition *
 *---------------------*******************************
 * This function calculations the entropy departure  *
 * of the liquid phase.								 *
 * Ref.												 *
 ****************************************************/
double CCrashTestDummy::GetSDepartLiquid_TP_USCS( double T, double P )
{
	return -1;
}

/***********************
 * Function Definition *
 *---------------------*******************************
 * This function calculations the entropy departure	 *
 * of the vapor phase.								 *
 * Ref.												 *
 ****************************************************/
double CCrashTestDummy::GetSDepartVapor_TP_USCS( double T, double P )
{
	return -1;
}

/***********************
 * Function Definition *
 *---------------------*******************************
 ****************************************************/
 double CCrashTestDummy::GetFalsePositionMolarDensity( double T, 
														double P,
														double initialMD )
 {

	return -1;

 }


 /***********************
 * Function Definition *
 *---------------------*******************************
 ****************************************************/
double CCrashTestDummy::EOS( double T, double MD )
{
	
	return -1;

}

/***********************
 * Function Definition *
 *---------------------*******************************
 ****************************************************/
double CCrashTestDummy::EOSFugacity( double T, double MD )
{

	return -1;

}

/***********************
 * Function Definition *
 *---------------------*******************************
 ****************************************************/
double CCrashTestDummy::GetFugacity_TP_USCS( double T, double P )
{

	return -1;

}

/***********************
 * Function Definition *
 *---------------------*******************************
 ****************************************************/
double CCrashTestDummy::GetFalsePositionFugacity( double T, double P, double initialMD )
{

	return -1;

}

 
double CCrashTestDummy::GetMolarDensity_TP_USCS( double T, double P )
{

	return -1;

}

double CCrashTestDummy::GetMolarDensity_TP_SI( double T, double P )
{

	return -1;

}
 /***********************
 * Function Definition *
 *---------------------*******************************
 ****************************************************/
 double CCrashTestDummy::GuessMolarDensity()
{

	return -1;

}

