#ifndef SENSITIVEDETECTOR_HH
#define SENSITIVEDETECTOR_HH 1

#include "EssHit.hh"
#include "G4VSensitiveDetector.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class SensitiveDetector : public G4VSensitiveDetector
{
public:
	SensitiveDetector(G4String);
	~SensitiveDetector();

	void   Initialize(G4HCofThisEvent*);
	G4bool ProcessHits(G4Step*, G4TouchableHistory*);
	void   EndOfEvent(G4HCofThisEvent*);

private:
	EssHitsCollection* hits;

};

#endif
