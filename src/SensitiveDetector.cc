#include "G4VSensitiveDetector.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

#include "EssHit.hh"
#include "SensitiveDetector.hh"

SensitiveDetector::SensitiveDetector(G4String name)
:G4VSensitiveDetector(name)
{
  collectionName.insert(name.c_str());
}


SensitiveDetector::~SensitiveDetector(){ }


void SensitiveDetector::Initialize(G4HCofThisEvent* hce)
{
	hits = new EssHitsCollection(SensitiveDetectorName, collectionName[0]);

	hce->AddHitsCollection(
	  (G4int)G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]),
	  hits);
  
}


G4bool SensitiveDetector::ProcessHits(G4Step* stp, G4TouchableHistory*)
{
  G4Track* trk = stp->GetTrack();
  /*
  if (stp->GetTrack()->GetTouchable()->GetVolume()->GetName() == "Pipe" && trk->GetDefinition()->GetPDGEncoding() == 11) {
	  G4cout << "PDG    --> " << stp->GetTrack()->GetDefinition()->GetPDGEncoding() << G4endl;
	  G4cout << "energy --> " << stp->GetPreStepPoint()->GetKineticEnergy() << G4endl;
	  G4cout << "copynumber --> " << stp->GetTrack()->GetTouchable()->GetCopyNumber() << G4endl;
  }
  G4cout << "First ------> " << stp->IsFirstStepInVolume() << G4endl;
  G4cout << "Last  ------> " << stp->IsLastStepInVolume() << G4endl;
  */
  //if( stp->GetStepLength() < 1e-2 * CLHEP::mm) stp->GetTrack()->SetTrackStatus(fStopAndKill);
  
  EssHit* ht;
  ht = new EssHit();

  ht->SetParticleId(trk->GetDefinition()->GetPDGEncoding());
  ht->SetTrackId(trk->GetTrackID());
  ht->SetEdeposit(stp->GetTotalEnergyDeposit());
  ht->SetPosition(trk->GetPosition());
  ht->SetMomentum(trk->GetMomentum());
  ht->SetCharge(trk->GetDynamicParticle()->GetCharge());
  //ht->SetAtomicMass(trk->GetDynamicParticle()
  //                  ->GetDefinition()->GetAtomicMass());
  //ht->SetAtomicNumber(trk->GetDynamicParticle()
  //                    ->GetDefinition()->GetAtomicNumber());
  ht->SetKineticEnergy(trk->GetKineticEnergy());
  //add
  ht->SetMomentumDirection(trk->GetMomentumDirection());
  ht->SetPhotoElecEnergy(stp->GetPreStepPoint()->GetKineticEnergy());
  ht->SetCopyNumber(trk->GetTouchable()->GetCopyNumber());
  ht->SetFlag(stp->IsFirstStepInVolume());
  //add
  hits->insert(ht);

  return true;
}


void SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
}
