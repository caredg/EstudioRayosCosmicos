#define PulseAnalysis_cxx
// The class definition in PulseAnalysis.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// root> T->Process("PulseAnalysis.C")
// root> T->Process("PulseAnalysis.C","some options")
// root> T->Process("PulseAnalysis.C+")
//

#include "PulseAnalysis.h"
#include <TH2.h>
#include <TStyle.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include "TVirtualFFT.h"



void PulseAnalysis::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

void PulseAnalysis::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

   //Define the objects that were declared in the header, and any other
   //object you need
   myfile = new TFile("eqhistos.root","RECREATE");

   //This is to fill the total charge as a simple example
   h_totcharge = new TH1F("h_totcharge", "Integrated Charge", 900, 0, 9000);

   //The first gps time in our data was 1460257200 and the last pulse
   //happend after 39794704 clocks from the gps second 1461023999, i.e.,
   //it happend at 1461023999+(25e-9)(39794704).  
   //Therefore we have 1461023999.9948676 - 1460257200 = 766799.998676
   //seconds of data.

   //Let's plot in bins of 5 minutes:
//   long int nbin = 2556;
   //or in bins of 1 minute:
   long int nbin = 12780;

   //start flux histograms from zero (which is the time we started
   //collecting data):
   long int down = 0;

   //until the highest second we have data for, which in our case is
   //766800.  This corresponds to about 8.875 days of data.
   long int up = 766800;

   //let's define the histogram for uncorrected and unnormalized flux
   h_urate = new TH1D("h_urate", "Unnormalized Rate of Cosmic Rays - Chimbito WCD",nbin, down, up);
   
   //In order to correct the for pressure effects, we need to store the per
   //minute information of pressure.  We obtained this information from an 
   //external source (specific weather station around the place where data
   //was taken).  The values of time are stored in the file
   //time.lt and the corresponding values of pressure in the file
   //presion.lst

   //We need to open those files and store the information.
   //We do that and dump the information in vector containers

   //for file containing pressure info
   ifstream fileP; 
   //for file containing the time at which pressure was taken
   //note that the actual times are listed in the "time2.lst" file
   //we start at the same time we started taking data with the WCD
   ifstream tFile; 

   //let's open those files
   fileP.open("presion.lst");
   tFile.open("time.lst");

   //

   

}

Bool_t PulseAnalysis::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either PulseAnalysis::GetEntry() or TBranch::GetEntry()
   // to read either all or the required parts of the data. When processing
   // keyed objects with PROOF, the object is already loaded and is available
   // via the fObject pointer.
   //
   // This function should contain the "body" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.
    
    //Get a total count of events
    ++eventsCounter;

    //inform about progress (comment out if need for speed)
    if (eventsCounter % 10000000 == 0)
      cout << "Next event -----> " << eventsCounter << endl;

    //Need to get the entry as indicated in the instructions above
    GetEntry(entry);

    //Fill a histogram with the total charge
    h_totcharge->Fill(channel1_total_charge);

   //According to the note in the SlaveBegin function, let's normalize
   //(rescale) the time information
    Double_t ntime = time-1460257200;

    //The uncorrected flux histogram would be
    h_urate->Fill(ntime);

    //However, we do need to correct for atmospheric influence, specially
    //pressure.

    //initialize output histograms for transform
    h_magFFT = 0;
    h_phaseFFT = 0;

    
    
    return kTRUE;
}

void PulseAnalysis::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

//beautify histogram
    //h_totcharge->SetLineColor(kBlue);
    //h_totcharge->SetLineWidth(2);
    //h_totcharge->GetXaxis()->SetTitle("ADC");
    //change canvas pointer to the adequate canvas
    //c_totcharge->cd();
    //draw the histo in the current canvas
    //h_totcharge->Draw();
    //Print an image
    //c_totcharge->Print("h_totcharge.png");
    h_urate->GetXaxis()->SetTitle("Unnormalized time");
    h_urate->GetYaxis()->SetTitle("Events/hour");
    
    //The implementation of a Fast Fourier transform (FFT) using the 
    //standard FFTW library (through ROOT) is shown below
    TVirtualFFT::SetTransform(0);
    //compute the transform and look at the magnitude
    h_magFFT = h_urate->FFT(h_magFFT, "MAG");
    h_magFFT->SetTitle("Magnitude of the transform");
    //look at the phase
    h_phaseFFT = h_urate->FFT(h_phaseFFT, "PH");
    h_phaseFFT->SetTitle("Phase of the transform");

    //Write out histograms
    h_totcharge->Write();
    h_urate->Write();
    h_magFFT->Write();
    h_phaseFFT->Write();
    
    
    myfile->Close();

    cout<<"Done slave-terminating...."<<endl;


}

void PulseAnalysis::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

    
}
