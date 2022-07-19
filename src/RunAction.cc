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
/// \file RunAction.cc
/// \brief Implementation of the RunAction class

#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
#include "HistoManager.hh"
// #include "Run.hh"
#include "g4root.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
	: G4UserRunAction(), fHistoManager(0)
{
	fHistoManager = new HistoManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
	delete fHistoManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* run)
{
	
	// inform the runManager to save random number seed
	G4RunManager::GetRunManager()->SetRandomNumberStore(false);
	
	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
	if (analysisManager->IsActive()) {
		analysisManager->OpenFile();
	}
	/*
	// *** 0706 Add, for input/output consistance
	G4int HEADER_SIZE = 1024;
	G4String STRUCTURE = "iddddddd";
	G4int SIZE_OF_INT = 4;
	G4int SIZE_OF_DOUBLE = 8;
	G4int SIZE_OF_STRUCTURE = SIZE_OF_INT * 1 + SIZE_OF_DOUBLE * 7;

	G4int NI__ = HEADER_SIZE / SIZE_OF_INT;

	int counter = 0;
	std::ofstream fout;
	fout.open(file_name, std::ios::out | std::ios::binary);
	fout.write((char*)&p, sizeof(garam::particle_io));
	fout.close();  
	*/
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* run)
{
	//if (isMaster) run->EndOfRun();
	
	//save histograms      
	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
	if (analysisManager->IsActive()) {
		analysisManager->Write();
		analysisManager->CloseFile();
	}

	// show Rndm status
	if (isMaster) G4Random::showEngineStatus();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

