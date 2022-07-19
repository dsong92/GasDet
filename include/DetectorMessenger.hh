#ifndef DETECTORMESSENGER_HH_
#define DETECTORMESSENGER_HH_

#include "G4UImessenger.hh"
#include "globals.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "ElectricFieldSetup.hh"
//#include "SingleLayer.hh"

class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAnInteger;
class G4VPhysicalVolume;
class G4UIcmdWithADouble;

class geom;

class DetectorMessenger : public G4UImessenger
{
public:
    DetectorMessenger();
    ~DetectorMessenger();

private:
    G4UIdirectory*              det;

    //G4double                    electrodeW;
    G4UIcmdWithADoubleAndUnit*  electrode_width;

    //G4double                    electrodeL;
    G4UIcmdWithADoubleAndUnit*  electrode_length;

    //G4double                    Efield;
    G4UIcmdWithADouble*         volume_Efield;

    G4VPhysicalVolume*          p_world;

public:
    G4double                    tmp_width;
    G4double                    tmp_length;
    G4double                    tmp_Efield;


public:
    void SetNewValue(G4UIcommand* icmd, G4String istr);
    //TargetLayers& GetTargetLayers();
    G4VPhysicalVolume* GetWorldVolume();
 
    G4double GetElectrodeWidth() const;
    G4double GetElectrodeLength() const;
    G4double GetEfield() const;

};

#endif /* DETECTORMESSENGER_HH_ */

