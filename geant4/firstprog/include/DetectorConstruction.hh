#ifndef FPDetectorConstruction_h
#define FPDetectorConstruction_h

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;

namespace FP{
    class DetectorConstruction: public G4VUserDetectorConstruction{
        public:
        DetectorConstruction() = default;
        ~DetectorConstruction() override = default;
        G4VPhysicalVolume* Construct() override;
    }
}

#endif //FPDetectorConstruction_h
