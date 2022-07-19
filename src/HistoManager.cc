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
/// \file HistoManager.cc
/// \brief Implementation of the HistoManager class
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "HistoManager.hh"
#include "G4UnitsTable.hh"

#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::HistoManager()
	: fFileName("GasDetector")
{
	Book();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::~HistoManager()
{
	delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Book()
{
	
	// Create or get analysis manager
	// The choice of analysis technology is done via selection of a namespace
	// in HistoManager.hh
	//G4AnalysisManager* analysisManager = G4AnalysisManager::Instance(); // Just Histrogram
	G4RootAnalysisManager* analysisManager = G4RootAnalysisManager::Instance();
	analysisManager->SetFileName(fFileName);
	analysisManager->SetActivation(true);     //enable inactivation of histograms

	//analysisManager->CreateNtuple("ElectrodeA", "VariablesA");
	analysisManager->CreateNtuple("ElectronInfo", "ElecInfo");
	analysisManager->CreateNtupleDColumn("ElectronPos_X"); // column id = 0
	analysisManager->CreateNtupleDColumn("ElectronPos_Y"); // column id = 1
	analysisManager->CreateNtupleDColumn("ElectronPos_Z"); // column id = 2
	analysisManager->CreateNtupleDColumn("ElectronMomD_X"); // column id = 3
	analysisManager->CreateNtupleDColumn("ElectronMomD_Y"); // column id = 4
	analysisManager->CreateNtupleDColumn("ElectronMomD_Z"); // column id = 5
	analysisManager->CreateNtupleDColumn("PhotoElecEnergy"); // column id = 6
	analysisManager->FinishNtuple();

 	analysisManager->CreateH2("0", "Electrode_A", 100, -50, 50, 100, -150, 150);
	analysisManager->CreateH2("1", "Electrode_B", 100, -50, 50, 100, -150, 150);
	analysisManager->SetH2Activation(0, true);
	analysisManager->SetH2Activation(1, true);

	//analysisManager->CreateH2("1", "Photon_OUT", 100, -15, 15, 100, -40, -10);
	//analysisManager->CreateH2("2", "ElectronHit_B", 100, -15, 15, 100, -40, -10);
	//analysisManager->CreateH2("3", "Ekin_Vs_Phi", 400, -200, 200, 200, 0, 200);
	//analysisManager->CreateH2("4", "Ekin_Vs_Theta", 400, -200, 200, 200, 0, 200);
	//analysisManager->CreateH2("5", "Theta_Vs_Phi", 400, -200, 200, 400, 0, 200);
	//analysisManager->CreateH2("6", "Photon_Beam_Position", 400, -200, 200, 400, 0, 200);

	/*const G4int kMaxHisto = 16;
	const G4String id[] = { "0","1","2","3","4","5","6","7","8","9","10","11","12","13","14","15" };
	const G4String title[] =
	{
	 "dummy",							// id = 0
	 "dummy",							// id = 1
	 "Photon_Beam_IN_X",				// id = 2
	 "Photon_Beam_IN_Y",				// id = 3
	 "Photon_Beam_OUT_X",				// id = 4
	 "Photon_Beam_OUT_Y",				// id = 5
	 "Photon_Energy_IN",				// id = 6
	 "Photon_Energy_OUT",				// id = 7
	 "PhotonE_making_electron",			// id = 8
	 "ElectronInput_posX",   			// id = 9
	 "ElectronInput_posY",   			// id = 10
	 "ElectronInput_posZ",   			// id = 11
	 "ElectronInput_momX",   			// id = 12
	 "ElectronInput_momY",   			// id = 13
	 "ElectronInput_momZ",   			// id = 14
	 "ElectronInput_kinE"   			// id = 15
	};
	// Default values (to be reset via /analysis/h1/set command)
	G4int nbins = 100;
	G4double vmin = 0.;
	G4double vmax = 100.;

	for (G4int k = 0; k < kMaxHisto; k++) {
		G4int ih = analysisManager->CreateH1(id[k], title[k], nbins, vmin, vmax);
		analysisManager->SetH1Activation(ih, false);
	}*/
	analysisManager->CreateH1("0", "dummy", 100, -3, 3);
	analysisManager->CreateH1("1", "dummy", 100, -3, 3);
	analysisManager->CreateH1("2", "Electron_posX", 100, -3, 3);
	analysisManager->CreateH1("3", "Electron_posY", 100, -30, 0);
	analysisManager->CreateH1("4", "Electron_posZ", 100, -150, 150);
	analysisManager->CreateH1("5", "PhotonEnergy", 100, 0, 1000*CLHEP::eV);
	analysisManager->SetH1Activation(0, false);
	analysisManager->SetH1Activation(1, false);
	analysisManager->SetH1Activation(2, true);
	analysisManager->SetH1Activation(3, true);
	analysisManager->SetH1Activation(4, true);
	analysisManager->SetH1Activation(5, true);
	//analysisManager->SetH1Activation(6, true);
	//analysisManager->SetH2Activation(1, false);
	//
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
