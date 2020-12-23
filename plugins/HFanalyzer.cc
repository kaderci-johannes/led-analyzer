// -*- C++ -*-
//
// Package:    HFanalyzer
// Class:      HFanalyzer
// 
/**\class HFanalyzer HFanalyzer.cc UserCode/HFanalyzer/src/HFanalyzer.cc

   Description: [one line class summary]

   Implementation:
   [Notes on implementation]
*/
//

// user include files
#include "EventFilter/HcalRawToDigi/interface/HcalUnpacker.h"
#include "EventFilter/HcalRawToDigi/interface/HcalDCCHeader.h"
#include "EventFilter/HcalRawToDigi/interface/HcalDTCHeader.h"
#include "EventFilter/HcalRawToDigi/interface/AMC13Header.h"
#include "EventFilter/HcalRawToDigi/interface/HcalHTRData.h"
#include "EventFilter/HcalRawToDigi/interface/HcalUHTRData.h"
#include "DataFormats/HcalDetId/interface/HcalOtherDetId.h"
#include "DataFormats/HcalDigi/interface/HcalQIESample.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "EventFilter/HcalRawToDigi/interface/HcalTTPUnpacker.h"



#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "EventFilter/HcalRawToDigi/interface/HcalHTRData.h"
#include "EventFilter/HcalRawToDigi/interface/HcalDCCHeader.h"
#include "EventFilter/HcalRawToDigi/interface/HcalUnpacker.h"
#include "DataFormats/HcalDetId/interface/HcalOtherDetId.h"
#include "DataFormats/HcalDigi/interface/HcalQIESample.h"
#include "DataFormats/HcalDigi/interface/QIE10DataFrame.h"
#include "DataFormats/HcalDetId/interface/HcalSubdetector.h"
#include "DataFormats/HcalDetId/interface/HcalCalibDetId.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h"
#include "DataFormats/FEDRawData/interface/FEDHeader.h"
#include "DataFormats/FEDRawData/interface/FEDTrailer.h"
#include "DataFormats/FEDRawData/interface/FEDNumbering.h"
#include "DataFormats/FEDRawData/interface/FEDRawData.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"
#include "CalibFormats/HcalObjects/interface/HcalCalibrations.h"
#include "CalibFormats/HcalObjects/interface/HcalCoderDb.h"

//#include "HFcommissioning/Analysis/interface/ADC_Conversion.h"
#include "RecoTBCalo/HcalTBObjectUnpacker/interface/HcalTBSlowDataUnpacker.h"

#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
#include "TF1.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TFile.h"
#include "TSystem.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include "unpack_name.h"
#include <bitset>

using namespace std;


#define NUMADCS 256


//#define nQIEs   0
#define iPHIe  36
#define iETAe  26
#define nQ      4   // # of quadrants
#define nD      4   // # of depths

