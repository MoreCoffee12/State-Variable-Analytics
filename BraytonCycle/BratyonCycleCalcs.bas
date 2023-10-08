Attribute VB_Name = "BratyonCycleCalcs"
Option Explicit

' =============================================================================
' Function Name:  BalanceCombustion
' Purpose: Given the fuel, air, and diluent molecular formulas and mol weights,
'   this function attempts to balance the equation. This uses a brute force method
'   based on components present. It needs refinment to handle general cases.
'   It also assumes the dileunts do not interact and that they scale with the
'   air flow.
'   This code draws heavily from the paper "Flame Temperature and Species
'   Calculations with an Excel/VBA Computer Code." Refer to that paper for
'   additional details on nomenclature and derivations.
'
'   In general:
'   CxHyOzNuSv + m(O2 + fN2) -> n1CO + n2CO2 + n3H2O + n4N2 + n5O2 + n6H2 + n7SO2
'
'   Nomenclature:
'   x = carbonMols, assembled by adding all the carbons in the strFuel,
'       multiplied by the mols.
'   y = hydrogenMols, assembled by adding all the hydrogen in the strFuel,
'       multiplied by the mols.
'   z = oxygenFuelMols, assembled by adding all the hydrogen in the strFuel,
'       multiplied by the mols.
'   u = Not Used. None of the fuels in the EOS have nitrogen.
'   v = Not Used. None of the fuels in the EOS have sulfur.
'   ms = molStoichm, caclulated using the equation for stoichiometric ratio below:
'       Stoichiometric: m = ms = x + y/4 – z/2 + v
'   m = ( oxygenAirMols / 2 ), extracted from the strAir formula
'
'   n1 = molsProducts(idxCO), mols of carbon monoxide in the combustion product stream
'   n2 = molsProducts(idxCO2), mols of carbon in the combustion product stream
'   n3 = molsProducts(idxH2O), mols of hydrogen in the combustion product stream
'   n4 = molsProducts(idxN2), mols of nitrogen in the combustion product stream
'   n5 = molsProducts(idxO2), mols of oxygen in the product
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
    Dim carbonAtomsChem(25) As Double
    Dim hydrogenAtomsChem(25) As Double
    Dim oxygenAtomsChem(25) As Double
    
    ' Store the total atom counts
    Dim carbonMols As Double
    carbonMols = 0#
    Dim hydrogenMols As Double
    hydrogenMols = 0#
    Dim oxygenFuelMols As Double
    oxygenFuelMols = 0#
    Dim oxygenAirMols As Double
    oxygenAirMols = 0#
    
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
        
        ' Extract the number of atoms for each fluid in the database and total
        ' the mols for each one.
        carbonAtomsChem(idx) = GetElem(strChem(idx), "C")
        carbonMols = carbonMols + (molsFuel(idx) * carbonAtomsChem(idx))
        
        hydrogenAtomsChem(idx) = GetElem(strChem(idx), "H")
        hydrogenMols = hydrogenMols + (molsFuel(idx) * hydrogenAtomsChem(idx))
        
        oxygenAtomsChem(idx) = GetElem(strChem(idx), "O")
        oxygenFuelMols = oxygenFuelMols + (molsFuel(idx) * oxygenAtomsChem(idx))
        oxygenAirMols = oxygenAirMols + (molsAir(idx) * oxygenAtomsChem(idx))
        
    Next idx
    
    ' Some of the calculatioins are simpler if we define mols of air
    Dim molsAirO2 As Double
    molsAirO2 = (oxygenAirMols / 2#)
    
    ' To simplify the problem force the fuel to have hydrogen
    If hydrogenMols <= 0 Then
        Err.Raise 1, "FuelHasNoHydrogen", _
            "Assertion failed: Fuel must have at least one hydrogen atom"
    End If
    
    ' Setup the combustion product mol weight vector
    Dim molsProducts(25) As Double
    For idx = 0 To (iEOSCount - 1)
        molsProducts(idx) = 0#
    Next idx
    
    ' Set indexes to the assumed combustion products
    Dim idxCO2 As Long
    idxCO2 = 12
    Dim idxN2 As Long
    idxN2 = 14
    Dim idxH2O As Long
    idxH2O = 17
    Dim idxCO As Long
    idxCO = 19
    Dim idxO2 As Long
    idxO2 = 21
            
    ' Calculate total carbon and hydrogen mols in the fuel and oxygen in the air
    Dim molStoich As Double
    molStoich = carbonMols + (hydrogenMols / 4#) - (oxygenFuelMols / 2#)
    
    
    ' Calculate the equivalence ratio as:
    Dim equivRatio As Double
    equivRatio = molStoich / (oxygenAirMols / 2#)
        
    ' If allowed by input arguements, balance the elements
    If bStoichometric Or (Abs(equivRatio - 1) < dTrace) Then
    
        ' For the stoichiometric case (no dissasociation):
        ' molsProducts(idxCO), equals zero for this case (complete combustion)
        molsProducts(idxCO) = 0#
        
        ' molsProducts(idxCO2), equals the number of carbon mols (n2 = x)
        molsProducts(idxCO2) = carbonMols
        
        ' molsProducts(idxH2O), equals the half the number of hydrogen mols (n3 = y/2)
        molsProducts(idxH2O) = (hydrogenMols / 2#)
        
        ' molsProducts(idxN2), equals mols of air times mols of nitrogen (m * f)
        molsProducts(idxN2) = ((molStoich / molsAir(idxO2)) * molsAir(idxN2))
        
        ' Update input air stream to finish the balance
        molsAir(idxN2) = molsProducts(idxN2)
        molsAir(idxO2) = ((molStoich / molsAir(idxO2)) * molsAir(idxO2))
        
    
    Else
        
        ' For this case, the product stream may contain excess oxygen
        ' or excess fuel.
        
        If equivRatio > 1 Then
            
            ' Fuel rich case ignoring disassociation. From the paper the rationale is as follows:
            '   "If fuel rich, m < ms, and there is no O2 or H2 in the products, it is
            '   assumed that the lack of sufficient oxygen affects the CO2 only
            '   (part of the C oxidizing only as far as CO). The very reactive hydrogen, H,
            '   is assumed to fully oxidize to H2O.
            '   ...
            '   These equations hold for m ? mmin only. The cases m < mmin are of limited
            '   interest, for then not all the fuel burns, flame temperatures are low, ignition
            '   is a problem, and poor flames result. These extreme fuel-rich flames will be
            '   excluded from further discussion."
            
            ' n1 = molsProducts(idxCO), twice the difference between stoichiometery and input
            ' air mass, n1 = 2 *(ms – m)
            molsProducts(idxCO) = 2# * (molStoich - molsAirO2)
            
            ' Minimum air calculation (only oxidizes to CO and H2O)
            Dim molMin As Double
            molMin = (carbonMols / 2#) + (hydrogenMols / 4#)
            
            ' Enforce the limit on excessive fuel
            If molsAirO2 < molMin Then
                Err.Raise 1, "ExcessiveFuel", _
                    "Assertion failed: Fuel mass results in low flame temperatures and unlikely to ignite."
            End If
        
            ' Use the minimum air mass to determine rest of components
            ' n2 = molsProducts(idxCO2), equals the number of carbon mols (n2 =  2 *( m - mmin))
            molsProducts(idxCO2) = (2# * (molsAirO2 - molMin))
            
            ' n3 = molsProducts(idxH2O), All the reactive hydrogens bond to oxygen (n3 = y/2)
            molsProducts(idxH2O) = (hydrogenMols / 2#)
            
            ' Bring across the unburned fuel
            Dim molsFuelUnburned As Double
            molsFuelUnburned = carbonMols - (molsProducts(idxCO) + molsProducts(idxCO2))
            For idx = 0 To (iEOSCount - 1)
                If molsFuel(idx) > dTrace Then
                    molsProducts(idx) = molsFuelUnburned
                End If
            Next idx
            
            ' Finally, balance out the nitrogen and input air
            molsProducts(idxN2) = molsAir(idxN2)
            
        Else
        
            ' Fuel lean case
            
            ' molsProducts(idxCO), no CO in the products
            molsProducts(idxCO) = 0#
            
            ' molsProducts(idxCO2), equals the number of carbon mols (n2 = x)
            molsProducts(idxCO2) = carbonMols
            
            ' molsProducts(idxH2O), equals the half the number of hydrogen mols (n3 = y/2)
            molsProducts(idxH2O) = (hydrogenMols / 2#)
            
            ' n5 = molsProducts(idxO2), equals difference between stoichimetric mols and
            ' number of mols of air
            molsProducts(idxO2) = (molsAirO2 - molStoich)

            ' molsProducts(idxN2), equals mols of air times mols of nitrogen (m * f)
            molsProducts(idxN2) = (molsAir(idxN2))
            
        
        End If
        
         
    End If
    
    ' Finally bring across any dlieunts
    For idx = 0 To (iEOSCount - 1)
        molsProducts(idx) = molsProducts(idx) + molsDiluent(idx)
    Next idx
    
    ' Build the reaction strings
    Dim strFuelMod As String, strAirMod As String, strDiluentMod As String, strProd As String
    strFuelMod = ""
    strAirMod = ""
    strDiluentMod = ""
    strProd = ""
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
                strAirMod = FormatNumber(molsAir(idx), 4)
            Else
                strAirMod = strAirMod & " + " & FormatNumber(molsAir(idx), 4)
            End If
            
            strAirMod = strAirMod & strChem(idx)
            
        End If
            
        ' Build out the diluent formulas
        If molsDiluent(idx) > 0 Then
            
            ' Seed the string if it hasn't been done
            If Len(strDiluentMod) < 1 Then
                strDiluentMod = FormatNumber(molsDiluent(idx), 4)
            Else
                strDiluentMod = strDiluentMod & " + " & FormatNumber(molsDiluent(idx), 4)
            End If
            
            strDiluentMod = strDiluentMod & strChem(idx)
            
        End If
        
        ' Build out the combustion products stream
        If molsProducts(idx) > 0 Then
            
            ' Seed the string if it hasn't been done
            If Len(strProd) < 1 Then
                strProd = FormatNumber(molsProducts(idx), 4)
            Else
                strProd = strProd & " + " & FormatNumber(molsProducts(idx), 4)
            End If
            
            strProd = strProd & strChem(idx)
            
        End If
        
    Next idx

    ' Build the final balanced equation string
    outputString = strFuelMod & " + " & strAirMod
    If Len(strDiluentMod) > 1 Then
        outputString = outputString & " + " & strDiluentMod & "-> "
    Else
        outputString = outputString & "->"
    End If
    outputString = outputString & strProd

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

