
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
  void SetParticleId(G4int ipid){ particleId = ipid; };
  void SetTrackId(G4int itrkid){ trackId = itrkid; };
  void SetEdeposit(G4double iedep){ eDepo = iedep; };
  void SetPosition(G4ThreeVector ipos){ position = ipos; };
  void SetMomentum(G4ThreeVector imom){ momentum = imom; };
  void SetCharge(G4double icharge){ charge = icharge; };
  void SetAtomicNumber(G4double iatomicNumber){ atomicNumber = iatomicNumber; };
  void SetAtomicMass(G4double iatomicMass){ atomicMass = iatomicMass; };
  void SetKineticEnergy(G4double ikineticEnergy){ kineticEnergy = ikineticEnergy; };

  G4int GetParticleId(){ return particleId; };
  G4int GetTrackId(){ return trackId; };
  G4double GetEdeposit(){ return eDepo; };
  G4ThreeVector GetPosition(){ return position; };
  G4ThreeVector GetMomentum(){ return momentum; };
  G4double GetCharge(){ return charge; };
  G4double GetAtomicNumber(){ return atomicNumber; };
  G4double GetAtomicMass(){ return atomicMass; };
  G4double GetKineticEnergy(){ return kineticEnergy; };

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
};

typedef G4THitsCollection<EssHit> EssHitsCollection;
extern G4Allocator<EssHit> EssHitAllocator;

#endif // ESSHIT_HH
