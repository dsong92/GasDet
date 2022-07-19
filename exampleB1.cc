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
/// \file example.cc
/// \brief Main program of the  example
#include <iostream>
#include "GaramGlobal.hh"
#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4RunManagerFactory.hh"

#include "G4UImanager.hh"
#include "QBBC.hh"
#include "GaramGlobal.hh"
#include "PrimaryGeneratorAction.hh"
#include "PrimaryGeneratorMessenger.hh"
#include "G4RootAnalysisManager.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{

  //G4RootAnalysisManager* ra = G4RootAnalysisManager::Instance();
  //ra->OpenFile("test.root");

  G4RunManager* runManager = new G4RunManager;

  // Detect interactive mode (if no arguments) and define UI session
  //
  G4UIExecutive* ui = 0;
  if ( argc == 1 ) {
    ui = new G4UIExecutive(argc, argv);
  }

  G4int seconds = time(NULL);
  G4Random::setTheSeed(seconds);
 
  runManager->SetUserInitialization(new DetectorConstruction());  // 1. detector construction
  runManager->SetUserInitialization(new PhysicsList());           // 2. physics list
  PrimaryGeneratorAction* pga = new PrimaryGeneratorAction();
  runManager->SetUserAction(pga);                                 // 3. PGA
  //runManager->SetUserInitialization(new ActionInitialization()); // 4. ActionInit. 대신 밑의줄 추가
  
  RunAction* runAction = new RunAction; // ActionInit 안 에 있던 줄
  runManager->SetUserAction(runAction); // ActionInit 안 에 있던 줄

  EventAction* eventAction = new EventAction(runAction);  // ActionInit 안 에 있던 줄
  runManager->SetUserAction(eventAction);  // ActionInit 안 에 있던 줄
  runManager->SetUserAction(new SteppingAction(eventAction));  // ActionInit 안 에 있던 줄


  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // Process macro or start UI session
  //
  if ( ! ui ) { 
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }
  else { 
    // interactive mode
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  }

  //std::cout << "----GARAM---- before rman->Initialize();\n";
  runManager->Initialize();
  //std::cout << "----GARAM---- after main::run_init\n";
  runManager->BeamOn(pga->GetPGM()->GetPdist()->size());
  G4cout << "**-----------> "<<pga->GetPGM()->GetPdist()->size() <<" events END" << G4endl;

  //ra->Write();
  //ra->CloseFile();

  delete visManager;
  delete runManager;
  //delete ra;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
