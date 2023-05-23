#ifndef HEXDETECTORCONSTRUCTION
#define HEXDETECTORCONSTRUCTION

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4UserLimits.hh"

#include <vector>

namespace Hex{
	class DetectorConstruction: public G4VUserDetectorConstruction{
	public:
		DetectorConstruction();
		~DetectorConstruction() override;
		G4VPhysicalVolume* Construct() override;
		G4ThreeVector GetHexAssemblyCenter() const { return hexarr_center; }
		void ConstructSDandField() override;

	private:
		G4ThreeVector hexarr_center;
		G4int hex_count;
		std::vector<G4LogicalVolume*>* fLogHex = nullptr;
		G4UserLimits* fStepLimit = nullptr;
		G4bool fCheckOverlaps = true;
	};
}

#endif //HEXDETECTORCONSTRUCTION