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
		analysisManager->CreateNtupleIColumn("Layer"); 	//0 is PSP, 1 is CPV
		analysisManager->CreateNtupleIColumn("Cell");
		analysisManager->CreateNtupleDColumn("Edep");
		analysisManager->CreateNtupleIColumn("Particle");	//{"gamma",1},{"e-",2},{"e+",3}
		analysisManager->FinishNtuple();
	}

	void RunAction::BeginOfRunAction(const G4Run* aRun){
		auto analysisManager = G4AnalysisManager::Instance();
		G4String fileName = "../data_air/80MeV.root";
		analysisManager->OpenFile(fileName);

	}

	void RunAction::EndOfRunAction(const G4Run* aRun){
		auto analysisManager = G4AnalysisManager::Instance();
		analysisManager->Write();
		analysisManager->CloseFile();
	}
}