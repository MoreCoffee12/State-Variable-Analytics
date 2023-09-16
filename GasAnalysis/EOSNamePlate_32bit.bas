Attribute VB_Name = "EOSNamePlate_32bit"
'Attribute VB_Name = "EOSNamePlate"
'Option Explicit
'
''regretably, some global variables are used
''these ones help with the error tracking
'Public errcount As Integer
'Public priority01 As Double
'Public eline01 As String * 255
'Public i As Integer
'
''This function returns the number of fluids in
''the EOS engine
'Declare Function ShowFluidCount _
'    Lib "GasAnalysis.dll" () As Integer
'
''This function returns the primary name of a fluid
'Declare Function ShowName_Alt01 _
'    Lib "GasAnalysis.dll" (ByVal fluidindex As Double, _
'                            ByVal textline As String, _
'                            ByRef priority01 As Double, _
'                            ByVal textline01 As String) As Integer
'
''This function returns the auxillary or secondary
''name of a fluid
'Declare Function ShowName _
'    Lib "GasAnalysis.dll" (ByVal fluidindex As Double, _
'                            ByVal textline As String, _
'                            ByRef priority01 As Double, _
'                            ByVal textline01 As String) As Integer
'
''This function returns chemical formula
''for a fluid
'Declare Function ShowFormula _
'    Lib "GasAnalysis.dll" (ByVal fluidindex As Double, _
'                            ByVal textline As String, _
'                            ByRef priority01 As Double, _
'                            ByVal textline01 As String) As Integer
'
''This function returns molecular weight
''for a fluid
'Declare Function ShowMolWeight _
'    Lib "GasAnalysis.dll" (ByVal fluidindex As Double, _
'                            ByRef molweight As Double, _
'                            ByRef priority01 As Double, _
'                            ByVal textline01 As String) As Integer
'
''This function returns critcal temperature (USCS)
''for a fluid
'Declare Function ShowCritTemp_USCS _
'    Lib "GasAnalysis.dll" (ByVal fluidindex As Double, _
'                            ByRef crittemp As Double, _
'                            ByRef priority01 As Double, _
'                            ByVal textline01 As String) As Integer
'
''This function returns critcal temperature (SI)
''for a fluid
'Declare Function ShowCritTemp_SI _
'    Lib "GasAnalysis.dll" (ByVal fluidindex As Double, _
'                            ByRef crittemp As Double, _
'                            ByRef priority01 As Double, _
'                            ByVal textline01 As String) As Integer
'
''This function returns critcal pressure (USCS)
''for a fluid
'Declare Function ShowCritPres_USCS _
'    Lib "GasAnalysis.dll" (ByVal fluidindex As Double, _
'                            ByRef critpres As Double, _
'                            ByRef priority01 As Double, _
'                            ByVal textline01 As String) As Integer
'
''This function returns critcal pressure (SI)
''for a fluid
'Declare Function ShowCritPres_SI _
'    Lib "GasAnalysis.dll" (ByVal fluidindex As Double, _
'                            ByRef crittemp As Double, _
'                            ByRef priority01 As Double, _
'                            ByVal textline01 As String) As Integer
'
''This function returns critcal volume (USCS)
''for a fluid
'Declare Function ShowCritVol_USCS _
'    Lib "GasAnalysis.dll" (ByVal fluidindex As Double, _
'                            ByRef critpres As Double, _
'                            ByRef priority01 As Double, _
'                            ByVal textline01 As String) As Integer
'
''This function returns critcal volume (SI)
''for a fluid
'Declare Function ShowCritVol_SI _
'    Lib "GasAnalysis.dll" (ByVal fluidindex As Double, _
'                            ByRef crittemp As Double, _
'                            ByRef priority01 As Double, _
'                            ByVal textline01 As String) As Integer
'
''This function returns the eccentricity factor
''for a fluid
'Declare Function ShowEcc _
'    Lib "GasAnalysis.dll" (ByVal fluidindex As Double, _
'                            ByRef ecc As Double, _
'                            ByRef priority01 As Double, _
'                            ByVal textline01 As String) As Integer
'
''This function returns the gas constant
''for a fluid
'Declare Function ShowR_USCS _
'    Lib "GasAnalysis.dll" (ByVal fluidindex As Double, _
'                            ByRef r As Double, _
'                            ByRef priority01 As Double, _
'                            ByVal textline01 As String) As Integer
'
''This function returns the enthalpy of formation in USCS
''for a fluid
'Declare Function ShowHfo_USCS _
'    Lib "GasAnalysis.dll" (ByVal fluidindex As Double, _
'                            ByRef h As Double, _
'                            ByRef priority01 As Double, _
'                            ByVal textline01 As String) As Integer
'
''This function returns the enthalpy of formation in SI
''for a fluid
'Declare Function ShowHfo_SI _
'    Lib "GasAnalysis.dll" (ByVal fluidindex As Double, _
'                            ByRef h As Double, _
'                            ByRef priority01 As Double, _
'                            ByVal textline01 As String) As Integer
'
''This function returns the entropy of formation in USCS
''for a fluid
'Declare Function ShowSfo_USCS _
'    Lib "GasAnalysis.dll" (ByVal fluidindex As Double, _
'                            ByRef h As Double, _
'                            ByRef priority01 As Double, _
'                            ByVal textline01 As String) As Integer
'
''This function returns the entropy of formation in SI
''for a fluid
'Declare Function ShowSfo_SI _
'    Lib "GasAnalysis.dll" (ByVal fluidindex As Double, _
'                            ByRef h As Double, _
'                            ByRef priority01 As Double, _
'                            ByVal textline01 As String) As Integer
'
''This function returns the lower heating value in USCS
''for a fluid
'Declare Function ShowLHV_USCS _
'    Lib "GasAnalysis.dll" (ByVal fluidindex As Double, _
'                            ByRef lhv As Double, _
'                            ByRef priority01 As Double, _
'                            ByVal textline01 As String) As Integer
'
''This function returns the lower heating value in SI
''for a fluid
'Declare Function ShowLHV_SI _
'    Lib "GasAnalysis.dll" (ByVal fluidindex As Double, _
'                            ByRef lhv As Double, _
'                            ByRef priority01 As Double, _
'                            ByVal textline01 As String) As Integer
'
''This function returns the higher heating value in USCS
''for a fluid
'Declare Function ShowHHV_USCS _
'    Lib "GasAnalysis.dll" (ByVal fluidindex As Double, _
'                            ByRef hhv As Double, _
'                            ByRef priority01 As Double, _
'                            ByVal textline01 As String) As Integer
'
''This function returns the higher heating value in SI
''for a fluid
'Declare Function ShowHHV_SI _
'    Lib "GasAnalysis.dll" (ByVal fluidindex As Double, _
'                            ByRef hhv As Double, _
'                            ByRef priority01 As Double, _
'                            ByVal textline01 As String) As Integer
'
''This function returns the dipole moment in USCS
''for a fluid
'Declare Function ShowDipole_USCS _
'    Lib "GasAnalysis.dll" (ByVal fluidindex As Long, _
'                            ByRef dipole As Double, _
'                            ByRef priority01 As Double, _
'                            ByVal textline01 As String) As Integer
'
''This function returns the dipole moment in USCS
''for a fluid
'Declare Function ShowDipole_SI _
'    Lib "GasAnalysis.dll" (ByVal fluidindex As Long, _
'                            ByRef dipole As Double, _
'                            ByRef priority01 As Double, _
'                            ByVal textline01 As String) As Integer
'
'
'Public Sub UpdateErrorControls()
'
'    If (errcount < 100) Then
'
'        'If there is one message
'        If (i > 0) Then
'            'Place the values on the sheet for Gas Analysis
'            errcount = errcount + i
'        End If
'
'    End If
'
'End Sub
'Function VBShowFluidCount()
'
'    VBShowFluidCount = ShowFluidCount()
'
'End Function
'Function VBShowName(fluidindex As Long, ErrTolerance As Integer)
'
'    'Local variables
'    Dim tline As String
'
'    'Establish error trapping
'    On Error GoTo ErrorVBShowName
'
'    'Establish max string length
'    tline = String(255, vbNullChar)
'    priority01 = 0
'
'    'Check that the Error Tolerance is above minimum
'    'Threshold
'    If (ErrTolerance < 10) Then
'        ErrTolerance = 10
'    End If
'
'    'If we need to we can check the return value
'    'to see if there was an error
'    i = ShowName(fluidindex, _
'                    tline, _
'                    priority01, _
'                    eline01)
'
'    'return the value
'    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
'        VBShowName = eline01
'    Else
'        VBShowName = tline
'    End If
'
'    'Avoid the error handler
'    Exit Function
'
'ErrorVBShowName:
'   'Twiddle the Flag and display the message
'    VBShowName = (Str(Err.Number) & ":" & Err.Description)
'    Exit Function
'
'End Function
'
'Function VBShowName_Alt01(fluidindex As Long, ErrTolerance As Integer)
'
'    'Local variables
'    Dim tline As String
'
'    'Establish error trapping
'    On Error GoTo ErrorVBShowName_Alt01
'
'    'Establish max string length
'    tline = String(255, vbNullChar)
'    priority01 = 0
'
'    'Check that the Error Tolerance is above minimum
'    'Threshold
'    If (ErrTolerance < 10) Then
'        ErrTolerance = 10
'    End If
'
'    'If we need to we can check the return value
'    'to see if there was an error
'    i = ShowName_Alt01(fluidindex, _
'                    tline, _
'                    priority01, _
'                    eline01)
'
'    'return the value
'    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
'        VBShowName_Alt01 = eline01
'    Else
'        VBShowName_Alt01 = tline
'    End If
'
'    'Avoid the error handler
'    Exit Function
'
'ErrorVBShowName_Alt01:
'    VBShowName_Alt01 = (Str(Err.Number) & ":" & Err.Description)
'    Exit Function
'
'End Function
'
'Function VBShowFormula(fluidindex As Long, ErrTolerance As Integer)
'
'    'Local variables
'    Dim tline As String
'
'    'Establish error trapping
'    On Error GoTo ErrorVBShowFormula
'
'    'Establish max string length
'    tline = String(255, vbNullChar)
'    priority01 = 0
'
'    'Check that the Error Tolerance is above minimum
'    'Threshold
'    If (ErrTolerance < 10) Then
'        ErrTolerance = 10
'    End If
'
'    'If we need to we can check the return value
'    'to see if there was an error
'    i = ShowFormula(fluidindex, _
'                    tline, _
'                    priority01, _
'                    eline01)
'
'    'return the value
'    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
'        VBShowFormula = eline01
'    Else
'        VBShowFormula = tline
'    End If
'
'    'Avoid the error handler
'    Exit Function
'
'ErrorVBShowFormula:
'    VBShowFormula = (Str(Err.Number) & ":" & Err.Description)
'    Exit Function
'
'End Function
'
'Function VBShowMolWeight(fluidindex As Long, ErrTolerance As Integer)
'
'    'Local variables
'    Dim molweight As Double
'
'    'Establish error trapping
'    On Error GoTo ErrorVBShowMolWeight
'
'    'Establish max string length
'    molweight = 0
'    priority01 = 0
'
'    'Check that the Error Tolerance is above minimum
'    'Threshold
'    If (ErrTolerance < 10) Then
'        ErrTolerance = 10
'    End If
'
'
'    'If we need to we can check the return value
'    'to see if there was an error
'    i = ShowMolWeight(fluidindex, _
'                    molweight, _
'                    priority01, _
'                    eline01)
'
'    'return the value
'    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
'        VBShowMolWeight = eline01
'    Else
'        VBShowMolWeight = molweight
'    End If
'
'    'Avoid the error handler
'    Exit Function
'
'ErrorVBShowMolWeight:
'    VBShowMolWeight = (Str(Err.Number) & ":" & Err.Description)
'    Exit Function
'
'End Function
'
'Function VBShowCritTemp_USCS(fluidindex As Long, ErrTolerance As Integer)
'
'    'Local variables
'    Dim crittemp As Double
'
'    'Establish error trapping
'    On Error GoTo ErrorVBShowCritTemp_USCS
'
'    'Establish max string length
'    crittemp = 0
'    priority01 = 0
'
'    'Check that the Error Tolerance is above minimum
'    'Threshold
'    If (ErrTolerance < 10) Then
'        ErrTolerance = 10
'    End If
'
'    'If we need to we can check the return value
'    'to see if there was an error
'    i = ShowCritTemp_USCS(fluidindex, _
'                    crittemp, _
'                    priority01, _
'                    eline01)
'
'    'return the value
'    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
'        VBShowCritTemp_USCS = eline01
'    Else
'        VBShowCritTemp_USCS = crittemp
'    End If
'
'    'Avoid the error handler
'    Exit Function
'
'ErrorVBShowCritTemp_USCS:
'
'    VBShowCritTemp_USCS = (Str(Err.Number) & ":" & Err.Description)
'    Exit Function
'
'End Function
'
'Function VBShowCritTemp_SI(fluidindex As Long, ErrTolerance As Integer)
'
'    'Local variables
'    Dim crittemp As Double
'
'    'Establish error trapping
'    On Error GoTo ErrorVBShowCritTemp_SI
'
'    'Establish max string length
'    crittemp = 0
'    priority01 = 0
'
'    'Check that the Error Tolerance is above minimum
'    'Threshold
'    If (ErrTolerance < 10) Then
'        ErrTolerance = 10
'    End If
'
'    'If we need to we can check the return value
'    'to see if there was an error
'    i = ShowCritTemp_SI(fluidindex, _
'                    crittemp, _
'                    priority01, _
'                    eline01)
'
'    'return the value
'    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
'        VBShowCritTemp_SI = eline01
'    Else
'        VBShowCritTemp_SI = crittemp
'    End If
'
'    'Avoid the error handler
'    Exit Function
'
'ErrorVBShowCritTemp_SI:
'
'    VBShowCritTemp_SI = (Str(Err.Number) & ":" & Err.Description)
'    Exit Function
'
'End Function
'
'Function VBShowCritPres_USCS(fluidindex As Long, ErrTolerance As Integer)
'
'    'Local variables
'    Dim critpres As Double
'
'    'Establish error trapping
'    On Error GoTo ErrorVBShowCritPres_USCS
'
'    'Establish max string length
'    critpres = 0
'    priority01 = 0
'
'    'Check that the Error Tolerance is above minimum
'    'Threshold
'    If (ErrTolerance < 10) Then
'        ErrTolerance = 10
'    End If
'
'   'If we need to we can check the return value
'    'to see if there was an error
'    i = ShowCritPres_USCS(fluidindex, _
'                    critpres, _
'                    priority01, _
'                    eline01)
'
'    'return the value
'    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
'        VBShowCritPres_USCS = eline01
'    Else
'        VBShowCritPres_USCS = critpres
'    End If
'
'    'Avoid the error handler
'    Exit Function
'
'ErrorVBShowCritPres_USCS:
'
'    VBShowCritPres_USCS = (Str(Err.Number) & ":" & Err.Description)
'    Exit Function
'
'End Function
'
'Function VBShowCritPres_SI(fluidindex As Long, ErrTolerance As Integer)
'
'    'Local variables
'    Dim critpres As Double
'
'    'Establish error trapping
'    On Error GoTo ErrorVBShowCritPres_SI
'
'    'Establish max string length
'    critpres = 0
'    priority01 = 0
'
'    'Check that the Error Tolerance is above minimum
'    'Threshold
'    If (ErrTolerance < 10) Then
'        ErrTolerance = 10
'    End If
'
'    'If we need to we can check the return value
'    'to see if there was an error
'    i = ShowCritPres_SI(fluidindex, _
'                    critpres, _
'                    priority01, _
'                    eline01)
'
'    'return the value
'    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
'        VBShowCritPres_SI = eline01
'    Else
'        VBShowCritPres_SI = critpres
'    End If
'
'    'Avoid the error handler
'    Exit Function
'
'ErrorVBShowCritPres_SI:
'
'    VBShowCritPres_SI = (Str(Err.Number) & ":" & Err.Description)
'    Exit Function
'
'End Function
'
'Function VBShowCritVol_USCS(fluidindex As Long, ErrTolerance As Integer)
'
'    'Local variables
'    Dim critvol As Double
'
'    'Establish error trapping
'    On Error GoTo ErrorVBShowCritVol_USCS
'
'    'Establish max string length
'    critvol = 0
'    priority01 = 0
'
'    'Check that the Error Tolerance is above minimum
'    'Threshold
'    If (ErrTolerance < 10) Then
'        ErrTolerance = 10
'    End If
'
'    'If we need to we can check the return value
'    'to see if there was an error
'    i = ShowCritVol_USCS(fluidindex, _
'                    critvol, _
'                    priority01, _
'                    eline01)
'
'    'return the value
'    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
'        VBShowCritVol_USCS = eline01
'    Else
'        VBShowCritVol_USCS = critvol
'    End If
'
'    'Avoid the error handler
'    Exit Function
'
'ErrorVBShowCritVol_USCS:
'
'    VBShowCritVol_USCS = (Str(Err.Number) & ":" & Err.Description)
'    Exit Function
'
'End Function
'
'Function VBShowCritVol_SI(fluidindex As Long, ErrTolerance As Integer)
'
'    'Local variables
'    Dim critvol As Double
'
'    'Establish error trapping
'    On Error GoTo ErrorVBShowCritVol_SI
'
'    'Establish max string length
'    critvol = 0
'    priority01 = 0
'
'    'Check that the Error Tolerance is above minimum
'    'Threshold
'    If (ErrTolerance < 10) Then
'        ErrTolerance = 10
'    End If
'
'    'If we need to we can check the return value
'    'to see if there was an error
'    i = ShowCritVol_SI(fluidindex, _
'                    critvol, _
'                    priority01, _
'                    eline01)
'
'    'return the value
'    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
'        VBShowCritVol_SI = eline01
'    Else
'        VBShowCritVol_SI = critvol
'    End If
'
'    'Avoid the error handler
'    Exit Function
'
'ErrorVBShowCritVol_SI:
'
'    VBShowCritVol_SI = (Str(Err.Number) & ":" & Err.Description)
'    Exit Function
'
'End Function
'
'Function VBShowEcc(fluidindex As Long, ErrTolerance As Integer)
'
'    'Local variables
'    Dim ecc As Double
'
'    'Establish error trapping
'    On Error GoTo ErrorVBShowEcc
'
'    'Establish max string length
'    ecc = 0
'    priority01 = 0
'
'    'Check that the Error Tolerance is above minimum
'    'Threshold
'    If (ErrTolerance < 10) Then
'        ErrTolerance = 10
'    End If
'
'    'If we need to we can check the return value
'    'to see if there was an error
'    i = ShowEcc(fluidindex, _
'                    ecc, _
'                    priority01, _
'                    eline01)
'
'    'return the value
'    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
'        VBShowEcc = eline01
'    Else
'        VBShowEcc = ecc
'    End If
'
'    'Avoid the error handler
'    Exit Function
'
'ErrorVBShowEcc:
'
'    VBShowEcc = (Str(Err.Number) & ":" & Err.Description)
'    Exit Function
'
'End Function
'
'Function VBShowR_USCS(fluidindex As Long, ErrTolerance As Integer)
'
'    'Local variables
'    Dim r As Double
'
'    'Establish error trapping
'    On Error GoTo ErrorVBShowR_USCS
'
'    'Initialize local variables
'    r = 0
'    priority01 = 0
'
'    'Check that the Error Tolerance is above minimum
'    'Threshold
'    If (ErrTolerance < 10) Then
'        ErrTolerance = 10
'    End If
'
'    'If we need to we can check the return value
'    'to see if there was an error
'    i = ShowR_USCS(fluidindex, _
'                    r, _
'                    priority01, _
'                    eline01)
'
'    'return the value
'    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
'        VBShowR_USCS = eline01
'    Else
'        VBShowR_USCS = r
'    End If
'
'    'Avoid the error handler
'    Exit Function
'
'ErrorVBShowR_USCS:
'
'    VBShowR_USCS = (Str(Err.Number) & ":" & Err.Description)
'    Exit Function
'
'End Function
'
'Function VBShowHfo_SI(fluidindex As Long, ErrTolerance As Integer)
'
'    'Local variables
'    Dim h As Double
'
'    'Establish error trapping
'    On Error GoTo ErrorVBShowHfo_SI
'
'    'Initialize local variables
'    h = 0
'    priority01 = 0
'
'    'Check that the Error Tolerance is above minimum
'    'Threshold
'    If (ErrTolerance < 10) Then
'        ErrTolerance = 10
'    End If
'
'    'If we need to we can check the return value
'    'to see if there was an error
'    i = ShowHfo_SI(fluidindex, _
'                    h, _
'                    priority01, _
'                    eline01)
'
'    'return the value
'    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
'        VBShowHfo_SI = eline01
'    Else
'        VBShowHfo_SI = h
'    End If
'
'    'Avoid the error handler
'    Exit Function
'
'ErrorVBShowHfo_SI:
'
'    VBShowHfo_SI = (Str(Err.Number) & ":" & Err.Description)
'    Exit Function
'
'End Function
'
'Function VBShowHfo_USCS(fluidindex As Long, ErrTolerance As Integer)
'
'    'Local variables
'    Dim h As Double
'
'    'Establish error trapping
'    On Error GoTo ErrorVBShowHfo_USCS
'
'    'Initialize local variables
'    h = 0
'    priority01 = 0
'
'    'Check that the Error Tolerance is above minimum
'    'Threshold
'    If (ErrTolerance < 10) Then
'        ErrTolerance = 10
'    End If
'
'    'If we need to we can check the return value
'    'to see if there was an error
'    i = ShowHfo_USCS(fluidindex, _
'                    h, _
'                    priority01, _
'                    eline01)
'
'    'return the value
'    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
'        VBShowHfo_USCS = eline01
'    Else
'        VBShowHfo_USCS = h
'    End If
'
'    'Avoid the error handler
'    Exit Function
'
'ErrorVBShowHfo_USCS:
'
'    VBShowHfo_USCS = (Str(Err.Number) & ":" & Err.Description)
'    Exit Function
'
'End Function
'
'Function VBShowSfo_SI(fluidindex As Long, ErrTolerance As Integer)
'
'    'Local variables
'    Dim s As Double
'
'    'Establish error trapping
'    On Error GoTo ErrorVBShowSfo_SI
'
'    'Initialize local variables
'    s = 0
'    priority01 = 0
'
'    'Check that the Error Tolerance is above minimum
'    'Threshold
'    If (ErrTolerance < 10) Then
'        ErrTolerance = 10
'    End If
'
'    'If we need to we can check the return value
'    'to see if there was an error
'    i = ShowSfo_SI(fluidindex, _
'                    s, _
'                    priority01, _
'                    eline01)
'
'    'return the value
'    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
'        VBShowSfo_SI = eline01
'    Else
'        VBShowSfo_SI = s
'    End If
'
'    'Avoid the error handler
'    Exit Function
'
'ErrorVBShowSfo_SI:
'
'    VBShowSfo_SI = (Str(Err.Number) & ":" & Err.Description)
'    Exit Function
'
'End Function
'
'Function VBShowSfo_USCS(fluidindex As Long, ErrTolerance As Integer)
'
'    'Local variables
'    Dim s As Double
'
'    'Establish error trapping
'    On Error GoTo ErrorVBShowSfo_USCS
'
'    'Initialize local variables
'    s = 0
'    priority01 = 0
'
'    'Check that the Error Tolerance is above minimum
'    'Threshold
'    If (ErrTolerance < 10) Then
'        ErrTolerance = 10
'    End If
'
'    'If we need to we can check the return value
'    'to see if there was an error
'    i = ShowSfo_USCS(fluidindex, _
'                    s, _
'                    priority01, _
'                    eline01)
'
'    'return the value
'    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
'        VBShowSfo_USCS = eline01
'    Else
'        VBShowSfo_USCS = s
'    End If
'
'    'Avoid the error handler
'    Exit Function
'
'ErrorVBShowSfo_USCS:
'
'    VBShowSfo_USCS = (Str(Err.Number) & ":" & Err.Description)
'    Exit Function
'
'End Function
'
'Function VBShowHHV_SI(fluidindex As Long, ErrTolerance As Integer)
'
'    'Local variables
'    Dim hhv As Double
'
'    'Establish error trapping
'    On Error GoTo ErrorVBShowHHV_SI
'
'    'Initialize local variables
'    hhv = 0
'    priority01 = 0
'
'    'Check that the Error Tolerance is above minimum
'    'Threshold
'    If (ErrTolerance < 10) Then
'        ErrTolerance = 10
'    End If
'
'    'If we need to we can check the return value
'    'to see if there was an error
'    i = ShowHHV_SI(fluidindex, _
'                    hhv, _
'                    priority01, _
'                    eline01)
'
'    'return the value
'    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
'        VBShowHHV_SI = eline01
'    Else
'        VBShowHHV_SI = hhv
'    End If
'
'    'Avoid the error handler
'    Exit Function
'
'ErrorVBShowHHV_SI:
'
'    VBShowHHV_SI = (Str(Err.Number) & ":" & Err.Description)
'    Exit Function
'
'End Function
'
'Function VBShowHHV_USCS(fluidindex As Long, ErrTolerance As Integer)
'
'    'Local variables
'    Dim hhv As Double
'
'    'Establish error trapping
'    On Error GoTo ErrorVBShowHHV_USCS
'
'    'Initialize local variables
'    hhv = 0
'    priority01 = 0
'
'    'Check that the Error Tolerance is above minimum
'    'Threshold
'    If (ErrTolerance < 10) Then
'        ErrTolerance = 10
'    End If
'
'    'If we need to we can check the return value
'    'to see if there was an error
'    i = ShowHHV_USCS(fluidindex, _
'                    hhv, _
'                    priority01, _
'                    eline01)
'
'    'return the value
'    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
'        VBShowHHV_USCS = eline01
'    Else
'        VBShowHHV_USCS = hhv
'    End If
'
'    'Avoid the error handler
'    Exit Function
'
'ErrorVBShowHHV_USCS:
'
'    VBShowHHV_USCS = (Str(Err.Number) & ":" & Err.Description)
'    Exit Function
'
'End Function
'
'
'Function VBShowLHV_SI(fluidindex As Long, ErrTolerance As Integer)
'
'    'Local variables
'    Dim lhv As Double
'
'    'Establish error trapping
'    On Error GoTo ErrorVBShowLHV_SI
'
'    'Initialize local variables
'    lhv = 0
'    priority01 = 0
'
'    'Check that the Error Tolerance is above minimum
'    'Threshold
'    If (ErrTolerance < 10) Then
'        ErrTolerance = 10
'    End If
'
'    'If we need to we can check the return value
'    'to see if there was an error
'    i = ShowLHV_SI(fluidindex, _
'                    lhv, _
'                    priority01, _
'                    eline01)
'
'    'return the value
'    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
'        VBShowLHV_SI = eline01
'    Else
'        VBShowLHV_SI = lhv
'    End If
'
'    'Avoid the error handler
'    Exit Function
'
'ErrorVBShowLHV_SI:
'
'    VBShowLHV_SI = (Str(Err.Number) & ":" & Err.Description)
'    Exit Function
'
'End Function
'
'Function VBShowLHV_USCS(fluidindex As Long, ErrTolerance As Integer)
'
'    'Local variables
'    Dim lhv As Double
'
'    'Establish error trapping
'    On Error GoTo ErrorVBShowLHV_USCS
'
'    'Initialize local variables
'    lhv = 0
'    priority01 = 0
'
'    'Check that the Error Tolerance is above minimum
'    'Threshold
'    If (ErrTolerance < 10) Then
'        ErrTolerance = 10
'    End If
'
'    'If we need to we can check the return value
'    'to see if there was an error
'    i = ShowLHV_USCS(fluidindex, _
'                    lhv, _
'                    priority01, _
'                    eline01)
'
'    'return the value
'    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
'        VBShowLHV_USCS = eline01
'    Else
'        VBShowLHV_USCS = lhv
'    End If
'
'    'Avoid the error handler
'    Exit Function
'
'ErrorVBShowLHV_USCS:
'
'    VBShowLHV_USCS = (Str(Err.Number) & ":" & Err.Description)
'    Exit Function
'
'End Function
'
'Function VBShowDipole_USCS(fluidindex As Long, ErrTolerance As Integer)
'
'    'Local variables
'    Dim dipole As Double
'
'    'Establish error trapping
'    On Error GoTo ErrorVBShowDipole_USCS
'
'    'Initialize local variables
'    dipole = 0
'    priority01 = 0
'
'    'Check that the Error Tolerance is above minimum
'    'Threshold
'    If (ErrTolerance < 10) Then
'        ErrTolerance = 10
'    End If
'
'    'If we need to we can check the return value
'    'to see if there was an error
'    i = ShowDipole_USCS(fluidindex, _
'                    dipole, _
'                    priority01, _
'                    eline01)
'
'    'return the value
'    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
'        VBShowDipole_USCS = eline01
'    Else
'        VBShowDipole_USCS = dipole
'    End If
'
'    'Avoid the error handler
'    Exit Function
'
'ErrorVBShowDipole_USCS:
'
'    VBShowDipole_USCS = (Str(Err.Number) & ":" & Err.Description)
'    Exit Function
'
'End Function
'
'Function VBShowDipole_SI(fluidindex As Long, ErrTolerance As Integer)
'
'    'Local variables
'    Dim dipole As Double
'
'    'Establish error trapping
'    On Error GoTo ErrorVBShowDipole_SI
'
'    'Initialize local variables
'    dipole = 0
'    priority01 = 0
'
'    'Check that the Error Tolerance is above minimum
'    'Threshold
'    If (ErrTolerance < 10) Then
'        ErrTolerance = 10
'    End If
'
'    'If we need to we can check the return value
'    'to see if there was an error
'    i = ShowDipole_SI(fluidindex, _
'                    dipole, _
'                    priority01, _
'                    eline01)
'
'    'return the value
'    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
'        VBShowDipole_SI = eline01
'    Else
'        VBShowDipole_SI = dipole
'    End If
'
'    'Avoid the error handler
'    Exit Function
'
'ErrorVBShowDipole_SI:
'
'    VBShowDipole_SI = (Str(Err.Number) & ":" & Err.Description)
'    Exit Function
'
'End Function
'
'
'
'
'
'
