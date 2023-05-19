#ifndef HEXACTIONINITIALIZATION
#define HEXACTIONINITIALIZATION

#include "G4VUserActionInitialization.hh"

namespace Hex{
	class ActionInitialization: public G4VUserActionInitialization{
	public:
		ActionInitialization() = default;
		~ActionInitialization() override = default;
		void Build() const override;
		void BuildForMaster() const override;

	};
}

#endif //HEXACTIONINITIALIZATION