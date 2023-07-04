#include "cluster.C"

// const int hexcols=10;
// const int hexrows=20;

pair<int,int> inv_hex_map(int hex){
    int hexcount = (hexrows/2)*(2*hexcols+1)+(hexrows%2)*hexcols;
    int q1 = hex/(2*hexcols+1);
    int r1 = hex%(2*hexcols+1);
    std::pair<int,int> rowcol;
    rowcol.first = 2*q1+(r1>=hexcols);
    rowcol.second = 2*r1+(r1>=hexcols?-2*hexcols:1);
    return rowcol;
}

Double_t checkev(int energy,int cell_cut){
    string filename = "../data_cu/hex_"+to_string(energy)+"MeV.root";
    TFile* f = new TFile(filename.data());
    TTree* hits = (TTree*)f->Get("Hits");
    Int_t event,layer,cell;
    Double_t edep;
    hits->SetBranchAddress("Event",&event);
    hits->SetBranchAddress("Layer",&layer);
    hits->SetBranchAddress("Cell",&cell);
    hits->SetBranchAddress("Edep",&edep);

    set<pair<Int_t,Int_t>> evcells;
    vector<tuple<Int_t,Int_t,Double_t>> evcells2;
    map<Int_t,vector<Int_t>> cells;

    for(int iHit = 0;hits->LoadTree(iHit) >= 0; iHit++){
        hits->GetEntry(iHit);
        if(layer == 1)  continue;
        evcells.insert({event,cell});
        evcells2.push_back({event,cell,edep});
    }
    for(auto p:evcells) cells[p.first].push_back(p.second);
    Int_t single_clusts = 0;
    Int_t split_clusts = 0;
    for(auto p:cells){
        vector<pair<Int_t,Int_t>> data;
        for(auto cell:p.second){
            data.push_back(inv_hex_map(cell));
        }
        // for(auto cell:data) cout<<cell.first<<" "<<cell.second<<endl;
        Int_t clusts=cluster(data);
        if(clusts>1)    split_clusts++;
        else    single_clusts++;
    }
    return (Double_t)split_clusts/(single_clusts+split_clusts);
    // vector<int> keys;
    // int key;
    // for(auto p:cells){
    //     if(p.second.size()>=cell_cut){
    //         keys.push_back(p.first);
    //     }
    // }
    // std::random_device rd;
    // std::mt19937 g(rd());
    // std::shuffle(keys.begin(),keys.end(),g);
    // key=keys.at(0);
    // TH1D* cnts = new TH1D("cnts","",20,0,20);
    // for(auto p:cells){
    //     cnts->Fill((Int_t)p.second.size());
    // }
    // cnts->Draw();
    // TCanvas* c1 = new TCanvas("10cell","",640,1080);
    // TH2Poly* seehits = new TH2Poly();
    // seehits->SetTitle("Event display");
    // seehits->Honeycomb(0,0,1,hexcols+1,hexrows);
    // for(auto p:evcells2){
    //     if(get<0>(p)!=key)   continue;
    //     double mvup = 2/sqrt(3);
    //     int row,col;
    //     tie(row,col)=inv_hex_map(get<1>(p));
    //     seehits->Fill(1+col,mvup+1+row,get<2>(p));
    // }
    // // gStyle->SetOptStat(0);
    // seehits->Draw("colz text");
}

void show_split_clusts(){
    vector<Double_t> energies{10,20,50,80,100,200,500,1000,5000,10000};
    vector<Double_t> split_clusts;
    for(auto energy:energies){
        split_clusts.push_back(checkev(energy,0));
        // split_clusts.push_back(1);
    }
    for(int i=0;i<energies.size();i++){
        cout<<energies[i]<<'\t'<<split_clusts[i]<<endl;
    }
    string drawopts = "AL*";
    auto c1 = new TCanvas("c1","",1920,1080);
    c1->cd(1);
    TGraph* g1 = new TGraph((Int_t)energies.size(),energies.data(),split_clusts.data());
    g1->SetTitle("No. of split clusters vs gamma energy;Gamma energy [MeV];Split cluster count");
    g1->Draw(drawopts.data());
}