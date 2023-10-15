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
' - dTemp_K: Double, Temperature of reactants and diluents in kelvin
' - dPres_bar: Double, Pressure of reactants and diluents in bar (absolute)
' - strFuel: String, String contains fuel chemical formula.
' - strAir: String, String contains air chemical formula.
' - strDiluent: String, String contains diluent chemical formula.
'
' Returns:
' - Variant: Arrary with the following values:
'   - String: Balanced chemical reaction equation
'   - Double: Adiabatic flame temperature
'   - Array Double: Remaining entries have fluid percent MW
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
Function BalanceCombustion(dTemp_K As Double, dPres_bar As Double, strFuel As Variant, strAir As Variant, _
    strDiluent As Variant, bStoichometric As Boolean) As Variant

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
    
    ' This function returns a variant array with the plain test
    ' chemical reaction equation, adiabatic temperature, and
    ' percent mol weights for each fluid.
    Dim arrOutput(26) As Variant
        
    ' Default to a null return string
    Dim outputString As String
    outputString = ""
    
    ' Establish error trapping
    On Error GoTo ErrorBalanceCombustion
    
    ' Validate inputs
    If Len(strFuel) < 2 Then
        Err.Raise 1, "AssertNotNullOrEmpty", "Assertion failed: Fuel string is null or has too few characters"
    End If
    If Len(strAir) < 2 Then
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
    
    ' Configure the solver
    Dim dPrecision As Double, dMaxIter As Double, eosset As Long, iErrTolerance As Long
    dPrecision = 0.00001
    dMaxIter = 200
    eosset = 1
    iErrTolerance = 10
    Dim eline01 As String
    eline01 = String(256, "a")
    Dim i As Long
    Dim priority01 As Double
    priority01 = 0
    
    ' Begin adiabatic temperature estimation by defining percent mole
    ' weights (mol fraction)
    Dim molsLHS(25) As Double, molWeightLHS As Double, molWeightProd As Double
    molWeightLHS = 0#
    molWeightProd = 0#
    For idx = 0 To (iEOSCount - 1)
        
        ' Assemble all the left-hand side components
        molsLHS(idx) = molsFuel(idx) + molsAir(idx) + molsDiluent(idx)
                
        ' Sum mole weights
        molWeightLHS = molWeightLHS + molsLHS(idx)
        molWeightProd = molWeightProd + molsProducts(idx)
        
    Next idx
    
    ' There is no reason the mol weights should be zero at this point
    If molWeightLHS <= 0 Then
        Err.Raise 1, "EquationLHSZeroMolWeight", _
            "Reaction equation left-hand side (air, fuel, and diluent) have zero mol weight"
    End If
    If molWeightProd <= 0 Then
        Err.Raise 1, "EquationProdZeroMolWeight", _
            "Reaction equation combustion products have zero mol weight"
    End If
    
                            
    ' With the mol weights known for each side, calculate the fraction
    Dim fractLHS(25) As Double, fractProd(25) As Double
    For idx = 0 To (iEOSCount - 1)
        fractLHS(idx) = molsLHS(idx) / molWeightLHS
        fractProd(idx) = molsProducts(idx) / molWeightProd
    Next idx
    
    ' We need the molecular weight for the products. For the
    ' 'TestHarness_AdiabaticFlame' tab case:
    '   dMWProd = 27.64
    Dim dMWProd As Double
    dMWProd = 0#
    ' Call to the DLL to mixtore molecular weight
    i = ShowMolWeight_mx(dMWProd, _
                    priority01, _
                    eline01, _
                    fractProd(0))
    ' See if the EOS threw any serious errors
    If ((priority01 > 0) And (priority01 <= iErrTolerance)) Then
         Err.Raise 1, "ShowMolWeight_mx for the producvts", _
            "Calculation error in the product molecular weight calculation"
    End If
    
    
    ' Calculating ethalpy of formation (hfo) for both sides
    Dim hfoLHS_mols As Double, hfoProd_mols As Double, dhfo As Double
    hfoLHS_mols = 0#
    hfoProd_mols = 0#
    dhfo = 0#
    For idx = 0 To (iEOSCount - 1)
        
        ' Sum enthalpy of formation for LHS...this will have units
        ' of J/gmol
        i = ShowHfo_SI(idx + 1, _
                        dhfo, _
                        priority01, _
                        eline01)
        hfoLHS_mols = hfoLHS_mols + (dhfo * fractLHS(idx))
        hfoProd_mols = hfoProd_mols + (dhfo * fractProd(idx))
        
    Next idx
    
    ' Standard enthalpy of formation values at STP. For the
    ' test case documented in the 'TestHarness_AdiabaticFlame'
    ' tab, the  following values should be returned:
    '   dHfoLHS_mol = -7079
    '   dHfoProd_mol = -83378
    Dim dHfoLHS_mol As Double, dHfoProd_mol As Double
    dHfoLHS_mol = 0#
    i = ShowHfo_mx_SI(eosset, _
                    fractLHS(0), _
                    dPrecision, _
                    dMaxIter, _
                    dHfoLHS_mol, _
                    priority01, _
                    eline01)
    ' See if the EOS threw any serious errors for the reactants (LHS)
    If ((priority01 > 0) And (priority01 <= iErrTolerance)) Then
         Err.Raise 1, "ShowHfo_mx_SI for the LHS", _
            "The reactant hfo enthalpy calculation at STP conditions failed to converge"
    End If
    
    ' Calculations for the  combustion side
    dHfoProd_mol = 0#
    i = ShowHfo_mx_SI(eosset, _
                    fractProd(0), _
                    dPrecision, _
                    dMaxIter, _
                    dHfoProd_mol, _
                    priority01, _
                    eline01)
    ' See if the EOS threw any serious errors for the combustion products
    If ((priority01 > 0) And (priority01 <= iErrTolerance)) Then
         Err.Raise 1, "ShowHfo_mx_SI for the combustion products", _
            "The product hfo enthalpy calculation at STP conditions failed to converge"
    End If
    
                            

    ' Estimate the product enthalpies at inlet conditions. Check the return value
    ' to see if there was an error. From the 'TestHarness_AdiabaticFlame'
    ' tab the values for the methane stoichimetric adiabatic flame
    ' should be about:
    '   dEnthProd_298_kg = 345.1 (kJ/kg)
    '   dEnthProd_298_mol = 9538.9 (kJ/kg)
    Dim dEnthProd_298_kg As Double, dEnthProd_298_mol As Double, _
        dEnthProd_abs_mol As Double
    dEnthProd_298_kg = 0
    i = ShowH_TP_SI(eosset, _
                    dTemp_K, _
                    dPres_bar, _
                    fractProd(0), _
                    dPrecision, _
                    dMaxIter, _
                    dEnthProd_298_kg, _
                    priority01, _
                    eline01)

    ' See if the EOS threw any serious errors
    If ((priority01 > 0) And (priority01 <= iErrTolerance)) Then
         Err.Raise 1, "EnthalpyCalcFailed", _
            "The combustion product enthalpy calculation at inlet conditions failed to converge"
    End If
    
    ' Convert to molar mass basis
    dEnthProd_298_mol = dEnthProd_298_kg * dMWProd
    
    ' Begin half-interval search technique to find temperature
    '
    ' Initialize the flame temperature to 4000 K in prep for the
    ' trial and error search
    Dim dFlameTemp_K As Double, dDeltaT_K As Double, _
        dTempConv As Double, dIterNum As Double
    dFlameTemp_K = 4000
    dDeltaT_K = 2000
    dTempConv = 1#
    dIterNum = 0#

