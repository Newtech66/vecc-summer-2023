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
		ntuple_id = analysisManager->CreateNtuple("hex","Edep per cell");
		analysisManager->CreateNtupleIColumn("Event#");
		analysisManager->CreateNtupleIColumn("Cell#");
		analysisManager->CreateNtupleDColumn("Edep_keV");
		analysisManager->FinishNtuple();

		event_histo_id = analysisManager->CreateH1("Events vs Edep","Number of events as a function of Edep",
													1000,0.,10.*keV,"keV");
		//G4cout<<"SEE THIS HISTO STUFF "<<det_obj->GetHexCols()<<" "<<det_obj->GetHexRows()<<G4endl;
		cell_histo_id = analysisManager->CreateH2("Cell plot of Edep","Counts",
													2*(det_obj->GetHexCols())+1,0,2*(det_obj->GetHexCols())+1,
													det_obj->GetHexRows(),0,det_obj->GetHexRows());
		// cell_histo_id = analysisManager->CreateH2("Cell plot of Edep","Counts",
		// 											100,0,10,
		// 											100,0,10);
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