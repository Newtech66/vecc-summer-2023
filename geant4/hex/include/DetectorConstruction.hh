#ifndef HEXDETECTORCONSTRUCTION
#define HEXDETECTORCONSTRUCTION

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"

namespace Hex{
	class DetectorConstruction: public G4VUserDetectorConstruction{
	public:
		DetectorConstruction() = default;
		~DetectorConstruction() override = default;
		G4VPhysicalVolume* Construct() override;

	};
}

#endif //HEXDETECTORCONSTRUCTION