'    ' Begin the loop
    Dim dEnthProd_kg As Double, dQ As Double
    dEnthProd_kg = 0#
    dQ = 0#
    While dDeltaT_K > dTempConv And dIterNum < dMaxIter

        ' Calculate enthalpy for the current temperature
        dEnthProd_kg = 0
        i = ShowH_TP_SI(eosset, _
                        dFlameTemp_K, _
                        dPres_bar, _
                        fractProd(0), _
                        dPrecision, _
                        dMaxIter, _
                        dEnthProd_kg, _
                        priority01, _
                        eline01)

        ' See if the EOS threw any serious errors
        If ((priority01 > 0) And (priority01 <= iErrTolerance)) Then
             Err.Raise 1, "EnthalpyCalcFailed", _
                "The reaction left-hand side (LHS) enthalpy calculation failed to converge"
        End If
        
        ' Calculate the absolute enthalpy in molar mass
        ' From the 'TestHarness_AdiabaticFlame' values should be:
        '   dEnthProd_kg = 9798.3
        '   dEnthProd_abs_mol = 177888.6
        '   dHfoProd_mol = -83378.13688
        dEnthProd_abs_mol = ((dEnthProd_kg * dMWProd) - dEnthProd_298_mol) + _
            dHfoProd_mol

        ' Calculate the heat input, Q using absolute enthalpies
        ' From the 'TestHarness_AdiabaticFlame' values should be:
        '   dQ = 94510.4
        dQ = (dEnthProd_abs_mol + dHfoProd_mol)
        
        Debug.Print "dFlameTemp_K: " + Format$(dFlameTemp_K, "###0.00") + _
            " | dQ: " + Format$(dQ, "#.00") + _
            " | dEnthProd_abs_mol: " + Format$(dEnthProd_abs_mol, "#.00")

        ' Calculate the next temperature
        If dQ > 0 Then

            ' The product stream had too much energy, reduce temperature
            dFlameTemp_K = dFlameTemp_K - dDeltaT_K

        Else

            ' The product stream had too little energy, increase temperature
            dFlameTemp_K = dFlameTemp_K + dDeltaT_K

        End If
        

        ' Reduce step size
        dDeltaT_K = (dDeltaT_K / 1.25)

        ' Increment the number of iterations
        dIterNum = dIterNum + 1#

    Wend

    ' Did we exit because we found the correct temperature or
    ' reached the maximum number of iterations?
    If Abs(dIterNum - dMaxIter) < dTrace Then
        dFlameTemp_K = -1#
    End If

    ' Return the balanced equation, adiabatic temperature, and
    ' the percent mol weights
    arrOutput(0) = outputString
    arrOutput(1) = dFlameTemp_K
    For idx = 0 To (iEOSCount - 1)
        arrOutput(2 + idx) = fractProd(idx)
    Next idx
    BalanceCombustion = arrOutput
    
    'Avoid the error handler
    Exit Function

