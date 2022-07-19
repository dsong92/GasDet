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
/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class

#include "DetectorConstruction.hh"
#include "ElectricFieldSetup.hh"
#include "SensitiveDetector.hh"
#include "DetectorMessenger.hh"

#include "G4SDStructure.hh"
#include "G4SDManager.hh"

#include "G4GenericTrap.hh"
#include "G4AutoDelete.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4ExtrudedSolid.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trap.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
	: G4VUserDetectorConstruction()
	//fLogicElectrode(0), fLogicElectrodeB(0)
{ 
	dm = new DetectorMessenger();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
	// Get nist material manager
	G4NistManager* nist = G4NistManager::Instance();

	//G4double fChamber_SizeXY = 1.5 * cm;
	//G4double fChamber_SizeZ = 5. * cm;

	G4double fChamber_SizeX = 30 * mm;
	G4double fChamber_SizeY = 30 * mm;
	G4double fChamber_SizeZ = 150. * mm;

	G4double fPipeForElectron_SizeX = 20.0 * mm;
	G4double fPipeForElectron_SizeY = 10.0 * mm;
	G4double fPipeForElectron_SizeZ = 145 * mm;

	G4double fBeamCheck_SizeX = 30.0 * mm;
	G4double fBeamCheck_SizeY = 30.0 * mm;
	G4double fBeamCheck_SizeZ = 0.1 * mm;

	G4double fElectrode_SizeXZ = 1 * cm;
	G4double fElectrode_SizeY = 0.05 * mm;

	G4Material* fAir = nist->FindOrBuildMaterial("G4_AIR");
	G4Material* fCopper = nist->FindOrBuildMaterial("G4_Cu");
	G4Material* fVacuum = nist->FindOrBuildMaterial("G4_Galactic");
	G4Material* fStainless = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
	G4Material* fXenon = nist->FindOrBuildMaterial("G4_Xe");


	//G4double temperature = CLHEP::STP_Temperature;
	G4double temperature = 300 * kelvin;

	//G4double pressure = 1e-0 * hep_pascal;
	//G4double Xe_density = 5.252e-5 * mg / cm3;

	//G4double pressure = 1e-1 * hep_pascal;
	//double Xe_density = 5.252e-6 * mg / cm3;

	G4double pressure = 1e-2 * hep_pascal;
	double Xe_density = 5.252e-7 * mg / cm3;

	//G4double pressure = 1e-5 * hep_pascal;
	//double Xe_density = 5.252e-10 * mg / cm3;

	//G4double pressure = 1e-3 * hep_pascal;
	//double Xe_density = 5.252e-8 * mg / cm3;

	G4double a = 131.29 * g / mole;
	G4Element* elXe = new G4Element("Xenon", "Xe", 54., a);

	G4double density = 5.48 * mg / cm3;
	G4Material* xenon = new G4Material("Xe_gas", density, 1);
	xenon->AddElement(elXe, 1);

	//G4double Xe_density  = 5.48*mg/cm3; // The density of Xe gas at STP
	//G4double Xe_density  = 5.252e-8*mg/cm3;

	G4Material* my_gas = new G4Material("LowP_Xe_Gas", Xe_density, 1,
		kStateGas, temperature, pressure);
	//my_gas->AddMaterial(xenon, 1 );
	my_gas->AddMaterial(fXenon, 1);
	G4Material* fChamber_gas = my_gas;

	// Option to switch on/off checking of volumes overlaps
	G4bool checkOverlaps = true;

	//     
	// World
	//
	G4double fWorld_SizeXY = 2 * fChamber_SizeX;
	G4double fWorld_SizeZ = 2 * fChamber_SizeZ;

	G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");

	G4Box* fsolidWorld =
		new G4Box("World",                       //its name
			fWorld_SizeXY,
			fWorld_SizeXY,
			fWorld_SizeZ);     //its size

	G4LogicalVolume* flogicWorld =
		new G4LogicalVolume(
			fsolidWorld,          //its solid
			fVacuum,			  //world_mat
			"World");             //its name

	G4VPhysicalVolume* fphysWorld =
		new G4PVPlacement(0,                     //no rotation
			G4ThreeVector(),       //at (0,0,0)
			flogicWorld,            //its logical volume
			"World",               //its name
			0,                     //its mother  volume
			false,                 //no boolean operation
			0,                     //copy number
			checkOverlaps);        //overlaps checking

	G4Box* fSolidEnv =
		new G4Box("Envelope",
			0.8 * fWorld_SizeXY,
			0.8 * fWorld_SizeXY,
			0.8 * fWorld_SizeZ);

	G4LogicalVolume* fLogicEnv =
		new G4LogicalVolume(fSolidEnv,
			fVacuum,			   // its material
			//fAir,			       // its material
			"Envelope");

	G4VPhysicalVolume* fPhysEnv =
		new G4PVPlacement(0,
			G4ThreeVector(),
			"Envelope",
			fLogicEnv,
			fphysWorld,
			false,
			0,
			checkOverlaps);

	G4Box* fSolidChamber =
		new G4Box("Chamber",
			fChamber_SizeX,
			fChamber_SizeY,
			fChamber_SizeZ);

	G4LogicalVolume* fLogicChamber =
		new G4LogicalVolume(fSolidChamber,
			fChamber_gas,
			//fVacuum,
			"Chamber");

	G4VPhysicalVolume* PhysiChamber =
		new G4PVPlacement(0,
			G4ThreeVector(),
			"Chamber",
			fLogicChamber,
			fPhysEnv,
			false,
			0,
			checkOverlaps);

	//fScoringVolume = fLogicElectrode;

	// Sensitive Detectors
	G4SDManager* SDman = G4SDManager::GetSDMpointer();

	SensitiveDetector* SD_A = new SensitiveDetector("ElectrodeSD_A");
	SDman->AddNewDetector(SD_A);

	SensitiveDetector* SD_B = new SensitiveDetector("ElectrodeSD_B");
	SDman->AddNewDetector(SD_B);

	SensitiveDetector* SD_BeamCheck_IN = new SensitiveDetector("BeamCheckIN");
	SDman->AddNewDetector(SD_BeamCheck_IN);

	SensitiveDetector* SD_BeamCheck_OUT = new SensitiveDetector("BeamCheckOUT");
	SDman->AddNewDetector(SD_BeamCheck_OUT);

	SensitiveDetector* SD_PipeForElectron = new SensitiveDetector("SDForElectron");
	SDman->AddNewDetector(SD_PipeForElectron);

	/*G4cout << "length : " << dm->GetElectrodeLength() << G4endl;
	G4cout << "width : " << dm->GetElectrodeWidth() << G4endl;
	G4cout << "efield : " << dm->GetEfield() << G4endl;*/
	//std::vector<G4TwoVector> verticesA = { {10,10}, {-10,-10}, {-10,10} };
	//std::vector<G4TwoVector> verticesB = { {10,10}, {-10,-10}, { 10,10} };

	//G4double Electrode_thickness = 1 * mm;
	//std::vector<G4TwoVector> zsec = { {-Electrode_thickness / 2, {0,0}, 1}
	//                                   { Electrode_thickness / 2, {0,0}, 1} };

	/*
	G4ExtrudedSolid* fSolidElectrodeA = new G4ExtrudedSolid(
		"ElectrodeA",
		{ {10,10}, {-10,-10}, {-10,10} },
		{ {0,{0,0},1}, {1,{0,0},1} });
	G4ExtrudedSolid* fSolidElectrodeB = new G4ExtrudedSolid(
		"ElectrodeB",
		{ {10,10}, {-10,-10}, {10,-10} },
		{ {0,{0,0},1}, {1,{0,0},1} });
	*/
	/*G4ExtrudedSolid* fSolidElectrodeA = new G4ExtrudedSolid(
		"ElectrodeA",
		{ {20,180}, {-20,-20}, {-20,180} },
		{ {0,{0,0},1}, {1,{0,0},1} });
	G4ExtrudedSolid* fSolidElectrodeB = new G4ExtrudedSolid(
		"ElectrodeB",
		{ {20,180}, {-20,-20}, {20,-20} },
		{ {0,{0,0},1}, {1,{0,0},1} });*/
		/*G4ExtrudedSolid* fSolidElectrodeA = new G4ExtrudedSolid(
			"ElectrodeA",
			{ {20,-100}, {-20,-100}, {-20,100} },
			{ {0,{0,0},1}, {1,{0,0},1} });
		G4ExtrudedSolid* fSolidElectrodeB = new G4ExtrudedSolid(
			"ElectrodeB",
			{ {20,-100}, {-20,100}, {20,100} },
			{ {0,{0,0},1}, {1,{0,0},1} });*/
	
	/*G4ExtrudedSolid* fSolidElectrodeA = new G4ExtrudedSolid(
		"ElectrodeA",
		{ {19.65, -100.35}, {-20.35, -100.35}, {-20.35, 99.65} },
		{ {0,{0,0},1}, {1,{0,0},1} });
	G4ExtrudedSolid* fSolidElectrodeB = new G4ExtrudedSolid(
		"ElectrodeB",
		{ {20.35, -99.65}, {-19.65, 100.35}, {20.35, 100.35} },
		{ {0,{0,0},1}, {1,{0,0},1} });*/

	G4double tmp_width = dm->GetElectrodeWidth();
	G4double tmp_length = dm->GetElectrodeLength();
	G4double pitch = 0.35;
	G4ExtrudedSolid* fSolidElectrodeA = new G4ExtrudedSolid(
		"ElectrodeA",
		{ {tmp_width - pitch, -tmp_length - pitch}, {-tmp_width - pitch, -tmp_length - pitch}, {-tmp_width - pitch, tmp_length - pitch} },
		{ {0,{0,0},1}, {1,{0,0},1} });
	G4ExtrudedSolid* fSolidElectrodeB = new G4ExtrudedSolid(
		"ElectrodeB",
		{ {tmp_width + pitch, -tmp_length + pitch}, {-tmp_width + pitch, tmp_length + pitch}, {tmp_width + pitch, tmp_length + pitch} },
		{ {0,{0,0},1}, {1,{0,0},1} });

	G4LogicalVolume* fLogicElectrodeA =
		new G4LogicalVolume(fSolidElectrodeA,
			//fCopper,
			fVacuum,
			"ElectrodeA",
			0,
			SD_A);

	G4RotationMatrix RotMatA = G4RotationMatrix();
	RotMatA.rotateX(90 * deg);
	RotMatA.rotateY(0);
	RotMatA.rotateZ(0);
	G4ThreeVector Pos_ElecA = G4ThreeVector(0, -fChamber_SizeY, 0);
	//G4ThreeVector Pos_ElecA = G4ThreeVector(0, 0, 0);
	G4Transform3D TransformA = G4Transform3D(RotMatA, Pos_ElecA);

	//G4VPhysicalVolume* PhysiElectrodeA =
	new G4PVPlacement(TransformA,
		"ElectrodeA",
		fLogicElectrodeA,
		fPhysEnv,
		false,
		3,
		checkOverlaps);

	G4LogicalVolume* fLogicElectrodeB =
		new G4LogicalVolume(fSolidElectrodeB,
			//fCopper,
			fVacuum,
			"ElectrodeB",
			0,
			SD_B);

	G4RotationMatrix RotMatB = G4RotationMatrix();
	RotMatB.rotateX(90 * deg);
	RotMatB.rotateY(0);
	RotMatB.rotateZ(0);
	G4ThreeVector Pos_ElecB = G4ThreeVector(0, -fChamber_SizeY, 0);
	//G4ThreeVector Pos_ElecB = G4ThreeVector(0, 0, 0);
	G4Transform3D TransformB = G4Transform3D(RotMatB, Pos_ElecB);

	//G4VPhysicalVolume* PhysiElectrodeB =
	new G4PVPlacement(TransformB,
		"ElectrodeB",
		fLogicElectrodeB,
		fPhysEnv,
		false,
		4,
		checkOverlaps);
	
	G4Box* fSolidSDforElec =
		new G4Box("SDforElec",
			fPipeForElectron_SizeX,
			fPipeForElectron_SizeY,
			fPipeForElectron_SizeZ);

	G4LogicalVolume* fLogicSDforElec =
		new G4LogicalVolume(fSolidSDforElec,
			fChamber_gas,
			"SDforElec",
			0, SD_PipeForElectron);


	//G4VPhysicalVolume* PhysiChamber = 
	new G4PVPlacement(0,
		G4ThreeVector(0, -19 * mm, 0),
		//G4ThreeVector(0, 0, 0),
		"Pipe",
		fLogicSDforElec,
		PhysiChamber,
		false,
		11,
		checkOverlaps);


	G4Box* fSolidBeamCheckIN =
		new G4Box("BeamCheckIN",
			fBeamCheck_SizeX,
			fBeamCheck_SizeY,
			fBeamCheck_SizeZ);

	G4LogicalVolume* fLogicBeamCheckIN =
		new G4LogicalVolume(fSolidBeamCheckIN,
			fVacuum,
			//fChamber_gas,
			"BeamCheckIN",
			0, 
			SD_BeamCheck_IN);

	//G4VPhysicalVolume* PhysiChamber = 
	new G4PVPlacement(0,
		//G4ThreeVector(0, -13 * mm, -49.9 * mm),
		G4ThreeVector(0, -0 * mm, -fPipeForElectron_SizeZ - 0.2 * mm),
		"BeamCheckIN",
		fLogicBeamCheckIN,
		PhysiChamber,
		false,
		0,
		checkOverlaps);

	G4Box* fSolidBeamCheckOUT =
		new G4Box("BeamCheckOut",
			fBeamCheck_SizeX,
			fBeamCheck_SizeY,
			fBeamCheck_SizeZ);

	G4LogicalVolume* fLogicBeamCheckOUT =
		new G4LogicalVolume(fSolidBeamCheckOUT,
			fVacuum,
			//fChamber_gas,
			"BeamCheck",
			0, 
			SD_BeamCheck_OUT);

	//G4VPhysicalVolume* PhysiChamber = 
	new G4PVPlacement(0,
		//G4ThreeVector(0, -13 * mm, -49.9 * mm),
		G4ThreeVector(0, -0 * mm, +fPipeForElectron_SizeZ + 0.1 * mm),
		"BeamCheckOUT",
		fLogicBeamCheckOUT,
		PhysiChamber,
		false,
		0,
		checkOverlaps);


	// set region for SetCuts
	//G4String regName[] = { "chamber", "electrode", "envelope", "beampipe", "beamcheck" };
	G4String regName[] = { "chamber", "envelope", "beampipe", "beamcheckIN", "beamcheckOUT", "Electrode"};

	ChamberRegion = new G4Region(regName[0]);
	fLogicChamber->SetRegion(ChamberRegion);
	ChamberRegion->AddRootLogicalVolume(fLogicChamber);

	//ElectrodeRegion = new G4Region(regName[1]);
	//fLogicElectrodeA->SetRegion(ElectrodeRegion);
	//ElectrodeRegion->AddRootLogicalVolume(fLogicElectrodeA);
	//fLogicElectrodeB->SetRegion(ElectrodeRegion);
	//ElectrodeRegion->AddRootLogicalVolume(fLogicElectrodeB);

	EnvelopeRegion = new G4Region(regName[1]);
	fLogicEnv->SetRegion(EnvelopeRegion);
	EnvelopeRegion->AddRootLogicalVolume(fLogicEnv);

	PipeRegion = new G4Region(regName[2]);
	fLogicSDforElec->SetRegion(PipeRegion);
	PipeRegion->AddRootLogicalVolume(fLogicSDforElec);

	BeamCheckRegion = new G4Region(regName[3]);
	fLogicBeamCheckIN->SetRegion(BeamCheckRegion);
	BeamCheckRegion->AddRootLogicalVolume(fLogicBeamCheckIN);

	BeamCheckRegion2 = new G4Region(regName[4]);
	fLogicBeamCheckOUT->SetRegion(BeamCheckRegion2);
	BeamCheckRegion2->AddRootLogicalVolume(fLogicBeamCheckOUT);

	ElectrodeRegion = new G4Region(regName[5]);
	fLogicElectrodeA->SetRegion(ElectrodeRegion);
	ElectrodeRegion->AddRootLogicalVolume(fLogicElectrodeA);
	fLogicElectrodeB->SetRegion(ElectrodeRegion);
	ElectrodeRegion->AddRootLogicalVolume(fLogicElectrodeB);
	

	G4cout << *(G4Material::GetMaterialTable()) << G4endl;
	return fphysWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructSDandField()
{
	//fEMfield = new G4UniformElectricField(G4ThreeVector(0.0, 1.5 * kilovolt / cm, 0.0));
	//fEMfield = new G4UniformElectricField(G4ThreeVector(0.0, 0.0, 0.0));
	G4double tmp_field_value = dm->GetEfield();
	if (!fEmFieldSetup.Get()) {
		ElectricFieldSetup* fieldSetup = new ElectricFieldSetup(tmp_field_value);
		G4AutoDelete::Register(fieldSetup); //Kernel will delete the messenger
		fEmFieldSetup.Put(fieldSetup);
	}

}