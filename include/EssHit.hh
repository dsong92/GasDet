
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
	void SetKineticEnergy(G4double ikineticEnergy) { kineticEnergy = ikineticEnergy; };
	void SetMomentumDirection(G4ThreeVector imomentumdirection) { momentumdirection = imomentumdirection; };
	void SetPhotoElecEnergy(G4double iPhotElecEnergy) { PhotElecEnergy = iPhotElecEnergy; };
	void SetCopyNumber(G4int icopynum) { copynum = icopynum; };
	void SetFlag(G4bool iflag) { flag = iflag; };
	void SetStepLength(G4double istplength) { stplength = istplength; };
	void SetCurrentStepNum(G4int icurrentStepNum) { currentStepNum = icurrentStepNum; };

	G4int GetParticleId() { return particleId; };
	G4int GetTrackId() { return trackId; };
	G4double GetEdeposit() { return eDepo; };
	G4ThreeVector GetPosition() { return position; };
	G4ThreeVector GetMomentum() { return momentum; };
	G4double GetCharge() { return charge; };
	G4double GetKineticEnergy() { return kineticEnergy; };
	G4ThreeVector GetMomentumDirection() { return momentumdirection; };
	G4double GetPhotoElecEnergy() { return PhotElecEnergy; };
	G4int GetCopyNumber() { return copynum; };
	G4bool GetFlag() { return flag; };
	G4double GetStepLength() { return stplength; };
	G4int GetCurrentStepNum() { return currentStepNum; };

private:
	G4int          particleId;
	G4int          trackId;
	G4double       eDepo;
	G4ThreeVector  position;
	G4ThreeVector  momentum;
	G4double       charge;
	G4double       kineticEnergy;
	G4ThreeVector  momentumdirection;
	G4double	   PhotElecEnergy;
	G4int		   copynum;
	G4bool		   flag;
	G4double	   stplength;
	G4int		   currentStepNum;
};

typedef G4THitsCollection<EssHit> EssHitsCollection;
extern G4Allocator<EssHit> EssHitAllocator;

#endif // ESSHIT_HH

