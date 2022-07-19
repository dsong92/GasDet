#ifndef PRIMARYGENERATORMESSENGER_HH
#define PRIMARYGENERATORMESSENGER_HH 1

#include "PrimaryGeneratorAction.hh"
#include "G4UImessenger.hh"
#include "globals.hh"
#include "BeamInfo.hh"
#include "GaramGlobal.hh"

class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADouble;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3Vector;

class PrimaryGeneratorMessenger : public G4UImessenger
{
public:
    PrimaryGeneratorMessenger(PrimaryGeneratorAction* ipga);
    ~PrimaryGeneratorMessenger();

public:
    void SetNewValue(G4UIcommand* icmd, G4String istr);
    const BeamInfo& GetBeamInfo();

    std::vector<garam::particle_io>* GetPdist() const;
    G4double GetBeamShift() const;
    G4double GetBeamHeight() const;

private:
    PrimaryGeneratorAction*    ga;
    BeamInfo                   binfo;

    G4UIdirectory*             beam;
    G4UIcmdWithAString*        beam_pdist;

    G4UIcmdWithAString*        beam_pencil;

    G4UIdirectory*             beam_gauss;
    G4UIcmdWithADoubleAndUnit*        beam_shift; //0711
    G4UIcmdWithADoubleAndUnit*        beam_height; //0711

    G4UIcmdWithAString*        beam_gauss_xe;
    G4UIcmdWithAString*        beam_gauss_ye;

    G4UIcmdWith3Vector*        beam_gauss_xabe;
    G4UIcmdWith3Vector*        beam_gauss_yabe;

    G4UIcmdWithADoubleAndUnit* beam_energy;
    G4UIcmdWithADouble*        beam_espread;
    G4UIcmdWithADouble*        beam_mspread;
    G4UIcmdWithAnInteger*      beam_rseed;

    G4UIdirectory*             pgm;
    G4UIcmdWithAnInteger*      pgm_book;

    std::vector<garam::particle_io>* pts;
    G4double                   tmp_shift; //0711
    G4double                   tmp_height; //0711

};


#endif // PRIMARYGENERATORMESSENGER_HH
