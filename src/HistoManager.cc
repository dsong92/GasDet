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
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  //analysisManager->SetDefaultFileType("root");
  analysisManager->SetFileName(fFileName);
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetActivation(true);     //enable inactivation of histograms

  // Define histograms start values
  const G4int kMaxHisto = 8;
  //const G4String id[] = {"0","1","2","3","4","5","6","7","8","9",
  //                       "10","11","12","13","14","15","16","17","18"};
  const G4String id[] = {"0","1","2","3","4","5","6","7"};
  const G4String title[] = 
      {"Position e^{-}, electrode in", // 0
       "Position e^{-}, electrode A",  // 1
       "Position e^{-}, electrode B",  // 2  
       "Pos_X_e^{-}_Electrode_A",      // 0
       "Pos_X_e^{-}_Electrode_B",      // 1
       "Pos_Z_e^{-}_Electrode_A",      // 2
       "Pos_Z_e^{-}_Electrode_B",      // 3
	   "E_{kin} of electron, electrode", // 4

      };
  // Default values (to be reset via /analysis/h1/set command) 
  G4int nbins = 100;
  G4double vmin = 0.;
  G4double vmax = 100.;
  // Create all histograms as inactivated 
  // as we have not yet set nbins, vmin, vmax
  //for(G4int k=0; k<kMaxHisto; k++){
	//		G4int ig = analysisManager->CreateH2(id[k], title[k], 100, -200, 200, 100, -200, 200);
	//		analysisManager->SetH2Activation(ig, false);
  //};

	for (G4int k=0; k<kMaxHisto; k++) {
			if(k<3){
					G4int ig = analysisManager->CreateH2(id[k], title[k], nbins, vmin, vmax, nbins, vmin, vmax);
					analysisManager->SetH2Activation(ig, false);}
			else{
					G4int ih = analysisManager->CreateH1(id[k], title[k], nbins, vmin, vmax);
					analysisManager->SetH1Activation(ih, false);}
  }




}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
