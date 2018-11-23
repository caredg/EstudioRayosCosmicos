//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Mar 23 23:14:00 2017 by ROOT version 6.04/14
// from TTree pulse/pulse from ascii file
// found on file: ESPOCH_nogps_2016_04_10_12h00v5.root
//////////////////////////////////////////////////////////

#ifndef PulseAnalysis_h
#define PulseAnalysis_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TCanvas.h>

#include <iostream>
#include <fstream>
#include <string>

// Header file for the classes stored in the TTree if any.
using namespace std;

class PulseAnalysis : public TSelector {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Double_t        time;
   ULong64_t       counter;
   Int_t           trigger;
   Int_t           length;
   Int_t           channel1_trace[12];
   Int_t           channel1_peak;
   Int_t           channel1_total_charge;
   Int_t           channel1_charge;
   Float_t         channel1_get_baseline;
   Int_t           channel2_trace[12];
   Int_t           channel2_peak;
   Int_t           channel2_total_charge;
   Int_t           channel2_charge;
   Float_t         channel2_get_baseline;
   Int_t           channel3_trace[12];
   Int_t           channel3_peak;
   Int_t           channel3_total_charge;
   Int_t           channel3_charge;
   Float_t         channel3_get_baseline;

   // List of branches
   TBranch        *b_time;   //!
   TBranch        *b_counter;   //!
   TBranch        *b_trigger;   //!
   TBranch        *b_length;   //!
   TBranch        *b_channel1;   //!
   TBranch        *b_channel2;   //!
   TBranch        *b_channel3;   //!

   PulseAnalysis(TTree * /*tree*/ =0) : fChain(0) { }
   virtual ~PulseAnalysis() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   //Here we can declare all our histograms, etc.
   int eventsCounter;
   TH1F* h_totcharge;
   TH1D* h_urate;
   TH1F* h_cleanflux;
   TH1F* chargepeak;
   TH1F* chargepeakclean;
   TH1F* h_pressure;
   TH2D* fluxVSpressure;
   float baseline = 50;
   vector<double> presion;
   vector<double> minutos;
   //for the magnitude of FFT
   TH1* h_magFFT;
   //for the phase of FFT
   TH1* h_phaseFFT;
   int j;//para la correccion de presion
   
   
   //TH2D* h2_rate;
   
   //root file for storage
   TFile* myfile;
   
   //This next line needs to be commented out in order to compile the code
   //a la C++
   //  ClassDef(PulseAnalysis,0);
};

#endif

#ifdef PulseAnalysis_cxx
void PulseAnalysis::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("time", &time, &b_time);
   fChain->SetBranchAddress("counter", &counter, &b_counter);
   fChain->SetBranchAddress("trigger", &trigger, &b_trigger);
   fChain->SetBranchAddress("length", &length, &b_length);
   fChain->SetBranchAddress("channel1", channel1_trace, &b_channel1);
   fChain->SetBranchAddress("channel2", channel2_trace, &b_channel2);
   fChain->SetBranchAddress("channel3", channel3_trace, &b_channel3);

   eventsCounter = 0;
   
}

Bool_t PulseAnalysis::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef PulseAnalysis_cxx