// NEEDS UPDATING
double adc2fC_QIE10[NUMADCS]={
  // - - - - - - - range 0 - - - - - - - -
  //subrange0 
  1.58, 4.73, 7.88, 11.0, 14.2, 17.3, 20.5, 23.6, 
  26.8, 29.9, 33.1, 36.2, 39.4, 42.5, 45.7, 48.8,
  //subrange1
  53.6, 60.1, 66.6, 73.0, 79.5, 86.0, 92.5, 98.9,
  105, 112, 118, 125, 131, 138, 144, 151,
  //subrange2
  157, 164, 170, 177, 186, 199, 212, 225,
  238, 251, 264, 277, 289, 302, 315, 328,
  //subrange3
  341, 354, 367, 380, 393, 406, 418, 431,
  444, 464, 490, 516, 542, 568, 594, 620,

  // - - - - - - - range 1 - - - - - - - -
  //subrange0
  569, 594, 619, 645, 670, 695, 720, 745,
  771, 796, 821, 846, 871, 897, 922, 947,
  //subrange1
  960, 1010, 1060, 1120, 1170, 1220, 1270, 1320,
  1370, 1430, 1480, 1530, 1580, 1630, 1690, 1740,
  //subrange2
  1790, 1840, 1890, 1940,  2020, 2120, 2230, 2330,
  2430, 2540, 2640, 2740, 2850, 2950, 3050, 3150,
  //subrange3
  3260, 3360, 3460, 3570, 3670, 3770, 3880, 3980,
  4080, 4240, 4450, 4650, 4860, 5070, 5280, 5490,
  
  // - - - - - - - range 2 - - - - - - - - 
  //subrange0
  5080, 5280, 5480, 5680, 5880, 6080, 6280, 6480,
  6680, 6890, 7090, 7290, 7490, 7690, 7890, 8090,
  //subrange1
  8400, 8810, 9220, 9630, 10000, 10400, 10900, 11300,
  11700, 12100, 12500, 12900, 13300, 13700, 14100, 14500,
  //subrange2
  15000, 15400, 15800, 16200, 16800, 17600, 18400, 19300,
  20100, 20900, 21700, 22500, 23400, 24200, 25000, 25800,
  //subrange3
  26600, 27500, 28300, 29100, 29900, 30700, 31600, 32400,
  33200, 34400, 36100, 37700, 39400, 41000, 42700, 44300,

  // - - - - - - - range 3 - - - - - - - - -
  //subrange0
  41100, 42700, 44300, 45900, 47600, 49200, 50800, 52500,
  54100, 55700, 57400, 59000, 60600, 62200, 63900, 65500,
  //subrange1
  68000, 71300, 74700, 78000, 81400, 84700, 88000, 91400,
  94700, 98100, 101000, 105000, 108000, 111000, 115000, 118000,
  //subrange2
  121000, 125000, 128000, 131000, 137000, 145000, 152000, 160000,
  168000, 176000, 183000, 191000, 199000, 206000, 214000, 222000,
  //subrange3
  230000, 237000, 245000, 253000, 261000, 268000, 276000, 284000,
  291000, 302000, 316000, 329000, 343000, 356000, 370000, 384000};
////////////////////////////////////////////////////////////////////////////////////////////
static const float adc2fC[128]={-0.5,0.5,1.5,2.5,3.5,4.5,5.5,6.5,7.5,8.5,9.5, 10.5,11.5,12.5,
		   13.5,15.,17.,19.,21.,23.,25.,27.,29.5,32.5,35.5,38.5,42.,46.,50.,54.5,59.5,
		   64.5,59.5,64.5,69.5,74.5,79.5,84.5,89.5,94.5,99.5,104.5,109.5,114.5,119.5,
		   124.5,129.5,137.,147.,157.,167.,177.,187.,197.,209.5,224.5,239.5,254.5,272.,
		   292.,312.,334.5,359.5,384.5,359.5,384.5,409.5,434.5,459.5,484.5,509.5,534.5,
		   559.5,584.5,609.5,634.5,659.5,684.5,709.5,747.,797.,847.,897.,947.,997.,
		   1047.,1109.5,1184.5,1259.5,1334.5,1422.,1522.,1622.,1734.5,1859.5,1984.5,
		   1859.5,1984.5,2109.5,2234.5,2359.5,2484.5,2609.5,2734.5,2859.5,2984.5,
		   3109.5,3234.5,3359.5,3484.5,3609.5,3797.,4047.,4297.,4547.,4797.,5047.,
		   5297.,5609.5,5984.5,6359.5,6734.5,7172.,7672.,8172.,8734.5,9359.5,9984.5};
////////////////////////////////////////////////////////////////////////////////////////////







int EventNumber;
const int _nsteps = 15;
const int _eps = 2000;
TFile *_file[_nsteps];

