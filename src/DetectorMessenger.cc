#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"
//#include "AnalysisManager.hh"
#include "G4RootAnalysisManager.hh"
#include "GaramGlobal.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4NistManager.hh"
#include "G4VPhysicalVolume.hh"

using namespace std;

DetectorMessenger::DetectorMessenger()
{

    // directories
    det = new G4UIdirectory("/det/");
    det->SetGuidance("detector dir");

    // parameters

    electrode_width = new G4UIcmdWithADoubleAndUnit("/det/electrode/width", this);
    electrode_width->SetGuidance("length");
    electrode_width->SetDefaultUnit("mm");
    electrode_width->SetUnitCandidates("um mm cm m");
    electrode_width->AvailableForStates(G4State_PreInit);

    electrode_length = new G4UIcmdWithADoubleAndUnit("/det/electrode/length", this);
    electrode_length->SetGuidance("length");
    electrode_length->SetDefaultUnit("mm");
    electrode_length->SetUnitCandidates("um mm cm m");
    electrode_length->AvailableForStates(G4State_PreInit);

    volume_Efield = new G4UIcmdWithADouble("/det/Efield", this);
    volume_Efield->SetGuidance("E-field(kV)");
    volume_Efield->AvailableForStates(G4State_PreInit);

}


DetectorMessenger::~DetectorMessenger()
{
    delete det;
    delete electrode_width;
    delete electrode_length;
    delete volume_Efield;

}


void DetectorMessenger::SetNewValue(G4UIcommand* icmd, G4String istr)
{
    if (icmd == electrode_width)
    {
        tmp_width = electrode_width->GetNewDoubleValue(istr);
    }
    else if (icmd == electrode_length)
    {
        tmp_length = electrode_length->GetNewDoubleValue(istr);
    }
    else if (icmd == volume_Efield)
    {
        tmp_Efield = volume_Efield->GetNewDoubleValue(istr);
    }

}

G4double DetectorMessenger::GetElectrodeWidth() const { return tmp_width; } // 0711

G4double DetectorMessenger::GetElectrodeLength() const { return tmp_length; } // 0711

G4double DetectorMessenger::GetEfield() const { return tmp_Efield; } // 0711