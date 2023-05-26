#ifndef HEXRUNACTION
#define HEXRUNACTION

#include "G4UserRunAction.hh"
#include "G4Run.hh"

namespace Hex{
	class RunAction: public G4UserRunAction{
	public:
		RunAction();
		~RunAction() override = default;

		void BeginOfRunAction(const G4Run*) override;
		void EndOfRunAction(const G4Run*) override;

		G4int GetNtupleId() const { return ntuple_id; }
		G4int GetEventHistoId() const { return event_histo_id; }
		G4int GetCellHistoId() const { return cell_histo_id; }

	private:
		G4int ntuple_id = -1;
		G4int event_histo_id = -1;
		G4int cell_histo_id = -1;
	};
}

#endif //HEXRUNACTION