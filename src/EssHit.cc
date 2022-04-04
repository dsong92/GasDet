#include "EssHit.hh"
class G4VHit;

G4Allocator<EssHit> EssHitAllocator;

EssHit::EssHit() {}
EssHit::~EssHit() {}

EssHit::EssHit(const EssHit& right)
	: G4VHit()
{
	particleId = right.particleId;
	trackId = right.trackId;
	eDepo = right.eDepo;
	position = right.position;
	momentum = right.momentum;
	charge = right.charge;
	kineticEnergy = right.kineticEnergy;
	momentumdirection = right.momentumdirection;
	PhotElecEnergy = right.PhotElecEnergy;
	copynum = right.copynum;
	flag = right.flag;
	stplength = right.stplength;
	currentStepNum = right.currentStepNum;
}

const EssHit& EssHit::operator=(const EssHit& right)
{
	particleId = right.particleId;
	trackId = right.trackId;
	eDepo = right.eDepo;
	position = right.position;
	momentum = right.momentum;
	charge = right.charge;
	kineticEnergy = right.kineticEnergy;
	momentumdirection = right.momentumdirection;
	PhotElecEnergy = right.PhotElecEnergy;
	copynum = right.copynum;
	flag = right.flag;
	stplength = right.stplength;
	currentStepNum = right.currentStepNum;
	return *this;
}

G4int EssHit::operator==(const EssHit& right) const
{
	return (this == &right) ? 1 : 0;
}
