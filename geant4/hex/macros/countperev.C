void countperev(){
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
    Int_t evt;
    hits->SetBranchAddress("Event",&evt);

    TH1I* evtcnt = new TH1I("evtcnt","Counts per event",57,0,57);

    for(int iHit = 0;hits->LoadTree(iHit) >= 0; iHit++){
        hits->GetEntry(iHit);
        evtcnt->Fill(evt);
    }
    // evtcnt->Draw();

    TH1I* cnts = new TH1I("cnts","cnts",19000,0,19000);
    for(int bin=1;bin<=56;bin++){
        Int_t val = evtcnt->GetBinContent(bin);
        cout<<bin-1<<" "<<val<<endl;
        // if(1000<=val && val<=2000)   cout<<bin-1<<endl;
        if(val>0)   cnts->Fill(val);
    }
    cnts->Draw();
}