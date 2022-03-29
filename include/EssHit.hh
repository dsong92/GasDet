
#ifndef ESSHIT_HH
#define ESSHIT_HH 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class EssHit : public G4VHit
{
public:
	EssHit();
	~EssHit();
	EssHit(const EssHit&);
	const EssHit& operator=(const EssHit&);
	G4int operator==(const EssHit&) const;

public:
	void SetParticleId(G4int ipid) { particleId = ipid; };
	void SetTrackId(G4int itrkid) { trackId = itrkid; };
	void SetEdeposit(G4double iedep) { eDepo = iedep; };
	void SetPosition(G4ThreeVector ipos) { position = ipos; };
	void SetMomentum(G4ThreeVector imom) { momentum = imom; };
	void SetCharge(G4double icharge) { charge = icharge; };
	void SetAtomicNumber(G4double iatomicNumber) { atomicNumber = iatomicNumber; };
	void SetAtomicMass(G4double iatomicMass) { atomicMass = iatomicMass; };
	void SetKineticEnergy(G4double ikineticEnergy) { kineticEnergy = ikineticEnergy; };
	//add
	void SetMomentumDirection(G4ThreeVector imomentumdirection) { momentumdirection = imomentumdirection; };
	void SetPhotoElecEnergy(G4double iPhotElecEnergy) { PhotElecEnergy = iPhotElecEnergy; };
	void SetCopyNumber(G4int icopynum) { copynum = icopynum; };
	void SetFlag(G4bool iflag) { flag = iflag; };

	G4int GetParticleId() { return particleId; };
	G4int GetTrackId() { return trackId; };
	G4double GetEdeposit() { return eDepo; };
	G4ThreeVector GetPosition() { return position; };
	G4ThreeVector GetMomentum() { return momentum; };
	G4double GetCharge() { return charge; };
	G4double GetAtomicNumber() { return atomicNumber; };
	G4double GetAtomicMass() { return atomicMass; };
	G4double GetKineticEnergy() { return kineticEnergy; };
	//add
	G4ThreeVector GetMomentumDirection() { return momentumdirection; };
	G4double GetPhotoElecEnergy() { return PhotElecEnergy; };
	G4int GetCopyNumber() { return copynum;  };
	G4bool GetFlag() { return flag; };

private:
	G4int          particleId;
	G4int          trackId;
	G4double       eDepo;
	G4ThreeVector  position;
	G4ThreeVector  momentum;
	G4double       charge;
	G4int          atomicNumber;
	G4int          atomicMass;
	G4double       kineticEnergy;
	//add
	G4ThreeVector  momentumdirection;
	G4double	   PhotElecEnergy;
	G4int		   copynum;
	G4bool		   flag;
};

typedef G4THitsCollection<EssHit> EssHitsCollection;
extern G4Allocator<EssHit> EssHitAllocator;

#endif // ESSHIT_HH

