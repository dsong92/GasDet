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
	fEmPhysicsList = new G4EmStandardPhysics_option3();
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
	/*
	auto particleIterator=GetParticleIterator();
	particleIterator->reset();
	//AddTransportation();

	while( (*particleIterator)() ){
	  G4ParticleDefinition* particle = particleIterator->value();
	  G4ProcessManager* pmanager = particle->GetProcessManager();
	  G4String particleName = particle->GetParticleName();

	  //Applicability range for Livermore models
	  //for higher energies, the Standard models are used
	  G4double highEnergyLimit = 1*GeV;
	  G4double minEnergyLimit  = 100*eV; // trial-1

	  if (particleName == "gamma") {
		// gamma

		G4PhotoElectricEffect* phot = new G4PhotoElectricEffect();
		G4LivermorePhotoElectricModel*
		photModel = new G4LivermorePhotoElectricModel();
		photModel->SetHighEnergyLimit(highEnergyLimit);
		phot->AddEmModel(0, photModel);
		///list->RegisterProcess(phot, particle);
		pmanager->AddDiscreteProcess(phot);

		G4GammaConversion* conv = new G4GammaConversion();
		G4LivermoreGammaConversionModel*
		convModel = new G4LivermoreGammaConversionModel();
		convModel->SetHighEnergyLimit(highEnergyLimit);
		conv->AddEmModel(0, convModel);
		pmanager->AddDiscreteProcess(conv);

		G4ComptonScattering* compt = new G4ComptonScattering();
		G4LivermoreComptonModel*
		comptModel = new G4LivermoreComptonModel();
		comptModel->SetHighEnergyLimit(highEnergyLimit);
		compt->AddEmModel(0, comptModel);
		pmanager->AddDiscreteProcess(compt);
	  } else if (particleName == "e-") {
		//electron

		G4eIonisation* eIoni = new G4eIonisation();
		G4LivermoreIonisationModel*
		eIoniModel = new G4LivermoreIonisationModel();
		eIoniModel->SetHighEnergyLimit(highEnergyLimit);
		eIoniModel->SetHighEnergyLimit(highEnergyLimit);
		eIoni->AddEmModel(0, eIoniModel, new G4UniversalFluctuation() );
		pmanager->AddProcess(eIoni,                   -1,-1, 1);

		G4eBremsstrahlung* eBrem = new G4eBremsstrahlung();
		G4LivermoreBremsstrahlungModel*
		eBremModel = new G4LivermoreBremsstrahlungModel();
		eBremModel->SetHighEnergyLimit(highEnergyLimit);
		eBrem->AddEmModel(0, eBremModel);
		pmanager->AddProcess(eBrem,                   -1,-1, 2);

	  } else if (particleName == "e+") {
		//positron
		pmanager->AddProcess(new G4eIonisation,       -1,-1, 1);
		pmanager->AddProcess(new G4eBremsstrahlung,   -1,-1, 2);
		pmanager->AddProcess(new G4eplusAnnihilation,  0,-1, 3);
	  } else if( particleName == "alpha" || particleName == "GenericIon" ) {
		pmanager->AddProcess(new G4ionIonisation,     -1,-1, 1);

	  } else if( particleName == "GenericIon" ) {
		pmanager->AddProcess(new G4hMultipleScattering,  -1, 1,1);
		pmanager->AddProcess(new G4ionIonisation,        -1, 2,2);
	  } else if ((!particle->IsShortLived()) &&
				 (particle->GetPDGCharge() != 0.0) &&
				 (particle->GetParticleName() != "chargedgeantino")) {
		//all others charged particles except geantino
		pmanager->AddProcess(new G4hIonisation,       -1,-1, 1);
	  }

	}

	G4VModularPhysicsList::ConstructProcess();
	*/

	AddTransportation();
	fEmPhysicsList->ConstructProcess();
	fParticleList->ConstructProcess();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCuts()
{

	// default production thresholds for the world volume
	//SetCutsWithDefault();
	G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(1 * eV, 1 * GeV);
	//SetCutValue(0 * mm, "proton");
	//SetCutValue(1 * mm, "e-");
	//SetCutValue(1 * mm, "e+");
	//SetCutValue(1 * mm, "gamma");

	  // below lines are working but not used now
	  // Production thresholds for detector regions

	G4Region* region;
	G4String regName;
	G4ProductionCuts* cuts;

	regName = "envelope";
	region = G4RegionStore::GetInstance()->GetRegion(regName);
	cuts = new G4ProductionCuts;
	cuts->SetProductionCut(1 * mm); // same cuts for gamma, proton, e- and e+
	region->SetProductionCuts(cuts);

	regName = "chamber";
	region = G4RegionStore::GetInstance()->GetRegion(regName);
	cuts = new G4ProductionCuts;
	cuts->SetProductionCut(1 * mm); // same cuts for gamma, proton, e- and e+
	region->SetProductionCuts(cuts);

	regName = "beampipe";
	region = G4RegionStore::GetInstance()->GetRegion(regName);
	cuts = new G4ProductionCuts;
	cuts->SetProductionCut(1 * mm); // same cuts for gamma, proton, e- and e+
	region->SetProductionCuts(cuts);

	regName = "electrode";
	region = G4RegionStore::GetInstance()->GetRegion(regName);
	cuts = new G4ProductionCuts;
	cuts->SetProductionCut(10 * mm, G4ProductionCuts::GetIndex("gamma"));
	cuts->SetProductionCut(10 * mm, G4ProductionCuts::GetIndex("e-"));
	cuts->SetProductionCut(10 * mm, G4ProductionCuts::GetIndex("e+"));
	cuts->SetProductionCut(10 * mm, G4ProductionCuts::GetIndex("proton"));
	region->SetProductionCuts(cuts);

}