TH1F *AllSum[_nsteps][iETAe][iPHIe][nD];			// A 1D histogram for each (ieta, iphi, depth). x axis: Charge, y axis: counts (LED)
TH1F *Ped[_nsteps][iETAe][iPHIe][nD];			// A 1D histogram for each (ieta, iphi, depth). x axis: Charge, y axis: counts (Pedestal)
TProfile *psd[_nsteps][iETAe][iPHIe][nD];		// An array of profile histograms that contain pulse shape distributions. x axis: Time slice, y: Average ADC, y_err: STDEV ADC
TProfile *EvByEv[_nsteps][iETAe][iPHIe][nD];		// An array of profile histograms that contain the peak charge of each event. x axis: Event number, y: Peak charge
TProfile2D *hfp[_nsteps][4][nD];				// An array of 2D histograms for various depths of HF+. x axis: ieta, y: iphi, z: Mean charge
TProfile2D *hfm[_nsteps][4][nD];				// An array of 2D histograms for various depths of HF-. x axis: ieta, y: iphi, z: Mean charge
TProfile2D *stdevp[_nsteps][4][nD];				// An array of 2D histograms for various depths of HF+. x axis: ieta, y: iphi, z: Stdev of the pulse shape distribution.
TProfile2D *stdevm[_nsteps][4][nD];				// An array of 2D histograms for various depths of HF-. x axis: ieta, y: iphi, z: Stdev of the pulse shape distribution.
TProfile2D *gainp[_nsteps][4][nD];				// An array of 2D histograms for various depths of HF+. x axis: ieta, y: iphi, z: PMT Gain
TProfile2D *gainm[_nsteps][4][nD];				// An array of 2D histograms for various depths of HF-. x axis: ieta, y: iphi, z: PMT Gain

vector<vector<vector<vector<double>>>> Ev;

//TF1 *fit;					// Fit function for the pulse shape distribution.

//
// class declaration
//

class HFanalyzer : public edm::EDAnalyzer {
public:
  explicit HFanalyzer(const edm::ParameterSet&);
  ~HFanalyzer();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);

  virtual void endJob() ;

  int numChannels;
  string _outFileName;
  int _verbosity;
  int _mode;
  string _run;
  //int _eps;
  //int _nsteps;

  virtual void beginRun(edm::Run const&, edm::EventSetup const&);
  virtual void endRun(edm::Run const&, edm::EventSetup const&);
  virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
  virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

  edm::EDGetTokenT<HcalDataFrameContainer<QIE10DataFrame> > tok_QIE10DigiCollection_;
  edm::Handle<QIE10DigiCollection> qie10DigiCollection;

  edm::EDGetTokenT<HFDigiCollection> hf_token;

  //Slow data stuffs - raw_token is slow data
  //bool _sequencer_flag;
  edm::EDGetTokenT<FEDRawDataCollection> raw_token;
  edm::Handle<FEDRawDataCollection> raw_collection;

  hcaltb::HcalTBSlowDataUnpacker sdp;

};




