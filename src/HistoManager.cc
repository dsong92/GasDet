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

	/*
	// Define histograms start values
	const G4int kMaxHisto = 18;
	const G4String id[] = { "0","1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17"};
	const G4String title[] =
	{ "Position e^{-}, electrode in", // 0
	 "Position e^{-}, electrode A",  // 1
	 "Position e^{-}, electrode B",  // 2
	 "EkinvsPhi",					// 3
	 "EkinvsTheta",					//4
	 "ThetavsPhi",					//5
	 "dummy",						//6
	 "Pos_X_e^{-}_Electrode_A",      // 0
	 "Pos_X_e^{-}_Electrode_B",      // 1
	 "Pos_Z_e^{-}_Electrode_A",      // 2
	 "Pos_Z_e^{-}_Electrode_B",      // 3
	 "E_{kin} of electron, electrode", // 4
	 "incident gamma energy",          // 5
	 "ElectrodeEneOnlyElec",           // 6
	 "Position_X_Profile",            // 7
	 "Position_Z_Profile",			// 8
	 "ElectronEnergy",				//9
	 "Photo-Elec Energy"			//10
	};
	// Default values (to be reset via /analysis/h1/set command)
	G4int nbins = 100;
	G4double vmin = 0.;
	G4double vmax = 100.;

	for (G4int k = 0; k < kMaxHisto; k++) {
		if (k < 7) {
			G4int ig = analysisManager->CreateH2(id[k], title[k], nbins, vmin, vmax, nbins, vmin, vmax);
			analysisManager->SetH2Activation(ig, false);
		}
		else {
			G4int ih = analysisManager->CreateH1(id[k], title[k], nbins, vmin, vmax);
			analysisManager->SetH1Activation(ih, false);
		}
	}
	*/


	/*
	analysisManager->CreateNtuple("GasPBPM", "Variables");
	analysisManager->CreateNtupleDColumn("ProjectionX_A"); // column id = 0
	analysisManager->CreateNtupleDColumn("ProjectionX_B"); // column id = 1
	analysisManager->CreateNtupleDColumn("ProjectionZ_A"); // column id = 2
	analysisManager->CreateNtupleDColumn("ProjectionZ_B"); // column id = 3
	analysisManager->CreateNtupleDColumn("EnergyOfElectronAtElectrode"); // column id = 4
	analysisManager->CreateNtupleDColumn("BeamEnergy"); // column id = 5
	analysisManager->CreateNtupleIColumn("EventID"); // column id = 6
	analysisManager->CreateNtupleDColumn("ProjectionX_AB"); // column id = 7
	analysisManager->CreateNtupleDColumn("ProjectionZ_AB"); // column id = 8
	analysisManager->CreateNtupleDColumn("dummy2"); // column id = 9
	analysisManager->CreateNtupleDColumn("PhotoElectron_Energy"); // column id = 10
	*/

	analysisManager->CreateNtuple("ElectrodeA", "VariablesA");
	analysisManager->CreateNtupleDColumn("ProjectionX_A"); // column id = 0
	analysisManager->CreateNtupleDColumn("ProjectionZ_A"); // column id = 1
	analysisManager->FinishNtuple(0);

	analysisManager->CreateNtuple("ElectrodeB", "VariablesB");
	analysisManager->CreateNtupleDColumn("ProjectionX_B"); // column id = 0
	analysisManager->CreateNtupleDColumn("ProjectionZ_B"); // column id = 1
	analysisManager->FinishNtuple(1);

	analysisManager->CreateNtuple("SDpipe", "Variables");
	//analysisManager->CreateNtupleDColumn("PhotoElectron_Energy"); // column id = 0
	analysisManager->CreateNtupleDColumn("ElecEnergy"); // column id = 0
	analysisManager->CreateNtupleDColumn("px"); // column id = 1
	analysisManager->CreateNtupleDColumn("py"); // column id = 2
	analysisManager->CreateNtupleDColumn("pz"); // column id = 4
	analysisManager->CreateNtupleDColumn("phi"); // column id = 4
	analysisManager->CreateNtupleDColumn("Pt"); // column id = 5
	analysisManager->FinishNtuple(2);


	analysisManager->CreateH2("0", "ElectronHit_AB", 100, -15, 15, 100, -40, -10);
	analysisManager->CreateH2("1", "ElectronHit_A", 100, -15, 15, 100, -40, -10);
	analysisManager->CreateH2("2", "ElectronHit_B", 100, -15, 15, 100, -40, -10);
	analysisManager->CreateH2("3", "Ekin_Vs_Phi", 400, -200, 200, 200, 0, 200);
	analysisManager->CreateH2("4", "Ekin_Vs_Theta", 400, -200, 200, 200, 0, 200);
	analysisManager->CreateH2("5", "Theta_Vs_Phi", 400, -200, 200, 400, 0, 200);

	// Define histograms start values
	const G4int kMaxHisto = 14;
	const G4String id[] = { "0","1","2","3","4","5","6","7","8","9","10", "11", "12", "13"};
	const G4String title[] =
	{ "ProjectionX_ElectrodeA",			// id = 0
	 "ProjectionX_ElectrodeB",		    // id = 1
	 "ProjectionZ_ElectrodeA",			// id = 2
	 "ProjectionZ_ElectrodeB",			// id = 3
	 "E_{kin} of e^{-} at Electrode",   // id = 4
	 "dummy1"				,           // id = 5
	 "dummy2",						    // id = 6
	 "ProjX_ElectrodeAB",               // id = 7
	 "ProjZ_ElectrodeAB",				// id = 8
	 "dummy3",							// id = 9
	 "Photo-Elec Energy",				// id = 10
	 "dist. of phi",					// id = 11
     "dsit. of theta",		   		    // id = 12
	 "dist. of P_{t}"					// id = 13
	};
	// Default values (to be reset via /analysis/h1/set command)
	G4int nbins = 100;
	G4double vmin = 0.;
	G4double vmax = 100.;

	for (G4int k = 0; k < kMaxHisto; k++) {
		G4int ih = analysisManager->CreateH1(id[k], title[k], nbins, vmin, vmax);
	}


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
