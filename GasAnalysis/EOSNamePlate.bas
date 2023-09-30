Attribute VB_Name = "EOSNamePlate"
Option Explicit

'This function returns the number of fluids in
'the EOS engine
Declare PtrSafe Function ShowFluidCount _
    Lib "GasAnalysis.dll" () As Integer

'This function returns the primary name of a fluid
Declare PtrSafe Function ShowName _
    Lib "GasAnalysis.dll" (ByRef fluidindex As Long, _
    ByVal textline As String, _
    ByRef priority01 As Double, _
    ByVal textline01 As String) As Long

'This function returns the auxillary or secondary
'name of a fluid
Declare PtrSafe Function ShowName_Alt01 _
    Lib "GasAnalysis.dll" (ByRef fluidindex As Long, _
                            ByVal textline As String, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Long
                            
'This function returns fluid chemical formula
Declare PtrSafe Function ShowFormula _
    Lib "GasAnalysis.dll" (ByRef fluidindex As Long, _
                            ByVal textline As String, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Long
                            
                            
'This function returns the fluid molecular weight
Declare PtrSafe Function ShowMolWeight _
    Lib "GasAnalysis.dll" (ByRef fluidindex As Long, _
                            ByRef molweight As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Long
                            
                            
'This function returns the fluid critcal temperature (USCS)
Declare PtrSafe Function ShowCritTemp_USCS _
    Lib "GasAnalysis.dll" (ByRef fluidindex As Long, _
                            ByRef crittemp As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Long
                            
'This function returns the fluid critcal temperature (SI)
Declare PtrSafe Function ShowCritTemp_SI _
    Lib "GasAnalysis.dll" (ByRef fluidindex As Long, _
                            ByRef crittemp As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Long

'This function returns critcal pressure (USCS)
'for a fluid
Declare PtrSafe Function ShowCritPres_USCS _
    Lib "GasAnalysis.dll" (ByRef fluidindex As Long, _
                            ByRef critpres As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Long

'This function returns critcal pressure (SI)
'for a fluid
Declare PtrSafe Function ShowCritPres_SI _
    Lib "GasAnalysis.dll" (ByRef fluidindex As Long, _
                            ByRef critpres As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Long

'This function returns critcal volume (USCS)
'for a fluid
Declare PtrSafe Function ShowCritVol_USCS _
    Lib "GasAnalysis.dll" (ByRef fluidindex As Long, _
                            ByRef critvol As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Long

'This function returns critcal volume (SI)
'for a fluid
Declare PtrSafe Function ShowCritVol_SI _
    Lib "GasAnalysis.dll" (ByRef fluidindex As Long, _
                            ByRef critvol As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Long

'This function returns the eccentricity factor
'for a fluid
Declare PtrSafe Function ShowEcc _
    Lib "GasAnalysis.dll" (ByRef fluidindex As Long, _
                            ByRef ecc As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Long

'This function returns the gas constant
'for a fluid
Declare PtrSafe Function ShowR_USCS _
    Lib "GasAnalysis.dll" (ByRef fluidindex As Long, _
                            ByRef r As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Long

'This function returns the enthalpy of formation in USCS
'for a fluid
Declare PtrSafe Function ShowHfo_USCS _
    Lib "GasAnalysis.dll" (ByRef fluidindex As Long, _
                            ByRef h As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Long

'This function returns the enthalpy of formation in SI
'for a fluid
Declare PtrSafe Function ShowHfo_SI _
    Lib "GasAnalysis.dll" (ByRef fluidindex As Long, _
                            ByRef h As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Long

'This function returns the entropy of formation in USCS
'for a fluid
Declare PtrSafe Function ShowSfo_USCS _
    Lib "GasAnalysis.dll" (ByRef fluidindex As Long, _
                            ByRef h As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Long

'This function returns the entropy of formation in SI
'for a fluid
Declare PtrSafe Function ShowSfo_SI _
    Lib "GasAnalysis.dll" (ByRef fluidindex As Long, _
                            ByRef h As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Long

'This function returns the lower heating value in USCS
'for a fluid
Declare PtrSafe Function ShowLHV_USCS _
    Lib "GasAnalysis.dll" (ByRef fluidindex As Long, _
                            ByRef lhv As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Long

'This function returns the lower heating value in SI
'for a fluid
Declare PtrSafe Function ShowLHV_SI _
    Lib "GasAnalysis.dll" (ByRef fluidindex As Long, _
                            ByRef lhv As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Long

'This function returns the higher heating value in USCS
'for a fluid
Declare PtrSafe Function ShowHHV_USCS _
    Lib "GasAnalysis.dll" (ByRef fluidindex As Long, _
                            ByRef hhv As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Long

'This function returns the higher heating value in SI
'for a fluid
Declare PtrSafe Function ShowHHV_SI _
    Lib "GasAnalysis.dll" (ByRef fluidindex As Long, _
                            ByRef hhv As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Long

'This function returns the dipole moment in USCS
'for a fluid
Declare PtrSafe Function ShowDipole_USCS _
    Lib "GasAnalysis.dll" (ByRef fluidindex As Long, _
                            ByRef dipole As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Long

'This function returns the dipole moment in USCS
'for a fluid
Declare PtrSafe Function ShowDipole_SI _
    Lib "GasAnalysis.dll" (ByRef fluidindex As Long, _
                            ByRef dipole As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Long
                            
' =============================================================================
' Function Name:  VBShowFluidCount
' Purpose:        This function interacts with a C++ DLL to retrieve the
'                 number of fluids.
'                 It serves as a wrapper for the ShowFluidCount DLL function.
'
' Parameters:
'   - [None]
'
' Returns:
'   - A Long with number of fluids stored in the DLL.
'
' Usage:
'   num_fluids = VBShowFluidCount()
'
' Error Handling:
'   - The DLL handles all errors
'
' Author:         Brian Howard
' Date Created:   2001
' Last Updated:   30 Sep 2023
' =============================================================================
Function VBShowFluidCount()
 
    VBShowFluidCount = ShowFluidCount()
 
End Function



' =============================================================================
' Function Name:  VBShowName
' Purpose:        This function interacts with a C++ DLL to retrieve the fluid
'                 name based on the fluid index and error tolerance parameters.
'                 It serves as a wrapper for the ShowName DLL function.
'
' Parameters:
'   - fluidindex: The index representing the type of fluid.
'   - ErrTolerance: The error tolerance level.
'
' Returns:
'   - A string value containing either error line information or a text line
'     based on priority and error tolerance.
'
' Usage:
'   result = VBShowName(5, 15)
'
' Error Handling:
'   - Implements custom error handling through ErrorVBShowName label.
'   - If an error occurs, it returns a string with the error number and description.
'
' Author:         Brian Howard
' Date Created:   2001
' Last Updated:   3 Sep 2023
' =============================================================================
Function VBShowName(fluidindex As Long, ErrTolerance As Integer)

    'Local variables
    Dim tline As String
    Dim priority01 As Double
    Dim eline01 As String
    Dim i As Long

    'Establish error trapping
    On Error GoTo ErrorVBShowName

    'Establish max string length
    tline = String(256, "a")
    priority01 = 0
    eline01 = String(256, "a")

    'Check that the Error Tolerance is above minimum
    'Threshold
    If (ErrTolerance < 10) Then
        ErrTolerance = 10
    End If

    'If we need to we can check the return value
    'to see if there was an error
    i = ShowName(fluidindex, _
                    tline, _
                    priority01, _
                    eline01)

    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowName = eline01
    Else
        VBShowName = tline
    End If

    'Avoid the error handler
    Exit Function

ErrorVBShowName:
   'Twiddle the Flag and display the message
    VBShowName = (Str(Err.Number) & ":" & Err.Description)
'    Exit Function

End Function

' =============================================================================
' Function Name:  VBShowName_Alt01
' Purpose:        This function interacts with a C++ DLL to retrieve the fluid
'                 alternate (or secondary) name based on the fluid index and
'                 error tolerance parameters.
'                 It serves as a wrapper for the ShowName_Alt01 DLL function.
'
' Parameters:
'   - fluidindex: The index representing the type of fluid.
'   - ErrTolerance: The error tolerance level.
'
' Returns:
'   - A string value containing either error line information or a text line
'     based on priority and error tolerance.
'
' Usage:
'   result = VBShowName_Alt01(5, 15)
'
' Error Handling:
'   - Implements custom error handling through ErrorVBShowName label.
'   - If an error occurs, it returns a string with the error number and description.
'
' Author:         Brian Howard
' Date Created:   2001
' Last Updated:   3 Sep 2023
' =============================================================================
Function VBShowName_Alt01(fluidindex As Long, ErrTolerance As Integer)

    'Local variables
    Dim tline As String
    Dim priority01 As Double
    Dim eline01 As String
    Dim i As Long

    'Establish error trapping
    On Error GoTo ErrorVBShowName_Alt01

    'Establish max string length
    tline = String(256, "a")
    priority01 = 0
    eline01 = String(256, "a")

    'Check that the Error Tolerance is above minimum
    'Threshold
    If (ErrTolerance < 10) Then
        ErrTolerance = 10
    End If

    'If we need to we can check the return value
    'to see if there was an error
    i = ShowName_Alt01(fluidindex, _
                    tline, _
                    priority01, _
                    eline01)

    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowName_Alt01 = eline01
    Else
        VBShowName_Alt01 = tline
    End If

    'Avoid the error handler
    Exit Function

ErrorVBShowName_Alt01:
   'Twiddle the Flag and display the message
    VBShowName_Alt01 = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

' =============================================================================
' Function Name:  VBShowFormula
' Purpose:        This function interacts with a C++ DLL to retrieve the fluid
'                 chemical formula based on the fluid index and error tolerance
'                 parameters.
'                 It serves as a wrapper for the ShowName_Alt01 DLL function.
'
' Parameters:
'   - fluidindex: The index representing the type of fluid.
'   - ErrTolerance: The error tolerance level.
'
' Returns:
'   - A string value containing either error line information or a text line
'     based on priority and error tolerance.
'
' Usage:
'   result = VBShowName_Alt01(5, 15)
'
' Error Handling:
'   - Implements custom error handling through ErrorVBShowName label.
'   - If an error occurs, it returns a string with the error number and description.
'
' Author:         Brian Howard
' Date Created:   2001
' Last Updated:   3 Sep 2023
' =============================================================================
Function VBShowFormula(fluidindex As Long, ErrTolerance As Integer)

    'Local variables
    Dim tline As String
    Dim priority01 As Double
    Dim eline01 As String
    Dim i As Long
    
    'Establish error trapping
    On Error GoTo ErrorVBShowFormula
    
    'Establish max string length
    tline = String(256, "a")
    priority01 = 0
    eline01 = String(256, "a")
    
    'Check that the Error Tolerance is above minimum
    'Threshold
    If (ErrTolerance < 10) Then
        ErrTolerance = 10
    End If
    
    'If we need to we can check the return value
    'to see if there was an error
    i = ShowFormula(fluidindex, _
                    tline, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowFormula = eline01
    Else
        VBShowFormula = tline
    End If
    
    'Avoid the error handler
    Exit Function

ErrorVBShowFormula:
    VBShowFormula = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

' =============================================================================
' Function Name:  VBShowMolWeight
' Purpose:        This function interacts with a C++ DLL to retrieve the fluid
'                 molecular weight based on the fluid index and error tolerance
'                 parameters.
'                 It serves as a wrapper for the ShowName_Alt01 DLL function.
'
' Parameters:
'   - fluidindex: The index representing the type of fluid.
'   - ErrTolerance: The error tolerance level.
'
' Returns:
'   - A double value containing either error line information or a mol weight
'     based on priority and error tolerance.
'
' Usage:
'   result = VBShowMolWeight(5, 15)
'
' Error Handling:
'   - Implements custom error handling through ErrorVBShowName label.
'   - If an error occurs, it returns a string with the error number and description.
'
' Author:         Brian Howard
' Date Created:   2001
' Last Updated:   4 Sep 2023
' =============================================================================
Function VBShowMolWeight(fluidindex As Long, ErrTolerance As Integer)

    'Local variables
    Dim molweight As Double
    Dim priority01 As Double
    Dim eline01 As String
    Dim i As Long
    
    'Establish error trapping
    On Error GoTo ErrorVBShowMolWeight

    'Initialize local variables
    molweight = 0
    priority01 = 0
    eline01 = String(256, "a")
    
    'Check that the Error Tolerance is above minimum
    'Threshold
    If (ErrTolerance < 10) Then
        ErrTolerance = 10
    End If


    'If we need to we can check the return value
    'to see if there was an error
    i = ShowMolWeight(fluidindex, _
                    molweight, _
                    priority01, _
                    eline01)

    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowMolWeight = eline01
    Else
        VBShowMolWeight = molweight
    End If

    'Avoid the error handler
    Exit Function

ErrorVBShowMolWeight:
    VBShowMolWeight = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function


' =============================================================================
' Function Name:  VBShowCritTemp_USCS
' Purpose:        This function interacts with a C++ DLL to retrieve the fluid
'                 critical temperature in USCS units, Rankine, based on the fluid
'                 index and error tolerance parameters.
'                 It serves as a wrapper for the ShowCritTemp_USCS DLL function.
'
' Parameters:
'   - fluidindex: The index representing the type of fluid.
'   - ErrTolerance: The error tolerance level.
'
' Returns:
'   - A double value containing either error line information or critical temperature
'     based on priority and error tolerance.
'
' Usage:
'   result = VBShowCritTemp_USCS(5, 15)
'
' Error Handling:
'   - Implements custom error handling through ErrorVBShowName label.
'   - If an error occurs, it returns a string with the error number and description.
'
' Author:         Brian Howard
' Date Created:   2001
' Last Updated:   4 Sep 2023
' =============================================================================
Function VBShowCritTemp_USCS(fluidindex As Long, ErrTolerance As Integer)

    'Local variables
    Dim crittemp As Double
    Dim priority01 As Double
    Dim eline01 As String
    Dim i As Long
    
    'Establish error trapping
    On Error GoTo ErrorVBShowCritTemp_USCS

    'Initialize local variables
    crittemp = 0
    priority01 = 0
    eline01 = String(256, "a")
    
    'Check that the Error Tolerance is above minimum
    'Threshold
    If (ErrTolerance < 10) Then
        ErrTolerance = 10
    End If

    'If we need to we can check the return value
    'to see if there was an error
    i = ShowCritTemp_USCS(fluidindex, _
                    crittemp, _
                    priority01, _
                    eline01)

    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowCritTemp_USCS = eline01
    Else
        VBShowCritTemp_USCS = crittemp
    End If

    'Avoid the error handler
    Exit Function

ErrorVBShowCritTemp_USCS:

    VBShowCritTemp_USCS = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

' =============================================================================
' Function Name:  VBShowCritTemp_SI
' Purpose:        This function interacts with a C++ DLL to retrieve the fluid
'                 critical temperature in USCS units, kelvin, based on the fluid
'                 index and error tolerance parameters.
'                 It serves as a wrapper for the ShowCritTemp_SI DLL function.
'
' Parameters:
'   - fluidindex: The index representing the type of fluid.
'   - ErrTolerance: The error tolerance level.
'
' Returns:
'   - A double value containing either error line information or critical temperature
'     based on priority and error tolerance.
'
' Usage:
'   result = VBShowCritTemp_SI(5, 15)
'
' Error Handling:
'   - Implements custom error handling through ErrorVBShowName label.
'   - If an error occurs, it returns a string with the error number and description.
'
' Author:         Brian Howard
' Date Created:   2001
' Last Updated:   4 Sep 2023
' =============================================================================
Function VBShowCritTemp_SI(fluidindex As Long, ErrTolerance As Integer)

    'Local variables
    Dim crittemp As Double
    Dim priority01 As Double
    Dim eline01 As String
    Dim i As Long

    'Establish error trapping
    On Error GoTo ErrorVBShowCritTemp_SI

    'Establish max string length
    crittemp = 0
    priority01 = 0
    eline01 = String(256, "a")

    'Check that the Error Tolerance is above minimum
    'Threshold
    If (ErrTolerance < 10) Then
        ErrTolerance = 10
    End If

    'If we need to we can check the return value
    'to see if there was an error
    i = ShowCritTemp_SI(fluidindex, _
                    crittemp, _
                    priority01, _
                    eline01)

    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowCritTemp_SI = eline01
    Else
        VBShowCritTemp_SI = crittemp
    End If

    'Avoid the error handler
    Exit Function

ErrorVBShowCritTemp_SI:

    VBShowCritTemp_SI = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

' =============================================================================
' Function Name:  VBShowCritPres_USCS
' Purpose:        This function interacts with a C++ DLL to retrieve the fluid
'                 critical pressure in USCS units, PSIA, based on the fluid
'                 index and error tolerance parameters.
'                 It serves as a wrapper for the ShowCritPres_USCS DLL function.
'
' Parameters:
'   - fluidindex: The index representing the type of fluid.
'   - ErrTolerance: The error tolerance level.
'
' Returns:
'   - A double value containing either error line information or critical pressure
'     based on priority and error tolerance.
'
' Usage:
'   result = VBShowCritTemp_USCS(5, 15)
'
' Error Handling:
'   - Implements custom error handling through ErrorVBShowName label.
'   - If an error occurs, it returns a string with the error number and description.
'
' Author:         Brian Howard
' Date Created:   2001
' Last Updated:   4 Sep 2023
' =============================================================================
Function VBShowCritPres_USCS(fluidindex As Long, ErrTolerance As Integer)

    'Local variables
    Dim critpres As Double
    Dim priority01 As Double
    Dim eline01 As String
    Dim i As Long

    'Establish error trapping
    On Error GoTo ErrorVBShowCritPres_USCS

    'Establish max string length
    critpres = 0
    priority01 = 0
    eline01 = String(256, "a")

    'Check that the Error Tolerance is above minimum
    'Threshold
    If (ErrTolerance < 10) Then
        ErrTolerance = 10
    End If

   'If we need to we can check the return value
    'to see if there was an error
    i = ShowCritPres_USCS(fluidindex, _
                    critpres, _
                    priority01, _
                    eline01)

    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowCritPres_USCS = eline01
    Else
        VBShowCritPres_USCS = critpres
    End If

    'Avoid the error handler
    Exit Function

ErrorVBShowCritPres_USCS:

    VBShowCritPres_USCS = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

' =============================================================================
' Function Name:  VBShowCritPres_SI
' Purpose:        This function interacts with a C++ DLL to retrieve the fluid
'                 critical pressure in SI units, bar(a), based on the fluid
'                 index and error tolerance parameters.
'                 It serves as a wrapper for the ShowCritPres_SI DLL function.
'
' Parameters:
'   - fluidindex: The index representing the type of fluid.
'   - ErrTolerance: The error tolerance level.
'
' Returns:
'   - A double value containing either error line information or critical pressure
'     based on priority and error tolerance.
'
' Usage:
'   result = VBShowCritPres_SI(5, 15)
'
' Error Handling:
'   - Implements custom error handling through ErrorVBShowName label.
'   - If an error occurs, it returns a string with the error number and description.
'
' Author:         Brian Howard
' Date Created:   2001
' Last Updated:   4 Sep 2023
' =============================================================================
Function VBShowCritPres_SI(fluidindex As Long, ErrTolerance As Integer)

    'Local variables
    Dim critpres As Double
    Dim priority01 As Double
    Dim eline01 As String
    Dim i As Long

    'Establish error trapping
    On Error GoTo ErrorVBShowCritPres_SI

    'Initialize local variables
    critpres = 0
    priority01 = 0
    eline01 = String(256, "a")

    'Check that the Error Tolerance is above minimum
    'Threshold
    If (ErrTolerance < 10) Then
        ErrTolerance = 10
    End If

    'If we need to we can check the return value
    'to see if there was an error
    i = ShowCritPres_SI(fluidindex, _
                    critpres, _
                    priority01, _
                    eline01)

    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowCritPres_SI = eline01
    Else
        VBShowCritPres_SI = critpres
    End If

    'Avoid the error handler
    Exit Function

ErrorVBShowCritPres_SI:

    VBShowCritPres_SI = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

' =============================================================================
' Function Name:  VBShowCritVol_USCS
' Purpose:        This function interacts with a C++ DLL to retrieve the fluid
'                 critical volume in USCS units, ft3/lbmol, based on the fluid
'                 index and error tolerance parameters.
'                 It serves as a wrapper for the ShowCritVol_USCS DLL function.
'
' Parameters:
'   - fluidindex: The index representing the type of fluid.
'   - ErrTolerance: The error tolerance level.
'
' Returns:
'   - A double value containing either error line information or critical volume
'     based on priority and error tolerance.
'
' Usage:
'   result = VBShowCritVol_USCS(5, 15)
'
' Error Handling:
'   - Implements custom error handling through ErrorVBShowName label.
'   - If an error occurs, it returns a string with the error number and description.
'
' Author:         Brian Howard
' Date Created:   2001
' Last Updated:   4 Sep 2023
' =============================================================================
Function VBShowCritVol_USCS(fluidindex As Long, ErrTolerance As Integer)

    'Local variables
    Dim critvol As Double
    Dim priority01 As Double
    Dim eline01 As String
    Dim i As Long

    'Establish error trapping
    On Error GoTo ErrorVBShowCritVol_USCS

    'Initialize local variables
    critvol = 0
    priority01 = 0
    eline01 = String(256, "a")

    'Check that the Error Tolerance is above minimum
    'Threshold
    If (ErrTolerance < 10) Then
        ErrTolerance = 10
    End If

    'If we need to we can check the return value
    'to see if there was an error
    i = ShowCritVol_USCS(fluidindex, _
                    critvol, _
                    priority01, _
                    eline01)

    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowCritVol_USCS = eline01
    Else
        VBShowCritVol_USCS = critvol
    End If

    'Avoid the error handler
    Exit Function

ErrorVBShowCritVol_USCS:

    VBShowCritVol_USCS = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

' =============================================================================
' Function Name:  VBShowCritVol_SI
' Purpose:        This function interacts with a C++ DLL to retrieve the fluid
'                 critical volume in SI units, cm3/gmol, based on the fluid
'                 index and error tolerance parameters.
'                 It serves as a wrapper for the ShowCritVol_SI DLL function.
'
' Parameters:
'   - fluidindex: The index representing the type of fluid.
'   - ErrTolerance: The error tolerance level.
'
' Returns:
'   - A double value containing either error line information or critical volume
'     based on priority and error tolerance.
'
' Usage:
'   result = VBShowCritVol_SI(5, 15)
'
' Error Handling:
'   - Implements custom error handling through ErrorVBShowName label.
'   - If an error occurs, it returns a string with the error number and description.
'
' Author:         Brian Howard
' Date Created:   2001
' Last Updated:   4 Sep 2023
' =============================================================================
Function VBShowCritVol_SI(fluidindex As Long, ErrTolerance As Integer)

    'Local variables
    Dim critvol As Double
    Dim priority01 As Double
    Dim eline01 As String
    Dim i As Long

    'Establish error trapping
    On Error GoTo ErrorVBShowCritVol_SI

    'Establish max string length, initialize local variables
    critvol = 0
    priority01 = 0
    eline01 = String(256, "a")


    'Check that the Error Tolerance is above minimum
    'Threshold
    If (ErrTolerance < 10) Then
        ErrTolerance = 10
    End If

    'If we need to we can check the return value
    'to see if there was an error
    i = ShowCritVol_SI(fluidindex, _
                    critvol, _
                    priority01, _
                    eline01)

    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowCritVol_SI = eline01
    Else
        VBShowCritVol_SI = critvol
    End If

    'Avoid the error handler
    Exit Function

ErrorVBShowCritVol_SI:

    VBShowCritVol_SI = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

' =============================================================================
' Function Name:  VBShowEcc
' Purpose:        This function interacts with a C++ DLL to retrieve the fluid
'                 eccentricity factor (unitless), based on the fluid
'                 index and error tolerance parameters.
'                 It serves as a wrapper for the ShowEcc DLL function.
'
' Parameters:
'   - fluidindex: The index representing the type of fluid.
'   - ErrTolerance: The error tolerance level.
'
' Returns:
'   - A double value containing either error line information or critical volume
'     based on priority and error tolerance.
'
' Usage:
'   result = VBShowEcc(5, 15)
'
' Error Handling:
'   - Implements custom error handling through ErrorVBShowName label.
'   - If an error occurs, it returns a string with the error number and description.
'
' Author:         Brian Howard
' Date Created:   2001
' Last Updated:   4 Sep 2023
' =============================================================================
Function VBShowEcc(fluidindex As Long, ErrTolerance As Integer)

    'Local variables
    Dim ecc As Double
    Dim priority01 As Double
    Dim eline01 As String
    Dim i As Long

    'Establish error trapping
    On Error GoTo ErrorVBShowEcc

    'Establish max string length
    ecc = 0
    priority01 = 0
    eline01 = String(256, "a")

    'Check that the Error Tolerance is above minimum
    'Threshold
    If (ErrTolerance < 10) Then
        ErrTolerance = 10
    End If

    'If we need to we can check the return value
    'to see if there was an error
    i = ShowEcc(fluidindex, _
                    ecc, _
                    priority01, _
                    eline01)

    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowEcc = eline01
    Else
        VBShowEcc = ecc
    End If

    'Avoid the error handler
    Exit Function

ErrorVBShowEcc:

    VBShowEcc = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

' =============================================================================
' Function Name:  VBShowR_USCS
' Purpose:        This function interacts with a C++ DLL to retrieve the fluid
'                 gas constant in USCS (psia-ft3)/(lbmol-R), based on the fluid
'                 index and error tolerance parameters.
'                 It serves as a wrapper for the ShowR_USCS DLL function.
'
' Parameters:
'   - fluidindex: The index representing the type of fluid.
'   - ErrTolerance: The error tolerance level.
'
' Returns:
'   - A double value containing either error line information or critical volume
'     based on priority and error tolerance.
'
' Usage:
'   result = VBShowR_USCS(5, 15)
'
' Error Handling:
'   - Implements custom error handling through ErrorVBShowName label.
'   - If an error occurs, it returns a string with the error number and description.
'
' Author:         Brian Howard
' Date Created:   2001
' Last Updated:   4 Sep 2023
' =============================================================================
Function VBShowR_USCS(fluidindex As Long, ErrTolerance As Integer)

    'Local variables
    Dim r As Double
    Dim priority01 As Double
    Dim eline01 As String
    Dim i As Long

    'Establish error trapping
    On Error GoTo ErrorVBShowR_USCS

    'Initialize local variables
    r = 0
    priority01 = 0
    eline01 = String(256, "a")

    'Check that the Error Tolerance is above minimum
    'Threshold
    If (ErrTolerance < 10) Then
        ErrTolerance = 10
    End If

    'If we need to we can check the return value
    'to see if there was an error
    i = ShowR_USCS(fluidindex, _
                    r, _
                    priority01, _
                    eline01)

    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowR_USCS = eline01
    Else
        VBShowR_USCS = r
    End If

    'Avoid the error handler
    Exit Function

ErrorVBShowR_USCS:

    VBShowR_USCS = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

' =============================================================================
' Function Name:  VBShowHfo_SI
' Purpose:        This function interacts with a C++ DLL to retrieve the fluid
'                 entropy of formation in USCS units,J/(gmol-K), based on the fluid
'                 index and error tolerance parameters.
'                 It serves as a wrapper for the ShowHfo_SI DLL function.
'
' Parameters:
'   - fluidindex: The index representing the type of fluid.
'   - ErrTolerance: The error tolerance level.
'
' Returns:
'   - A double value containing either error line information or critical volume
'     based on priority and error tolerance.
'
' Usage:
'   result = VBShowHfo_SI(5, 15)
'
' Error Handling:
'   - Implements custom error handling through ErrorVBShowName label.
'   - If an error occurs, it returns a string with the error number and description.
'
' Author:         Brian Howard
' Date Created:   2001
' Last Updated:   4 Sep 2023
' =============================================================================
Function VBShowHfo_SI(fluidindex As Long, ErrTolerance As Integer)

    'Local variables
    Dim h As Double
    Dim priority01 As Double
    Dim eline01 As String
    Dim i As Long

    'Establish error trapping
    On Error GoTo ErrorVBShowHfo_SI

    'Initialize local variables
    h = 0
    priority01 = 0
    eline01 = String(256, "a")

    'Check that the Error Tolerance is above minimum
    'Threshold
    If (ErrTolerance < 10) Then
        ErrTolerance = 10
    End If

    'If we need to we can check the return value
    'to see if there was an error
    i = ShowHfo_SI(fluidindex, _
                    h, _
                    priority01, _
                    eline01)

    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowHfo_SI = eline01
    Else
        VBShowHfo_SI = h
    End If

    'Avoid the error handler
    Exit Function

ErrorVBShowHfo_SI:

    VBShowHfo_SI = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function
' =============================================================================
' Function Name:  VBShowHfo_USCS
' Purpose:        This function interacts with a C++ DLL to retrieve the fluid
'                 enthalpy of formation in USCS units, BTU/lbmol, based on the fluid
'                 index and error tolerance parameters.
'                 It serves as a wrapper for the ShowHfo_USCS DLL function.
'
' Parameters:
'   - fluidindex: The index representing the type of fluid.
'   - ErrTolerance: The error tolerance level.
'
' Returns:
'   - A double value containing either error line information or critical volume
'     based on priority and error tolerance.
'
' Usage:
'   result = VBShowHfo_USCS(5, 15)
'
' Error Handling:
'   - Implements custom error handling through ErrorVBShowName label.
'   - If an error occurs, it returns a string with the error number and description.
'
' Author:         Brian Howard
' Date Created:   2001
' Last Updated:   4 Sep 2023
' =============================================================================
Function VBShowHfo_USCS(fluidindex As Long, ErrTolerance As Integer)

    'Local variables
    Dim h As Double
    Dim priority01 As Double
    Dim eline01 As String
    Dim i As Long

    'Establish error trapping
    On Error GoTo ErrorVBShowHfo_USCS

    'Initialize local variables
    h = 0
    priority01 = 0
    eline01 = String(256, "a")

    'Check that the Error Tolerance is above minimum
    'Threshold
    If (ErrTolerance < 10) Then
        ErrTolerance = 10
    End If

    'If we need to we can check the return value
    'to see if there was an error
    i = ShowHfo_USCS(fluidindex, _
                    h, _
                    priority01, _
                    eline01)

    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowHfo_USCS = eline01
    Else
        VBShowHfo_USCS = h
    End If

    'Avoid the error handler
    Exit Function

ErrorVBShowHfo_USCS:

    VBShowHfo_USCS = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

' =============================================================================
' Function Name:  VBShowSfo_SI
' Purpose:        This function interacts with a C++ DLL to retrieve the fluid
'                  entropy of formation in SI units, BTU/(lbmol-R), based on the fluid
'                 index and error tolerance parameters.
'                 It serves as a wrapper for the ShowSfo_SI DLL function.
'
' Parameters:
'   - fluidindex: The index representing the type of fluid.
'   - ErrTolerance: The error tolerance level.
'
' Returns:
'   - A double value containing either error line information or critical volume
'     based on priority and error tolerance.
'
' Usage:
'   result = VBShowSfo_SI(5, 15)
'
' Error Handling:
'   - Implements custom error handling through ErrorVBShowName label.
'   - If an error occurs, it returns a string with the error number and description.
'
' Author:         Brian Howard
' Date Created:   2001
' Last Updated:   4 Sep 2023
' =============================================================================
Function VBShowSfo_SI(fluidindex As Long, ErrTolerance As Integer)

    'Local variables
    Dim s As Double
    Dim priority01 As Double
    Dim eline01 As String
    Dim i As Long

    'Establish error trapping
    On Error GoTo ErrorVBShowSfo_SI

    'Initialize local variables
    s = 0
    priority01 = 0
    eline01 = String(256, "a")

    'Check that the Error Tolerance is above minimum
    'Threshold
    If (ErrTolerance < 10) Then
        ErrTolerance = 10
    End If

    'If we need to we can check the return value
    'to see if there was an error
    i = ShowSfo_SI(fluidindex, _
                    s, _
                    priority01, _
                    eline01)

    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowSfo_SI = eline01
    Else
        VBShowSfo_SI = s
    End If

    'Avoid the error handler
    Exit Function

ErrorVBShowSfo_SI:

    VBShowSfo_SI = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

' =============================================================================
' Function Name:  VBShowSfo_USCS
' Purpose:        This function interacts with a C++ DLL to retrieve the fluid
'                 entropy of formation in USCS units,J/(gmol-K), based on the fluid
'                 index and error tolerance parameters.
'                 It serves as a wrapper for the ShowSfo_USCS DLL function.
'
' Parameters:
'   - fluidindex: The index representing the type of fluid.
'   - ErrTolerance: The error tolerance level.
'
' Returns:
'   - A double value containing either error line information or critical volume
'     based on priority and error tolerance.
'
' Usage:
'   result = VBShowSfo_USCS(5, 15)
'
' Error Handling:
'   - Implements custom error handling through ErrorVBShowName label.
'   - If an error occurs, it returns a string with the error number and description.
'
' Author:         Brian Howard
' Date Created:   2001
' Last Updated:   4 Sep 2023
' =============================================================================
Function VBShowSfo_USCS(fluidindex As Long, ErrTolerance As Integer)

    'Local variables
    Dim s As Double
    Dim priority01 As Double
    Dim eline01 As String
    Dim i As Long

    'Establish error trapping
    On Error GoTo ErrorVBShowSfo_USCS

    'Initialize local variables
    s = 0
    priority01 = 0
    eline01 = String(256, "a")

    'Check that the Error Tolerance is above minimum
    'Threshold
    If (ErrTolerance < 10) Then
        ErrTolerance = 10
    End If

    'If we need to we can check the return value
    'to see if there was an error
    i = ShowSfo_USCS(fluidindex, _
                    s, _
                    priority01, _
                    eline01)

    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowSfo_USCS = eline01
    Else
        VBShowSfo_USCS = s
    End If

    'Avoid the error handler
    Exit Function

ErrorVBShowSfo_USCS:

    VBShowSfo_USCS = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

' =============================================================================
' Function Name:  VBShowHHV_SI
' Purpose:        This function interacts with a C++ DLL to retrieve the fluid
'                 higher heating value in SI units, (J/gmol), based on the fluid
'                 index and error tolerance parameters.
'                 It serves as a wrapper for the ShowHHV_SI DLL function.
'
' Parameters:
'   - fluidindex: The index representing the type of fluid.
'   - ErrTolerance: The error tolerance level.
'
' Returns:
'   - A double value containing either error line information or critical volume
'     based on priority and error tolerance.
'
' Usage:
'   result = VBShowHHV_SI(5, 15)
'
' Error Handling:
'   - Implements custom error handling through ErrorVBShowName label.
'   - If an error occurs, it returns a string with the error number and description.
'
' Author:         Brian Howard
' Date Created:   2001
' Last Updated:   4 Sep 2023
' =============================================================================
Function VBShowHHV_SI(fluidindex As Long, ErrTolerance As Integer)

    'Local variables
    Dim hhv As Double
    Dim priority01 As Double
    Dim eline01 As String
    Dim i As Long

    'Establish error trapping
    On Error GoTo ErrorVBShowHHV_SI

    'Initialize local variables
    hhv = 0
    priority01 = 0
    eline01 = String(256, "a")

    'Check that the Error Tolerance is above minimum
    'Threshold
    If (ErrTolerance < 10) Then
        ErrTolerance = 10
    End If

    'If we need to we can check the return value
    'to see if there was an error
    i = ShowHHV_SI(fluidindex, _
                    hhv, _
                    priority01, _
                    eline01)

    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowHHV_SI = eline01
    Else
        VBShowHHV_SI = hhv
    End If

    'Avoid the error handler
    Exit Function

ErrorVBShowHHV_SI:

    VBShowHHV_SI = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

' =============================================================================
' Function Name:  VBShowHHV_USCS
' Purpose:        This function interacts with a C++ DLL to retrieve the fluid
'                 higher heating value in USCS units, (BTU/lbmol), based on the fluid
'                 index and error tolerance parameters.
'                 It serves as a wrapper for the ShowHHV_USCS DLL function.
'
' Parameters:
'   - fluidindex: The index representing the type of fluid.
'   - ErrTolerance: The error tolerance level.
'
' Returns:
'   - A double value containing either error line information or critical volume
'     based on priority and error tolerance.
'
' Usage:
'   result = VBShowHHV_USCS(5, 15)
'
' Error Handling:
'   - Implements custom error handling through ErrorVBShowName label.
'   - If an error occurs, it returns a string with the error number and description.
'
' Author:         Brian Howard
' Date Created:   2001
' Last Updated:   4 Sep 2023
' =============================================================================
Function VBShowHHV_USCS(fluidindex As Long, ErrTolerance As Integer)

    'Local variables
    Dim hhv As Double
    Dim priority01 As Double
    Dim eline01 As String
    Dim i As Long

    'Establish error trapping
    On Error GoTo ErrorVBShowHHV_USCS

    'Initialize local variables
    hhv = 0
    priority01 = 0
    eline01 = String(256, "a")

    'Check that the Error Tolerance is above minimum
    'Threshold
    If (ErrTolerance < 10) Then
        ErrTolerance = 10
    End If

    'If we need to we can check the return value
    'to see if there was an error
    i = ShowHHV_USCS(fluidindex, _
                    hhv, _
                    priority01, _
                    eline01)

    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowHHV_USCS = eline01
    Else
        VBShowHHV_USCS = hhv
    End If

    'Avoid the error handler
    Exit Function

ErrorVBShowHHV_USCS:

    VBShowHHV_USCS = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

' =============================================================================
' Function Name:  VBShowLHV_SI
' Purpose:        This function interacts with a C++ DLL to retrieve the fluid
'                 lower heating value in SI units, (BTU/lbmol), based on the fluid
'                 index and error tolerance parameters.
'                 It serves as a wrapper for the ShowLHV_SI DLL function.
'
' Parameters:
'   - fluidindex: The index representing the type of fluid.
'   - ErrTolerance: The error tolerance level.
'
' Returns:
'   - A double value containing either error line information or critical volume
'     based on priority and error tolerance.
'
' Usage:
'   result = VBShowLHV_SI(5, 15)
'
' Error Handling:
'   - Implements custom error handling through ErrorVBShowName label.
'   - If an error occurs, it returns a string with the error number and description.
'
' Author:         Brian Howard
' Date Created:   2001
' Last Updated:   4 Sep 2023
' =============================================================================
Function VBShowLHV_SI(fluidindex As Long, ErrTolerance As Integer)

    'Local variables
    Dim lhv As Double
    Dim priority01 As Double
    Dim eline01 As String
    Dim i As Long

    'Establish error trapping
    On Error GoTo ErrorVBShowLHV_SI

    'Initialize local variables
    lhv = 0
    priority01 = 0
    eline01 = String(256, "a")

    'Check that the Error Tolerance is above minimum
    'Threshold
    If (ErrTolerance < 10) Then
        ErrTolerance = 10
    End If

    'If we need to we can check the return value
    'to see if there was an error
    i = ShowLHV_SI(fluidindex, _
                    lhv, _
                    priority01, _
                    eline01)

    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowLHV_SI = eline01
    Else
        VBShowLHV_SI = lhv
    End If

    'Avoid the error handler
    Exit Function

ErrorVBShowLHV_SI:

    VBShowLHV_SI = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

' =============================================================================
' Function Name:  VBShowLHV_USCS
' Purpose:        This function interacts with a C++ DLL to retrieve the fluid
'                 lower heating value in USCS units, (BTU/lbmol), based on the fluid
'                 index and error tolerance parameters.
'                 It serves as a wrapper for the ShowLHV_USCS DLL function.
'
' Parameters:
'   - fluidindex: The index representing the type of fluid.
'   - ErrTolerance: The error tolerance level.
'
' Returns:
'   - A double value containing either error line information or critical volume
'     based on priority and error tolerance.
'
' Usage:
'   result = VBShowLHV_USCS(5, 15)
'
' Error Handling:
'   - Implements custom error handling through ErrorVBShowName label.
'   - If an error occurs, it returns a string with the error number and description.
'
' Author:         Brian Howard
' Date Created:   2001
' Last Updated:   4 Sep 2023
' =============================================================================
Function VBShowLHV_USCS(fluidindex As Long, ErrTolerance As Integer)

    'Local variables
    Dim lhv As Double
    Dim priority01 As Double
    Dim eline01 As String
    Dim i As Long

    'Establish error trapping
    On Error GoTo ErrorVBShowLHV_USCS

    'Initialize local variables
    lhv = 0
    priority01 = 0
    eline01 = String(256, "a")

    'Check that the Error Tolerance is above minimum
    'Threshold
    If (ErrTolerance < 10) Then
        ErrTolerance = 10
    End If

    'If we need to we can check the return value
    'to see if there was an error
    i = ShowLHV_USCS(fluidindex, _
                    lhv, _
                    priority01, _
                    eline01)

    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowLHV_USCS = eline01
    Else
        VBShowLHV_USCS = lhv
    End If

    'Avoid the error handler
    Exit Function

ErrorVBShowLHV_USCS:

    VBShowLHV_USCS = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

' =============================================================================
' Function Name:  VBShowDipole_USCS
' Purpose:        This function interacts with a C++ DLL to retrieve the fluid
'                 dipole moment in USCS units, (-), based on the fluid
'                 index and error tolerance parameters.
'                 It serves as a wrapper for the ShowDipole_USCS DLL function.
'
' Parameters:
'   - fluidindex: The index representing the type of fluid.
'   - ErrTolerance: The error tolerance level.
'
' Returns:
'   - A double value containing either error line information or critical volume
'     based on priority and error tolerance.
'
' Usage:
'   result = VBShowDipole_USCS(5, 15)
'
' Error Handling:
'   - Implements custom error handling through ErrorVBShowName label.
'   - If an error occurs, it returns a string with the error number and description.
'
' Author:         Brian Howard
' Date Created:   2001
' Last Updated:   4 Sep 2023
' =============================================================================
Function VBShowDipole_USCS(fluidindex As Long, ErrTolerance As Integer)

    'Local variables
    Dim dipole As Double
    Dim priority01 As Double
    Dim eline01 As String
    Dim i As Long

    'Establish error trapping
    On Error GoTo ErrorVBShowDipole_USCS

    'Initialize local variables
    dipole = 0
    priority01 = 0
    eline01 = String(256, "a")

    'Check that the Error Tolerance is above minimum
    'Threshold
    If (ErrTolerance < 10) Then
        ErrTolerance = 10
    End If

    'If we need to we can check the return value
    'to see if there was an error
    i = ShowDipole_USCS(fluidindex, _
                    dipole, _
                    priority01, _
                    eline01)

    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowDipole_USCS = eline01
    Else
        VBShowDipole_USCS = dipole
    End If

    'Avoid the error handler
    Exit Function

ErrorVBShowDipole_USCS:

    VBShowDipole_USCS = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

' =============================================================================
' Function Name:  VBShowDipole_SI
' Purpose:        This function interacts with a C++ DLL to retrieve the fluid
'                 dipole moment in SI units, (debye), based on the fluid
'                 index and error tolerance parameters.
'                 It serves as a wrapper for the ShowDipole_SI DLL function.
'
' Parameters:
'   - fluidindex: The index representing the type of fluid.
'   - ErrTolerance: The error tolerance level.
'
' Returns:
'   - A double value containing either error line information or critical volume
'     based on priority and error tolerance.
'
' Usage:
'   result = VBShowDipole_SI(5, 15)
'
' Error Handling:
'   - Implements custom error handling through ErrorVBShowName label.
'   - If an error occurs, it returns a string with the error number and description.
'
' Author:         Brian Howard
' Date Created:   2001
' Last Updated:   4 Sep 2023
' =============================================================================
Function VBShowDipole_SI(fluidindex As Long, ErrTolerance As Integer)

    'Local variables
    Dim dipole As Double
    Dim priority01 As Double
    Dim eline01 As String
    Dim i As Long

    'Establish error trapping
    On Error GoTo ErrorVBShowDipole_SI

    'Initialize local variables
    dipole = 0
    priority01 = 0
    eline01 = String(256, "a")

    'Check that the Error Tolerance is above minimum
    'Threshold
    If (ErrTolerance < 10) Then
        ErrTolerance = 10
    End If

    'If we need to we can check the return value
    'to see if there was an error
    i = ShowDipole_SI(fluidindex, _
                    dipole, _
                    priority01, _
                    eline01)

    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowDipole_SI = eline01
    Else
        VBShowDipole_SI = dipole
    End If

    'Avoid the error handler
    Exit Function

ErrorVBShowDipole_SI:

    VBShowDipole_SI = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function