HFanalyzer::HFanalyzer(const edm::ParameterSet& iConfig) :
  _outFileName(iConfig.getUntrackedParameter<string>("OutFileName")),
  _verbosity(iConfig.getUntrackedParameter<int>("Verbosity")),
  _mode(iConfig.getUntrackedParameter<int>("Mode")),
  _run(iConfig.getUntrackedParameter<string>("Run"))
  /*_sequencer_flag(iConfig.getUntrackedParameter<int>("Sequencer_Flag")),*/
  /*_eps(iConfig.getUntrackedParameter<int>("Eps"))*/
  /*_nsteps(iConfig.getUntrackedParameter<int>("Nsteps"))*/
{
  cout<<"Constructor"<<endl<<endl;

  char hName[1024], hTitle[1024], dName[1024];	// Histogram name, histogram title and directory name.
  
  EventNumber = 0;

  //_file->cd();


  for(int f=0;f<_nsteps;f++){
    sprintf(dName,"%s_%i.root",_outFileName.c_str(),f);
    _file[f] = new TFile(dName, "recreate");
  }


  tok_QIE10DigiCollection_ = consumes<HcalDataFrameContainer<QIE10DataFrame> >(edm::InputTag("hcalDigis"));
  hf_token = consumes<HFDigiCollection>(edm::InputTag("hcalDigis"));
  raw_token = consumes<FEDRawDataCollection>(edm::InputTag("source")); //Slow data

  for(int f=0;f<_nsteps;f++){
    _file[f]->mkdir("Q1HFM");
    _file[f]->mkdir("Q2HFM");
    _file[f]->mkdir("Q3HFM");
    _file[f]->mkdir("Q4HFM");
    _file[f]->mkdir("Q1HFP");
    _file[f]->mkdir("Q2HFP");
    _file[f]->mkdir("Q3HFP");
    _file[f]->mkdir("Q4HFP");
    for(int i=0;i<nQ;i++){
      sprintf(dName,"Q%iHFM/AllSumCh",i+1);
      _file[f]->mkdir(dName);
      sprintf(dName,"Q%iHFM/Ped",i+1);
      _file[f]->mkdir(dName);
      sprintf(dName,"Q%iHFM/2D",i+1);
      _file[f]->mkdir(dName);
      sprintf(dName,"Q%iHFM/PSD",i+1);
      _file[f]->mkdir(dName);
      sprintf(dName,"Q%iHFM/EventByEvent",i+1);
      _file[f]->mkdir(dName);
      sprintf(dName,"Q%iHFP/AllSumCh",i+1);
      _file[f]->mkdir(dName);
      sprintf(dName,"Q%iHFP/Ped",i+1);
      _file[f]->mkdir(dName);
      sprintf(dName,"Q%iHFP/2D",i+1);
      _file[f]->mkdir(dName);
      sprintf(dName,"Q%iHFP/PSD",i+1);
      _file[f]->mkdir(dName);
      sprintf(dName,"Q%iHFP/EventByEvent",i+1);
      _file[f]->mkdir(dName);
    }
  }

  for(int f=0;f<_nsteps;f++) for(int i=0;i<iETAe;i++) for(int j=0;j<iPHIe;j++) for(int k=0;k<nD;k++) psd[f][i][j][k] = new TProfile;
 // This is needed for the psd[i][j][k]->GetEntries() that will come up not to crash. nTS is needed to define the # of bins and the x-axis range. Therefore, the ranges, etc. of psd[i][j][k] are defined in the HFanalyzer::analyze part.

  for(int f=0;f<_nsteps;f++){
    for(int i=0;i<iETAe;i++){
      for(int j=0;j<iPHIe;j++){
        for(int k=0;k<nD;k++){
          if(i<13){
            sprintf(hName,"SumCharge_m%i_%i_%i",41-i,2*j+1,k+1);
            sprintf(hTitle,"PMT Charge (ieta: %i, iphi: %i, Depth: %i)",i-41,2*j+1,k+1);
          }
          else{
            sprintf(hName,"SumCharge_p%i_%i_%i",i+16,2*j+1,k+1);
            sprintf(hTitle,"PMT Charge (ieta: %i, iphi: %i, Depth: %i)",i+16,2*j+1,k+1);
          }
          AllSum[f][i][j][k] = new TH1F(hName,hTitle,1000,0,10000);
        }
      }
    }
  }

  for(int f=0;f<_nsteps;f++){
    for(int i=0;i<iETAe;i++){
      for(int j=0;j<iPHIe;j++){
        for(int k=0;k<nD;k++){
          if(i<13){
            sprintf(hName,"Ped_m%i_%i_%i",41-i,2*j+1,k+1);
            sprintf(hTitle,"Pedestal (ieta: %i, iphi: %i, Depth: %i)",i-41,2*j+1,k+1);
          }
          else{
            sprintf(hName,"Ped_p%i_%i_%i",i+16,2*j+1,k+1);
            sprintf(hTitle,"Pedestal (ieta: %i, iphi: %i, Depth: %i)",i+16,2*j+1,k+1);
          }
          Ped[f][i][j][k] = new TH1F(hName,hTitle,100,0,1000);
        }
      }
    }
  }

  for(int f=0;f<_nsteps;f++){
    for(int j=0;j<nQ;j++){
      for(int k=0;k<nD;k++){
        sprintf(hName,"hfp_%i_%i",j+1,k+1);
        sprintf(hTitle,"PMT Mean Charge (HF+ Quadrant %i Depth %i)",j+1,k+1);
        hfp[f][j][k] = new TProfile2D(hName,hTitle,13,28.5,41.5,9,j*18,(j+1)*18);
        sprintf(hName,"hfm_%i_%i",j+1,k+1);
        sprintf(hTitle,"PMT Mean Charge (HF- Quadrant %i Depth %i)",j+1,k+1);
        hfm[f][j][k] = new TProfile2D(hName,hTitle,13,-41.5,-28.5,9,j*18,(j+1)*18);
        sprintf(hName,"stdevp_%i_%i",j+1,k+1);
        sprintf(hTitle,"Stdev of the PMT Charge (HF+ Quadrant %i Depth %i)",j+1,k+1);
        stdevp[f][j][k] = new TProfile2D(hName,hTitle,13,28.5,41.5,9,j*18,(j+1)*18);
        sprintf(hName,"stdevm_%i_%i",j+1,k+1);
        sprintf(hTitle,"Stdev of the PMT Charge (HF- Quadrant %i Depth %i)",j+1,k+1);
        stdevm[f][j][k] = new TProfile2D(hName,hTitle,13,-41.5,-28.5,9,j*18,(j+1)*18);
        sprintf(hName,"gainp_%i_%i",j+1,k+1);
        sprintf(hTitle,"PMT Gains (HF+ Quadrant %i Depth %i)",j+1,k+1);
        gainp[f][j][k] = new TProfile2D(hName,hTitle,13,28.5,41.5,9,j*18,(j+1)*18);
        sprintf(hName,"gainm_%i_%i",j+1,k+1);
        sprintf(hTitle,"PMT Gains (HF- Quadrant %i Depth %i)",j+1,k+1);
        gainm[f][j][k] = new TProfile2D(hName,hTitle,13,-41.5,-28.5,9,j*18,(j+1)*18);
      }
    }
  }

  Ev.resize(iETAe);
  for(int i=0;i<iETAe;i++){
    Ev[i].resize(iPHIe);
    for(int j=0;j<iPHIe;j++){
      Ev[i][j].resize(nD);
    }
  }

// for histo stuff
  numChannels=0;
}


