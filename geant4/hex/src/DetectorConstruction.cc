#include "DetectorConstruction.hh"
#include "TrackerSD.hh"

#include "G4SDManager.hh"
#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4AssemblyVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Polyhedra.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "globals.hh"

namespace Hex{
	DetectorConstruction::DetectorConstruction(){
		fLogHex = new std::vector<G4LogicalVolume*>();
	}

	DetectorConstruction::~DetectorConstruction(){
		delete fLogHex;
		delete fStepLimit;
	}

	G4VPhysicalVolume* DetectorConstruction::Construct(){
		G4NistManager* man = G4NistManager::Instance();

		//world
		auto world_mat = man->FindOrBuildMaterial("G4_AIR");
		G4double world_x = 1*m, world_y = 1*m, world_z = 1*m;
		auto solidWorld = new G4Box("World",
									world_x,
									world_y,
									world_z);
		auto logWorld = new G4LogicalVolume(solidWorld,
											world_mat,
											"World");
		auto physWorld = new G4PVPlacement(nullptr,
											G4ThreeVector(),
											logWorld,
											"World",
											nullptr,
											false,
											0,
											fCheckOverlaps);
		//hexagon
		auto hex_mat = man->FindOrBuildMaterial("G4_Cu");
		int hex_rows = 3, hex_cols = 3;
		G4double hex_depth = 8.0*mm;
		G4double hex_rin = 5.0*mm;
		G4double hex_thickness = 0.2*mm;
		G4double hex_rout = hex_rin+hex_thickness;
		G4double hex_zplanes[] = {0.0*mm,hex_depth};
		G4double hex_rinner[] = {hex_rin,hex_rin};
		G4double hex_router[] = {hex_rout,hex_rout};
		auto solidHex = new G4Polyhedra("hex",
										.0,
										CLHEP::twopi,
										6,
										2,
										hex_zplanes,
										hex_rinner,
										hex_router);
		auto logHex = new G4LogicalVolume(solidHex,
											hex_mat,
											"hex");
		/*auto physHex = new G4PVPlacement(nullptr,
											G4ThreeVector(),
											logHex,
											"hex",
											logWorld,
											false,
											0,
											true);*/
		//tracker region
		auto tracker_mat = man->FindOrBuildMaterial("G4_AIR");
		auto solidTracker = new G4Box("Tracker",
									5*hex_rout*tan(30*deg)*(3*hex_rows+1),
									5*hex_rout*(hex_cols+1),
									5*hex_depth);
		auto logTracker = new G4LogicalVolume(solidTracker,
											tracker_mat,
											"Tracker");

		//physical tracker placement
		auto physTracker = new G4PVPlacement(nullptr,
											G4ThreeVector(),
											logTracker,
											"Tracker",
											logWorld,
											false,
											0,
											fCheckOverlaps);

		//hexagon assembly
		auto assemblyHex = new G4AssemblyVolume();
		G4ThreeVector t_hex;
		G4RotationMatrix r_hex;
		r_hex.rotateZ(15*deg);
		G4Transform3D tr_hex;
		tr_hex = G4Transform3D(r_hex,t_hex);
		assemblyHex->AddPlacedVolume(logHex,tr_hex);
		for(int row = 0;row < hex_rows;row++){
			G4ThreeVector t_hexarr;
			G4double shift = 0.*mm;
			if(row % 2 == 1)	shift = -hex_rout;
			for(int col = 0;col < hex_cols+(row%2);col++){
				t_hexarr.setX(shift+col*hex_rout*2);
				t_hexarr.setY(row*tan(twopi/6.0)*hex_rout);
				G4Transform3D tr_hexarr(r_hex,t_hexarr);
				assemblyHex->MakeImprint(logTracker,tr_hexarr);
				hex_count++;
				hexarr_center += t_hexarr;
				G4cout <<"SEE THIS " << t_hexarr << G4endl;
			}
		}
		hexarr_center /= hex_count;

		//Ar-CO2 mixture
		auto Ar = man->FindOrBuildMaterial("G4_Ar");
		auto CO2 = man->FindOrBuildMaterial("G4_CARBON_DIOXIDE");
		G4double gas_density = 0.7*Ar->GetDensity()
								+0.3*CO2->GetDensity();
		auto gas_mat = new G4Material("AR_CO2",gas_density,2);
		gas_mat->AddMaterial(Ar,70*perCent);
		gas_mat->AddMaterial(CO2,30*perCent);
		G4double gas_depth = 8.0*mm;
		G4double gas_rin = 0.0*mm;
		G4double gas_thickness = 5.0*mm;
		G4double gas_zplanes[] = {0.0*mm,gas_depth};
		G4double gas_rinner[] = {gas_rin,gas_rin};
		G4double gas_router[] = {gas_rin+gas_thickness,gas_rin+gas_thickness};
		auto solidGas = new G4Polyhedra("gas",
										.0,
										CLHEP::twopi,
										6,
										2,
										gas_zplanes,
										gas_rinner,
										gas_router);
		auto logGas = new G4LogicalVolume(solidGas,
											hex_mat,
											"gas");
		
		//gas assembly
		/*auto assemblyGas = new G4AssemblyVolume();
		G4ThreeVector t_gas;
		G4RotationMatrix r_gas;
		r_gas.rotateZ(15*deg);
		G4Transform3D tr_gas;
		tr_gas = G4Transform3D(r_gas,t_gas);
		assemblyGas->AddPlacedVolume(logGas,tr_gas);
		for(int row = 0;row < hex_rows;row++){
			G4double gas_R = (hex_thickness+gas_thickness);
			G4ThreeVector t_gasarr;
			G4double shift = 0.*mm;
			if(row % 2 == 1)	shift = -gas_R;
			for(int col = 0;col < hex_cols+(row%2);col++){
				t_gasarr.setX(shift+col*gas_R*2);
				t_gasarr.setY(row*tan(twopi/6.0)*gas_R);
				G4Transform3D tr_gasarr(r_gas,t_gasarr);
				assemblyGas->MakeImprint(logWorld,tr_gasarr);
				
			}
		}*/

		//set step limits
		G4double maxStep = 0.1*hex_depth;
		fStepLimit = new G4UserLimits(maxStep);

		G4ThreeVector t_gas;
		G4RotationMatrix r_gas;
		r_gas.rotateZ(30*deg);
		G4Transform3D tr_gas;
		for(int row = 0,copyNo = 0;row < hex_rows;row++){
			G4double gas_R = (hex_thickness+gas_thickness);
			G4ThreeVector t_gasarr;
			G4double shift = 0.*mm;
			if(row % 2 == 1)	shift = -gas_R;
			for(int col = 0;col < hex_cols+(row%2);col++,copyNo++){
				t_gasarr.setX(shift+col*gas_R*2);
				t_gasarr.setY(row*tan(twopi/6.0)*gas_R);
				G4Transform3D tr_gasarr(r_gas,t_gasarr);
				fLogHex->push_back(new G4LogicalVolume(solidGas,gas_mat,"DetectorLV",nullptr,nullptr,nullptr));
				fLogHex->back()->SetUserLimits(fStepLimit);
				new G4PVPlacement(tr_gasarr,
								fLogHex->back(),
								"DetectorPV",
								logTracker,
								false,
								(G4int)fLogHex->size()-1,
								fCheckOverlaps);
			}
		}

		return physWorld;
	}

	void DetectorConstruction::ConstructSDandField(){
	  // Sensitive detectors

	  G4String trackerChamberSDname = "/TrackerChamberSD";
	  auto aTrackerSD = new TrackerSD(trackerChamberSDname, "TrackerHitsCollection");
	  G4SDManager::GetSDMpointer()->AddNewDetector(aTrackerSD);
	  // Setting aTrackerSD to all logical volumes with the same name
	  // of "Chamber_LV".
	  SetSensitiveDetector("DetectorLV", aTrackerSD, true);
	}
}