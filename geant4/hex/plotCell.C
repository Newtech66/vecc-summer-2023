{
	auto f = new TFile("hex.root");
	auto c1 = new TCanvas("c1","",1920,640);
	c1->Divide(2,1);
	c1->cd(1);
	TH2D* hist = (TH2D*)f->Get("Cell plot of counts");
	hist->Draw("HIST");
	c1->cd(2);
	//c1->cd(1);
	TH2D* hist2 = (TH2D*)f->Get("Cell plot of Edep");
	hist2->Draw("HIST");
}