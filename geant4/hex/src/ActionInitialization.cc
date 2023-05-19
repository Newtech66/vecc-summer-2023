#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"

namespace Hex{
	void ActionInitialization::Build() const{
		SetUserAction(new PrimaryGeneratorAction);
	}
	void ActionInitialization::BuildForMaster() const{

	}
}