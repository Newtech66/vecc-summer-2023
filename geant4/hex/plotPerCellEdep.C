{
	auto f = new TFile("hex.root");
	auto c1 = new TCanvas("c1","",1920,1080);
	int hexrows = 3,hexcols = 3;
	int hexcount = (hexrows/2)*(2*hexcols+1)+(hexrows%2)*hexcols;
	auto inv_hex_map = [hexcols](int hex){
		int q1 = hex/(2*hexcols+1);
		int r1 = hex%(2*hexcols+1);
		std::pair<int,int> rowcol;
		rowcol.first = 2*q1+(r1>=hexcols);
		rowcol.second = 2*r1+(r1>=hexcols?-2*hexcols:1);
		return rowcol;
	};
	c1->Divide(2*hexcols+1,hexrows);
	for(int i=0;i<hexcount;i++){
		string name = "Events vs Edep in cell " + std::to_string(i);
		TH1D* hist = (TH1D*)f->Get(name.data());
		auto rowcol = inv_hex_map(i);
		c1->cd(1+rowcol.second+rowcol.first*(2*hexcols+1));
		hist->Draw("HIST");
	}
}