ErrorBalanceCombustion:

    BalanceCombustion = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

' =============================================================================
' Function Name:  GetAbsEnth
' Purpose: Given an index, return the absolute enthalpy for that fluid. From
' "Flame Temperature and Species Calculations with an Excel/VBA Computer Code"
'
' Parameters:
' - idxChem: Long, Fluid index.
'
' Returns:
' - Double: estimated absolute enthalpy, units of kJ/kg-mol
' - Double: temperature, K
'
' Error Handling:
' The function has built-in error handling
'
' Notes:
' - The function returns zero if the chemical formula is not found in the equation
'
' Author: Brian Howard
' Date: 8 Oct 2023
' =============================================================================

Function GetAbsEnth(idxChem As Long, dTemp_K As Double) As Double

    Dim dEnthAbs As Double
    dEnthAbs = 0#
    
    Dim dA As Double
    Dim dB As Double
    Dim dC As Double
    
    ' Establish error trapping
    On Error GoTo ErrorGetAbsEnth
    
    ' Try to find the index and formula
    Select Case idxChem
    
    ' CO
    Case 19
        dA = 309070
        dB = 39.29
        dC = -6201.9
        
    ' CO2
    Case 12
        dA = 93048
        dB = 68.58
        dC = -16979
        
    ' H2O
    Case 17
        dA = 154670
        dB = 60.43
        dC = 19212#
        
    ' N2
    Case 14
        dA = 44639
        dB = 39.32
        dC = -6753.4
        
    ' O2
    Case 21
        dA = 127010
        dB = 46.25
        dC = -18798#
        
    ' H2
    Case 15
        dA = 461750
        dB = 46.23
        dC = -27649#
        
    ' Fall through to default
    Case Else
        dA = 0#
        dB = 0#
        dC = 0#
    
    End Select
    
    ' Return the value
    dEnthAbs = dA + (dB * dTemp_K) + (dC * Log(dTemp_K))
    GetAbsEnth = dEnthAbs
    
    'Avoid the error handler
    Exit Function

ErrorGetAbsEnth:

    MsgBox (Str(Err.Number) & ":" & Err.Description)
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
    
    ' Establish error trapping
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
    
    ' Establish error trapping
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

