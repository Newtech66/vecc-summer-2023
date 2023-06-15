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
		G4int GetHexRows() const { return hex_rows; }
		G4int GetHexCols() const { return hex_cols; }
		void ConstructSDandField() override;

	private:
		G4ThreeVector hexarr_center;
		G4int hex_rows = 20;
		G4int hex_cols = 10;
		G4int hex_count = 0;
		std::vector<G4LogicalVolume*>* fLogHex = nullptr;
		G4UserLimits* fGasStepLimit = nullptr;
		G4UserLimits* fConvStepLimit = nullptr;
		G4UserLimits* fPlateStepLimit = nullptr;
		G4bool fCheckOverlaps = true;
	};
}

#endif //HEXDETECTORCONSTRUCTION