HFanalyzer::~HFanalyzer()
{
  cout<<"Destructor"<<endl<<endl;
  int chan=1;
  double gain;
  char hName[1024], hTitle[1024], dName[1024];

  for(int f=0;f<_nsteps;f++){
    cout<<"EvByEv file: "<<f<<endl;
    for(int i=0;i<iETAe;i++){
      for(int j=0;j<iPHIe;j++){
        for(int k=0;k<nD;k++){
          if(i<13){
            sprintf(hName,"PeakQ_m%i_%i_%i",41-i,2*j+1,k+1);
            sprintf(hTitle,"Peak Charge (ieta: %i, iphi: %i, Depth: %i)",i-41,2*j+1,k+1);
          }
          else{
            sprintf(hName,"PeakQ_p%i_%i_%i",i+16,2*j+1,k+1);
            sprintf(hTitle,"Peak Charge (ieta: %i, iphi: %i, Depth: %i)",i+16,2*j+1,k+1);
          }
          //EvByEv[f][i][j][k] = new TProfile(hName,hTitle,EventNumber,0.5,EventNumber+0.5);
          //if(Ev[i][j][k].size()!=0) for(int l=0;l<EventNumber;l++) EvByEv[f][i][j][k]->Fill(l+1,Ev[i][j][k][l]);	/////////// !!!!!!!!!!!!!! FIX THIS !!!!!!!!!!!!!!! /////////////////
        }
      }
    }
  }

  for(int f=0;f<_nsteps;f++){
    for(int i=0;i<iETAe;i++){
      for(int j=0;j<iPHIe;j++){
        for(int k=0;k<nD;k++){
          if(AllSum[f][i][j][k]->GetEntries()!=0){
            if(i<13) {sprintf(dName,"Q%iHFM/AllSumCh",j/9+1);}
            else {sprintf(dName,"Q%iHFP/AllSumCh",j/9+1);}
            _file[f]->cd(dName);
            AllSum[f][i][j][k]->SetXTitle("Charge (fC)");
            AllSum[f][i][j][k]->SetYTitle("Counts");
            AllSum[f][i][j][k]->SetMinimum(0.);
            AllSum[f][i][j][k]->Write();
            if(i<13) {sprintf(dName,"Q%iHFM/Ped",j/9+1);}
            else {sprintf(dName,"Q%iHFP/Ped",j/9+1);}
            _file[f]->cd(dName);
            Ped[f][i][j][k]->SetXTitle("Charge (fC)");
            Ped[f][i][j][k]->SetYTitle("Counts");
            Ped[f][i][j][k]->SetMinimum(0.);
            Ped[f][i][j][k]->Write();
            if(i<13) {sprintf(dName,"Q%iHFM/PSD",j/9+1);}
            else {sprintf(dName,"Q%iHFP/PSD",j/9+1);}
            _file[f]->cd(dName);
            psd[f][i][j][k]->SetXTitle("TS");
            psd[f][i][j][k]->SetYTitle("Mean ADC");
            psd[f][i][j][k]->SetMinimum(0.);
            psd[f][i][j][k]->Write();
            if(i<13) {sprintf(dName,"Q%iHFM/EventByEvent",j/9+1);}
            else {sprintf(dName,"Q%iHFP/EventByEvent",j/9+1);}
            _file[f]->cd(dName);
            EvByEv[f][i][j][k]->SetXTitle("Event");
            EvByEv[f][i][j][k]->SetYTitle("Peak Charge (fC)");
            EvByEv[f][i][j][k]->SetMinimum(0.);
            EvByEv[f][i][j][k]->Write();
            //fit = new TF1("fit","gaus",-20.,20.);
            //psd[i][j][k]->Fit("fit","Q");
            gain = AllSum[f][i][j][k]->GetMean()/(1.05*1.602e-4*Ped[f][i][j][k]->GetMean()*Ped[f][i][j][k]->GetMean()/(Ped[f][i][j][k]->GetStdDev()*Ped[f][i][j][k]->GetStdDev()));
            if(i<13){
              hfm[f][j/9][k]->Fill(i-41,2*j+1,AllSum[f][i][j][k]->GetMean());
              stdevm[f][j/9][k]->Fill(i-41,2*j+1,AllSum[f][i][j][k]->GetStdDev());
              gainm[f][j/9][k]->Fill(i-41,2*j+1,gain);
            }
            else{
              hfp[f][j/9][k]->Fill(i+16,2*j+1,AllSum[f][i][j][k]->GetMean());
              stdevp[f][j/9][k]->Fill(i+16,2*j+1,AllSum[f][i][j][k]->GetStdDev());
              gainp[f][j/9][k]->Fill(i+16,2*j+1,gain);
              //stdevp[k]->Fill(i+16,2*j+1,fit->GetParameter("Sigma"));
            }
            if(_verbosity) cout<<chan<<endl;
            chan++;
          }
        }
      }
    }
  }

  for(int f=0;f<_nsteps;f++){
    for(int j=0;j<nQ;j++){
      for(int k=0;k<nD;k++){
        sprintf(dName,"Q%iHFP/2D",j+1);
        _file[f]->cd(dName);
        hfp[f][j][k]->SetStats(0);
        hfp[f][j][k]->SetXTitle("ieta");
        hfp[f][j][k]->SetYTitle("iphi");
        hfp[f][j][k]->SetZTitle("Mean Charge (fC)");
        hfp[f][j][k]->Write();
        sprintf(dName,"Q%iHFM/2D",j+1);
        _file[f]->cd(dName);
        hfm[f][j][k]->SetStats(0);
        hfm[f][j][k]->SetXTitle("ieta");
        hfm[f][j][k]->SetYTitle("iphi");
        hfm[f][j][k]->SetZTitle("Mean Charge (fC)");
        hfm[f][j][k]->Write();
      }

      for(int k=0;k<nD;k++){
        sprintf(dName,"Q%iHFP/2D",j+1);
        _file[f]->cd(dName);
        stdevp[f][j][k]->SetStats(0);
        stdevp[f][j][k]->SetXTitle("ieta");
        stdevp[f][j][k]->SetYTitle("iphi");
        stdevp[f][j][k]->SetZTitle("#sigma_{TS}");
        stdevp[f][j][k]->Write();
        sprintf(dName,"Q%iHFM/2D",j+1);
        _file[f]->cd(dName);
        stdevm[f][j][k]->SetStats(0);
        stdevm[f][j][k]->SetXTitle("ieta");
        stdevm[f][j][k]->SetYTitle("iphi");
        stdevm[f][j][k]->SetZTitle("#sigma_{TS}");
        stdevm[f][j][k]->Write();
      }

      for(int k=0;k<nD;k++){
        sprintf(dName,"Q%iHFP/2D",j+1);
        _file[f]->cd(dName);
        gainp[f][j][k]->SetStats(0);
        gainp[f][j][k]->SetXTitle("ieta");
        gainp[f][j][k]->SetYTitle("iphi");
        gainp[f][j][k]->SetZTitle("#sigma_{TS}");
        gainp[f][j][k]->Write();
        sprintf(dName,"Q%iHFM/2D",j+1);
        _file[f]->cd(dName);
        gainm[f][j][k]->SetStats(0);
        gainm[f][j][k]->SetXTitle("ieta");
        gainm[f][j][k]->SetYTitle("iphi");
        gainm[f][j][k]->SetZTitle("#sigma_{TS}");
        gainm[f][j][k]->Write();
      }
    }
  }

  //gROOT->SetBatch(kTRUE);
  system("mkdir -p Results/mean/p Results/mean/m Results/stdev/p Results/stdev/m Results/gain/p Results/gain/m");
  TCanvas *c1 = new TCanvas;
  c1->cd();
  for(int f=0;f<_nsteps;f++){
    for(int j=0;j<nQ;j++){
      for(int k=0;k<nD;k++){
        hfp[f][j][k]->Draw("colz");
        sprintf(dName,"./Results/mean/p/%i_step_%i_mean_p_q%i_d%i.png",stoi(_run),f,j+1,k+1);
        c1->SaveAs(dName);
        c1->Clear();
        hfm[f][j][k]->Draw("colz");
        sprintf(dName,"./Results/mean/m/%i_step_%i_mean_m_q%i_d%i.png",stoi(_run),f,j+1,k+1);
        c1->SaveAs(dName);
        c1->Clear();
        stdevp[f][j][k]->Draw("colz");
        sprintf(dName,"./Results/stdev/p/%i_step_%i_stdev_p_q%i_d%i.png",stoi(_run),f,j+1,k+1);
        c1->SaveAs(dName);
        c1->Clear();
        stdevm[f][j][k]->Draw("colz");
        sprintf(dName,"./Results/stdev/m/%i_step_%i_stdev_m_q%i_d%i.png",stoi(_run),f,j+1,k+1);
        c1->SaveAs(dName);
        c1->Clear();
        gainp[f][j][k]->Draw("colz");
        sprintf(dName,"./Results/gain/p/%i_step_%i_gain_p_q%i_d%i.png",stoi(_run),f,j+1,k+1);
        c1->SaveAs(dName);
        c1->Clear();
        gainm[f][j][k]->Draw("colz");
        sprintf(dName,"./Results/gain/m/%i_step_%i_gain_m_q%i_d%i.png",stoi(_run),f,j+1,k+1);
        c1->SaveAs(dName);
        c1->Clear();
      }
    }
    _file[f]->Write();
    _file[f]->Close();
  }


  /////////////////////////////////////
}



