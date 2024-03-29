Attribute VB_Name = "EOSCalcs"
Option Explicit
Option Base 0

'The variables help to prevent recursive errors
Public RecursiveFlag As Boolean

'These are interface function for the EOS engine
Declare Function ShowVapPres_T_USCS _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal temp As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef ps As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowVapPres_T_SI _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal temp As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef ps As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowSatVapV_T_USCS _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal temp As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef v As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowSatVapV_T_SI _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal temp As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef v As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowSatLiqV_T_USCS _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal temp As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef v As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowSatLiqV_T_SI _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal temp As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef v As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowSatVapH_T_USCS _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal temp As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef h As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowSatVapH_T_SI _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal temp As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef h As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowSatLiqH_T_USCS _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal temp As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef h As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowSatLiqH_T_SI _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal temp As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef h As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowSatVapS_T_USCS _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal temp As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef s As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowSatLiqS_T_USCS _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal temp As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef s As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowVapTemp_P_USCS _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal temp As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef ts As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowVapTemp_P_SI _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal temp As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef ts As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowSatVapV_P_SI _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal pres As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef v As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowSatVapV_P_USCS _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal pres As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef v As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowSatLiqV_P_SI _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal pres As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef v As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowSatLiqV_P_USCS _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal pres As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef v As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowSatVapH_P_USCS _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal pres As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef h As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowSatVapH_P_SI _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal pres As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef h As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowSatLiqH_P_USCS _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal pres As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef h As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowSatLiqH_P_SI _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal pres As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef h As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowSatVapS_P_SI _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal pres As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef s As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowSatVapS_P_USCS _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal pres As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef s As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowSatLiqS_P_SI _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal pres As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef s As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowSatLiqS_P_USCS _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal pres As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef s As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowP_VT_USCS _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal md As Double, _
                            ByVal temp As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef pres As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowP_VT_SI _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal md As Double, _
                            ByVal temp As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef pres As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowP_VS_SI _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal v As Double, _
                            ByVal s As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef pres As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowP_VS_USCS _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal v As Double, _
                            ByVal s As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef pres As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowP_MT_USCS _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal md As Double, _
                            ByVal temp As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef pres As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String, _
                            ByVal iErrorLineSize As Integer) As Integer

Declare Function ShowT_HP_SI _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal h As Double, _
                            ByVal p As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef t As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer
                            
Declare Function ShowT_HP_USCS _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal h As Double, _
                            ByVal p As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef t As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer
                            
