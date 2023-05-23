#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
#include "G4RunManager.hh"

#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

class G4ParticleDefinition;

namespace Hex{
	PrimaryGeneratorAction::PrimaryGeneratorAction(){
		G4int n_particle = 1;
		fParticleGun = new G4ParticleGun(n_particle);

		//particle
		G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
		G4String particleName;
		G4ParticleDefinition* particle = particleTable->FindParticle(particleName="e+");
		fParticleGun->SetParticleDefinition(particle);
		fParticleGun->SetParticleEnergy(100.*MeV);
	}

	PrimaryGeneratorAction::~PrimaryGeneratorAction(){
		delete fParticleGun;
	}	

	void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent){
		auto det_obj = static_cast<const DetectorConstruction*>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
		fParticleGun->SetParticlePosition(det_obj->GetHexAssemblyCenter() + G4ThreeVector(.0*cm,.0*cm,-10.*cm));
		G4double rand_x = (G4UniformRand())/5,rand_y = (G4UniformRand())/5;
		fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.)+G4ThreeVector(rand_x,rand_y));
		fParticleGun->GeneratePrimaryVertex(anEvent);
	}
}