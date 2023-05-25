#ifndef HEXEVENTACTION
#define HEXEVENTACTION

#include "G4UserEventAction.hh"
#include "G4Event.hh"

namespace Hex{
	class EventAction: public G4UserEventAction{
	public:
		EventAction() = default;
		~EventAction() override = default;

		void BeginOfEventAction(const G4Event*) override;
		void EndOfEventAction(const G4Event*) override;
	};
}

#endif //HEXEVENTACTION