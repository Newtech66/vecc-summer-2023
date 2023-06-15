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
		std::map<G4String, G4int> name_id_map{{"gamma",1},{"e-",2},{"e+",3}};
		auto analysisManager = G4AnalysisManager::Instance();
		auto HC = anEvent->GetHCofThisEvent()->GetHC(0);

		G4int nofHits = HC->GetSize();
		for(int i = 0;i < nofHits;i++){
			auto hit = static_cast<TrackerHit*>(HC->GetHit(i));
			analysisManager->FillNtupleIColumn(tupleid,0,anEvent->GetEventID());
			analysisManager->FillNtupleIColumn(tupleid,1,hit->GetChamberNb());
			analysisManager->FillNtupleDColumn(tupleid,2,hit->GetEdep()/keV);
			analysisManager->FillNtupleIColumn(tupleid,3,name_id_map[hit->GetParticleName()]);
			analysisManager->AddNtupleRow();
		}
	}
}