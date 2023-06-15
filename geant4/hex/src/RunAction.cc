#include "RunAction.hh"
#include "DetectorConstruction.hh"

#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

namespace Hex{
	RunAction::RunAction(){
		G4RunManager::GetRunManager()->SetPrintProgress(1000);
		auto det_obj = static_cast<const DetectorConstruction*>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
		auto analysisManager = G4AnalysisManager::Instance();

		analysisManager->SetNtupleMerging(true);
		ntuple_id = analysisManager->CreateNtuple("Hits","");
		analysisManager->CreateNtupleIColumn("Event");
		analysisManager->CreateNtupleIColumn("Cell");
		analysisManager->CreateNtupleDColumn("Edep");
		analysisManager->CreateNtupleIColumn("Particle");
		analysisManager->FinishNtuple();
	}

	void RunAction::BeginOfRunAction(const G4Run* aRun){
		auto analysisManager = G4AnalysisManager::Instance();
		G4String fileName = "../hex_10000MeV.root";
		analysisManager->OpenFile(fileName);

	}

	void RunAction::EndOfRunAction(const G4Run* aRun){
		auto analysisManager = G4AnalysisManager::Instance();
		analysisManager->Write();
		analysisManager->CloseFile();
	}
}