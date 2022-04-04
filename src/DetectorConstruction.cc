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
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
	// Get nist material manager
	G4NistManager* nist = G4NistManager::Instance();

	G4double env_sizeXY = 0.5 * cm;
	G4double env_sizeZ = 5 * cm;

	G4double fChamber_SizeXY = 1.5 * cm;
	G4double fChamber_SizeZ = 5. * cm;

	G4double fPipeForElectron_SizeXY = 1.5 * mm;
	G4double fPipeForElectron_SizeZ = 49.5 * mm;

	G4double fBeamCheck_SizeXY = 1.499 * cm;
	G4double fBeamCheck_SizeZ = 0.01 * cm;

	G4double fElectrode_SizeXZ = 1 * cm;
	G4double fElectrode_SizeY = 0.005 * cm;

	G4Material* fAir = nist->FindOrBuildMaterial("G4_AIR");
	G4Material* fCopper = nist->FindOrBuildMaterial("G4_Cu");
	G4Material* fVacuum = nist->FindOrBuildMaterial("G4_Galactic");
	G4Material* fStainless = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
	G4Material* fXenon = nist->FindOrBuildMaterial("G4_Xe");


	//G4double temperature = CLHEP::STP_Temperature;
	G4double temperature = 300 * kelvin;

	G4double pressure = 1e-0 * hep_pascal;
	G4double Xe_density = 5.252e-5 * mg / cm3;

	//G4double pressure = 1e-1 * hep_pascal;
	//double Xe_density = 5.252e-6 * mg / cm3;

	//G4double pressure = 1e-1 * hep_pascal;
	//double Xe_density = 5.252e-6 * mg / cm3;

	G4double a = 131.29 * g / mole;
	G4Element* elXe = new G4Element("Xenon", "Xe", 54., a);

	G4double density = 5.48 * mg / cm3;
	G4Material* xenon = new G4Material("Xe_gas", density, 1);
	xenon->AddElement(elXe, 1);

	//G4double Xe_density  = 5.48*mg/cm3; // The density of Xe gas at STP
	//G4double Xe_density  = 5.252e-8*mg/cm3; // The density of Xe gas at STP

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
	G4double fWorld_SizeXY = 2 * fChamber_SizeXY;
	G4double fWorld_SizeZ = 2 * fChamber_SizeZ;

	G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");

	G4Box* fsolidWorld =
		new G4Box("World",                       //its name
			fWorld_SizeXY,
			fWorld_SizeXY,
			fWorld_SizeZ);     //its size

	G4LogicalVolume* flogicWorld =
		new G4LogicalVolume(fsolidWorld,          //its solid
							//world_mat,           //its material
			fVacuum,
			"World");            //its name

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
			//fStainless,         // its material
			fVacuum,         // its material
			//fAir,         // its material
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

	// Electrode - trial 1
