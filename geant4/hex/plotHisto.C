{
	auto f_H = new TFile("../data_cu/hex_20MeV.root");
	auto c_H = new TCanvas("c_H","",1920,1080);
	c_H->cd(1);
	TH1D* hist = (TH1D*)f_H->Get("Events vs Edep");
	hist->Draw();
	auto result_H = hist->Fit("landau","S Q");
	cout<< "MPV" << " is " << *(result_H->GetParams()+1) << " keV" << endl;
	gStyle->SetOptFit();
}