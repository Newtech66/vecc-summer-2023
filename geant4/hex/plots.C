#include <vector>
#include <memory>
#include <iostream>
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"

using namespace std;

TH1D *f1 = nullptr;
TH1D* f2 = nullptr;

tuple<Int_t,Double_t,Double_t> func(int energy){
    string filename = "../hex_"+to_string(energy)+"MeV.root";
    TFile* f = new TFile(filename.data());
    //There is a tree (ntuple) named Hits
    //It has branches: Event, Cell, Edep
    TTree* hits = (TTree*)f->Get("Hits");
    Int_t event,layer,cell,pid;
    Double_t edep;
    hits->SetBranchAddress("Event",&event);
    hits->SetBranchAddress("Layer",&layer);
    hits->SetBranchAddress("Cell",&cell);
    hits->SetBranchAddress("Edep",&edep);
    // hits->SetBranchAddress("Particle",&pid);
    map<Int_t,map<Int_t,Double_t>> evcellsum;
    map<Int_t,Double_t> evsum,evmax,evcell;
    for(int iHit = 0;hits->LoadTree(iHit) >= 0; iHit++){
        hits->GetEntry(iHit);
        if(layer == 1)  continue;
        evcellsum[event][cell] += edep;
        evsum[event] += edep;
    }
    for(auto p:evcellsum){
        Double_t mx = 0,cnt = 0;
        for(auto celle:p.second){
            mx = max(mx,celle.second);
            if(celle.second > 0.025)    evcell[p.first]++;
        }
        evmax[p.first] = mx;
    }
    TH1D* t1 = new TH1D("evsum","",10000,0,100);
    for(auto p:evsum){
        t1->Fill(p.second);
    }
    //auto result_sum = t1->Fit("landau","S Q N");
    //Double_t sum = *(result_sum->GetParams()+1);
    TH1D* t2 = new TH1D("evmax","",10000,0,100);
    for(auto p:evmax){
        t2->Fill(p.second);
    }
    //auto result_max = t2->Fit("landau","S Q N");
    //Double_t max = *(result_max->GetParams()+1);
    TH1D* t3 = new TH1D("evcell","",220,0,220);
    for(auto p:evcell){
        t3->Fill(p.second);
    }
    auto c1 = new TCanvas("c1","",1920,1080);
    c1->cd(1);
    //t3->Draw("HIST");
    TH2D* t4 = new TH2D("ncell_edep","",100,0,100,10000,0,100);
    for(auto p:evcell){
        t4->Fill(p.second,evsum[p.first]);
    }
    t4->Draw("HIST");
    //auto result_cell = t3->Fit("landau","S Q N");
    //Double_t max = *(result_max->GetParams()+1);
    //f->Close();
    //delete f;
    return make_tuple(0,0,0);
    //return make_tuple(0,max,sum);
}

void plots(){
    //vector<Double_t> energies{10,50,100,200,500,1000,10000};
    func(100);
    /*vector<Double_t> energies{100};
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
    g4->Draw(drawopts.data());*/
}