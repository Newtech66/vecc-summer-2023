#include "clusterreal.C"

void get_data(int detector,int module,int event,vector<tuple<Int_t,Int_t,Double_t>> &evcells){
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
        if(det==detector && smn==module && evt==event){
            evcells.push_back({row,col,edep});
        }
    }
}

TH2Poly* event_histo(int detector,int module,int event){
    vector<tuple<Int_t,Int_t,Double_t>> evcells;
    get_data(detector,module,event,evcells);

    TH2Poly* seehits = new TH2Poly();
    // seehits->SetTitle("Event display");
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
}

void event_display(int detector,int module,int event){
    TH2Poly* seehits = event_histo(detector,module,event);
    string title = "Event display of event " + to_string(event)+ " in module " + to_string(module) + " in " + (detector==0?"PRE":"CPV") + " plane";
    TCanvas* c1 = new TCanvas("event_display","",1920,800);
    gStyle->SetOptStat(0);
    seehits->SetTitle(title.data());
    seehits->Draw("collz");
}

void event_display_all_modules(int detector,int event){
    string title = "Event display of event " + to_string(event) + " in " + (detector==0?"PRE":"CPV") + " plane";
    TCanvas* c1 = new TCanvas("event_display_all_modules",title.data(),1920,800);
    c1->Divide(5,4);
    TH2Poly* seehits;
    int curPad=1;
    gStyle->SetOptStat(0);
    for(int smn=0;smn<24;smn++){
        if(smn==0 || smn==6 || smn==13 || smn==19)  continue;
        c1->cd(curPad);
        seehits = event_histo(detector,smn,event);
        string title = "Module number "+to_string(smn);
        seehits->SetTitle(title.data());
        seehits->Draw("collz");
        curPad++;
    }
}

void adcspec(int detector,int module){
    string title = "ADC spectrum for isolated cells in module " + to_string(module) + " in " + (detector==0?"PRE":"CPV") + " plane";
    TH1D* adcspec = new TH1D("adcspec",title.data(),18000,0,1800);
    for(int event=0;event<56;event++){
        vector<tuple<Int_t,Int_t,Double_t>> evcells;
        get_data(detector,module,event,evcells);
        vector<pair<Int_t,Int_t>> cells;
        for(auto p:evcells) cells.push_back({get<0>(p),get<1>(p)});
        vector<Int_t> cl = cluster(cells);
        map<Int_t,Int_t> cnts;
        for(auto label:cl)  cnts[label]++;
        vector<Int_t> single_labels;
        for(auto p:cnts){
            if(p.first<0)   continue;
            if(p.second==1) single_labels.push_back(p.first);
        }
        for(int cell = 0;cell<cl.size();cell++){
            if(binary_search(single_labels.begin(),single_labels.end(),cl[cell])){
                adcspec->Fill(get<2>(evcells[cell]));
            }
        }
    }
    TCanvas* c1 = new TCanvas("adcspec","",1920,800);
    adcspec->Draw();
}