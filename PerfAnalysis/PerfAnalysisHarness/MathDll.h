// MathDll.h: definition of the MATH_API macro
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATHDLL_H__96F06E1E_90DF_46E2_BB0A_A0302728AB7D__INCLUDED_)
#define AFX_MATHDLL_H__96F06E1E_90DF_46E2_BB0A_A0302728AB7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MATH_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MATH_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef MATH_EXPORTS
#define MATH_API __declspec(dllexport)
#else
#define MATH_API __declspec(dllimport)
#endif


#endif // !defined(AFX_MATHDLL_H__96F06E1E_90DF_46E2_BB0A_A0302728AB7D__INCLUDED_)
