// meshan.cpp: implementation of the meshan class.
//
// This object holds is designed to support message handling
// needs of other objects.
//
//////////////////////////////////////////////////////////////////////

#include "meshan.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMeshan::CMeshan()
{
	//initialize class data
	strcpy( maintext, "" );
	strcpy( auxtext, "" );
	priority			= 0;

	//initialize pointers
	next				= NULL;
}

CMeshan::~CMeshan()
{

}

 /**********************************
 * Function Definition: GetAuxText *
 *---------------------------------******************
 * Returns a pointer to the auxillary or secondary	*
 * text message										*
 ****************************************************/
char *CMeshan::GetAuxText()
{
	//all done
	return auxtext;
}

 /***********************************
 * Function Definition: GetMainText *
 *----------------------------------*****************
 * Returns a pointer to the main text message		*										*
 ****************************************************/
char *CMeshan::GetMainText()
{
	//all done
	return maintext;
}

 /***********************************
 * Function Definition: GetPriority *
 *----------------------------------*****************
 * Returns the priority level of the message		*
 ****************************************************/
int CMeshan::GetPriority()
{
	//all done
	return priority;
}

 /**********************************
 * Function Definition: SetAuxText *
 *---------------------------------******************
 * Loads the auxillary or secondary text message	*
 ****************************************************/
bool CMeshan::SetAuxText( char *textline )
{
	//load the data
	strcpy( auxtext, textline );

	//all done
	return true;
}

 /***********************************
 * Function Definition: SetMainText *
 *-----------------------------------****************
 * Loads the main text message						*
 ****************************************************/
bool CMeshan::SetMainText( char *textline )
{
	//load the data
	strcpy( maintext, textline );

	//all done
	return true;
}

 /************************************
 * Function Definition: SetPriority  *
 *-----------------------------------****************
 * Loads the priority of the message				*
 ****************************************************/
bool CMeshan::SetPriority( int messagepriority )
{
	//load the data
	priority		= messagepriority;

	//all done
	return true;
}
