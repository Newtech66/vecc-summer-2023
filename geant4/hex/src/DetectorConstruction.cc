#include "DetectorConstruction.hh"
#include "G4NistManager.hh"
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
#include "globals.hh"


namespace Hex{
	G4VPhysicalVolume* DetectorConstruction::Construct(){
		G4NistManager* man = G4NistManager::Instance();

		//world
		auto world_mat = man->FindOrBuildMaterial("G4_AIR");
		auto solidWorld = new G4Box("World",
									6*cm,
									6*cm,
									6*cm);
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
											true);
		//hexagon
		auto hex_mat = man->FindOrBuildMaterial("G4_Cu");
		G4double hex_depth = 8.0*mm;
		G4double hex_rin = 5.0*mm;
		G4double hex_thickness = 0.8*mm;
		G4double hex_zplanes[] = {0.0*mm,hex_depth};
		G4double hex_rinner[] = {hex_rin,hex_rin};
		G4double hex_router[] = {hex_rin+hex_thickness,hex_rin+hex_thickness};
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

		//hexagon assembly
		int hex_rows = 3, hex_cols = 3;
		auto assemblyHex = new G4AssemblyVolume();
		G4ThreeVector t_hex;
		G4RotationMatrix r_hex;
		G4Transform3D tr_hex;
		tr_hex = G4Transform3D(r_hex,t_hex);
		assemblyHex->AddPlacedVolume(logHex,tr_hex);
		for(int row = 0;row < hex_rows;row++){
			G4double hex_R = 2*(hex_rin+hex_thickness)/sin(CLHEP::twopi/6.0);
			G4ThreeVector t_hexarr;
			G4double shift = 0.*mm;
			if(row % 2 == 1)	shift = hex_R*sin(CLHEP::twopi/6.0);
			for(int col = 0;col < hex_cols;col++){
				t_hexarr.setX(shift+col*hex_R);
				t_hexarr.setY(row*1.5*hex_R);
				G4Transform3D tr_hexarr(r_hex,t_hexarr);
				assemblyHex->MakeImprint(logWorld,tr_hexarr);
			}
		}

		return physWorld;
	}
}