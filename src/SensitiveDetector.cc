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


SensitiveDetector::~SensitiveDetector() { }


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

	//if( stp->GetStepLength() < 1e-2 * CLHEP::mm) stp->GetTrack()->SetTrackStatus(fStopAndKill);

	EssHit* ht;
	ht = new EssHit();

	ht->SetParticleId(trk->GetDefinition()->GetPDGEncoding());
	ht->SetTrackId(trk->GetTrackID());
	ht->SetEdeposit(stp->GetTotalEnergyDeposit());
	//ht->SetPositionT(trk->GetPosition());
	ht->SetPosition(stp->GetPreStepPoint()->GetPosition());
	//ht->SetPosition(stp->GetPostStepPoint()->GetPosition());
	//ht->SetMomentum(trk->GetMomentum());
	ht->SetMomentum(stp->GetPreStepPoint()->GetMomentum());
	ht->SetCharge(trk->GetDynamicParticle()->GetCharge());
	ht->SetKineticEnergy(stp->GetPreStepPoint()->GetKineticEnergy());
	ht->SetKineticEnergyT(trk->GetKineticEnergy()); // Track에서 얻은 Kinetic energy, StepPoint와 비교해보기 위함
	//ht->SetMomentumDirection(trk->GetMomentumDirection());
	ht->SetFlag(stp->IsFirstStepInVolume());
	ht->SetMomentumDirection(stp->GetPreStepPoint()->GetMomentumDirection()); // 생성된 입자의 초기 진행방향
	ht->SetPhotoElecEnergy(stp->GetPreStepPoint()->GetKineticEnergy()); // photo-electron의 에너지
	ht->SetCurrentStepNum(stp->GetTrack()->GetCurrentStepNumber());

	hits->insert(ht);

	return true;
}


void SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
}
