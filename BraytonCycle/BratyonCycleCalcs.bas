Attribute VB_Name = "BratyonCycleCalcs"
Option Explicit

' =============================================================================
' Function Name:  BalanceCombustion
' Purpose: Given the fuel, air, and diluent molecular formulas and mol weights,
'   this function attempts to balance the equation. This uses a brute force method
'   based on components present. It needs refinment to handle general cases.
'   It also assumes the dileunts do not interact and that they scale with the
'   air flow.
'
' Parameters:
' - inputString: String, String contains the left-hand side of the equation.
'   For example: 0.0321CH4 + 0.0274N2 + 0.0223H2O + 0.0073O2
'
' Returns:
' - String: Balanced right-hand side of the equation
'
' Error Handling:
' The function has built-in error handling which returns the error number and description.
'
' Notes:
' - Failure to balance needs to be determined
'
' Author: Brian Howard
' Date: 6 Oct 2023
' =============================================================================
Function BalanceCombustion(strFuel As Variant, strAir As Variant, _
    strDiluent As Variant, bStoichometric As Boolean) As String

    ' We want to work with molar quantities
    Dim CH4index As Long
    Dim molsFuel(25) As Double
    Dim molsAir(25) As Double
    Dim molsDiluent(25) As Double
    Dim carbonAtoms(25) As Double
    Dim hydrogenAtoms(25) As Double
    Dim oxygenAtoms(25) As Double
    
    ' Indexing and arrays for EOS information
    Dim iChems As Long
    Dim strChem(25) As String
    Dim iEOSCount As Long
    Dim iPos As Long
    iEOSCount = VBShowFluidCount()
    Dim idx As Long
    
    ' Calculation defaults
    Dim dTrace As Double
    dTrace = 0.0000000001
    
    ' Default to a null return string
    Dim outputString As String
    outputString = ""
    
    'Establish error trapping
    On Error GoTo ErrorBalanceCombustion
    
    ' Validate inputs
    If Len(strFuel) < 2 Then
        Err.Raise 1, "AssertNotNullOrEmpty", "Assertion failed: Fuel string is null or has too few characters"
    End If
    If Len(strAir) < 3 Then
        Err.Raise 1, "AssertNotNullOrEmpty", "Assertion failed: Air string is null or has too few characters"
    End If
    
    ' Format inputs: append a space to ensure we trigger the end of chemical flag
    strFuel = strFuel + " "
    strAir = strAir + " "
    strDiluent = strDiluent + " "
    
    ' Extract the number of mols for each chemical in the left hand side
    For idx = 0 To (iEOSCount - 1)
        
        ' Retrieve null terminated string and format
        strChem(idx) = VBShowFormula(idx + 1, 50)
        iPos = InStr(strChem(idx), Chr$(0))
        If iPos > 0 Then
            strChem(idx) = Left(strChem(idx), iPos - 1)
        End If
        
        ' Append a space to indicate termination of the chemical formula. Otherwise
        ' the code confuses 'H2' and 'H2O'
        strChem(idx) = strChem(idx) + " "
        
        ' Call the function to calculate the fuel, air, and diluent mols
        molsFuel(idx) = GetMols(strChem(idx), strFuel)
        molsAir(idx) = GetMols(strChem(idx), strAir)
        molsDiluent(idx) = GetMols(strChem(idx), strDiluent)
        
        ' Extract the number of atoms for each fluid in the database
        carbonAtoms(idx) = GetElem(strChem(idx), "C")
        hydrogenAtoms(idx) = GetElem(strChem(idx), "H")
        oxygenAtoms(idx) = GetElem(strChem(idx), "O")
        
    Next idx
    
    ' Setup the combustion product mol weight vector
    Dim molsProducts(25) As Double
    For idx = 0 To (iEOSCount - 1)
        molsProducts(idx) = 0#
    Next idx
    
    ' Setup indexes to the assumed combustion products
    Dim idxCO2 As Long
    idxCO2 = 12
    Dim idxN2 As Long
    idxN2 = 14
    Dim idxH2O As Long
    idxH2O = 17
    Dim idxO2 As Long
    idxO2 = 21
            
    ' Calculate total carbon and hydrogen mols in the fuel and oxygen in the air
    Dim molsO As Double
    molsO = 0#
    For idx = 0 To (iEOSCount - 1)
    
        molsProducts(idxCO2) = molsProducts(idxCO2) + (molsFuel(idx) * carbonAtoms(idx))
        molsProducts(idxH2O) = molsProducts(idxH2O) + (molsFuel(idx) * hydrogenAtoms(idx)) / 2
        molsO = molsO + (molsAir(idx) * oxygenAtoms(idx))
        
    Next idx
    
    
    ' To simplify the problem force the fuel to have hydrogen
    If molsProducts(idxH2O) <= 0 Then
        Err.Raise 1, "FuelHasNoHydrogen", _
            "Assertion failed: Fuel must have hydrogen"
    End If

    ' Calculate required oxygen for complete combustion
    Dim molsOComplete As Double
    ' CO2 requires 2 O per C, and H2O requires 1 O per H
    molsOComplete = (2 * molsProducts(idxCO2) + (molsProducts(idxH2O) * 2#) / 2#)
    
    ' Does the reaction require additional oxygen need to
    ' balance for complete combustion?
    Dim AirIncrease As Double
    AirIncrease = 1#
    If molsOComplete > molsO Then
        AirIncrease = molsOComplete / molsO
    End If
    
    ' Finally, balance out the nitrogen
    molsProducts(idxN2) = AirIncrease * molsAir(14)
    
    ' Build the reaction streams
    Dim strFuelMod As String, strAirMod As String, strDiluentMod As String
    strFuelMod = ""
    strAirMod = ""
    strDiluentMod = ""
    For idx = 0 To (iEOSCount - 1)
            
        ' Build out the fuel formulas
        If molsFuel(idx) > 0 Then
            
            ' Seed the string if it hasn't been done
            If Len(strFuelMod) < 1 Then
                strFuelMod = FormatNumber(molsFuel(idx), 4)
            Else
                strFuelMod = strFuelMod & " + " & FormatNumber(molsFuel(idx), 4)
            End If
            
            strFuelMod = strFuelMod & strChem(idx)
            
        End If
            
        ' Build out the air formulas
        If molsAir(idx) > 0 Then
            
            ' Seed the string if it hasn't been done
            If Len(strAirMod) < 1 Then
                strAirMod = FormatNumber(AirIncrease * molsAir(idx), 4)
            Else
                strAirMod = strAirMod & " + " & FormatNumber(AirIncrease * molsAir(idx), 4)
            End If
            
            strAirMod = strAirMod & strChem(idx)
            
        End If
            
        ' Build out the diluent formulas
        If molsDiluent(idx) > 0 Then
            
            ' Seed the string if it hasn't been done
            If Len(strAirMod) < 1 Then
                strDiluentMod = FormatNumber(AirIncrease * molsDiluent(idx), 4)
            Else
                strDiluentMod = strDiluentMod & " + " & FormatNumber(AirIncrease * molsDiluent(idx), 4)
            End If
            
            strDiluentMod = strDiluentMod & strChem(idx)
            
        End If
        
    Next idx

    ' Build the balanced equation string
    outputString = strFuelMod & " + " & strAirMod
    If Len(strDiluentMod) > 1 Then
        outputString = outputString & " + " & strDiluentMod & "-> "
    Else
        outputString = outputString & "->"
    End If
    If molsProducts(idxCO2) > dTrace Then
        outputString = outputString & FormatNumber(molsProducts(idxCO2), 4) & "CO2 + "
    End If
    If molsProducts(idxH2O) > dTrace Then
        outputString = outputString & FormatNumber(molsProducts(idxH2O), 4) & "H2O + "
    End If
    If molsProducts(idxN2) > dTrace Then
        outputString = outputString & FormatNumber(molsProducts(idxN2), 4) & "N2"
    End If

    ' Return the balanced equation
    BalanceCombustion = outputString
    
    'Avoid the error handler
    Exit Function

ErrorBalanceCombustion:

    BalanceCombustion = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

' =============================================================================
' Function Name:  GetMols
' Purpose: Given a chemical formula and full equation, extract the number of mols
'   for that chemical from the equation.
'
' Parameters:
' - strChem: String, String contains the chemical formula of interest, such as CH4.
' - strEqn: String, String contains the left-hand side of the equation.
'   For example: 0.0321CH4 + 0.0274N2 + 0.0223H2O + 0.0073O2
'
' Returns:
' - Double: mol value for that chemical
'
' Error Handling:
' The function has built-in error handling which returns the error number and description.
' The function throws an error if it cannot cast the string to a valid mol weight value.
'
' Notes:
' - The function returns zero if the chemical formula is not found in the equation
'
' Author: Brian Howard
' Date: 6 Oct 2023
' =============================================================================
Function GetMols(strChem As String, strEqn As Variant) As Double

    Dim idxChem As Long
    Dim arrChem() As String
    Dim strLeft As String
    Dim dmols As Double
    dmols = 0#
    
    'Establish error trapping
    On Error GoTo ErrorGetMols
    
    ' Try to find the chemical formula in the reaction equation
    idxChem = InStr(1, strEqn, strChem)
    If idxChem >= 1 Then
    
        ' Attempt to pull a string from the left of the chemical
        strLeft = Left(strEqn, idxChem - 1)
        
        ' When the mol weights in an equation are 1, they typically
        ' get left out. For example, 1 mol of CH4 would be written
        ' as 'CH4'.  For these cases the 'strLeft' will not have a
        ' length
        If Len(strLeft) < 1 Then
            dmols = 1#
        Else
        
            ' Something exists to the left of the chemical formula so
            ' split the string .
            arrChem = Split(strLeft, " ")
            
            
            ' The last item in this list should be the mols for CH4
            dmols = CDbl(arrChem(UBound(arrChem)))
            
            ' Validate the molar value
            If dmols <= 0# Then
                Err.Raise 1, "InvalidMolWeight", "Assertion failed: " + strChem + "mol weight is less than or equal to zero"
            End If
        
        End If
    
    End If
    
    GetMols = dmols
    
    'Avoid the error handler
    Exit Function

ErrorGetMols:

    MsgBox (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

' =============================================================================
' Function Name:  GetElem
' Purpose: Given a chemical formula and full equation, extract the number of mols
'   for that chemical from the equation.
'
' Parameters:
' - strChem: String, String contains the chemical formula of interest, such as 'CH4'.
' - strElem: String, String contains the element of interest, such as 'C'
'
' Returns:
' - Double: Number of molecules of strElem in
'
' Error Handling:
' The function has built-in error handling which returns the error number and description.
' The function throws an error if it cannot cast the string to a valid number of molecules.
'
' Notes:
' - The function returns zero if the element is not found in the equation
'
' Author: Brian Howard
' Date: 6 Oct 2023
' =============================================================================
Function GetElem(strChem As String, strElem As Variant) As Double

    Dim idxElem As Long
    Dim arrChem() As String
    Dim dAtoms As Double
    Dim iNumeric As Long
    dAtoms = 0#
    
    'Establish error trapping
    On Error GoTo ErrorGetElem
    
    ' Try to find the chemical formula in the reaction equation
    idxElem = InStr(1, strChem, strElem)
    If idxElem > 0 Then
    
        ' We have at least one molecule
        dAtoms = 1#
    
        ' Is the next character numeric?
        iNumeric = 0
        If IsNumeric(Mid(strChem, idxElem + 1, 1)) Then
            
            iNumeric = iNumeric + 1
            
            ' Is there a second numeric character?
            If IsNumeric(Mid(strChem, idxElem + 2, 1)) Then
                
                iNumeric = iNumeric + 1
                
            End If
                    
        End If
        
        ' If numeric characters have been found, extract the number
        If iNumeric > 0 Then
            dAtoms = CDbl(Mid(strChem, idxElem + 1, iNumeric))
        End If
        
        ' Validate the number of atoms
        If dAtoms <= 0# Then
            Err.Raise 1, "InvalidNumberOfAtoms", "Assertion failed: " + strChem + "atoms is less than or equal to zero"
        End If
        
    End If
    
    GetElem = dAtoms
    
    'Avoid the error handler
    Exit Function

ErrorGetElem:

    MsgBox (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

