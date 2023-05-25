#include "EventAction.hh"

#include "TrackerHit.hh"
#include "G4UnitsTable.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"

namespace Hex{
	void EventAction::BeginOfEventAction(const G4Event* anEvent){

	}

	void EventAction::EndOfEventAction(const G4Event* anEvent){
		auto analysisManager = G4AnalysisManager::Instance();
		auto HC = anEvent->GetHCofThisEvent()->GetHC(0);
		std::map<G4int,G4double> totalEdep;	//{hexNumber,totalEdep}
		G4int nofHits = HC->GetSize();
		for(int i = 0;i < nofHits;i++){
			auto hit = static_cast<TrackerHit*>(HC->GetHit(i));
			totalEdep[hit->GetChamberNb()] += hit->GetEdep();
		}
		//G4cout << "In this event:" << G4endl;
		//if(nofHits == 0)	G4cout<< "There were no hits." << G4endl;
		for(auto [hexNo,Edep]:totalEdep){
			analysisManager->FillNtupleIColumn(0,anEvent->GetEventID());
			analysisManager->FillNtupleIColumn(1,hexNo);
			analysisManager->FillNtupleDColumn(2,Edep/keV);
			analysisManager->AddNtupleRow();
			//G4cout << "Hex number " << hexNo << " registered " << G4BestUnit(Edep,"Energy") << G4endl;
		}
	}
}