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
/// \file SteppingAction.cc
/// \brief Implementation of the SteppingAction class

#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4UserRunAction.hh"
#include "g4root.hh"
#include "G4UnitsTable.hh"

#include <stdio.h>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(EventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction)
  //fScoringVolume(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* step)
{
/*
  if(step->GetTrack()->GetTrackID() == 1 ) return;

	//G4int PID, TID;
	const G4ParticleDefinition* particle = step->GetTrack()->GetParticleDefinition();

	G4String PreStepVol  = step->GetPreStepPoint()->GetTouchable()->GetVolume()->GetName();
	if(PreStepVol=="World") return; // If PreStepPoint is World then track is disappeared so volume of poststep is null
	G4String PostStepVol = step->GetPostStepPoint()->GetTouchable()->GetVolume()->GetName();
 

	// only for electron, counting [PreStep : chamber -> PostStep : Electrode] or [PreStep : Electrode -> PostStep : Chamber]
	if( particle->GetParticleName() == "e-"){
			if( PreStepVol == "Chamber" ){
					if( PostStepVol == "Electrode" ){
							fEventAction->CountElecIn(1); // counting number of electron
							fEventAction->Push_Position_Electrode_In(step->GetPreStepPoint()->GetPosition()); // Push_back position vector
							fEventAction->EkinElectronIn(step->GetPostStepPoint()->GetKineticEnergy()); // sum of Ekin of electron and devide by number of electron
					}
					if( PostStepVol == "Envelope" ){
							fEventAction->Push_Position_Touch_Frame(step->GetPostStepPoint()->GetPosition()); // Push_back position vector
					}
			}
			if( PreStepVol == "Electrode" && PostStepVol == "Chamber" && step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() == "Transportation"){
					fEventAction->CountElecOut(1);
					fEventAction->EkinElectronOut(step->GetPostStepPoint()->GetKineticEnergy());
			}
	}
*/
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

