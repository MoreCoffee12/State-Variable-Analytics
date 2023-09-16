Attribute VB_Name = "ModulePerfAnalysis_32bit"
''Don't be lazy
'Option Explicit
'
'Declare PtrSafe Function SweptVolume_SI _
'    Lib "PerfAnalysis.dll" _
'    (ByVal stroke As Double, _
'        ByVal bore As Double, _
'        ByVal rod As Double) As Double
'
'Declare PtrSafe Function SweptVolume_USCS _
'    Lib "PerfAnalysis.dll" _
'    (ByVal stroke As Double, _
'        ByVal bore As Double, _
'        ByVal rod As Double) As Double
'
'Declare PtrSafe Function CCOMPEXP _
'    Lib "PerfAnalysis.dll" _
'        (ByVal connrodlength As Double, _
'         ByVal stroke As Double, _
'         ByVal bore As Double, _
'         ByVal rod As Double, _
'         ByVal crankangle As Double, _
'         ByVal clearance As Double, _
'         ByVal nexp As Double, _
'         ByVal ncomp As Double, _
'         ByVal psuct As Double, _
'         ByVal pdish As Double, _
'         ByVal headend As Boolean) As Double
'
'Declare PtrSafe Function TheoreticalVE _
'    Lib "PerfAnalysis.dll" _
'        (ByVal connrodlength As Double, _
'         ByVal stroke As Double, _
'         ByVal bore As Double, _
'         ByVal rod As Double, _
'         ByVal clearance As Double, _
'         ByVal nexp As Double, _
'         ByVal ncomp As Double, _
'         ByVal psuct As Double, _
'         ByVal pdish As Double) As Double
'
'Dim MsgString As String
'Dim ErrorLocation As String
'
'Public Function CompExp(connrodlength As Double, _
'        stroke As Double, _
'        bore As Double, _
'        rod As Double, _
'        crankangle As Double, _
'        clearance As Double, _
'        nexp As Double, _
'        ncomp As Double, _
'        psuct As Double, _
'        pdish As Double, _
'        headend As Boolean)
'
'    On Error GoTo ErrorCompExp
'
'
'    'Call the .dll routine for compression/expansion
'    CompExp = CCOMPEXP(connrodlength, _
'                            stroke, _
'                            bore, _
'                            rod, _
'                            crankangle, _
'                            clearance, _
'                            nexp, _
'                            ncomp, _
'                            psuct, _
'                            pdish, _
'                            headend)
'
'    'Avoid Error handler
'    Exit Function
'
'ErrorCompExp:
'    ErrorLocation = "Error Calculating CompExp"
'    MsgString = ErrorLocation & "->" & Str$(Err.Number) & ": " & Err.Description
'    MsgBox MsgString
'    Exit Function
'
'End Function
'Public Function VBShowSweptVolume_SI(stroke, bore, rod) As Double
'
'    On Error GoTo ErrorVBShowSweptVolume_SI
'
'
'    'Call the .dll routine for swept volume calculation
'    VBShowSweptVolume_SI = SweptVolume_SI(stroke, _
'                            bore, _
'                            rod)
'
'    'Avoid Error handler
'    Exit Function
'
'ErrorVBShowSweptVolume_SI:
'    ErrorLocation = "Error Calculating SweptVolume_SI"
'    MsgString = ErrorLocation & "->" & Str$(Err.Number) & ": " & Err.Description
'    MsgBox MsgString
'    Exit Function
'
'End Function
'Public Function VBShowSweptVolume_USCS(stroke, bore, rod) As Double
'
'    On Error GoTo ErrorVBShowSweptVolume_USCS
'
'
'    'Call the .dll routine for swept volume calculation
'    VBShowSweptVolume_USCS = SweptVolume_USCS(stroke, _
'                            bore, _
'                            rod)
'
'    'Avoid Error handler
'    Exit Function
'
'ErrorVBShowSweptVolume_USCS:
'    ErrorLocation = "Error Calculating SweptVolume_USCS"
'    MsgString = ErrorLocation & "->" & Str$(Err.Number) & ": " & Err.Description
'    MsgBox MsgString
'    Exit Function
'
'End Function
'
'Public Function VBShowTheoreticalVE(connrodlength As Double, _
'        stroke As Double, _
'        bore As Double, _
'        rod As Double, _
'        clearance As Double, _
'        nexp As Double, _
'        ncomp As Double, _
'        psuct As Double, _
'        pdish As Double)
'
'    On Error GoTo ErrorVBShowTheoreticalVE
'
'
'    'Call the .dll routine for Theoretical
'    VBShowTheoreticalVE = TheoreticalVE(connrodlength, _
'                            stroke, _
'                            bore, _
'                            rod, _
'                            clearance, _
'                            nexp, _
'                            ncomp, _
'                            psuct, _
'                            pdish)
'
'    'Avoid Error handler
'    Exit Function
'
'ErrorVBShowTheoreticalVE:
'    ErrorLocation = "Error Calculating VBShowTheoreticalVE"
'    MsgString = ErrorLocation & "->" & Str$(Err.Number) & ": " & Err.Description
'    MsgBox MsgString
'    Exit Function
'
'End Function
'