Declare Function ShowT_SP_SI _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal s As Double, _
                            ByVal p As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef t As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowT_SP_USCS _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal s As Double, _
                            ByVal p As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef t As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowV_TP_SI _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal temp As Double, _
                            ByVal pres As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef v As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowV_TP_USCS _
    Lib "GasAnalysis.dll" (ByVal eosset As Integer, _
                            ByVal temp As Double, _
                            ByVal pres As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef v As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowS_TP_SI _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal temp As Double, _
                            ByVal pres As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef s As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowS_TP_USCS _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal temp As Double, _
                            ByVal pres As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef s As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowH_TP_SI _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal temp As Double, _
                            ByVal pres As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef h As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowH_TP_USCS _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal temp As Double, _
                            ByVal pres As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef h As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowH_TS_SI _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal temp As Double, _
                            ByVal entr As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef h As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowH_TS_USCS _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal temp As Double, _
                            ByVal entr As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef h As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowF_TP_USCS _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal temp As Double, _
                            ByVal pres As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef f As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowHfo_mx_SI _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef h As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowHfo_mx_USCS _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef h As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer
                            
Declare Function ShowSfo_mx_SI _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef h As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowSfo_mx_USCS _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef h As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer
                            
Declare Function ShowH_H298_SI _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal temp As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef h As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowH_H298_USCS _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal temp As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef h As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer
                            
Declare Function ShowSo_SI _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal temp As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef h As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowSo_USCS _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal temp As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef h As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowHIdeal_SI _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal temp As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef h As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowHIdeal_USCS _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal temp As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef h As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer
                            
Declare Function ShowViscosityGas_TP_SI _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal temp As Double, _
                            ByVal pres As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef v As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowViscosityGas_TP_USCS _
    Lib "GasAnalysis.dll" (ByVal eosset As Double, _
                            ByVal temp As Double, _
                            ByVal pres As Double, _
                            MixtureArray As Double, _
                            ByVal Precision As Double, _
                            ByVal MaxIterations As Double, _
                            ByRef v As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String) As Integer

Declare Function ShowMolWeight_mx _
    Lib "GasAnalysis.dll" (ByRef molweight As Double, _
                            ByRef priority01 As Double, _
                            ByVal textline01 As String, _
                             MixtureArray As Double) As Integer


Function VBShowVapPres_T_USCS(eosset As Integer, _
                        Temperature As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim ps As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowVapPres_T_USCS
    
    'Initialize local variables
    ps = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    i = ShowVapPres_T_USCS(eosset, _
                    Temperature, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    ps, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowVapPres_T_USCS = eline01
    Else
        VBShowVapPres_T_USCS = ps
    End If
      
    'Avoid the error handler
    Exit Function

ErrorVBShowVapPres_T_USCS:
    
    VBShowVapPres_T_USCS = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function
Function VBShowVapPres_T_SI(eosset As Integer, _
                        Temperature As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim ps As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowVapPres_T_SI
    
    'Initialize local variables
    ps = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    i = ShowVapPres_T_SI(eosset, _
                    Temperature, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    ps, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowVapPres_T_SI = eline01
    Else
        VBShowVapPres_T_SI = ps
    End If
      
    'Avoid the error handler
    Exit Function

ErrorVBShowVapPres_T_SI:
    
    VBShowVapPres_T_SI = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowSatVapV_T_USCS(eosset As Integer, _
                        Temperature As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim v As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowSatVapV_T_USCS
    
    'Initialize local variables
    v = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    i = ShowSatVapV_T_USCS(eosset, _
                    Temperature, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    v, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowSatVapV_T_USCS = eline01
    Else
        VBShowSatVapV_T_USCS = v
    End If
      
    'Avoid the error handler
    Exit Function

ErrorVBShowSatVapV_T_USCS:
    
    VBShowSatVapV_T_USCS = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowSatVapV_T_SI(eosset As Integer, _
                        Temperature As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim v As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowSatVapV_T_SI
    
    'Initialize local variables
    v = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    i = ShowSatVapV_T_SI(eosset, _
                    Temperature, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    v, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowSatVapV_T_SI = eline01
    Else
        VBShowSatVapV_T_SI = v
    End If
      
    'Avoid the error handler
    Exit Function

ErrorVBShowSatVapV_T_SI:
    
    VBShowSatVapV_T_SI = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowSatLiqV_T_USCS(eosset As Integer, _
                        Temperature As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim v As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowSatLiqV_T_USCS
    
    'Initialize local variables
    v = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane

    
    'If we need to we can check the return value
    'to see if there was an error
    i = ShowSatLiqV_T_USCS(eosset, _
                    Temperature, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    v, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowSatLiqV_T_USCS = eline01
    Else
        VBShowSatLiqV_T_USCS = v
    End If
      
    'Avoid the error handler
    Exit Function

ErrorVBShowSatLiqV_T_USCS:
    
    VBShowSatLiqV_T_USCS = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowSatLiqV_T_SI(eosset As Integer, _
                        Temperature As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim v As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowSatLiqV_T_SI
    
    'Initialize local variables
    v = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    i = ShowSatLiqV_T_SI(eosset, _
                    Temperature, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    v, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowSatLiqV_T_SI = eline01
    Else
        VBShowSatLiqV_T_SI = v
    End If
      
    'Avoid the error handler
    Exit Function

ErrorVBShowSatLiqV_T_SI:
    
    VBShowSatLiqV_T_SI = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowSatVapH_T_SI(eosset As Integer, _
                        Temperature As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim h As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowSatVapH_T_SI
    
    'Initialize local variables
    h = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    i = ShowSatVapH_T_SI(eosset, _
                    Temperature, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    h, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowSatVapH_T_SI = eline01
    Else
        VBShowSatVapH_T_SI = h
    End If
      
    'Avoid the error handler
    Exit Function

ErrorVBShowSatVapH_T_SI:
    
    VBShowSatVapH_T_SI = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowSatVapH_T_USCS(eosset As Integer, _
                        Temperature As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim h As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowSatVapH_T_USCS
    
    'Initialize local variables
    h = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    i = ShowSatVapH_T_USCS(eosset, _
                    Temperature, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    h, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowSatVapH_T_USCS = eline01
    Else
        VBShowSatVapH_T_USCS = h
    End If
      
    'Avoid the error handler
    Exit Function

ErrorVBShowSatVapH_T_USCS:
    
    VBShowSatVapH_T_USCS = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowSatVapS_T_USCS(eosset As Integer, _
                        Temperature As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim s As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowSatVapS_T_USCS
    
    'Initialize local variables
    s = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    i = ShowSatVapS_T_USCS(eosset, _
                    Temperature, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    s, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowSatVapS_T_USCS = eline01
    Else
        VBShowSatVapS_T_USCS = s
    End If
      
    'Avoid the error handler
    Exit Function

ErrorVBShowSatVapS_T_USCS:
    
    VBShowSatVapS_T_USCS = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowSatLiqH_T_SI(eosset As Integer, _
                        Temperature As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim h As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowSatLiqH_T_SI
    
    'Initialize local variables
    h = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    i = ShowSatLiqH_T_SI(eosset, _
                    Temperature, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    h, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowSatLiqH_T_SI = eline01
    Else
        VBShowSatLiqH_T_SI = h
    End If
      
    'Avoid the error handler
    Exit Function

ErrorVBShowSatLiqH_T_SI:
    
    VBShowSatLiqH_T_SI = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowSatLiqH_T_USCS(eosset As Integer, _
                        Temperature As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim h As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowSatLiqH_T_USCS
    
    'Initialize local variables
    h = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    i = ShowSatLiqH_T_USCS(eosset, _
                    Temperature, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    h, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowSatLiqH_T_USCS = eline01
    Else
        VBShowSatLiqH_T_USCS = h
    End If
      
    'Avoid the error handler
    Exit Function

ErrorVBShowSatLiqH_T_USCS:
    
    VBShowSatLiqH_T_USCS = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowSatLiqS_T_USCS(eosset As Integer, _
                        Temperature As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim s As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowSatLiqS_T_USCS
    
    'Initialize local variables
    s = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    i = ShowSatLiqS_T_USCS(eosset, _
                    Temperature, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    s, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowSatLiqS_T_USCS = eline01
    Else
        VBShowSatLiqS_T_USCS = s
    End If
      
    'Avoid the error handler
    Exit Function

ErrorVBShowSatLiqS_T_USCS:
    
    VBShowSatLiqS_T_USCS = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowHfo_mx_SI(eosset As Integer, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim h As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowHfo_mx_SI
    
    'Initialize local variables
    h = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    EOSNamePlate.i = ShowHfo_mx_SI(eosset, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    h, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowHfo_mx_SI = eline01
    Else
        VBShowHfo_mx_SI = h
    End If
    
    'Avoid the error handler
    Exit Function

ErrorVBShowHfo_mx_SI:
    
    VBShowHfo_mx_SI = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowHfo_mx_USCS(eosset As Integer, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim h As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowHfo_mx_USCS
    
    'Initialize local variables
    h = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    EOSNamePlate.i = ShowHfo_mx_USCS(eosset, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    h, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowHfo_mx_USCS = eline01
    Else
        VBShowHfo_mx_USCS = h
    End If
    
    'Avoid the error handler
    Exit Function

ErrorVBShowHfo_mx_USCS:
    
    VBShowHfo_mx_USCS = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowH_H298_SI(eosset As Integer, _
                        Temperature As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim h As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowH_H298_SI
    
    'Initialize local variables
    h = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    i = ShowH_H298_SI(eosset, _
                    Temperature, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    h, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowH_H298_SI = eline01
    Else
        VBShowH_H298_SI = h
    End If
      
    'Avoid the error handler
    Exit Function

ErrorVBShowH_H298_SI:
    
    VBShowH_H298_SI = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowH_H298_USCS(eosset As Integer, _
                        Temperature As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim h As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowH_H298_USCS
    
    'Initialize local variables
    h = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    i = ShowH_H298_USCS(eosset, _
                    Temperature, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    h, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowH_H298_USCS = eline01
    Else
        VBShowH_H298_USCS = h
    End If
      
    'Avoid the error handler
    Exit Function

ErrorVBShowH_H298_USCS:
    
    VBShowH_H298_USCS = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowHIdeal_SI(eosset As Integer, _
                        Temperature As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim h As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowHIdeal_SI
    
    'Initialize local variables
    h = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    i = ShowHIdeal_SI(eosset, _
                    Temperature, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    h, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowHIdeal_SI = eline01
    Else
        VBShowHIdeal_SI = h
    End If
      
    'Avoid the error handler
    Exit Function

ErrorVBShowHIdeal_SI:
    
    VBShowHIdeal_SI = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowHIdeal_USCS(eosset As Integer, _
                        Temperature As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim h As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowHIdeal_USCS
    
    'Initialize local variables
    h = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    i = ShowHIdeal_USCS(eosset, _
                    Temperature, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    h, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowHIdeal_USCS = eline01
    Else
        VBShowHIdeal_USCS = h
    End If
      
    'Avoid the error handler
    Exit Function

ErrorVBShowHIdeal_USCS:
    
    VBShowHIdeal_USCS = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowMolWeight_mx(PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim mw As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowMolWeight_mx
    
    'Initialize local variables
    mw = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    i = ShowMolWeight_mx(mw, _
                    priority01, _
                    eline01, _
                    MixingArray(0))
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowMolWeight_mx = eline01
    Else
        VBShowMolWeight_mx = mw
    End If
      
    'Avoid the error handler
    Exit Function

ErrorVBShowMolWeight_mx:
    
    VBShowMolWeight_mx = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowP_MT_USCS(eosset As Integer, _
                        MolarDensity As Double, Temperature As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim pres As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowP_MT_USCS
    
    'Initialize local variables
    pres = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    i = ShowP_MT_USCS(eosset, _
                    MolarDensity, _
                    Temperature, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    pres, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowP_MT_USCS = eline01
    Else
        VBShowP_MT_USCS = pres
    End If
      
    'Avoid the error handler
    Exit Function

ErrorVBShowP_MT_USCS:
    
    VBShowP_MT_USCS = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowP_VT_SI(eosset As Integer, _
                        MolarDensity As Double, Temperature As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim pres As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowP_VT_SI
    
    'Initialize local variables
    pres = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    EOSNamePlate.i = ShowP_VT_SI(eosset, _
                    MolarDensity, _
                    Temperature, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    pres, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowP_VT_SI = eline01
    Else
        VBShowP_VT_SI = pres
    End If
      
    'Avoid the error handler
    Exit Function

ErrorVBShowP_VT_SI:
    
    VBShowP_VT_SI = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowP_VT_USCS(eosset As Integer, _
                        MolarDensity As Double, Temperature As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim pres As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowP_VT_USCS
    
    'Initialize local variables
    pres = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    EOSNamePlate.i = ShowP_VT_USCS(eosset, _
                    MolarDensity, _
                    Temperature, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    pres, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowP_VT_USCS = eline01
    Else
        VBShowP_VT_USCS = pres
    End If
      
    'Avoid the error handler
    Exit Function

ErrorVBShowP_VT_USCS:
    
    VBShowP_VT_USCS = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowP_VS_SI(eosset As Integer, _
                        MolarDensity As Double, Temperature As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim pres As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowP_VS_SI
    
    'Initialize local variables
    pres = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    EOSNamePlate.i = ShowP_VS_SI(eosset, _
                    MolarDensity, _
                    Temperature, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    pres, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowP_VS_SI = eline01
    Else
        VBShowP_VS_SI = pres
    End If
      
    'Avoid the error handler
    Exit Function

ErrorVBShowP_VS_SI:
    
    VBShowP_VS_SI = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowP_VS_USCS(eosset As Integer, _
                        v As Double, s As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim pres As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowP_VS_USCS
    
    'Initialize local variables
    pres = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    EOSNamePlate.i = ShowP_VS_USCS(eosset, _
                    v, _
                    s, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    pres, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowP_VS_USCS = eline01
    Else
        VBShowP_VS_USCS = pres
    End If
      
    'Avoid the error handler
    Exit Function

ErrorVBShowP_VS_USCS:
    
    VBShowP_VS_USCS = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function


Function VBShowSo_SI(eosset As Integer, _
                        Temperature As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim s As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowSo_SI
    
    'Initialize local variables
    s = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    i = ShowSo_SI(eosset, _
                    Temperature, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    s, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowSo_SI = eline01
    Else
        VBShowSo_SI = s
    End If
      
    'Avoid the error handler
    Exit Function

ErrorVBShowSo_SI:
    
    VBShowSo_SI = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowSo_USCS(eosset As Integer, _
                        Temperature As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim s As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowSo_USCS
    
    'Initialize local variables
    s = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    i = ShowSo_USCS(eosset, _
                    Temperature, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    s, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowSo_USCS = eline01
    Else
        VBShowSo_USCS = s
    End If
      
    'Avoid the error handler
    Exit Function

ErrorVBShowSo_USCS:
    
    VBShowSo_USCS = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowT_HP_SI(eosset As Integer, _
                        Enthalpy As Double, Pressure As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim t As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowT_HP_SI
    
    'Initialize local variables
    t = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Check that the Error Tolerance is above minimum
    'Threshold
    If (ErrTolerance < 10) Then
        ErrTolerance = 10
    End If
     
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    EOSNamePlate.i = ShowT_HP_SI(eosset, _
                    Enthalpy, _
                    Pressure, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    t, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowT_HP_SI = eline01
    Else
        VBShowT_HP_SI = t
    End If
    
    'Avoid the error handler
    Exit Function

ErrorVBShowT_HP_SI:
    
    VBShowT_HP_SI = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowT_HP_USCS(eosset As Integer, _
                        Enthalpy As Double, Pressure As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim t As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowT_HP_USCS
    
    'Initialize local variables
    t = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Check that the Error Tolerance is above minimum
    'Threshold
    If (ErrTolerance < 10) Then
        ErrTolerance = 10
    End If
     
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    EOSNamePlate.i = ShowT_HP_USCS(eosset, _
                    Enthalpy, _
                    Pressure, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    t, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowT_HP_USCS = eline01
    Else
        VBShowT_HP_USCS = t
    End If
    
    'Avoid the error handler
    Exit Function

ErrorVBShowT_HP_USCS:
    
    VBShowT_HP_USCS = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowT_SP_SI(eosset As Integer, _
                        Entropy As Double, Pressure As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim t As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowT_SP_SI
    
    'Initialize local variables
    t = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Check that the Error Tolerance is above minimum
    'Threshold
    If (ErrTolerance < 10) Then
        ErrTolerance = 10
    End If
     
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    EOSNamePlate.i = ShowT_SP_SI(eosset, _
                    Entropy, _
                    Pressure, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    t, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowT_SP_SI = eline01
    Else
        VBShowT_SP_SI = t
    End If
    
    'Avoid the error handler
    Exit Function

ErrorVBShowT_SP_SI:
    
    VBShowT_SP_SI = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowT_SP_USCS(eosset As Integer, _
                        Entropy As Double, Pressure As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim t As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowT_SP_USCS
    
    'Initialize local variables
    t = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Check that the Error Tolerance is above minimum
    'Threshold
    If (ErrTolerance < 10) Then
        ErrTolerance = 10
    End If
     
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    EOSNamePlate.i = ShowT_SP_USCS(eosset, _
                    Entropy, _
                    Pressure, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    t, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowT_SP_USCS = eline01
    Else
        VBShowT_SP_USCS = t
    End If
    
    'Avoid the error handler
    Exit Function

ErrorVBShowT_SP_USCS:
    
    VBShowT_SP_USCS = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowV_TP_SI(eosset As Integer, _
                        Temperature As Double, Pressure As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim v As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowV_TP_SI
    
    'Initialize local variables
    v = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Check that the Error Tolerance is above minimum
    'Threshold
    If (ErrTolerance < 10) Then
        ErrTolerance = 10
    End If
     
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    EOSNamePlate.i = ShowV_TP_SI(eosset, _
                    Temperature, _
                    Pressure, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    v, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowV_TP_SI = eline01
    Else
        VBShowV_TP_SI = v
    End If
    
    'Avoid the error handler
    Exit Function

ErrorVBShowV_TP_SI:
    
    VBShowV_TP_SI = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowV_TP_USCS(eosset As Integer, _
                        Temperature As Double, Pressure As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim v As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowV_TP_USCS
    
    'Initialize local variables
    v = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Check that the Error Tolerance is above minimum
    'Threshold
    If (ErrTolerance < 10) Then
        ErrTolerance = 10
    End If
     
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    EOSNamePlate.i = ShowV_TP_USCS(eosset, _
                    Temperature, _
                    Pressure, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    v, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowV_TP_USCS = eline01
    Else
        VBShowV_TP_USCS = v
    End If
    
    'Avoid the error handler
    Exit Function

ErrorVBShowV_TP_USCS:
    
    VBShowV_TP_USCS = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowS_TP_SI(eosset As Integer, _
                        Temperature As Double, Pressure As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim s As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowS_TP_SI
    
    'Initialize local variables
    s = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Check that the Error Tolerance is above minimum
    'Threshold
    If (ErrTolerance < 10) Then
        ErrTolerance = 10
    End If
     
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    EOSNamePlate.i = ShowS_TP_SI(eosset, _
                    Temperature, _
                    Pressure, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    s, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowS_TP_SI = eline01
    Else
        VBShowS_TP_SI = s
    End If
    
    'Avoid the error handler
    Exit Function

ErrorVBShowS_TP_SI:
    
    VBShowS_TP_SI = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowS_TP_USCS(eosset As Integer, _
                        Temperature As Double, Pressure As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim s As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowS_TP_USCS
    
    'Initialize local variables
    s = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Check that the Error Tolerance is above minimum
    'Threshold
    If (ErrTolerance < 10) Then
        ErrTolerance = 10
    End If
     
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    EOSNamePlate.i = ShowS_TP_USCS(eosset, _
                    Temperature, _
                    Pressure, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    s, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowS_TP_USCS = eline01
    Else
        VBShowS_TP_USCS = s
    End If
    
    'Avoid the error handler
    Exit Function

ErrorVBShowS_TP_USCS:
    
    VBShowS_TP_USCS = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowH_TP_SI(eosset As Integer, _
                        Temperature As Double, Pressure As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim h As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowH_TP_SI
    
    'Initialize local variables
    h = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    EOSNamePlate.i = ShowH_TP_SI(eosset, _
                    Temperature, _
                    Pressure, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    h, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowH_TP_SI = eline01
    Else
        VBShowH_TP_SI = h
    End If
    
    'Avoid the error handler
    Exit Function

ErrorVBShowH_TP_SI:
    
    VBShowH_TP_SI = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowH_TP_USCS(eosset As Integer, _
                        Temperature As Double, Pressure As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim h As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowH_TP_USCS
    
    'Initialize local variables
    h = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    EOSNamePlate.i = ShowH_TP_USCS(eosset, _
                    Temperature, _
                    Pressure, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    h, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowH_TP_USCS = eline01
    Else
        VBShowH_TP_USCS = h
    End If
    
    'Avoid the error handler
    Exit Function

ErrorVBShowH_TP_USCS:
    
    VBShowH_TP_USCS = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowH_TS_SI(eosset As Integer, _
                        Temperature As Double, Entropy As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim h As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowH_TS_SI
    
    'Initialize local variables
    h = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    EOSNamePlate.i = ShowH_TS_SI(eosset, _
                    Temperature, _
                    Entropy, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    h, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowH_TS_SI = eline01
    Else
        VBShowH_TS_SI = h
    End If
    
    'Avoid the error handler
    Exit Function

ErrorVBShowH_TS_SI:
    
    VBShowH_TS_SI = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowH_TS_USCS(eosset As Integer, _
                        Temperature As Double, Entropy As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim h As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowH_TS_USCS
    
    'Initialize local variables
    h = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    EOSNamePlate.i = ShowH_TS_USCS(eosset, _
                    Temperature, _
                    Entropy, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    h, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowH_TS_USCS = eline01
    Else
        VBShowH_TS_USCS = h
    End If
    
    'Avoid the error handler
    Exit Function

ErrorVBShowH_TS_USCS:
    
    VBShowH_TS_USCS = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowF_TP_USCS(eosset As Integer, _
                        Temperature As Double, Pressure As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim f As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowF_TP_USCS
    
    'Initialize local variables
    f = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    EOSNamePlate.i = ShowF_TP_USCS(eosset, _
                    Temperature, _
                    Pressure, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    f, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowF_TP_USCS = eline01
    Else
        VBShowF_TP_USCS = f
    End If
    
    'Avoid the error handler
    Exit Function

ErrorVBShowF_TP_USCS:
    
    VBShowF_TP_USCS = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowVapTemp_P_USCS(eosset As Integer, _
                        Temperature As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim ts As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowVapTemp_P_USCS
    
    'Initialize local variables
    ts = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    i = ShowVapTemp_P_USCS(eosset, _
                    Temperature, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    ts, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowVapTemp_P_USCS = eline01
    Else
        VBShowVapTemp_P_USCS = ts
    End If
      
    'Avoid the error handler
    Exit Function

ErrorVBShowVapTemp_P_USCS:
    
    VBShowVapTemp_P_USCS = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowVapTemp_P_SI(eosset As Integer, _
                        Temperature As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim ts As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowVapTemp_P_SI
    
    'Initialize local variables
    ts = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    i = ShowVapTemp_P_SI(eosset, _
                    Temperature, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    ts, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowVapTemp_P_SI = eline01
    Else
        VBShowVapTemp_P_SI = ts
    End If
      
    'Avoid the error handler
    Exit Function

ErrorVBShowVapTemp_P_SI:
    
    VBShowVapTemp_P_SI = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowSfo_mx_SI(eosset As Integer, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim s As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowSfo_mx_SI
    
    'Initialize local variables
    s = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    EOSNamePlate.i = ShowSfo_mx_SI(eosset, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    s, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowSfo_mx_SI = eline01
    Else
        VBShowSfo_mx_SI = s
    End If
    
    'Avoid the error handler
    Exit Function

ErrorVBShowSfo_mx_SI:
    
    VBShowSfo_mx_SI = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowSfo_mx_USCS(eosset As Integer, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim s As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowSfo_mx_USCS
    
    'Initialize local variables
    s = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    EOSNamePlate.i = ShowSfo_mx_USCS(eosset, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    s, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowSfo_mx_USCS = eline01
    Else
        VBShowSfo_mx_USCS = s
    End If
    
    'Avoid the error handler
    Exit Function

ErrorVBShowSfo_mx_USCS:
    
    VBShowSfo_mx_USCS = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowSatLiqV_P_SI(eosset As Integer, _
                        Pressure As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim v As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowSatLiqV_P_SI
    
    'Initialize local variables
    v = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    i = ShowSatLiqV_P_SI(eosset, _
                    Pressure, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    v, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowSatLiqV_P_SI = eline01
    Else
        VBShowSatLiqV_P_SI = v
    End If
      
    'Avoid the error handler
    Exit Function

ErrorVBShowSatLiqV_P_SI:
    
    VBShowSatLiqV_P_SI = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowSatLiqV_P_USCS(eosset As Integer, _
                        Pressure As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim v As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowSatLiqV_P_USCS
    
    'Initialize local variables
    v = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    i = ShowSatLiqV_P_USCS(eosset, _
                    Pressure, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    v, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowSatLiqV_P_USCS = eline01
    Else
        VBShowSatLiqV_P_USCS = v
    End If
      
    'Avoid the error handler
    Exit Function

ErrorVBShowSatLiqV_P_USCS:
    
    VBShowSatLiqV_P_USCS = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowSatVapV_P_SI(eosset As Integer, _
                        Pressure As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim v As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowSatVapV_P_SI
    
    'Initialize local variables
    v = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    i = ShowSatVapV_P_SI(eosset, _
                    Pressure, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    v, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowSatVapV_P_SI = eline01
    Else
        VBShowSatVapV_P_SI = v
    End If
      
    'Avoid the error handler
    Exit Function

ErrorVBShowSatVapV_P_SI:
    
    VBShowSatVapV_P_SI = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowSatVapV_P_USCS(eosset As Integer, _
                        Pressure As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim v As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowSatVapV_P_USCS
    
    'Initialize local variables
    v = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    i = ShowSatVapV_P_USCS(eosset, _
                    Pressure, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    v, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowSatVapV_P_USCS = eline01
    Else
        VBShowSatVapV_P_USCS = v
    End If
      
    'Avoid the error handler
    Exit Function

ErrorVBShowSatVapV_P_USCS:
    
    VBShowSatVapV_P_USCS = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowSatLiqH_P_SI(eosset As Integer, _
                        Pressure As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim h As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowSatLiqH_P_SI
    
    'Initialize local variables
    h = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    i = ShowSatLiqH_P_SI(eosset, _
                    Pressure, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    h, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowSatLiqH_P_SI = eline01
    Else
        VBShowSatLiqH_P_SI = h
    End If
      
    'Avoid the error handler
    Exit Function

ErrorVBShowSatLiqH_P_SI:
    
    VBShowSatLiqH_P_SI = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowSatLiqH_P_USCS(eosset As Integer, _
                        Pressure As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim h As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowSatLiqH_P_USCS
    
    'Initialize local variables
    h = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    i = ShowSatLiqH_P_USCS(eosset, _
                    Pressure, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    h, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowSatLiqH_P_USCS = eline01
    Else
        VBShowSatLiqH_P_USCS = h
    End If
      
    'Avoid the error handler
    Exit Function

ErrorVBShowSatLiqH_P_USCS:
    
    VBShowSatLiqH_P_USCS = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowSatVapH_P_SI(eosset As Integer, _
                        Pressure As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim h As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowSatVapH_P_SI
    
    'Initialize local variables
    h = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    i = ShowSatVapH_P_SI(eosset, _
                    Pressure, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    h, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowSatVapH_P_SI = eline01
    Else
        VBShowSatVapH_P_SI = h
    End If
      
    'Avoid the error handler
    Exit Function

ErrorVBShowSatVapH_P_SI:
    
    VBShowSatVapH_P_SI = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowSatVapH_P_USCS(eosset As Integer, _
                        Pressure As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim h As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowSatVapH_P_USCS
    
    'Initialize local variables
    h = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    i = ShowSatVapH_P_USCS(eosset, _
                    Pressure, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    h, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowSatVapH_P_USCS = eline01
    Else
        VBShowSatVapH_P_USCS = h
    End If
      
    'Avoid the error handler
    Exit Function

ErrorVBShowSatVapH_P_USCS:
    
    VBShowSatVapH_P_USCS = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowSatLiqS_P_SI(eosset As Integer, _
                        Pressure As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim s As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowSatLiqS_P_SI
    
    'Initialize local variables
    s = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    i = ShowSatLiqS_P_SI(eosset, _
                    Pressure, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    s, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowSatLiqS_P_SI = eline01
    Else
        VBShowSatLiqS_P_SI = s
    End If
      
    'Avoid the error handler
    Exit Function

ErrorVBShowSatLiqS_P_SI:
    
    VBShowSatLiqS_P_SI = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowSatLiqS_P_USCS(eosset As Integer, _
                        Pressure As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim s As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowSatLiqS_P_USCS
    
    'Initialize local variables
    s = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    i = ShowSatLiqS_P_USCS(eosset, _
                    Pressure, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    s, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowSatLiqS_P_USCS = eline01
    Else
        VBShowSatLiqS_P_USCS = s
    End If
      
    'Avoid the error handler
    Exit Function

ErrorVBShowSatLiqS_P_USCS:
    
    VBShowSatLiqS_P_USCS = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowSatVapS_P_SI(eosset As Integer, _
                        Pressure As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim s As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowSatVapS_P_SI
    
    'Initialize local variables
    s = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    i = ShowSatVapS_P_SI(eosset, _
                    Pressure, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    s, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowSatVapS_P_SI = eline01
    Else
        VBShowSatVapS_P_SI = s
    End If
      
    'Avoid the error handler
    Exit Function

ErrorVBShowSatVapS_P_SI:
    
    VBShowSatVapS_P_SI = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowSatVapS_P_USCS(eosset As Integer, _
                        Pressure As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim s As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowSatVapS_P_USCS
    
    'Initialize local variables
    s = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    i = ShowSatVapS_P_USCS(eosset, _
                    Pressure, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    s, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowSatVapS_P_USCS = eline01
    Else
        VBShowSatVapS_P_USCS = s
    End If
      
    'Avoid the error handler
    Exit Function

ErrorVBShowSatVapS_P_USCS:
    
    VBShowSatVapS_P_USCS = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function
Function VBShowViscosityGas_TP_SI(eosset As Integer, _
                        Temperature As Double, _
                        Pressure As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim v As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowViscosityGas_TP_SI
    
    'Initialize local variables
    v = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    i = ShowViscosityGas_TP_SI(eosset, _
                    Temperature, _
                    Pressure, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    v, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowViscosityGas_TP_SI = eline01
    Else
        VBShowViscosityGas_TP_SI = v
    End If
      
    'Avoid the error handler
    Exit Function

ErrorVBShowViscosityGas_TP_SI:
    
    VBShowViscosityGas_TP_SI = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function

Function VBShowViscosityGas_TP_USCS(eosset As Integer, _
                        Temperature As Double, _
                        Pressure As Double, _
                        Precision As Double, MaxIterations As Double, _
                        PercentMethane As Double, PercentEthane As Double, _
                        PercentPropane As Double, PercentI_Butane As Double, _
                        PercentN_Butane As Double, PercentI_Pentane As Double, _
                        PercentN_Pentane As Double, PercentN_Hexane As Double, _
                        PercentN_Heptane As Double, _
                        PercentN_Octane As Double, _
                        PercentEthylene As Double, _
                        PercentPropylene As Double, _
                        PercentCarbonDioxide As Double, _
                        PercentHydrogenSulfide As Double, _
                        PercentNitrogen As Double, _
                        PercentHydrogen As Double, _
                        PercentAmmonia As Double, _
                        PercentWater As Double, _
                        PercentAir As Double, _
                        PercentCarbonMonoxide As Double, _
                        PercentArgon As Double, _
                        PercentOxygen As Double, _
                        PercentSulfDiox As Double, _
                        PercentR134a As Double, _
                        PercentSilane As Double)

    'Local variables
    Dim v As Double
    Dim MixingArray(25) As Double
    Dim ErrTolerance As Integer
    
    'Establish error trapping
    On Error GoTo ErrorVBShowViscosityGas_TP_USCS
    
    'Initialize local variables
    v = 0
    priority01 = 0
    ErrTolerance = 10
    
    'Begin by filling the mixing array with these values
    MixingArray(0) = PercentMethane
    MixingArray(1) = PercentEthane
    MixingArray(2) = PercentPropane
    MixingArray(3) = PercentI_Butane
    MixingArray(4) = PercentN_Butane
    MixingArray(5) = PercentI_Pentane
    MixingArray(6) = PercentN_Pentane
    MixingArray(7) = PercentN_Hexane
    MixingArray(8) = PercentN_Heptane
    MixingArray(9) = PercentN_Octane
    MixingArray(10) = PercentEthylene
    MixingArray(11) = PercentPropylene
    MixingArray(12) = PercentCarbonDioxide
    MixingArray(13) = PercentHydrogenSulfide
    MixingArray(14) = PercentNitrogen
    MixingArray(15) = PercentHydrogen
    MixingArray(16) = PercentAmmonia
    MixingArray(17) = PercentWater
    MixingArray(18) = PercentAir
    MixingArray(19) = PercentCarbonMonoxide
    MixingArray(20) = PercentArgon
    MixingArray(21) = PercentOxygen
    MixingArray(22) = PercentSulfDiox
    MixingArray(23) = PercentR134a
    MixingArray(24) = PercentSilane
    
    'If we need to we can check the return value
    'to see if there was an error
    i = ShowViscosityGas_TP_USCS(eosset, _
                    Temperature, _
                    Pressure, _
                    MixingArray(0), _
                    Precision, _
                    MaxIterations, _
                    v, _
                    priority01, _
                    eline01)
    
    'return the value
    If ((priority01 > 0) And (priority01 <= ErrTolerance)) Then
        VBShowViscosityGas_TP_USCS = eline01
    Else
        VBShowViscosityGas_TP_USCS = v
    End If
      
    'Avoid the error handler
    Exit Function

ErrorVBShowViscosityGas_TP_USCS:
    
    VBShowViscosityGas_TP_USCS = (Str(Err.Number) & ":" & Err.Description)
    Exit Function

End Function



