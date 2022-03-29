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
	//atomicMass = right.atomicMass;
	//atomicNumber = right.atomicNumber;
	kineticEnergy = right.kineticEnergy;
	//add
	momentumdirection = right.momentumdirection;
	PhotElecEnergy = right.PhotElecEnergy;
	copynum = right.copynum;
	flag = right.flag;
}

const EssHit& EssHit::operator=(const EssHit& right)
{
	particleId = right.particleId;
	trackId = right.trackId;
	eDepo = right.eDepo;
	position = right.position;
	momentum = right.momentum;
	charge = right.charge;
	//atomicMass = right.atomicMass;
	//atomicNumber = right.atomicNumber;
	kineticEnergy = right.kineticEnergy;
	//add
	momentumdirection = right.momentumdirection;
	PhotElecEnergy = right.PhotElecEnergy;
	copynum = right.copynum;
	flag = right.flag;
	return *this;
}

G4int EssHit::operator==(const EssHit& right) const
{
	return (this == &right) ? 1 : 0;
}
