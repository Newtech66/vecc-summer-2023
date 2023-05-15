#ifndef FPPrimaryGeneratorAction_h
#define FPPrimaryGeneratorAction_h

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;

namespace FP{
	class PrimaryGeneratorAction: public G4VUserPrimaryGeneratorAction{
		public:
		PrimaryGeneratorAction() = default;
		~PrimaryGeneratorAction() override = default;
		void GeneratePrimaries(G4Event*) override;

		private:
		G4ParticleGun* fParticleGun;
	}
}

#endif //FPPrimaryGeneratorAction_h