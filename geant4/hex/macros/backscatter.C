Double_t backscatter_single(string pref, int energy){
    string filename = pref+to_string(energy)+"MeV.root";
    TFile* f = new TFile(filename.data());
    if(f == nullptr){
    	cout<<"File not found."<<endl;
    	return -1.0;
    }
    //There is a tree (ntuple) named Hits
    //It has branches: Event, Cell, Edep
    TTree* hits = (TTree*)f->Get("Hits");
    Int_t event,layer,cell,pid;
    Double_t edep;
    hits->SetBranchAddress("Event",&event);
    hits->SetBranchAddress("Layer",&layer);
    hits->SetBranchAddress("Cell",&cell);
    hits->SetBranchAddress("Edep",&edep);
    map<Int_t,map<Int_t,Double_t>> evcellsum;
    for(int iHit = 0;hits->LoadTree(iHit) >= 0; iHit++){
        hits->GetEntry(iHit);
        if(layer == 0)  continue;
        evcellsum[event][cell] += edep;
    }
    TH1D* ncells = new TH1D("ncells","",10,-0.5,9.5);
    for(auto p:evcellsum){
    	Int_t cnt = 0;
    	for(auto c:p.second){
    		if(c.second>0.025)	cnt++;
    	}
    	ncells->Fill(cnt);
    }
    Double_t tot = 0.0;
    for(Int_t bin=2;bin<=10;bin++){
    	tot += ncells->GetBinContent(bin);
    }
    // ncells->Draw();
    return tot;
    // return ncells->GetMean(1);
}

void backscatter(string pref){
    vector<Double_t> energies{10,20,50,80,100,200,500,1000,5000,10000};
    vector<Double_t> ncells;
    for(auto energy:energies){
        ncells.push_back(backscatter_single(pref,energy));
    }
    TCanvas* c1 = new TCanvas("backscatter","",1920,1080);
    string drawopts = "AL*";
    TGraph* g1 = new TGraph((Int_t)energies.size(),energies.data(),ncells.data());
    g1->SetTitle("NCells CPV vs gamma energy;Gamma energy [MeV];NCells CPV");
    g1->Draw(drawopts.data());
}