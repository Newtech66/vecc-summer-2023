#ifndef HEXPRIMARYGENERATORACTION
#define HEXPRIMARYGENERATORACTION

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;

namespace Hex{
    class PrimaryGeneratorAction: public G4VUserPrimaryGeneratorAction{
        public:
        PrimaryGeneratorAction();
        ~PrimaryGeneratorAction() override;
        void GeneratePrimaries(G4Event*) override;
        const G4ParticleGun* GetParticleGun() const {return fParticleGun;}

        private:
        G4ParticleGun* fParticleGun = nullptr;
    };
}

#endif //HEXPRIMARYGENERATORACTION