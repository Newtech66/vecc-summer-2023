#include "clusterreal.C"

void get_data(int module,int event,vector<tuple<Int_t,Int_t,Double_t>> &evcells){
    string filename = "../AnalysisResults.root";
    TFile* f = new TFile(filename.data());
    if(f == nullptr){
        cout<<"Could not find file."<<endl;
        return;
    }
    TList* dir = (TList*)f->Get("PMD");
    if(dir == nullptr){
        cout<<"Could not find directory."<<endl;
        return;
    }
    TTree* hits = (TTree*)dir->FindObject("fTreeModule");
    if(hits == nullptr){
        cout<<"Could not find tree."<<endl;
        return;
    }
    Int_t evt,det,smn,row,col;
    Float_t edep;
    hits->SetBranchAddress("Event",&evt);
    hits->SetBranchAddress("Detector",&det);
    hits->SetBranchAddress("Module",&smn);
    hits->SetBranchAddress("Row",&row);
    hits->SetBranchAddress("Column",&col);
    hits->SetBranchAddress("ADC",&edep);

    for(int iHit = 0;hits->LoadTree(iHit) >= 0; iHit++){
        hits->GetEntry(iHit);
         // && evt==event
        if(det==0 && smn==module && evt==event){
            evcells.push_back({row,col,edep});
        }
    }
}

TH2Poly* event_histo(int module,int event){
    vector<tuple<Int_t,Int_t,Double_t>> evcells;
    get_data(module,event,evcells);

    TH2Poly* seehits = new TH2Poly();
    seehits->SetTitle("Event display");
    seehits->Honeycomb(0,0,1,hexcols+1,hexrows);
    for(auto p:evcells){
        Double_t mvr = sqrt(3)/2;
        Int_t row,col;
        Double_t edep;
        tie(row,col,edep) = p;
        if(row%2==0){
            seehits->Fill(mvr+2*mvr*col,1+1.5*row,edep);
        }else{
            seehits->Fill(2*mvr+2*mvr*col,1+1.5*row,edep);
        }
    }
    return seehits;

    // TCanvas* c1 = new TCanvas("event_display","",1920,800);
    // gStyle->SetOptStat(0);
    // seehits->Draw("collz");
}

void event_display(int module,int event){
    TH2Poly* seehits = event_histo(module,event);
    TCanvas* c1 = new TCanvas("event_display","",1920,800);
    // gStyle->SetOptStat(0);
    seehits->Draw("collz");
}

void event_display_all_modules(int event){
    TCanvas* c1 = new TCanvas("event_display","",1920,800);
    c1->Divide(5,4);
    TH2Poly* seehits;
    int curPad=1;
    gStyle->SetOptStat(0);
    for(int smn=0;smn<24;smn++){
        if(smn==0 || smn==6 || smn==13 || smn==19)  continue;
        c1->cd(curPad);
        seehits = event_histo(smn,event);
        string title = "Module number "+to_string(smn);
        seehits->SetTitle(title.data());
        seehits->Draw("collz");
        curPad++;
    }
}

//Events with <=2000 hits
// 2
// 7
// 9
// 10
// 11
// 12
// 18
// 19
// 27
// 31
// 32
// 36
// 43
// 44
// 45
// 47
// 51
// 56

// Events with 1000 <= hits <= 2000
// 7
// 9
// 12
// 44
// 51

//Events | Hits
