//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file PrimaryGeneratorAction.cc
/// \brief Implementation of the PrimaryGeneratorAction class

#include "PrimaryGeneratorAction.hh"
#include "PrimaryGeneratorMessenger.hh"

#include "G4GeneralParticleSource.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4UIdirectory.hh"
#include "G4IonTable.hh"
#include "G4UnitsTable.hh"
#include "G4RootAnalysisManager.hh"

#include "globals.hh"
#include "G4ParticleTable.hh"

#include "GaramGlobal.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction():
    tmp_x(-999), tmp_y(-999)
	//: G4VUserPrimaryGeneratorAction(),
	//fParticleGun(0)
	//fEnvelopeBox(0),X0(0.),Y0(0.),Z0(0.),SigmaX(0.),SigmaY(0.)
{
	gm = new PrimaryGeneratorMessenger(this);
	gun = new G4ParticleGun(1);
	//fParticleGun = new G4GeneralParticleSource();
	
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PrimaryGeneratorAction::BookHisto()
{
    if (gm->GetPdist()->size() == 0)
    {
        const BeamInfo b = gm->GetBeamInfo();
        const G4double xm = b.X() * 3.0;
        const G4double xpm = b.Xp() * 3.0;
        const G4double ym = b.Y() * 3.0;
        const G4double ypm = b.Yp() * 3.0;
        const G4double w = b.MeanEnergy();
        G4double dw = b.EsRatio() * b.MeanEnergy() * 3.0;
        if (dw == 0.)
        {
            dw = w * 0.05;
        }

        G4RootAnalysisManager* ra = G4RootAnalysisManager::Instance();
        id_xbeamh2 =
            ra->CreateH2("ibeam_xphase", "Input Beam Information - X-X'",
                100, -xm, xm,
                100, -xpm, xpm);

        id_ybeamh2 =
            ra->CreateH2("ibeam_yphase", "Input Beam Information - Y-Y'",
                100, -ym, ym,
                100, -ypm, ypm);

        id_energyh1 =
            ra->CreateH1("ibeam_wdist", "Input Beam Information - Kinetic Energy",
                100, w - dw, w + dw);
    }


}



PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
	delete gm;
	delete gun;
	//delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    //G4double half_world = 0.5 * myg->get_length_sum();

    G4double xi = 0;                   // mm
    G4double xpi = 0; // mrad
    G4double yi = 0;                   // mm
    G4double ypi = 0; // mard
    G4double zi = 0;
    G4double zpi = 0;
    G4double wi = 1;  // mev/amu

    if ( gm->GetPdist()->size() == 0)
    {
        //const BeamInfo& b = gm->GetBeamInfo();

        //xi = G4RandGauss::shoot(0.0, b.X());                   // mm
        //xpi = G4RandGauss::shoot(0.0, b.Xp()) + b.Slopx() * xi; // mrad
        //yi = G4RandGauss::shoot(0.0, b.Y());                   // mm
        //ypi = G4RandGauss::shoot(0.0, b.Yp()) + b.Slopy() * yi; // mard
        //wi = G4RandGauss::shoot(b.MeanEnergy(), b.MeanEnergy() * b.EsRatio());  // mev/amu
        G4cout << "*** gun is empty ****" << G4endl;

    }
    else
    {
        std::vector<garam::particle_io>* particles = gm->GetPdist();
        std::vector<garam::particle_io>::iterator pit = particles->begin();
        pit += anEvent->GetEventID();

        G4ParticleTable* ptab = G4ParticleTable::GetParticleTable();
        G4ParticleDefinition* pdef;
        if ((*pit).is_ion())
        {
        //    pdef = ptab->GetIonTable()->GetIon((*pit).get_Z(), (*pit).get_A(), 0, 0);
        }
        else // elementary particle (has pid)
        {
            pdef = ptab->FindParticle((*pit).get_pid());
        }
        gun->SetParticleDefinition(pdef);
        
        xi = (*pit).get_x(); // mm
        yi = (*pit).get_y(); // mm
        zi = (*pit).get_z(); // mm
        xpi = (*pit).get_xp();
        ypi = (*pit).get_yp();
        zpi = (*pit).get_zp();
        wi = (*pit).get_w(); // MeV 
    }
    //gun->SetParticlePosition(G4ThreeVector(xi, yi, -149));
    
    tmp_x = xi + gm->GetBeamShift();
    tmp_y = yi + gm->GetBeamHeight();
    //gun->SetParticlePosition(G4ThreeVector(xi+gm->GetBeamShift(), yi+gm->GetBeamHeight(), zi));
    gun->SetParticlePosition(G4ThreeVector(tmp_x, tmp_y, zi));
    //std::cout << "xi+ gm->GetBeamShift() : " << xi + gm->GetBeamShift() << std::endl; // 0711

    //gun->SetParticleMomentumDirection(G4ThreeVector(0, 0, 1.).unit());
    gun->SetParticleMomentumDirection(G4ThreeVector(xpi, ypi, zpi).unit());
    gun->SetParticleEnergy(wi);
    gun->GeneratePrimaryVertex(anEvent); 
    //G4cout << "X : " << xi << " || Y : " << yi << " || Z : " << zi <<" || Energy : "<< G4BestUnit(wi,"Energy") << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

