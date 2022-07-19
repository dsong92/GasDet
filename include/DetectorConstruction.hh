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
#include "G4ElectricField.hh"

#include "globals.hh"
#include "G4Cache.hh"
#include "G4Region.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class DetectorMessenger;

class ElectricFieldSetup;
class G4UniformMagField;

class geom;

/// Detector construction class to define materials and geometry.

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
	DetectorConstruction();
	virtual ~DetectorConstruction();

	virtual G4VPhysicalVolume* Construct();
	virtual void ConstructSDandField();

	G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }

	G4ElectricField* fEMfield;

	G4Region* ChamberRegion;		// cut region
	G4Region* ElectrodeRegion;	    // cut region
	G4Region* EnvelopeRegion;		// cut region
	G4Region* PipeRegion;			// cut region
	G4Region* BeamCheckRegion;	    // cut region
	G4Region* BeamCheckRegion2;	    // cut region

	void SetGeom(geom* ig) { myg = ig; };
	geom* GetGeom() const { return myg; };

protected:
	G4LogicalVolume* fScoringVolume;
private:
	G4Cache<ElectricFieldSetup*> fEmFieldSetup;
	DetectorMessenger* dm;
	geom* myg;

};

class geom // **** 0701 *******
{
public:
	geom()
	{
		const G4double cm = 10.0, mm = 1.0;

		// drift length
		drift_l = 10.0 * cm;

		// Oxigen free copper photon absorber, box shape
		phab_w = 6.0 * cm;
		phab_h = 3.0 * cm;
		phab_l = 5.0 * cm;

		// aluminum flange, box shape
		flan_w = 41.3 * cm;
		flan_h = 10.0 * cm;
		flan_l = 3.0 * cm;

		// lead shield plate
		lead_w = 2.0 * flan_w;
		lead_h = 10.0 * flan_h;
		lead_l = 2.0 * mm;

		// gaps
		gap_btw_phab_flan = 112.0 * mm;
		gap_btw_flan_pb = 5.0 * mm;

		// margin
		world_margin = 2.0 * cm;
	};
	~geom();

	G4double get_starting_point() const
	{
		return -0.5 * get_length_sum();
	};

	G4double get_length_sum() const
	{
		return drift_l + phab_l + gap_btw_phab_flan + flan_l + gap_btw_flan_pb + lead_l;

	};
public:
	// drift length
	G4double       drift_l;

	// Oxigen free copper photon absorber, box shape
	G4double        phab_w, phab_h, phab_l;

	// aluminum flange, box shape
	G4double        flan_w, flan_h, flan_l;

	// lead shield plate
	G4double        lead_w, lead_h, lead_l;

	G4double        gap_btw_phab_flan, gap_btw_flan_pb;

	G4double        world_margin;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

