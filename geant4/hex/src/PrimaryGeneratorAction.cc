#include "PrimaryGeneratorAction.hh"

#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"

class G4ParticleDefinition;

namespace Hex{
	PrimaryGeneratorAction::PrimaryGeneratorAction(){
		G4int n_particle = 1;
		fParticleGun = new G4ParticleGun(n_particle);

		//particle
		G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
		G4String particleName;
		G4ParticleDefinition* particle = particleTable->FindParticle(particleName="gamma");
		fParticleGun->SetParticleDefinition(particle);
		fParticleGun->SetParticleMomentumDirection(G4ThreeVector(1.,0.,0.));
		fParticleGun->SetParticleEnergy(4.2*eV);
	}

	PrimaryGeneratorAction::~PrimaryGeneratorAction(){
		delete fParticleGun;
	}	

	void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent){
		fParticleGun->SetParticlePosition(G4ThreeVector(-9.0*m,.0*m,.0*m));
		fParticleGun->GeneratePrimaryVertex(anEvent);
	}
}