#include <vector>
#include <memory>
#include <iostream>
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"

using namespace std;

void plots(){
    std::unique_ptr<TFile> f(TFile::Open("hex_gamma.root"));
    //There is a tree (ntuple) named Hits
    //It has branches: Event, Cell, Edep
    auto hits = f->Get<TTree>("Hits");
    int event,cell,edep;
    hits->SetBranchAddress("Event",&event);
    hits->SetBranchAddress("Cell",&cell);
    hits->SetBranchAddress("Edep",&edep);
    for(int iHit = 0;hits->LoadTree(iHit) >= 0; iHit++){
        hits->GetEntry(iHit);
        cout<<event<<" "<<cell<<" "<<edep<<endl;
    }
    // auto inv_hex_map = [hexcols](G4int hex){
    //     G4int q1 = hex/(2*hexcols+1);
    //     G4int r1 = hex%(2*hexcols+1);
    //     std::pair<G4int,G4int> rowcol;
    //     rowcol.first = 2*q1+(r1>=hexcols);
    //     rowcol.second = 2*r1+(r1>=hexcols?-2*hexcols:1);
    //     return rowcol;
    // };
}