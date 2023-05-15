#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"

namespace FP{
	void ActionInitialization::Build() const{
		SetUserAction(new PrimaryGeneratorAction);
	}
	void ActionInitialization::BuildForMaster() const{
		
	}
}