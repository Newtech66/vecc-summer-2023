#include "DetectorConstruction.hh"

#include "G4Box.hh"
#include "G4Cylinder.hh"
#include "G4VLogicalVolume.hh"
#include "G4VPVPlacement.hh"
#include "G4SystemOfUnits.hh"

namespace FP{
    G4VPhysicalVolume* DetectorConstruction::Construct(){
        G4bool checkOverlaps = true;

        //World
        G4double world_x = 3.0*m;
        G4double world_y = 3.0*m;
        G4double world_z = 3.0*m;
        auto worldBox = new G4VBox("World",world_x,world_y,world_z);
        auto logWorld = new G4VLogicalVolume(worldBox,Ar,"World");
        auto physWorld = new G4VPVPlacement(nullptr,
            G4ThreeVector(),
            logWorld,
            "World",
            nullptr,
            false,
            0,
            checkOverlaps);

        //Tracker
        auto trackerCyl = new G4Tubs("Tracker",0,1.0*cm,0.5*m,0,twopi);
        auto logTracker = new G4VLogicalVolume(trackerCyl,Al,"Tracker");
        auto physTracker = new G4VPVPlacement(nullptr,
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


