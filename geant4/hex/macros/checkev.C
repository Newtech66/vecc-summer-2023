#include "cluster.C"

pair<int,int> inv_hex_map(int hex){
    int hexcount = (hexrows/2)*(2*hexcols+1)+(hexrows%2)*hexcols;
    int q1 = hex/(2*hexcols+1);
    int r1 = hex%(2*hexcols+1);
    std::pair<int,int> rowcol;
    rowcol.first = 2*q1+(r1>=hexcols);
    rowcol.second = 2*r1+(r1>=hexcols?-2*hexcols:1);
    return rowcol;
}

void get_data(int energy,vector<tuple<Int_t,Int_t,Double_t>> &evcells){
    string filename = "../data_cu/hex_"+to_string(energy)+"MeV.root";
    TFile* f = new TFile(filename.data());
    TTree* hits = (TTree*)f->Get("Hits");
    Int_t event,layer,cell;
    Double_t edep;
    hits->SetBranchAddress("Event",&event);
    hits->SetBranchAddress("Layer",&layer);
    hits->SetBranchAddress("Cell",&cell);
    hits->SetBranchAddress("Edep",&edep);

    // set<pair<Int_t,Int_t>> evcells;
    // vector<tuple<Int_t,Int_t,Double_t>> evcells2;
    // map<Int_t,vector<Int_t>> cells;

    for(int iHit = 0;hits->LoadTree(iHit) >= 0; iHit++){
        hits->GetEntry(iHit);
        if(layer == 1)  continue;
        // evcells.insert({event,cell});
        evcells.push_back({event,cell,edep});
    }
    // for(auto p:evcells) cells[p.first].push_back(p.second);
}

void get_cells(vector<tuple<Int_t,Int_t,Double_t>> evcells, map<Int_t,vector<Int_t>> &cells){
    for(auto p:evcells) cells[get<0>(p)].push_back(get<1>(p));
    for(auto &p:cells){
        sort(p.second.begin(),p.second.end());
        p.second.erase(unique(p.second.begin(),p.second.end()),p.second.end());
    }
}

Double_t split_clust_frac(int energy){
    vector<tuple<Int_t,Int_t,Double_t>> evcells;
    map<Int_t,vector<Int_t>> cells;

    get_data(energy,evcells);
    get_cells(evcells,cells);

    Int_t single_clusts = 0;
    Int_t split_clusts = 0;
    for(auto p:cells){
        vector<pair<Int_t,Int_t>> data;
        for(auto cell:p.second){
            data.push_back(inv_hex_map(cell));
        }
        Int_t clusts=cluster(data);
        if(clusts>1)    split_clusts++;
        else    single_clusts++;
    }
    return (Double_t)split_clusts/(single_clusts+split_clusts);
}

void show_split_clusts(){
    vector<Double_t> energies{10,20,50,80,100,200,500,1000,5000,10000};
    vector<Double_t> split_clusts;
    for(auto energy:energies){
        split_clusts.push_back(split_clust_frac(energy));
    }
    for(int i=0;i<energies.size();i++){
        cout<<energies[i]<<'\t'<<split_clusts[i]<<endl;
    }
    string drawopts = "AL*";
    auto c1 = new TCanvas("split_clusts","",1920,1080);
    c1->cd(1);
    TGraph* g1 = new TGraph((Int_t)energies.size(),energies.data(),split_clusts.data());
    g1->SetTitle("Fraction of split clusters vs gamma energy;Gamma energy [MeV];Split cluster fraction");
    g1->Draw(drawopts.data());
}

void event_display(int energy,int cell_cut){
    vector<tuple<Int_t,Int_t,Double_t>> evcells;
    map<Int_t,vector<Int_t>> cells;

    get_data(energy,evcells);
    get_cells(evcells,cells);

    vector<int> keys;
    int key;
    for(auto p:cells){
        if(p.second.size()>=cell_cut){
            keys.push_back(p.first);
        }
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(keys.begin(),keys.end(),g);
    key=keys.at(0);
    TH1D* cnts = new TH1D("cnts","",20,0,20);
    for(auto p:cells){
        cnts->Fill((Int_t)p.second.size());
    }
    cnts->Draw();
    TCanvas* c1 = new TCanvas("event_display","",640,1080);
    TH2Poly* seehits = new TH2Poly();
    seehits->SetTitle("Event display");
    seehits->Honeycomb(0,0,1,hexcols+1,hexrows);
    for(auto p:evcells){
        if(get<0>(p)!=key)   continue;
        Double_t mvr = sqrt(3)/2;
        int row,col;
        tie(row,col)=inv_hex_map(get<1>(p));
        if(row%2==0){
            seehits->Fill(mvr*(1+col),1+1.5*row,get<2>(p));
        }else{
            seehits->Fill(mvr*(1+col),1+1.5*row,get<2>(p));
        }
    }
    // gStyle->SetOptStat(0);
    seehits->Draw("colz text");
}