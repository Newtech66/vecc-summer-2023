#include "RunAction.hh"

#include "G4AnalysisManager.hh"

namespace Hex{
	RunAction::RunAction(){
		auto analysisManager = G4AnalysisManager::Instance();
		analysisManager->SetNtupleMerging(true);
		analysisManager->CreateNtuple("hex","Edep per cell");
		analysisManager->CreateNtupleIColumn("Event#");
		analysisManager->CreateNtupleIColumn("Cell#");
		analysisManager->CreateNtupleDColumn("Edep_keV");
		analysisManager->FinishNtuple();
	}

	void RunAction::BeginOfRunAction(const G4Run* aRun){
		auto analysisManager = G4AnalysisManager::Instance();
		G4String fileName = "hex.root";
		analysisManager->OpenFile(fileName);

	}

	void RunAction::EndOfRunAction(const G4Run* aRun){
		auto analysisManager = G4AnalysisManager::Instance();
		analysisManager->Write();
		analysisManager->CloseFile();
	}
}