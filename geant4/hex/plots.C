#include <vector>
#include <memory>
#include <iostream>
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"

using namespace std;

tuple<Int_t,Double_t,Double_t> func(int energy){
    string filename = "../data/hex_"+to_string(energy)+"MeV.root";
    TFile* f = new TFile(filename.data());
    //There is a tree (ntuple) named Hits
    //It has branches: Event, Cell, Edep
    TTree* hits = (TTree*)f->Get("Hits");
    Int_t event,cell;
    Double_t edep;
    hits->SetBranchAddress("Event",&event);
    hits->SetBranchAddress("Cell",&cell);
    hits->SetBranchAddress("Edep",&edep);
    set<Int_t> cells; //ncell = size of set
    map<Int_t,Double_t> cellmax,cellsum;
    Double_t sum_edep = 0; //max edep frac is max/sum
    for(int iHit = 0;hits->LoadTree(iHit) >= 0; iHit++){
        hits->GetEntry(iHit);
        // cellmax[cell] = max(edep,cellmax[cell]);
        cellsum[cell] += edep;
        sum_edep += edep;
    }
    Double_t cmax = 0;
    for(auto p:cellsum)    cmax = max(cmax,p.second);
    for(auto p:cellsum){
        if(p.second>cmax/100)    cells.insert(p.first);
    }
    f->Close();
    delete f;
    return make_tuple((Int_t)cells.size(),cmax,sum_edep);
}

void plots(){
    vector<Double_t> energies{10,50,100,200,500,1000,10000};
    vector<Double_t> ncells;
    vector<Double_t> sum_edep,max_edep_frac;
    for(auto energy:energies){
        Int_t ncell;
        Double_t sume,maxe;
        tie(ncell,maxe,sume) = func(energy);
        ncells.push_back(ncell);
        sum_edep.push_back(sume);
        max_edep_frac.push_back(maxe/sume);
    }
    for(auto x:ncells)  cout<<x<<" ";cout<<endl;
    for(auto x:sum_edep)  cout<<x<<" ";cout<<endl;
    for(auto x:max_edep_frac)  cout<<x<<" ";cout<<endl;
    // auto inv_hex_map = [hexcols](G4int hex){
    //     G4int q1 = hex/(2*hexcols+1);
    //     G4int r1 = hex%(2*hexcols+1);
    //     std::pair<G4int,G4int> rowcol;
    //     rowcol.first = 2*q1+(r1>=hexcols);
    //     rowcol.second = 2*r1+(r1>=hexcols?-2*hexcols:1);
    //     return rowcol;
    // };
    string drawopts = "AL*";
    auto c1 = new TCanvas("c1","",1920,1080);
    c1->Divide(2,2);
    c1->cd(1);
    TGraph* g1 = new TGraph((Int_t)energies.size(),energies.data(),ncells.data());
    g1->Draw(drawopts.data());
    c1->cd(2);
    TGraph* g2 = new TGraph((Int_t)energies.size(),energies.data(),sum_edep.data());
    g2->Draw(drawopts.data());
    c1->cd(3);
    TGraph* g3 = new TGraph((Int_t)energies.size(),energies.data(),max_edep_frac.data());
    g3->Draw(drawopts.data());
    c1->cd(4);
    vector<pair<Double_t,Double_t>> ncells_copy;
    vector<Double_t> a_c, b_c;
    for(int i = 0;i < (int)ncells.size();i++){
        ncells_copy.push_back({ncells[i], max_edep_frac[i]});
    }
    sort(ncells_copy.begin(),ncells_copy.end());
    for(auto p:ncells_copy) a_c.push_back(p.first), b_c.push_back(p.second);
    TGraph* g4 = new TGraph((Int_t)ncells.size(),a_c.data(),b_c.data());
    g4->Draw(drawopts.data());
}