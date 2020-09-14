#include <iostream>

void hfled_colz(){
	gROOT->SetBatch(kTRUE);
	TFile *f = new TFile;
	TCanvas *c1 = new TCanvas("c1","HF- Mean Charge",1500,1000);
	TCanvas *c2 = new TCanvas("c2","HF- Stdev Charge",1500,1000);
	TCanvas *c3 = new TCanvas("c3","HF- Gain",1500,1000);
	TCanvas *c4 = new TCanvas("c4","HF+ Mean Charge",1500,1000);
	TCanvas *c5 = new TCanvas("c5","HF+ Stdev Charge",1500,1000);
	TCanvas *c6 = new TCanvas("c6","HF+ Gain",1500,1000);
	c1->Divide(2,2);
	c2->Divide(2,2);
	c3->Divide(2,2);
	c4->Divide(2,2);
	c5->Divide(2,2);
	c6->Divide(2,2);
	TH2F *h = new TH2F;
	char inName[1024];
	char outName[1024];

	gSystem->Exec("mkdir Results");
	gSystem->Exec("ls | grep .root | sed 's/[^0-9]*//g;s/^.//' > led_runs.txt");	// Refresh the led_runs.txt
	gSystem->Exec("a=`wc -l < led_runs.txt`;echo \"$a)\" > .led_colz_tmp");
	ifstream runs("led_runs.txt");
	string run;

	int i=0;
	if(runs.is_open())
	while(getline(runs,run)){
		i++;
		f = TFile::Open(("HFP5comm_"+run+".root").c_str());

		cout<<endl<<"("<<i<<"/"<<ifstream(".led_colz_tmp").rdbuf();
		cout<<"Processing: "<<f->GetName()<<endl;

		c1->cd(1);
		h = (TH2F*)f->Get("2D/hfm_1");
		h->SetStats(0);
		h->SetXTitle("ieta");
		h->SetYTitle("iphi");
		h->SetZTitle("Charge (fC)");
		h->Draw("colz");

		c1->cd(2);
		h = (TH2F*)f->Get("2D/hfm_2");
		h->SetStats(0);
		h->SetXTitle("ieta");
		h->SetYTitle("iphi");
		h->SetZTitle("Charge (fC)");
		h->Draw("colz");

		c1->cd(3);
		h = (TH2F*)f->Get("2D/hfm_3");
		h->SetStats(0);
		h->SetXTitle("ieta");
		h->SetYTitle("iphi");
		h->SetZTitle("Charge (fC)");
		h->Draw("colz");

		c1->cd(4);
		h = (TH2F*)f->Get("2D/hfm_4");
		h->SetStats(0);
		h->SetXTitle("ieta");
		h->SetYTitle("iphi");
		h->SetZTitle("Charge (fC)");
		h->Draw("colz");

		c1->SaveAs(("Results/"+run+"_mean_q_m.png").c_str());

		c2->cd(1);
		h = (TH2F*)f->Get("2D/stdevm_1");
		h->SetStats(0);
		h->SetXTitle("ieta");
		h->SetYTitle("iphi");
		h->SetZTitle("Charge (fC)");
		h->Draw("colz");

		c2->cd(2);
		h = (TH2F*)f->Get("2D/stdevm_2");
		h->SetStats(0);
		h->SetXTitle("ieta");
		h->SetYTitle("iphi");
		h->SetZTitle("Charge (fC)");
		h->Draw("colz");

		c2->cd(3);
		h = (TH2F*)f->Get("2D/stdevm_3");
		h->SetStats(0);
		h->SetXTitle("ieta");
		h->SetYTitle("iphi");
		h->SetZTitle("Charge (fC)");
		h->Draw("colz");

		c2->cd(4);
		h = (TH2F*)f->Get("2D/stdevm_4");
		h->SetStats(0);
		h->SetXTitle("ieta");
		h->SetYTitle("iphi");
		h->SetZTitle("Charge (fC)");
		h->Draw("colz");

		c2->SaveAs(("Results/"+run+"_stdev_q_m.png").c_str());

		c3->cd(1);
		h = (TH2F*)f->Get("2D/gainm_1");
		h->SetStats(0);
		h->SetXTitle("ieta");
		h->SetYTitle("iphi");
		h->SetZTitle("Gain");
		h->Draw("colz");

		c3->cd(2);
		h = (TH2F*)f->Get("2D/gainm_2");
		h->SetStats(0);
		h->SetXTitle("ieta");
		h->SetYTitle("iphi");
		h->SetZTitle("Gain");
		h->Draw("colz");

		c3->cd(3);
		h = (TH2F*)f->Get("2D/gainm_3");
		h->SetStats(0);
		h->SetXTitle("ieta");
		h->SetYTitle("iphi");
		h->SetZTitle("Gain");
		h->Draw("colz");

		c3->cd(4);
		h = (TH2F*)f->Get("2D/gainm_4");
		h->SetStats(0);
		h->SetXTitle("ieta");
		h->SetYTitle("iphi");
		h->SetZTitle("Gain");
		h->Draw("colz");

		c3->SaveAs(("Results/"+run+"_gain_m.png").c_str());

		c4->cd(1);
		h = (TH2F*)f->Get("2D/hfp_1");
		h->SetStats(0);
		h->SetXTitle("ieta");
		h->SetYTitle("iphi");
		h->SetZTitle("Charge (fC)");
		h->Draw("colz");

		c4->cd(2);
		h = (TH2F*)f->Get("2D/hfp_2");
		h->SetStats(0);
		h->SetXTitle("ieta");
		h->SetYTitle("iphi");
		h->SetZTitle("Charge (fC)");
		h->Draw("colz");

		c4->cd(3);
		h = (TH2F*)f->Get("2D/hfp_3");
		h->SetStats(0);
		h->SetXTitle("ieta");
		h->SetYTitle("iphi");
		h->SetZTitle("Charge (fC)");
		h->Draw("colz");

		c4->cd(4);
		h = (TH2F*)f->Get("2D/hfp_4");
		h->SetStats(0);
		h->SetXTitle("ieta");
		h->SetYTitle("iphi");
		h->SetZTitle("Charge (fC)");
		h->Draw("colz");

		c4->SaveAs(("Results/"+run+"_mean_q_p.png").c_str());

		c5->cd(1);
		h = (TH2F*)f->Get("2D/stdevp_1");
		h->SetStats(0);
		h->SetXTitle("ieta");
		h->SetYTitle("iphi");
		h->SetZTitle("Charge (fC)");
		h->Draw("colz");

		c5->cd(2);
		h = (TH2F*)f->Get("2D/stdevp_2");
		h->SetStats(0);
		h->SetXTitle("ieta");
		h->SetYTitle("iphi");
		h->SetZTitle("Charge (fC)");
		h->Draw("colz");

		c5->cd(3);
		h = (TH2F*)f->Get("2D/stdevp_3");
		h->SetStats(0);
		h->SetXTitle("ieta");
		h->SetYTitle("iphi");
		h->SetZTitle("Charge (fC)");
		h->Draw("colz");

		c5->cd(4);
		h = (TH2F*)f->Get("2D/stdevp_4");
		h->SetStats(0);
		h->SetXTitle("ieta");
		h->SetYTitle("iphi");
		h->SetZTitle("Charge (fC)");
		h->Draw("colz");

		c5->SaveAs(("Results/"+run+"_stdev_q_p.png").c_str());

		c6->cd(1);
		h = (TH2F*)f->Get("2D/gainp_1");
		h->SetStats(0);
		h->SetXTitle("ieta");
		h->SetYTitle("iphi");
		h->SetZTitle("Gain");
		h->Draw("colz");

		c6->cd(2);
		h = (TH2F*)f->Get("2D/gainp_2");
		h->SetStats(0);
		h->SetXTitle("ieta");
		h->SetYTitle("iphi");
		h->SetZTitle("Gain");
		h->Draw("colz");

		c6->cd(3);
		h = (TH2F*)f->Get("2D/gainp_3");
		h->SetStats(0);
		h->SetXTitle("ieta");
		h->SetYTitle("iphi");
		h->SetZTitle("Gain");
		h->Draw("colz");

		c6->cd(4);
		h = (TH2F*)f->Get("2D/gainp_4");
		h->SetStats(0);
		h->SetXTitle("ieta");
		h->SetYTitle("iphi");
		h->SetZTitle("Gain");
		h->Draw("colz");

		c6->SaveAs(("Results/"+run+"_gain_p.png").c_str());
		}
	f->Close();
	runs.close();
	c1->Close();
	c2->Close();
	c3->Close();
	c4->Close();
	c5->Close();
	c6->Close();
	gSystem->Exec("rm .led_colz_tmp");
}
