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


#include <vector>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(RunAction* runAction)
	: G4UserEventAction(),
	fRunAction(runAction),
	fCollectionID_A(-1),
	fCollectionID_B(-1),
	fCollectionID_BC(-1)
	//pos_x_A(-999),pos_x_B(-999),pos_z_A(-999),pos_z_B(-999),
	//Energy_A(-999), Energy_B(-999)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event*)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventAction::EndOfEventAction(const G4Event* evt)
{

	G4HCofThisEvent* hcte = evt->GetHCofThisEvent();
	if (!hcte) return;

	G4AnalysisManager* AnalysisManager = G4AnalysisManager::Instance();
	G4SDManager* SDMan = G4SDManager::GetSDMpointer();
	fCollectionID_A = SDMan->GetCollectionID("ElectrodeSD_A");
	fCollectionID_B = SDMan->GetCollectionID("ElectrodeSD_B");
	fCollectionID_BC = SDMan->GetCollectionID("BeamCheckSD");

	EssHitsCollection* hc_A = 0;
	hc_A = (EssHitsCollection*)hcte->GetHC(fCollectionID_A);
	int nHits_A = hc_A->entries();
	for (G4int j = 0; j < nHits_A; ++j)
	{
		EssHit* hit = (*hc_A)[j];
		G4ThreeVector pos = hit->GetPosition();
		if (pos.y() != -15) return;
		G4double Ekin = hit->GetKineticEnergy();

		AnalysisManager->FillH2(0, pos.x(), pos.z());
		AnalysisManager->FillH2(1, pos.x(), pos.z());
		AnalysisManager->FillH1(0, pos.x());
		AnalysisManager->FillH1(2, pos.z());
		AnalysisManager->FillH1(4, Ekin);
		G4int PID = hit->GetParticleId();
		if (PID != 11) return;
		AnalysisManager->FillH2(3, pos.x(), pos.z());
		AnalysisManager->FillH1(6, Ekin);

		/*
		AnalysisManager->FillH2(0, pos_x_A, pos_z_A);
		AnalysisManager->FillH2(1, pos_x_A, pos_z_A);
		AnalysisManager->FillNtupleDColumn(0, pos_x_A);
		AnalysisManager->FillNtupleDColumn(2, pos_z_A);
		AnalysisManager->FillNtupleDColumn(4, Energy_A);
		*/
	}

	EssHitsCollection* hc_B = 0;
	hc_B = (EssHitsCollection*)hcte->GetHC(fCollectionID_B);
	int nHits_B = hc_B->entries();
	for (G4int j = 0; j < nHits_B; ++j)
	{
		EssHit* hit = (*hc_B)[j];
		G4ThreeVector pos = hit->GetPosition();
		if (pos.y() != -15) return;
		G4double Ekin = hit->GetKineticEnergy();

		AnalysisManager->FillH2(0, pos.x(), pos.z());
		AnalysisManager->FillH2(2, pos.x(), pos.z());
		AnalysisManager->FillH1(1, pos.x());
		AnalysisManager->FillH1(3, pos.z());
		AnalysisManager->FillH1(4, Ekin);
		G4int PID = hit->GetParticleId();
		if (PID != 11) return;
		AnalysisManager->FillH2(3, pos.x(), pos.z());
		AnalysisManager->FillH1(6, Ekin);

		//AnalysisManager->FillH2(0, pos_x_B, pos_z_B);
		//AnalysisManager->FillH2(2, pos_x_B, pos_z_B);
		//AnalysisManager->FillNtupleDColumn(1, pos_x_B);
		//AnalysisManager->FillNtupleDColumn(3, pos_z_B);
		//AnalysisManager->FillNtupleDColumn(4, Energy_B);
	}

	EssHitsCollection* hc_BC = 0;
	hc_BC = (EssHitsCollection*)hcte->GetHC(fCollectionID_BC); // beam check , thin vacuum SD
	int nHits_BC = hc_BC->entries();
	for (G4int j = 0; j < nHits_BC; ++j)
	{
		EssHit* hit = (*hc_BC)[j];
		G4ThreeVector pos = hit->GetPosition();
		G4int PID = hit->GetParticleId();
		if (PID != 22 && pos.z() != -47) return;
		G4double Ekin = hit->GetKineticEnergy();
		AnalysisManager->FillH1(5, Ekin);
	}

	//AnalysisManager->AddNtupleRow();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
