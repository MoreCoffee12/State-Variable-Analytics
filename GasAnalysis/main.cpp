#include "bwrs.h"
#include "leekes.h"
#include "stdio.h"
#include "iostream.h"
#include "logfile.h"
#include "math.h"

int main()
{


	//local variables
	double					MixtureArray[24];

	//Fill the Mixture Array
	MixtureArray[0]				= 1;	//Methane
	MixtureArray[1]				= 0;	//Ethane
	MixtureArray[2]				= 0;	//Propane
	MixtureArray[3]				= 0;	//I-Butane
	MixtureArray[4]				= 0;	//N-Butane
	MixtureArray[5]				= 0;	//I-Pentane
	MixtureArray[6]				= 0;	//N-Pentane
	MixtureArray[7]				= 0;	//N-Hexane
	MixtureArray[8]				= 0;	//N-Heptane
	MixtureArray[9]				= 0;	//N-Octane
	MixtureArray[10]			= 0;	//Ethylene
	MixtureArray[11]			= 0;	//Propylene
	MixtureArray[12]			= 0;	//Carbon Dioxide
	MixtureArray[13]			= 0;	//Hydrogen Sulfide
	MixtureArray[14]			= 0;	//Nitrogen
	MixtureArray[15]			= 0;	//Hydrogen
	MixtureArray[16]			= 0;	//Ammonia
	MixtureArray[17]			= 0;	//Water
	MixtureArray[18]			= 0;	//Air
	MixtureArray[19]			= 0;	//Carbon Monoxide
	MixtureArray[20]			= 0;	//Argon
	MixtureArray[21]			= 0;	//Oxygen
	MixtureArray[22]			= 0;	//Sulfur Dioxide
	MixtureArray[23]			= 0;	//R134a

	//Local variables
	CBWRS			bwrs;
	int				errs;
	int				i;
	char			*pmerrline;
	double			v;

	//initialize localvariables
	errs			= 0;
	i				= 0;
	pmerrline		= NULL;

	//...And load the mixture data into the BWRS object
	if(!bwrs.SetMixtureData(MixtureArray))
	{
		//Check to see if the action generated any errors
		errs			= bwrs.GetMessageCount();

	}

	//Now load the solver configuration
	bwrs.SetPrecision( 0.00001 );
	bwrs.SetMaxIterations( 200 );

	//and get the pressure
	v				= bwrs.GetV_TP_USCS( 199.67, 1 );
	printf( "Volume = %f\n",v);
	//Check to see if the action generated any errors
	errs			= bwrs.GetMessageCount();
	return errs;
}