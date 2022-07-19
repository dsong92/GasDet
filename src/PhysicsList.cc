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
/// \file field/field05/src/F05PhysicsList.cc
/// \brief Implementation of the F05PhysicsList class
//
//
#include "PhysicsList.hh"

#include "G4SystemOfUnits.hh"
#include "G4ProductionCuts.hh"
#include "G4UnitsTable.hh"
#include "G4RegionStore.hh"
#include "G4SpinDecayPhysics.hh"
#include "G4StepLimiterPhysics.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4HadronPhysicsQGSP_BIC_AllHP.hh"
#include "G4ProductionCutsTable.hh"
#include "FTFP_BERT.hh"

#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option1.hh"
#include "G4EmStandardPhysics_option2.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmStandardPhysics_option4.hh"

//------------------------------------------------
#include "G4BuilderType.hh"
#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
// gamma

#include "G4PhotoElectricEffect.hh"
#include "G4LivermorePhotoElectricModel.hh"

#include "G4ComptonScattering.hh"
#include "G4LivermoreComptonModel.hh"

#include "G4GammaConversion.hh"
#include "G4LivermoreGammaConversionModel.hh"

#include "G4RayleighScattering.hh"
#include "G4LivermoreRayleighModel.hh"

// e-

#include "G4eIonisation.hh"
#include "G4LivermoreIonisationModel.hh"
#include "G4UniversalFluctuation.hh"

#include "G4eBremsstrahlung.hh"
#include "G4LivermoreBremsstrahlungModel.hh"

// e+

#include "G4eplusAnnihilation.hh"

// hadrons, ions

#include "G4hIonisation.hh"
#include "G4ionIonisation.hh"
#include "G4hMultipleScattering.hh"

#include "G4EmStandardPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4LossTableManager.hh"

#include "G4HadronPhysicsFTFP_BERT.hh"
#include "G4IonPhysics.hh"

#include "G4LossTableManager.hh"
#include "G4VAtomDeexcitation.hh"
#include "G4UAtomicDeexcitation.hh"

#include "G4EmParameters.hh"
//------------------------------------------------


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::PhysicsList()
	: G4VModularPhysicsList(), fEmPhysicsList(0)
{
	//RegisterPhysics(new G4EmStandardPhysics());
	verboseLevel = 1;

	// Particles
	fParticleList = new G4DecayPhysics(verboseLevel);

	// EM physics
	//fEmPhysicsList = new G4EmStandardPhysics_option1();
	//fEmPhysicsList = new G4EmStandardPhysics_option2();
	fEmPhysicsList = new G4EmStandardPhysics_option3(); // original
	//fEmPhysicsList = new G4EmStandardPhysics_option4();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::~PhysicsList() {
	delete fParticleList;
	delete fEmPhysicsList;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::ConstructParticle()
{
	//G4VModularPhysicsList::ConstructParticle();
	fParticleList->ConstructParticle();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::ConstructProcess()
{

	AddTransportation();
	fEmPhysicsList->ConstructProcess();
	fParticleList->ConstructProcess();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCuts()
{

	// default production thresholds for the world volume 
	SetCutsWithDefault();
	// Xenon ionization energy is about 12 eV 
	//G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(12 * eV, 1 * GeV);
	
	//SetCutValue(0 * mm, "proton");
	//SetCutValue(1 * mm, "e-");
	//SetCutValue(1 * mm, "e+");
	//SetCutValue(1 * mm, "gamma");

	G4Region* region;
	G4String regName;
	G4ProductionCuts* cuts;
	/*
	regName = "envelope";
	region = G4RegionStore::GetInstance()->GetRegion(regName);
	cuts = new G4ProductionCuts;
	cuts->SetProductionCut(0.1 * mm); // same cuts for gamma, proton, e- and e+
	region->SetProductionCuts(cuts);

	regName = "chamber";
	region = G4RegionStore::GetInstance()->GetRegion(regName);
	cuts = new G4ProductionCuts;
	cuts->SetProductionCut(0.1 * mm); // same cuts for gamma, proton, e- and e+
	region->SetProductionCuts(cuts);

	regName = "beampipe";
	region = G4RegionStore::GetInstance()->GetRegion(regName);
	cuts = new G4ProductionCuts;
	cuts->SetProductionCut(0.1 * mm); // same cuts for gamma, proton, e- and e+
	region->SetProductionCuts(cuts);

	regName = "beamcheck";
	region = G4RegionStore::GetInstance()->GetRegion(regName);
	cuts = new G4ProductionCuts;
	cuts->SetProductionCut(0.1 * mm); // same cuts for gamma, proton, e- and e+
	region->SetProductionCuts(cuts);

	regName = "electrode";
	region = G4RegionStore::GetInstance()->GetRegion(regName);
	cuts = new G4ProductionCuts;
	cuts->SetProductionCut(10 * mm, G4ProductionCuts::GetIndex("gamma"));
	cuts->SetProductionCut(10 * mm, G4ProductionCuts::GetIndex("e-"));
	cuts->SetProductionCut(10 * mm, G4ProductionCuts::GetIndex("e+"));
	cuts->SetProductionCut(10 * mm, G4ProductionCuts::GetIndex("proton"));
	region->SetProductionCuts(cuts);
	*/
}
