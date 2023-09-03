// meshan.h: interface for the CMesHan class.
//
//////////////////////////////////////////////////////////////////////
#ifndef _CMESHAN
#define _CMESHAN

#include <string.h>

class CMeshan  
{
public:

	/*Variables
	******************************************************************/
	class			CMeshan *next;

	/*Functions
	******************************************************************/
					CMeshan();
	virtual			~CMeshan();
	
	//object definition functions
	bool			SetMainText( char *textline );
	bool			SetAuxText( char *textline );
	bool			SetPriority( int messagepriority );

	//object query functions
	char			*GetMainText();
	char			*GetAuxText();
	int				GetPriority();

protected:

	/*Variables
	******************************************************************/
	char			maintext[2048];
	char			auxtext[2048];
	int				priority;

};

#endif