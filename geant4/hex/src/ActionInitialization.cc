#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"

namespace Hex{
	void ActionInitialization::Build() const{
		SetUserAction(new PrimaryGeneratorAction);
		SetUserAction(new RunAction);
		SetUserAction(new EventAction);
	}
	void ActionInitialization::BuildForMaster() const{
		SetUserAction(new RunAction);
	}
}