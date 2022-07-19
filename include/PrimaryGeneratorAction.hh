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
/// \file PrimaryGeneratorAction.hh
/// \brief Definition of the PrimaryGeneratorAction class

#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"
#include "G4Event.hh"
#include "GaramGlobal.hh"
#include "Beaminfo.hh"

class G4ParticleGun;
class G4Event;
class DetectorConstruction;
class G4ParticleDefinition;
class G4RootAnalysisManager;
class G4GeneralParticleSource;
class PrimaryGeneratorMessenger;
class geom; // **** 0701 *******

/// The primary generator action class with particle gun.
///
/// The default kinematic is a 6 MeV gamma, randomly distribued 
/// in front of the phantom across 80% of the (X,Y) phantom size.

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
	PrimaryGeneratorAction();
	virtual ~PrimaryGeneratorAction();

public:
	void GeneratePrimaries(G4Event*);
	void BookHisto();
	PrimaryGeneratorMessenger* GetPGM() const { return gm; };


	
	void SetGeom(geom* ig) { myg = ig; }; // **** 0701 *******
	geom* GetGeom() const { return myg; }; // **** 0701 *******

private:
	G4ParticleGun*				 gun;
	PrimaryGeneratorMessenger*   gm;
	DetectorConstruction*        fDetConstruction;    //pointer to the geometry
	G4GeneralParticleSource*     fParticleGun;

	G4int                      id_xbeamh2;
	G4int                      id_ybeamh2;
	G4int                      id_energyh1;

	G4double				   tmp_x;
	G4double				   tmp_y;

	geom* myg; // **** 0701 *******
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
