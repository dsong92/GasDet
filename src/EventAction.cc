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
/// \file EventAction.cc
/// \brief Implementation of the EventAction class

#include "EventAction.hh"
#include "RunAction.hh"
#include "HistoManager.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4ThreeVector.hh"

#include "EssHit.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"

#include "G4RootAnalysisManager.hh"
#include "G4UnitsTable.hh"


#include <vector>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(RunAction* runAction)
	: G4UserEventAction(),
	fRunAction(runAction),
	fCollectionID_A(-1),
	fCollectionID_B(-1),
	fCollectionID_BC(-1),
	fCollectionID_Pipe(-1),
	pos_x_A(-999), pos_x_B(-999), pos_z_A(-999), pos_z_B(-999), px(-999), py(-999), pz(-999),
	Energy_A(-999), Energy_B(-999), Phot_Elec_Energy(-999), Phi(-999), Theta(-999), TrackID_A(-1), TrackID_B(-1)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event*)
{
	//pos_x_A = -999; pos_x_B = -999; pos_z_A = -999; pos_z_B = -999;
	//Energy_A = -999; Energy_B = -999; Phot_Elec_Energy = -999; Phi = -999; Theta = -999;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventAction::EndOfEventAction(const G4Event* evt)
{
	G4HCofThisEvent* hcte = evt->GetHCofThisEvent();
	if (!hcte) return;

	//G4AnalysisManager* AnalysisManager = G4AnalysisManager::Instance();
	G4RootAnalysisManager* AnalysisManager = G4RootAnalysisManager::Instance();

	G4SDManager* SDMan = G4SDManager::GetSDMpointer();
	fCollectionID_A = SDMan->GetCollectionID("ElectrodeSD_A");
	fCollectionID_B = SDMan->GetCollectionID("ElectrodeSD_B");
	fCollectionID_BC = SDMan->GetCollectionID("BeamCheckSD");
	fCollectionID_Pipe = SDMan->GetCollectionID("SDForElectron");
	//AnalysisManager->FillNtupleIColumn(6, evt->GetEventID());

	// Hit Collection in BeamCheck, for chekcing incident beam information
	EssHitsCollection* hc_BC = 0;
	hc_BC = (EssHitsCollection*)hcte->GetHC(fCollectionID_BC);
	int nHits_BC = hc_BC->entries();
	for (G4int j = 0; j < nHits_BC; ++j)
	{
		EssHit* hit = (*hc_BC)[j];
		G4int PID = hit->GetParticleId();
		//G4bool FirstStepInVolume = hit->GetFlag();
		TrackID_BC = hit->GetTrackId();

		// The surface of electrode start y = -15 to -16 ,
		if (PID == 22 && TrackID_BC == 1) {
			G4ThreeVector pos = hit->GetPosition();
			pos_x_BC = pos.x();
			pos_y_BC = pos.y();
			Energy_BC = hit->GetKineticEnergy();

			//AnalysisManager->FillH1(5, Energy_BC);
			AnalysisManager->FillH1(6, pos_x_BC);
			AnalysisManager->FillH1(7, pos_y_BC);

		}
	}


	// Hit Collection in Electrode_A
	EssHitsCollection* hc_A = 0;
	hc_A = (EssHitsCollection*)hcte->GetHC(fCollectionID_A);
	int nHits_A = hc_A->entries();
	for (G4int j = 0; j < nHits_A; ++j)
	{
		EssHit* hit = (*hc_A)[j];
		G4int PID = hit->GetParticleId();
		G4ThreeVector pos = hit->GetPosition();
		//G4bool FirstStepInVolume = hit->GetFlag();
		//TrackID_A = hit->GetTrackId();

		// The surface of electrode start y = -15 to -16 ,
		if (PID == 11 && pos.y() == -15) {

			pos_x_A = pos.x();
			pos_z_A = pos.z();
			Energy_A = hit->GetKineticEnergy();

			//G4cout << "-------------------------------------------------------------" << G4endl;
			//G4cout << "Position from Track     --> " << hit->GetPositionT() << G4endl;
			//G4cout << "Position from StepPoint --> " << hit->GetPosition() << G4endl;
			//G4cout << "Energy from Track --> " << hit->GetKineticEnergyT() << G4endl;
			//G4cout << "Energy from StepPoint --> " << hit->GetKineticEnergy() << G4endl;

			AnalysisManager->FillH1(0, pos_x_A);
			AnalysisManager->FillH1(2, pos_z_A);
			AnalysisManager->FillH1(4, Energy_A);
			AnalysisManager->FillH1(8, pos_x_A);
			AnalysisManager->FillH1(9, pos_z_A);

			AnalysisManager->FillH2(0, pos_x_A, pos_z_A);
			AnalysisManager->FillH2(1, pos_x_A, pos_z_A);

			AnalysisManager->FillNtupleDColumn(0, 0, pos_x_A);
			AnalysisManager->FillNtupleDColumn(0, 1, pos_z_A);
			AnalysisManager->AddNtupleRow(0);
		}
	}

	// Hit Collection in Electrode_B
	EssHitsCollection* hc_B = 0;
	hc_B = (EssHitsCollection*)hcte->GetHC(fCollectionID_B);
	int nHits_B = hc_B->entries();
	for (G4int j = 0; j < nHits_B; ++j)
	{
		EssHit* hit = (*hc_B)[j];
		G4int PID = hit->GetParticleId();
		G4ThreeVector pos = hit->GetPosition();
		//G4bool FirstStepInVolume = hit->GetFlag();
		//TrackID_B = hit->GetTrackId();

		if (PID == 11 && pos.y() == -15) {

			pos_x_B = pos.x();
			pos_z_B = pos.z();
			Energy_B = hit->GetKineticEnergy();

			AnalysisManager->FillH1(1, pos_x_B);
			AnalysisManager->FillH1(3, pos_z_B);
			AnalysisManager->FillH1(4, Energy_B);
			AnalysisManager->FillH1(8, pos_x_B);
			AnalysisManager->FillH1(9, pos_z_B);

			AnalysisManager->FillH2(0, pos_x_B, pos_z_B);
			AnalysisManager->FillH2(2, pos_x_B, pos_z_B);

			AnalysisManager->FillNtupleDColumn(1, 0, pos_x_B);
			AnalysisManager->FillNtupleDColumn(1, 1, pos_z_B);
			AnalysisManager->AddNtupleRow(1);
		}
	}

	// Hit Collection in pipe, pipe is thin and long sensitive detector having same material to chamber. 
	EssHitsCollection* hc_Pipe = 0;
	hc_Pipe = (EssHitsCollection*)hcte->GetHC(fCollectionID_Pipe);
	int nHits_Pipe = hc_Pipe->entries();
	for (G4int j = 0; j < nHits_Pipe; ++j)
	{
		EssHit* hit = (*hc_Pipe)[j];
		G4ThreeVector pos = hit->GetPosition();
		G4int PID = hit->GetParticleId();
		TrackID_Pipe = hit->GetTrackId();
		//G4bool FirstStepInVolume = hit->GetFlag();
		//G4int copynumber = hit->GetCopyNumber();

		if (PID == 11 && TrackID_Pipe == 1) {

			//Phot_Elec_Energy = hit->GetPhotoElecEnergy();
			Phot_Elec_Energy = hit->GetKineticEnergy();
			Phi = hit->GetMomentumDirection().getPhi() * (180 / CLHEP::pi);
			Theta = hit->GetMomentumDirection().getTheta() * (180 / CLHEP::pi);
			px = hit->GetMomentum().x();
			py = hit->GetMomentum().y();
			pz = hit->GetMomentum().z();

			AnalysisManager->FillH1(5, Phot_Elec_Energy);
			AnalysisManager->FillH1(10, Phot_Elec_Energy);
			AnalysisManager->FillH1(11, Phi);
			AnalysisManager->FillH1(14, pos.z());
			AnalysisManager->FillH2(3, Phi, Phot_Elec_Energy);
			AnalysisManager->FillH2(4, Theta, Phot_Elec_Energy);
			AnalysisManager->FillH2(5, Theta, Phi);


			AnalysisManager->FillNtupleDColumn(2, 0, Phot_Elec_Energy);
			//AnalysisManager->FillNtupleDColumn(2, 1, px);
			//AnalysisManager->FillNtupleDColumn(2, 2, py);
			//AnalysisManager->FillNtupleDColumn(2, 3, pz);
			//AnalysisManager->FillNtupleDColumn(2, 4, Phi);
			//AnalysisManager->FillNtupleDColumn(2, 6, pos.z());

			G4double pt = std::sqrt((px * px) + (py * py));
			AnalysisManager->FillNtupleDColumn(2, 5, pt);
			AnalysisManager->AddNtupleRow(2);
		}
	}

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......