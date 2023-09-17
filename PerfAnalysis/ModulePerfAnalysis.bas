Attribute VB_Name = "ModulePerfAnalysis"
'We want to force declarations of variables to be sure we have stack alignment across the VBA and C++ code.
Option Explicit


Declare PtrSafe Function CCOMPEXP _
    Lib "PerfAnalysis.dll" _
        (ByRef connrodlength As Double, _
         ByRef stroke As Double, _
         ByRef bore As Double, _
         ByRef rod As Double, _
         ByRef crankangle As Double, _
         ByRef clearance As Double, _
         ByRef nexp As Double, _
         ByRef ncomp As Double, _
         ByRef psuct As Double, _
         ByRef pdish As Double, _
         ByRef headend As Boolean) As Double

' =============================================================================
' Function Name:  CompExp
' Name: CompExp
' Purpose: Retrieves the chamber pressure at a particular crank angle for a given
'          suction pressure, discharge pressure, isentropic exponent, and compressor
'          kinematic arrangement. It returns a -1 value for any error.
'          It serves as a wrapper for the CCOMPEXP DLL function.
'
' Parameters:
' - connrodlength: The connecting rod length in USCS units, inches
' - stroke: The compressor stroke in USCS units, inches.
' - bore: The bore diameter in USCS units, inches.
' - rod: The rod diameter in USCS units, inches.
' - crankangle: The crank angle in degrees.
' - clearance: The clearance in percentage.
' - nexp: The polytropic expansion exponent (unitless).
' - ncomp: The polytropic compression exponent (unitless).
' - psuct: The absolute suction pressure in USCS units, PSIA.
' - pdish: The absolute discharge pressure in USCS units, PSIA.
' - headend: A boolean value set to true for the head end or false for the crank end.
'
' Returns:
' - Double: pressure calculated in USCS units, PSIA
'
' Error Handling:
' The function has built-in error handling which returns a -1 for any error.
'
' Notes:
' - The function relies on the CCOMPEXP function and handles its return values and errors
'
' Author: Brian Howard
' Date: 2001
' Revision: 17 Sep 2023, Upgraded to 64-bit code
' =============================================================================
Public Function CompExp(connrodlength As Double, _
        stroke As Double, _
        bore As Double, _
        rod As Double, _
        crankangle As Double, _
        clearance As Double, _
        nexp As Double, _
        ncomp As Double, _
        psuct As Double, _
        pdish As Double, _
        headend As Boolean)

    On Error GoTo ErrorCompExp
    
    ' Local variables
    Dim ErrorLocation As String
    Dim MsgString As String

    'Call the .dll routine for compression/expansion
    CompExp = CCOMPEXP(connrodlength, _
                            stroke, _
                            bore, _
                            rod, _
                            crankangle, _
                            clearance, _
                            nexp, _
                            ncomp, _
                            psuct, _
                            pdish, _
                            headend)

    'Avoid Error handler
    Exit Function

ErrorCompExp:
    ErrorLocation = "Error Calculating CompExp"
    MsgString = ErrorLocation & "->" & Str$(Err.Number) & ": " & Err.Description
    MsgBox MsgString
    Exit Function

End Function

