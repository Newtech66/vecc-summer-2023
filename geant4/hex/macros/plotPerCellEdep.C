{
	auto f_PCE = new TFile("hex.root");
	auto c_PCE = new TCanvas("c_PCE","",1920,1080);
	int hexrows = 3,hexcols = 3;
	int hexcount = (hexrows/2)*(2*hexcols+1)+(hexrows%2)*hexcols;
	auto inv_hex_map = [&](int hex){
		int q1 = hex/(2*hexcols+1);
		int r1 = hex%(2*hexcols+1);
		std::pair<int,int> rowcol;
		rowcol.first = 2*q1+(r1>=hexcols);
		rowcol.second = 2*r1+(r1>=hexcols?-2*hexcols:1);
		return rowcol;
	};
	/*string name = "Events vs Edep in cell 0";
	TH1D* hist = (TH1D*)f->Get(name.data());
	c_PCE->cd(1);
	hist->Draw("HIST");*/
	c_PCE->Divide(2*hexcols+1,hexrows);
	double mean = 0.0;
	for(int i=0;i<hexcount;i++){
		//auto canvas_name = string(1,'c')+std::to_string(i);
		//auto c_PCE = new TCanvas(canvas_name.data(),"",1920,1080);
		string name = "Events vs Edep in cell " + std::to_string(i);
		TH1D* hist = (TH1D*)f_PCE->Get(name.data());
		auto rowcol = inv_hex_map(i);
		c_PCE->cd(1+rowcol.second+rowcol.first*(2*hexcols+1));
		hist->Draw();
		//hist->Draw("HIST");
		auto result_PCE = hist->Fit("landau","S Q");
		cout<< "MPV for cell " << i << " is " << *(result_PCE->GetParams()+1) << " keV" << endl;
		mean += *(result_PCE->GetParams()+1);
		gStyle->SetOptFit();
	}
	cout<< "Mean MPV" << " is " << mean/hexcount << " keV" << endl;
}