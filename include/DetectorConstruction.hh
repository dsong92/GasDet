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
/// \file DetectorConstruction.hh
/// \brief Definition of the DetectorConstruction class

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "SensitiveDetector.hh"
#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4Cache.hh"
#include "G4Region.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

class ElectricFieldSetup;
class G4UniformMagField;

/// Detector construction class to define materials and geometry.

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
	DetectorConstruction();
	virtual ~DetectorConstruction();

	virtual G4VPhysicalVolume* Construct();
	virtual void ConstructSDandField();

	G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }
	//G4LogicalVolume*	fLogicElectrode; 
	//G4LogicalVolume*	fLogicElectrodeA; 
	//G4LogicalVolume*	fLogicElectrodeB; 

	G4Region* ChamberRegion; // cut region
	G4Region* ElectrodeRegion; // cut region
	G4Region* EnvelopeRegion; // cut region


protected:
	G4LogicalVolume* fScoringVolume;
private:
	G4Cache<ElectricFieldSetup*> fEmFieldSetup;
	//G4Cache<SensitiveDetector*>  fElectrodeSD_A;
	//G4Cache<SensitiveDetector*>  fElectrodeSD_B;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

