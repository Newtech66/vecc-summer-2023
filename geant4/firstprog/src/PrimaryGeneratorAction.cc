#include "PrimaryGeneratorAction.hh"

#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"

class G4ParticleDefinition;

namespace FP{
	PrimaryGeneratorAction::PrimaryGeneratorAction(){
		G4int n_particle = 1;
		fParticleGun = new G4ParticleGun(n_particle);

		//particle
		G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
		G4String particleName;
		G4ParticleDefinition* particle = particleTable->FindParticle(particleName="gamma");
		fParticleGun->SetParticleDefinition(particle);
		fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
		fParticleGun->SetParticleEnergy(6.*MeV);
	}

	PrimaryGeneratorAction::~PrimaryGeneratorAction(){
		delete fParticleGun;
	}	

	PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent){
		fParticleGun->SetParticlePosition(G4ThreeVector(-2.0*m,.0*m,.0*m));
		fParticleGun->GeneratePrimaryVertex(anEvent);
	}
}