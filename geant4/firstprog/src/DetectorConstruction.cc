#include "DetectorConstruction.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"

class G4Material;

namespace FP{
    G4VPhysicalVolume* DetectorConstruction::Construct(){
        G4NistManager* nist = G4NistManager::Instance();

        G4bool checkOverlaps = true;

        //World
        G4Material* world_mat = nist->FindOrBuildMaterial("G4_WATER");
        G4double world_x = 10.0*m;
        G4double world_y = 3.0*m;
        G4double world_z = 3.0*m;
        auto worldBox = new G4Box("World",world_x,world_y,world_z);
        auto logWorld = new G4LogicalVolume(worldBox,world_mat,"World");
        auto physWorld = new G4PVPlacement(nullptr,
            G4ThreeVector(),
            logWorld,
            "World",
            nullptr,
            false,
            0,
            checkOverlaps);

        //Tracker
        G4Material* trk_mat = nist->FindOrBuildMaterial("G4_Al");
        auto trackerCyl = new G4Tubs("Tracker",0,1.0*m,2.0*m,0,CLHEP::twopi);
        auto logTracker = new G4LogicalVolume(trackerCyl,trk_mat,"Tracker");
        auto physTracker = new G4PVPlacement(new G4RotationMatrix(CLHEP::pi/2.0,CLHEP::pi/2.0,CLHEP::pi/2.0),
            G4ThreeVector(),
            logTracker,
            "Tracker",
            logWorld,
            false,
            0,
            checkOverlaps);

        return physWorld;
    }
}