/**************************************************************************/

//
// member functions
//


// ------------ method called for each event  ------------
void HFanalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  
  using namespace edm;

  EventNumber++;

  //
  //  Extracting All the Collections containing useful Info
  iEvent.getByToken(tok_QIE10DigiCollection_,qie10DigiCollection);
  const QIE10DigiCollection& qie10dc=*(qie10DigiCollection);

  //  -----------------------------------------------------

  //cout << "### QIE10 Digis=" << qie10DigiCollection->size() << endl;

  //This main stuff - how to extract QIE10 information

  float SumCharge;
  float PedCharge;
  float PulMax;
  for (unsigned int j=0; j < qie10dc.size(); j++){

    QIE10DataFrame qie10df = static_cast<QIE10DataFrame>(qie10dc[j]);
    DetId detid = qie10df.detid();
    HcalDetId hcaldetid = HcalDetId(detid);

    int f = !(_mode==6)*(EventNumber%_eps);
    int depth = hcaldetid.depth(); 
    int ieta = hcaldetid.ieta();
    int iphi = hcaldetid.iphi();
    int nTS  = qie10df.samples();

    //How to extract information per time slice (nTS) and how to sum up the total charge in 10 ts

    char hName[1024], hTitle[1024];

    sprintf(hTitle,"Pulse Shape Distribution (ieta: %i, iphi: %i, Depth: %i)",ieta,iphi,depth);

    if(ieta<0){
      if(psd[f][ieta+41][(iphi-1)/2][depth-1]->GetEntries()==0){
        sprintf(hName,"psd_m%i_%i_%i",-ieta,iphi,depth);
        psd[f][ieta+41][(iphi-1)/2][depth-1] = new TProfile(hName,hTitle,nTS,-.5,nTS-.5);
      }
    }
    else{
      if(psd[f][ieta-16][(iphi-1)/2][depth-1]->GetEntries()==0){
        sprintf(hName,"psd_p%i_%i_%i",ieta,iphi,depth);
        psd[f][ieta-16][(iphi-1)/2][depth-1] = new TProfile(hName,hTitle,nTS,-.5,nTS-.5);
      }
    }


    SumCharge = 0;
    PedCharge = 0;
    PulMax = 0;
    for(int i=0; i<nTS; ++i){

        
      int   adc = qie10df[i].adc();    //adc counts per TS
      //int   tdc = qie10df[i].le_tdc(); //Leading edge of TDC per TS
      //int tetdc = qie10df[i].te_tdc(); //Not uset for now
      //int capid = qie10df[i].capid();  //CAPid per TS 
      //int   soi = qie10df[i].soi();  //Sample of interest - not used for now
      float charge = adc2fC_QIE10[ adc ]; //ADC count to charge conversion accordin the general conversion table


      if(ieta<0){
        psd[f][ieta+41][(iphi-1)/2][depth-1]->Fill(i,adc);
      }
      else{
        psd[f][ieta-16][(iphi-1)/2][depth-1]->Fill(i,adc);
      }

      SumCharge+=charge;
      if(i<2) PedCharge+=charge;
      if(PulMax<charge) PulMax=charge;
    }
    PedCharge*=nTS/2.;

    if(ieta<0){
      AllSum[f][ieta+41][(iphi-1)/2][depth-1]->Fill(SumCharge);
      Ped[f][ieta+41][(iphi-1)/2][depth-1]->Fill(PedCharge);
      Ev[ieta+41][(iphi-1)/2][depth-1].resize(EventNumber);
      Ev[ieta+41][(iphi-1)/2][depth-1][EventNumber-1] = PulMax;
      //stdevm[depth-1]->Fill(ieta,iphi,psd[ieta+41][(iphi-1)/2][depth-1]->GetStdDev());
      if(_verbosity)cout<<ieta<<" "<<iphi<<" "<<depth<<" "<<AllSum[f][ieta+41][(iphi-1)/2][depth-1]->GetName()<<" Charge: "<<SumCharge<<endl;
    }
    else{
      AllSum[f][ieta-16][(iphi-1)/2][depth-1]->Fill(SumCharge);
      Ped[f][ieta-16][(iphi-1)/2][depth-1]->Fill(PedCharge);
      Ev[ieta-16][(iphi-1)/2][depth-1].resize(EventNumber);
      Ev[ieta-16][(iphi-1)/2][depth-1][EventNumber-1] = PulMax;
      //stdevp[depth-1]->Fill(ieta,iphi,psd[ieta-16][(iphi-1)/2][depth-1]->GetStdDev());
      if(_verbosity)cout<<ieta<<" "<<iphi<<" "<<depth<<" "<<AllSum[f][ieta-16][(iphi-1)/2][depth-1]->GetName()<<" Charge: "<<SumCharge<<endl;
    }
  
  }
}

    

  



// ------------ method called once each job just before starting event loop  ------------
void 
HFanalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
HFanalyzer::endJob() 
{
  //      _file->Write();
  //      _file->Close();
}

// ------------ method called when starting to processes a run  ------------
void 
HFanalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
HFanalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{
  //_file->Write();
  //_file->Close();
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
HFanalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
HFanalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
HFanalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(HFanalyzer);
