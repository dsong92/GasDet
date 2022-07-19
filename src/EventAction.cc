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

#include "GaramGlobal.hh"
#include "G4String.hh"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cmath>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(RunAction* runAction)
	: G4UserEventAction(),
	fRunAction(runAction),
	fCollectionID_Electrode_A(-1),
	fCollectionID_Electrode_B(-1),
	fCollectionID_BC_IN(-1), fCollectionID_BC_OUT(-1),
	fCollectionID_Pipe(-1),
	pos_x_A(-999), pos_x_B(-999), pos_y_A(-999), pos_y_B(-999), pos_z_A(-999), pos_z_B(-999), px(-999), py(-999), pz(-999), pos_x_BC_IN(-999), pos_y_BC_IN(-999), pos_x_BC_OUT(-999), pos_y_BC_OUT(-999),
	Energy_A(-999), Energy_B(-999), Phot_Elec_Energy(-999), Phi(-999), Theta(-999), TrackID_A(-1), TrackID_B(-1), Phot_in_pipe(-999)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* evt)
{
	if (evt->GetEventID() % 10000 == 0)
		G4cout << "event: " << evt->GetEventID() << G4endl;
}

void EventAction::WriteParticle(string file_name, garam::particle_io *p)
{
	/*
	// add 0705
	G4int HEADER_SIZE = 1024;
	G4String STRUCTURE = "iddddddd";
	G4int SIZE_OF_INT = 4;
	G4int SIZE_OF_DOUBLE = 8;
	G4int SIZE_OF_STRUCTURE = SIZE_OF_INT * 1 + SIZE_OF_DOUBLE * 7;

	G4int NI__ = HEADER_SIZE / SIZE_OF_INT;


	int counter = 0;
	std::ofstream fout;
	fout.open(file_name);
	fout.write( (char*)&p , sizeof(garam::particle_io));
	fout.close();
	*/
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventAction::EndOfEventAction(const G4Event* evt)
{
	//G4cout << evt->GetEventID() << G4endl;
	G4RootAnalysisManager* AnalysisManager = G4RootAnalysisManager::Instance();

	G4SDManager* SDMan = G4SDManager::GetSDMpointer();
	fCollectionID_Pipe = SDMan->GetCollectionID("SDForElectron");
	fCollectionID_BC_IN = SDMan->GetCollectionID("BeamCheckIN");
	fCollectionID_Electrode_A = SDMan->GetCollectionID("ElectrodeSD_A");
	fCollectionID_Electrode_B = SDMan->GetCollectionID("ElectrodeSD_B");
	G4HCofThisEvent* hcte = evt->GetHCofThisEvent();
	if (!hcte) return;

	EssHitsCollection* hc_Pipe = 0;
	hc_Pipe = (EssHitsCollection*)hcte->GetHC(fCollectionID_Pipe);
	int nHits_Pipe = hc_Pipe->entries();
	for (G4int j = 0; j < nHits_Pipe; ++j) {
		EssHit* hit = (*hc_Pipe)[j];
		G4int PID = hit->GetParticleId();
		TrackID_Pipe = hit->GetTrackId();

		if (PID == 11) {
			//if (TrackID_Pipe == 1) {
			//	Phot_Elec_Energy = hit->GetKineticEnergy();
			//}
			if (TrackID_Pipe == 1 && hit->GetCurrentStepNum() == 1) {
				//G4int EvtID = evt->GetEventID();
				G4ThreeVector posElec = hit->GetPosition();
				G4ThreeVector momdirecElec = hit->GetMomentumDirection();
				G4double ElectronEnergy = hit->GetKineticEnergy();

				AnalysisManager->FillNtupleDColumn(0, posElec.x());
				AnalysisManager->FillNtupleDColumn(1, posElec.y());
				AnalysisManager->FillNtupleDColumn(2, posElec.z());
				AnalysisManager->FillNtupleDColumn(3, momdirecElec.x());
				AnalysisManager->FillNtupleDColumn(4, momdirecElec.y());
				AnalysisManager->FillNtupleDColumn(5, momdirecElec.z());
				AnalysisManager->FillNtupleDColumn(6, ElectronEnergy);
				AnalysisManager->AddNtupleRow();

				AnalysisManager->FillH1(2, posElec.x());
				AnalysisManager->FillH1(3, posElec.y());
				AnalysisManager->FillH1(4, posElec.z());
			}
		}
	}

	// Hit Collection in BeamCheck, for chekcing incident beam information
	EssHitsCollection* hc_BC_IN = 0;
	hc_BC_IN = (EssHitsCollection*)hcte->GetHC(fCollectionID_BC_IN);
	int nHits_BC_IN = hc_BC_IN->entries();
	for (G4int j = 0; j < nHits_BC_IN; ++j)
	{
		EssHit* hit = (*hc_BC_IN)[j];
		G4int PID = hit->GetParticleId();
		TrackID_BC = hit->GetTrackId();

		if (PID == 22 && TrackID_BC == 1) {
			G4ThreeVector pos = hit->GetPosition();
			pos_x_BC_IN = pos.x();
			pos_y_BC_IN = pos.y();
			AnalysisManager->FillH1(5, hit->GetKineticEnergy());
			
		}
	}

	// Hit Collection in Electrode_A
	EssHitsCollection* hc_A = 0;
	hc_A = (EssHitsCollection*)hcte->GetHC(fCollectionID_Electrode_A);
	int nHits_A = hc_A->entries();
	for (G4int j = 0; j < nHits_A; ++j)
	{
		EssHit* hit = (*hc_A)[j];
		G4int PID = hit->GetParticleId();
		G4ThreeVector pos = hit->GetPosition();

		// The surface of electrode start y = -15 to -16 ,
		if (PID == 11 && pos.y() == -30) {
			//G4cout << "ElectrodeA : " << hit->GetPosition() << G4endl;
			AnalysisManager->FillH2(0, hit->GetPosition().x(), hit->GetPosition().z());
		}
	}

	// Hit Collection in Electrode_B
	EssHitsCollection* hc_B = 0;
	hc_B = (EssHitsCollection*)hcte->GetHC(fCollectionID_Electrode_B);
	int nHits_B = hc_B->entries();
	for (G4int j = 0; j < nHits_B; ++j)
	{
		EssHit* hit = (*hc_B)[j];
		G4int PID = hit->GetParticleId();
		G4ThreeVector pos = hit->GetPosition();

		// The surface of electrode start y = -15 to -16 ,
		if (PID == 11 && pos.y() == -30) {
			//G4cout << "ElectrodeB : " << hit->GetPosition() << G4endl;
			AnalysisManager->FillH2(1, hit->GetPosition().x(), hit->GetPosition().z());
		}
	}



	/*
	G4HCofThisEvent* hcte = evt->GetHCofThisEvent();
	if (!hcte) return;

	//G4AnalysisManager* AnalysisManager = G4AnalysisManager::Instance();
	G4RootAnalysisManager* AnalysisManager = G4RootAnalysisManager::Instance();

	G4SDManager* SDMan = G4SDManager::GetSDMpointer();
	//fCollectionID_A = SDMan->GetCollectionID("ElectrodeSD_A");
	//fCollectionID_B = SDMan->GetCollectionID("ElectrodeSD_B");
	fCollectionID_BC_IN = SDMan->GetCollectionID("BeamCheckIN");
	fCollectionID_BC_OUT = SDMan->GetCollectionID("BeamCheckOUT");
	fCollectionID_Pipe = SDMan->GetCollectionID("SDForElectron");
	//AnalysisManager->FillNtupleIColumn(6, evt->GetEventID());

	// Hit Collection in BeamCheck, for chekcing incident beam information
	EssHitsCollection* hc_BC_IN = 0;
	hc_BC_IN = (EssHitsCollection*)hcte->GetHC(fCollectionID_BC_IN);
	int nHits_BC_IN = hc_BC_IN->entries();
	for (G4int j = 0; j < nHits_BC_IN; ++j)
	{
		EssHit* hit = (*hc_BC_IN)[j];
		G4int PID = hit->GetParticleId();
		TrackID_BC = hit->GetTrackId();

		if (PID == 22 && TrackID_BC == 1 ) {
			G4ThreeVector pos = hit->GetPosition();
			pos_x_BC_IN = pos.x();
			pos_y_BC_IN = pos.y();
			Energy_BC_IN = hit->GetKineticEnergy();

			AnalysisManager->FillH1(2, pos_x_BC_IN);
			AnalysisManager->FillH1(3, pos_y_BC_IN);
			AnalysisManager->FillH1(6, Energy_BC_IN);
			AnalysisManager->FillH2(0, pos_x_BC_IN, pos_y_BC_IN);

		}
	}
	EssHitsCollection* hc_BC_OUT = 0;
	hc_BC_OUT = (EssHitsCollection*)hcte->GetHC(fCollectionID_BC_OUT);
	int nHits_BC_OUT = hc_BC_OUT->entries();
	for (G4int j = 0; j < nHits_BC_OUT; ++j)
	{
		EssHit* hit = (*hc_BC_OUT)[j];
		G4int PID = hit->GetParticleId();
		TrackID_BC = hit->GetTrackId();

		if (PID == 22 && TrackID_BC == 1) {
			G4ThreeVector pos = hit->GetPosition();
			pos_x_BC_OUT = pos.x();
			pos_y_BC_OUT = pos.y();
			Energy_BC_OUT = hit->GetKineticEnergy();

			AnalysisManager->FillH1(4, pos_x_BC_OUT);
			AnalysisManager->FillH1(5, pos_y_BC_OUT);
			AnalysisManager->FillH1(7, Energy_BC_OUT);
			AnalysisManager->FillH2(1, pos_x_BC_OUT, pos_y_BC_OUT);
		}
	}

	// Hit Collection in pipe, pipe is thin and long sensitive detector having same material to chamber. 
	EssHitsCollection* hc_Pipe = 0;
	hc_Pipe = (EssHitsCollection*)hcte->GetHC(fCollectionID_Pipe);
	int nHits_Pipe = hc_Pipe->entries();
	for (G4int j = 0; j < nHits_Pipe; ++j){
		EssHit* hit = (*hc_Pipe)[j];
		//G4ThreeVector pos = hit->GetPosition();
		G4int PID = hit->GetParticleId();
		TrackID_Pipe = hit->GetTrackId();
		if (PID == 22 && TrackID_Pipe == 1 ){
			Phot_in_pipe = hit->GetKineticEnergy();
		};
		//if (PID == 11 && TrackID_Pipe == 2) {
		if (PID == 11 ){
			if (TrackID_Pipe == 1){
				Phot_Elec_Energy = hit->GetKineticEnergy();
				AnalysisManager->FillH1(9, hit->GetPosition().x());
				AnalysisManager->FillH1(10, hit->GetPosition().y());
				AnalysisManager->FillH1(11, hit->GetPosition().z());
				AnalysisManager->FillH1(12, hit->GetMomentum().x());
				AnalysisManager->FillH1(13, hit->GetMomentum().y());
				AnalysisManager->FillH1(14, hit->GetMomentum().z());
				AnalysisManager->FillH1(15, Phot_Elec_Energy);
			}
			if (TrackID_Pipe == 2) {
				G4int EvtID = evt->GetEventID();
				G4ThreeVector posElec = hit->GetPosition();
				Phi = hit->GetMomentumDirection().getPhi() * (180 / CLHEP::pi);
				AnalysisManager->FillNtupleDColumn(0, EvtID);
				AnalysisManager->FillNtupleDColumn(1, posElec.x());
				AnalysisManager->FillNtupleDColumn(2, posElec.y());
				AnalysisManager->FillNtupleDColumn(3, posElec.z());
				AnalysisManager->FillNtupleDColumn(4, hit->GetMomentum().x());
				AnalysisManager->FillNtupleDColumn(5, hit->GetMomentum().y());
				AnalysisManager->FillNtupleDColumn(6, hit->GetMomentum().z());
				AnalysisManager->FillNtupleDColumn(7, Phi);
				AnalysisManager->FillNtupleDColumn(8, Phot_in_pipe);
				AnalysisManager->FillNtupleDColumn(9, hit->GetKineticEnergy());
				AnalysisManager->FillNtupleDColumn(10, hit->GetMomentumDirection().x());
				AnalysisManager->FillNtupleDColumn(11, hit->GetMomentumDirection().y());
				AnalysisManager->FillNtupleDColumn(12, hit->GetMomentumDirection().z());
				AnalysisManager->AddNtupleRow();
				AnalysisManager->FillH1(8, Phot_in_pipe);
			}
		}
	}
	*/
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......