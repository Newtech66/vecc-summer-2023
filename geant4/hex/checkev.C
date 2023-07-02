const int hexcols=20;
const int hexrows=10;

pair<int,int> inv_hex_map(int hex){
    int hexcount = (hexrows/2)*(2*hexcols+1)+(hexrows%2)*hexcols;
    int q1 = hex/(2*hexcols+1);
    int r1 = hex%(2*hexcols+1);
    std::pair<int,int> rowcol;
    rowcol.first = 2*q1+(r1>=hexcols);
    rowcol.second = 2*r1+(r1>=hexcols?-2*hexcols:1);
    return rowcol;
}

void checkev(){
    TFile* f = new TFile("hex_500MeV.root");
    TTree* hits = (TTree*)f->Get("Hits");
    Int_t event,layer,cell;
    hits->SetBranchAddress("Event",&event);
    hits->SetBranchAddress("Layer",&layer);
    hits->SetBranchAddress("Cell",&cell);
    set<pair<Int_t,Int_t>> evcells;
    for(int iHit = 0;hits->LoadTree(iHit) >= 0; iHit++){
        hits->GetEntry(iHit);
        if(layer == 1)  continue;
        evcells.insert({event,cell});
    }
    map<Int_t,vector<Int_t>> cells;
    for(auto p:evcells) cells[p.first].push_back(p.second);
    TH1D* cnts = new TH1D("cnts","",20,0,20);
    for(auto p:cells){
        cnts->Fill((Int_t)p.second.size());
    }
    cnts->Draw();
    // TH2D* seehits = new TH2D("hits","",21,0,21,20,0,20);
    // auto it=evcells.begin();
    // for(int i=0;i<100;i++)  ++it;
    // Int_t ev = it->first;
    // for(auto p:evcells){
    //     int row,col;
    //     tie(row,col)=inv_hex_map(p.second);
    //     if(p.first==ev) seehits->Fill(col,row);
    // }
    // seehits->Draw();
}