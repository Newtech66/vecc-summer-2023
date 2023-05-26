#include "EventAction.hh"
#include "RunAction.hh"
#include "TrackerHit.hh"
#include "DetectorConstruction.hh"

#include "G4UnitsTable.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

namespace Hex{
	void EventAction::BeginOfEventAction(const G4Event* anEvent){

	}

	void EventAction::EndOfEventAction(const G4Event* anEvent){
		auto run = static_cast<const RunAction*>(G4RunManager::GetRunManager()->GetUserRunAction());
		G4int tupleid = run->GetNtupleId();
		G4int eventhistoid = run->GetEventHistoId();
		G4int cellhistoid = run->GetCellHistoId();
		//G4cout<<"SEE THIS CELLHISTOID "<<cellhistoid<<G4endl;
		auto analysisManager = G4AnalysisManager::Instance();
		auto HC = anEvent->GetHCofThisEvent()->GetHC(0);
		auto det_obj = static_cast<const DetectorConstruction*>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
		G4int hexcols = det_obj->GetHexCols();
		auto inv_hex_map = [hexcols](G4int hex){
			G4int q1 = hex/(2*hexcols+1);
			G4int r1 = hex%(2*hexcols+1);
			std::pair<G4int,G4int> rowcol;
			rowcol.first = 2*q1+(r1>=hexcols);
			rowcol.second = 2*r1+(r1>=hexcols?-2*hexcols:1);
			return rowcol;
		};

		std::map<G4int,G4double> totalEdep;	//{hexNumber,totalEdep}
		G4double EdepOfEvent = 0.;
		G4int nofHits = HC->GetSize();
		for(int i = 0;i < nofHits;i++){
			auto hit = static_cast<TrackerHit*>(HC->GetHit(i));
			totalEdep[hit->GetChamberNb()] += hit->GetEdep();
			EdepOfEvent += hit->GetEdep();
		}
		if(EdepOfEvent>0.)	analysisManager->FillH1(eventhistoid,EdepOfEvent);
		//G4cout << "In this event:" << G4endl;
		//if(nofHits == 0)	G4cout<< "There were no hits." << G4endl;
		//G4cout << tupleid <<" "<<eventhistoid<<G4endl;
		for(auto [hexNo,Edep]:totalEdep){
			analysisManager->FillNtupleIColumn(tupleid,0,anEvent->GetEventID());
			analysisManager->FillNtupleIColumn(tupleid,1,hexNo);
			analysisManager->FillNtupleDColumn(tupleid,2,Edep/keV);
			analysisManager->AddNtupleRow();
			//analysisManager->FillH2(cellhistoid,0,0);
			//G4cout<<"SEE THIS HEX "<<hexNo<<" "<<inv_hex_map(hexNo).first<<" "<<inv_hex_map(hexNo).second<<G4endl;
			analysisManager->FillH2(cellhistoid,inv_hex_map(hexNo).second,inv_hex_map(hexNo).first);
			//G4cout << "Hex number " << hexNo << " registered " << G4BestUnit(Edep,"Energy") << G4endl;
		}
	}
}