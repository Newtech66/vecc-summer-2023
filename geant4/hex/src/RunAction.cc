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

		event_hist_id = analysisManager->CreateH1("Events vs Edep","Number of events as a function of Edep",
													1000,0.,10.*keV,"keV");
		cell_hist_id = analysisManager->CreateH2("Cell plot of counts","Counts per cell",
													2*(det_obj->GetHexCols())+1,0,2*(det_obj->GetHexCols())+1,
													det_obj->GetHexRows(),0,det_obj->GetHexRows());
		cell_edep_hist_id = analysisManager->CreateH2("Cell plot of cumulative Edep","Energy deposited per cell",
													2*(det_obj->GetHexCols())+1,0,2*(det_obj->GetHexCols())+1,
													det_obj->GetHexRows(),0,det_obj->GetHexRows());
		G4int hexrows = det_obj->GetHexRows(),hexcols = det_obj->GetHexCols();
		G4int hexcount = (hexrows/2)*(2*hexcols+1)+(hexrows%2)*hexcols;
		auto inv_hex_map = [hexcols](G4int hex){
			G4int q1 = hex/(2*hexcols+1);
			G4int r1 = hex%(2*hexcols+1);
			std::pair<G4int,G4int> rowcol;
			rowcol.first = 2*q1+(r1>=hexcols);
			rowcol.second = 2*r1+(r1>=hexcols?-2*hexcols:1);
			return rowcol;
		};
		rc_edep = new std::map<int,G4int>();
		for(int hexNo=0;hexNo<hexcount;hexNo++){
			G4String name = "Events vs Edep in cell " + std::to_string(hexNo);
			G4String title = "Number of events as a function of Edep in cell " + std::to_string(hexNo);
			rc_edep->insert({hexNo,-1});
			rc_edep->at(hexNo) = analysisManager->CreateH1(name,title,1000,0.,10.*keV,"keV");
		}
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