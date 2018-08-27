#include "PulseAnalysis.h"
#include <iostream>
#include <fstream>
#include <string>
#include <TChain.h>

//--------------------------------------------------------------------------------------------

int main(int argc, char** argv) {

//--------------------------------------------------------------------------------------------

     //Check for arguments
    if(argc != 2) {
        cout << "You need to give a text file with the list of root files to be chained for analysis"<<endl;
        cout<<"Execute as: "<<argv[0]<<" "<<"<filelist.lst>"<<endl;
      return -1;
   }

    //check if file exists
    ifstream infile;
    string STRING;
    STRING = argv[1];
    infile.open(argv[1]);
    if(infile.fail()){
        cout<<"Error, file does not exists ..... quiting"<<endl;
        return 1;
    }
    infile.close();

    //create analysis object
    PulseAnalysis *mypanalysis = new PulseAnalysis();

    //chain root tree from all the files in the submitted list
    TChain* ch = new TChain("pulse");
    char _file[256];
    ifstream _files(argv[1]);
    while(_files>>_file){
        ch->Add(_file);
        cout<<"File "<<_file<<" has been chained"<<endl;
    }
    cout<<"Files from "<<argv[1]<<" have been chained"<<endl;
    cout<<"Total number fo events chained: "<<ch->GetEntries()<<endl;
    
    //execute the analysis
    ch->Process(mypanalysis);

    cout<<"I am out of Process"<<endl;
    
    return 0;
    
}//---------------------main()   
