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

tuple<Double_t,Double_t,Double_t> func(string pref, int energy){
    string filename = pref+to_string(energy)+"MeV.root";
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
    map<Int_t,Int_t> evclust;
    map<Int_t,Double_t> evsum,evmax,evcell,evmaxfrac; //sum, max, cells, max frac
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

    for(auto p:evsum){
        evmaxfrac[p.first] = evmax[p.first]/p.second;
    }

    TH1D* t1 = new TH1D("evsum","",10000,0,100);
    for(auto p:evsum){
        t1->Fill(p.second);
    }
    auto result_sum = t1->Fit("landau","S Q N");
    Double_t sum = *(result_sum->GetParams()+1);

    TH1D* t2 = new TH1D("evmaxfrac","",100,0,1);
    for(auto p:evmaxfrac){
        t2->Fill(p.second);
    }
    //t2->Draw();
    //auto c1 = new TCanvas("c1","",1920,1080);
    //c1->cd(1);
    //t2->Draw();
    Double_t max_frac = t2->GetMaximumBin()/100.0;

    TH1D* t3 = new TH1D("evcell","",220,-0.5,219.5);
    for(auto p:evcell){
        // if(p.second == 1)   continue;
        t3->Fill(p.second);
    }
    //auto c1 = new TCanvas("c1","",1920,1080);
    //c1->cd(1);
    //t3->Draw("HIST");

    //TH2D* t4 = new TH2D("ncell_edep","",100,0,100,10000,0,100);
    // for(auto p:evcell){
    //     t4->Fill(p.second,evsum[p.first]);
    // }
    // t4->Draw("HIST");
    Double_t ncell = t3->GetMean(1);

    TH1D* t4 = new TH1D("evclust","",5,0,5);
    for(auto p:evcellsum){
        vector<pair<Int_t,Int_t>> data;
        for(auto cell:p.second){
            data.push_back(inv_hex_map(cell.first));
        }
        evclust[p.first]=cluster(data);
        t4->Fill(evclust[p.first]);
    }
    t4->Draw();
    //f->Close();
    //delete f;
    //return make_tuple(0,0,0);
    return make_tuple(ncell,max_frac,sum);
}

void plots(){
    vector<Double_t> energies{10,20,50,80,100,200,500,1000,5000,10000};
    vector<Double_t> ncells;
    vector<Double_t> sum_edep,max_edep_frac;
    for(auto energy:energies){
        Double_t ncell;
        Double_t sume,max_frace;
        tie(ncell,max_frace,sume) = func("./hex_",energy);
        ncells.push_back(ncell);
        sum_edep.push_back(sume);
        max_edep_frac.push_back(max_frace);
    }
    for(auto x:ncells)  cout<<x<<" ";cout<<endl;
    for(auto x:sum_edep)  cout<<x<<" ";cout<<endl;
    for(auto x:max_edep_frac)  cout<<x<<" ";cout<<endl;

    string drawopts = "AL*";
    auto c1 = new TCanvas("c1","",1920,1080);
    c1->Divide(2,2);
    c1->cd(1);
    TGraph* g1 = new TGraph((Int_t)energies.size(),energies.data(),ncells.data());
    g1->SetTitle("NCells vs gamma energy;Gamma energy [MeV];NCells");
    g1->Draw(drawopts.data());
    c1->cd(2);
    TGraph* g2 = new TGraph((Int_t)energies.size(),energies.data(),sum_edep.data());
    g2->SetTitle("Sum of Edep vs gamma energy;Gamma energy [MeV];Sum of Edep (MPV) [KeV]");
    g2->Draw(drawopts.data());
    c1->cd(3);
    TGraph* g3 = new TGraph((Int_t)energies.size(),energies.data(),max_edep_frac.data());
    g3->SetTitle("Max Edep fraction vs gamma energy;Gamma energy [MeV];Max Edep fraction (MPV)");
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
    g4->SetTitle("Max Edep fraction vs NCells;NCells;Max Edep fraction (MPV)");
    g4->Draw(drawopts.data());
}

void overlay(){
    vector<Double_t> energies{10,20,50,80,100,200,500,1000,5000,10000};
    vector<Double_t> ncells_1,ncells_2,ncells_3;
    vector<Double_t> sum_edep_1,sum_edep_2,sum_edep_3;
    for(auto energy:energies){
        Double_t ncell;
        Double_t sume,max_frace;
        tie(ncell,max_frace,sume) = func("../data_air/hex_",energy);
        sum_edep_1.push_back(sume);
        ncells_1.push_back(ncell);
    }
    for(auto energy:energies){
        Double_t ncell;
        Double_t sume,max_frace;
        tie(ncell,max_frace,sume) = func("../data_cu/hex_",energy);
        sum_edep_2.push_back(sume);
        ncells_2.push_back(ncell);
    }
    for(auto energy:energies){
        Double_t ncell;
        Double_t sume,max_frace;
        tie(ncell,max_frace,sume) = func("../data_cu_0.1/hex_",energy);
        sum_edep_3.push_back(sume);
        ncells_3.push_back(ncell);
    }
    //TGraph* g1 = new TGraph((Int_t)energies.size(),energies.data(),sum_edep_1.data());
    //g1->SetTitle("Sum of Edep vs gamma energy;Gamma energy [MeV];Sum of Edep (MPV) [KeV]");
    TGraph* g1 = new TGraph((Int_t)energies.size(),energies.data(),ncells_1.data());
    g1->SetTitle("Ncells vs gamma energy;Gamma energy [MeV];Ncells (mean)");
    g1->SetLineColor(4);    //BLUE is air
    g1->Draw("AL*");
    //TGraph* g2 = new TGraph((Int_t)energies.size(),energies.data(),sum_edep_2.data());
    TGraph* g2 = new TGraph((Int_t)energies.size(),energies.data(),ncells_2.data());
    // g1->SetTitle("Sum of Edep vs gamma energy;Gamma energy [MeV];Sum of Edep (MPV) [KeV]");
    g2->SetLineColor(2);    //RED is 0.4
    g2->Draw("L*");
    TGraph* g3 = new TGraph((Int_t)energies.size(),energies.data(),ncells_3.data());
    // g1->SetTitle("Sum of Edep vs gamma energy;Gamma energy [MeV];Sum of Edep (MPV) [KeV]");
    g3->SetLineColor(3);    //GREEN is 0.2
    g3->Draw("L*");
}

TH1D* get_spectrum(string pref,int energy){
    string filename = pref+to_string(energy)+"MeV.root";
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
    map<Int_t,Double_t> evsum;
    for(int iHit = 0;hits->LoadTree(iHit) >= 0; iHit++){
        hits->GetEntry(iHit);
        if(layer == 1)  continue;
        evsum[event] += edep;
    }

    TH1D* t1 = new TH1D("evsum","",10000,0,100);
    for(auto p:evsum){
        t1->Fill(p.second);
    }
    return t1;
}

void show_spectra(){
    vector<Double_t> energies{10,20,50,80,100,200,500,1000,5000,10000};
    auto c1 = new TCanvas("c1","Air",1920,1080);
    c1->Divide(5,2);
    for(int i=0;i<energies.size();i++){
        c1->cd(i+1);
        auto p = get_spectrum("../data_air/hex_",energies[i]);
        p->Draw();
    }
    auto c2 = new TCanvas("c2","Cu 0.4",1920,1080);
    c2->Divide(5,2);
    for(int i=0;i<energies.size();i++){
        c2->cd(i+1);
        auto p = get_spectrum("../data_cu/hex_",energies[i]);
        p->Draw();
    }
}