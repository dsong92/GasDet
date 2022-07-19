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
/// \file EventAction.hh
/// \brief Definition of the EventAction class

#ifndef EventAction_h
#define EventAction_h 1

#include "G4ThreeVector.hh"
#include "G4UserEventAction.hh"
#include "globals.hh"
#include "GaramGlobal.hh"
#include <vector>

class RunAction;

/// Event action class
///

class EventAction : public G4UserEventAction
{
public:
	EventAction(RunAction* runAction);
	virtual ~EventAction();

	virtual void BeginOfEventAction(const G4Event* event);
	virtual void EndOfEventAction(const G4Event* event);
	virtual void WriteParticle(string file_name, garam::particle_io* p);
	// write particle is inserted at 07-05

	G4double pos_x_A; // photo-electron이 electrode A에 도달했을 떄 x posotion
	G4double pos_x_B;
	G4double pos_y_A;
	G4double pos_y_B;
	G4double pos_z_A;
	G4double pos_z_B;
	G4double pos_x_BC_IN; // BC : beam check
	G4double pos_y_BC_IN;
	G4double pos_x_BC_OUT; // BC : beam check
	G4double pos_y_BC_OUT;
	G4double Energy_A;
	G4double Energy_B;
	G4double Energy_BC_IN;
	G4double Energy_BC_OUT;
	G4double Energy_Monitor;
	G4double Phot_Elec_Energy;
	G4double Phot_in_pipe;
	G4double Phi;
	G4double Theta;
	G4double px;
	G4double py;
	G4double pz;
	G4int    TrackID_A;
	G4int    TrackID_B;
	G4int    TrackID_BC;
	G4int    TrackID_Pipe;
	
private:
	RunAction* fRunAction;
	G4int fCollectionID_Electrode_A;
	G4int fCollectionID_Electrode_B;
	G4int fCollectionID_BC_IN;
	G4int fCollectionID_BC_OUT;
	G4int fCollectionID_Pipe;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