/*
  G4double pZ = 0.1*cm;
  G4double pY = 1.5*cm;
  G4double pX = 1.5*cm;
  G4double pLTX = 0.01*cm;

  G4Trap* fSolidTrap =
		new G4Trap("GenitaliaTrap",
								pZ, pY, pX, pLTX);

  G4LogicalVolume* fLogicElectrode1 =
	new G4LogicalVolume(fSolidTrap,
												fAir,         // its material
												"Envelope");

  G4LogicalVolume* fLogicElectrode2 =
	new G4LogicalVolume(fSolidTrap,
												fAir,         // its material
												"Envelope");

	G4VPhysicalVolume* fPhysElectrode1 =
	new G4PVPlacement(0,
					  G4ThreeVector(),
					  "Envelope",
					  fLogicElectrode1,
					  fphysWorld,
					  false,
					  0,
					  checkOverlaps);

	G4VPhysicalVolume* fPhysElectrode2 =
	new G4PVPlacement(G4ThreeVector(0,0,90*degree),
					  G4ThreeVector(),
					  "Envelope",
					  fLogicElectrode2,
					  fphysWorld,
					  false,
					  0,
					  checkOverlaps);

*/
// Electrode - trial 1
/*
G4SDManager* SDman = G4SDManager::GetSDMpointer();

  SensitiveDetector* sdE = new SensitiveDetector("electrodeSD");
  SDman->AddNewDetector(sdE);

G4Box* fSolidElectrode =
  new G4Box("Electrode",
			 fElectrode_SizeXZ,
			 fElectrode_SizeY,
			 fElectrode_SizeXZ );

G4LogicalVolume* fLogicElectrode =
  new G4LogicalVolume(fSolidElectrode,
											  fCopper,
					  "Electrode",
					  0, sdE);

G4VPhysicalVolume* fPhysElectrode =
  new G4PVPlacement(0,
					G4ThreeVector(0,-(fChamber_SizeXY+fElectrode_SizeY), -fChamber_SizeZ/2),
					"Electrode",
					fLogicElectrode,
					fPhysEnv,
					false,
					0,
					checkOverlaps);
*/
	G4Box* fSolidChamber =
		new G4Box("Chamber",
			fChamber_SizeXY,
			fChamber_SizeXY,
			fChamber_SizeZ);

	G4LogicalVolume* fLogicChamber =
		new G4LogicalVolume(fSolidChamber,
			fChamber_gas,
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

	SensitiveDetector* SD_BeamCheck = new SensitiveDetector("BeamCheckSD");
	SDman->AddNewDetector(SD_BeamCheck);

	SensitiveDetector* SD_PipeForElectron = new SensitiveDetector("SDForElectron");
	SDman->AddNewDetector(SD_PipeForElectron);

	std::vector<G4TwoVector> verticesA = { {10,10}, {-10,-10}, {-10,10} };
	std::vector<G4TwoVector> verticesB = { {10,10}, {-10,-10}, { 10,10} };

	G4double Electrode_thickness = 1 * mm;
	//std::vector<G4TwoVector> zsec = { {-Electrode_thickness / 2, {0,0}, 1}
	//                                   { Electrode_thickness / 2, {0,0}, 1} };

	G4ExtrudedSolid* fSolidElectrodeA = new G4ExtrudedSolid(
		"ElectrodeA",
		{ {10,10}, {-10,-10}, {-10,10} },
		{ {0,{0,0},1}, {1,{0,0},1} });
	G4ExtrudedSolid* fSolidElectrodeB = new G4ExtrudedSolid(
		"ElectrodeB",
		{ {10,10}, {-10,-10}, {10,-10} },
		{ {0,{0,0},1}, {1,{0,0},1} });

	G4LogicalVolume* fLogicElectrodeA =
		new G4LogicalVolume(fSolidElectrodeA,
			fCopper,
			//fVacuum,
			"ElectrodeA",
			0,
			SD_A);

	G4RotationMatrix RotMatA = G4RotationMatrix();
	RotMatA.rotateX(90 * deg);
	RotMatA.rotateY(0);
	RotMatA.rotateZ(0);
	G4ThreeVector Pos_ElecA = G4ThreeVector(0, -(fChamber_SizeXY + fElectrode_SizeY) + 0.05 * mm, -(fChamber_SizeZ / 2));
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
			fCopper,
			//fVacuum,
			"ElectrodeB",
			0,
			SD_B);

	G4RotationMatrix RotMatB = G4RotationMatrix();
	RotMatB.rotateX(90 * deg);
	RotMatB.rotateY(0);
	RotMatB.rotateZ(0);
	G4ThreeVector Pos_ElecB = G4ThreeVector(0, -(fChamber_SizeXY + fElectrode_SizeY) + 0.05 * mm, -(fChamber_SizeZ / 2));
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
			fPipeForElectron_SizeXY,
			fPipeForElectron_SizeXY,
			fPipeForElectron_SizeZ);

	G4LogicalVolume* fLogicSDforElec =
		new G4LogicalVolume(fSolidSDforElec,
			fChamber_gas,
			"SDforElec",
			0, SD_PipeForElectron);


	//G4VPhysicalVolume* PhysiChamber = 
	new G4PVPlacement(0,
		G4ThreeVector(0, -13, 0),
		"Pipe",
		fLogicSDforElec,
		PhysiChamber,
		false,
		11,
		checkOverlaps);
	
	/*
	G4Box* fSolidBeamCheck =
		new G4Box("BeamCheck",
			fBeamCheck_SizeXY,
			fBeamCheck_SizeXY,
			fBeamCheck_SizeZ);

	G4LogicalVolume* fLogicBeamCheck =
		new G4LogicalVolume(fSolidBeamCheck,
			fVacuum,
			//fCopper,
			"BeamCheck",
			0, SD_BeamCheck);


	//G4VPhysicalVolume* PhysiChamber = 
	new G4PVPlacement(0,
		G4ThreeVector(0, 0, -4.8 * cm),
		"BeamCheck",
		fLogicBeamCheck,
		PhysiChamber,
		false,
		0,
		checkOverlaps);
		*/
	// set region for SetCuts
	G4String regName[] = { "chamber","electrode", "envelope","beampipe",};

	ChamberRegion = new G4Region(regName[0]);
	fLogicChamber->SetRegion(ChamberRegion);
	ChamberRegion->AddRootLogicalVolume(fLogicChamber);

	ElectrodeRegion = new G4Region(regName[1]);
	fLogicElectrodeA->SetRegion(ElectrodeRegion);
	ElectrodeRegion->AddRootLogicalVolume(fLogicElectrodeA);
	fLogicElectrodeB->SetRegion(ElectrodeRegion);
	ElectrodeRegion->AddRootLogicalVolume(fLogicElectrodeB);

	EnvelopeRegion = new G4Region(regName[2]);
	fLogicEnv->SetRegion(EnvelopeRegion);
	EnvelopeRegion->AddRootLogicalVolume(fLogicEnv);

	PipeRegion = new G4Region(regName[3]);
	fLogicSDforElec->SetRegion(PipeRegion);
	PipeRegion->AddRootLogicalVolume(fLogicSDforElec);

	G4cout << *(G4Material::GetMaterialTable()) << G4endl;
	return fphysWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructSDandField()
{
	if (!fEmFieldSetup.Get()) {
		ElectricFieldSetup* fieldSetup = new ElectricFieldSetup();
		G4AutoDelete::Register(fieldSetup); //Kernel will delete the messenger
		fEmFieldSetup.Put(fieldSetup);
	}

	// Sensitive Detectors
  //G4SDManager* SDman = G4SDManager::GetSDMpointer();

	//SensitiveDetector* ElectrodeSD_A = new SensitiveDetector("ElectrodeSD_A");
	//SDman->AddNewDetector(ElectrodeSD_A);
	//fLogicElectrode->SetSensitiveDetector(ElectrodeSD_A);

	//SensitiveDetector* ElectrodeSD_B = new SensitiveDetector("ElectrodeSD_B");
	//SDman->AddNewDetector(ElectrodeSD_B);
	//fLogicElectrodeB->SetSensitiveDetector(ElectrodeSD_B);


}
