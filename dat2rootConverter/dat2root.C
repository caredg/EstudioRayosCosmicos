/**                                            \
Original name: Macro lago_file.C
Original Description:
Converts files .dat to .root from DATA LAGO
*/
//
// Original Author:  Miguel Bonnett
// Modified by Ricardo Escobar and Edgar Carrera
//
/* Modification History:
   - 23 march 2017: several clean ups from Ricardo to make it
   compatible with new c++ standards.  
   - 23 march 2017: we have made it compilable.  It compiles with
   g++ -o lagodat2root lagodat2root.C `root-config --glibs --cflags`
   or with an equivalent Makefile with make

*/
/* How to run this program
   Compile as above and then do
   ./lagodat2root <filename.dat>
   It will generate a root file <filename>.root
 */


//It good be good to comment code

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <math.h>
#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include <TObject.h>
#include <Rtypes.h>
#include <TROOT.h>
using namespace std;

#define CODEVER 5

//-----------------------------------------------------------------
void lagodat2root(char* _file) {
//-----------------------------------------------------------------
//void lago_file(TString filename) {
	//gROOT->Reset();
	//gSystem->Load("libRootKernel");
    TString filename = TString(_file);
    cout<<"Processing file "<<filename<<endl;
//	TString filename = "ESPOCH_nogps_2016_04_10_12h00.dat";
//	TString filename = "ESPOCH_nogps_2016_04_10_03h00.dat";
	//ifstream in_file(filename);
	const char *name = (const char*)filename;
	FILE *in_file=fopen(name, "r");


	Float_t ver;
	//char line[256]; //Char_t line[256];
    char line[1024]; //Char_t line[256];

/// for pulse
	struct data {
		UInt_t trace[12];
		UInt_t peak;
		UInt_t total_charge;
		UInt_t charge;
		Float_t get_baseline;
		Float_t baseline;
	};
	data channel1, channel2, channel3;

	channel1.total_charge = 0;
	channel1.peak = 0;
	channel1.baseline = 0;
	channel2.total_charge = 0;
	channel2.peak = 0;
	channel2.baseline = 0;
	channel3.total_charge = 0;
	channel3.peak = 0;
	channel3.baseline = 0;
	
	Int_t kBaseline = 50; //////////////////depende de la version
	Double_t  time = 0;
	//ULong64_t counter = 0;
	UInt_t counter = 0;
	UInt_t ch_trigger, trigger, clock_count_trigger;
	ch_trigger = trigger = clock_count_trigger = 0;
	UInt_t length = 12;
	UInt_t ch1,ch2,ch3;
	ch1 = ch2 = ch3 = 0;
/////////////////////
/// for second
	UInt_t D1, D2, C1_DD, rate;
	D1 = D2 = C1_DD = rate =0;	
	UInt_t frequency, gps, gps_time;
	frequency = gps_time = gps = 0;
	Float_t temperature, pressure, altitude;
	temperature = pressure = altitude = 0;
	Float_t LAT, LON, ALT, gps_data[3];
	LAT=gps_data[0] = LON=gps_data[1] = ALT=gps_data[2] =0;
	char hora[8], fecha[10], utemp[1], upres[3], ualti[1];
	//Int_t pulse_second = 0;
	//Float_t	B1, B2, B3, baselines[3];
	//B1=baselines[0] = B2=baselines[1] = B3=baselines[2] = 0 ;
/////////////////////
/// for	Current_registers_setting
	//UInt_t T1, T2, T3, ST1, ST2, ST3, HV1, HV2, HV3;
	UInt_t T[3], ST[3], HV[3];
	T[0]=T[1]=T[2]=ST[0]=ST[1]=ST[2]=HV[0]=HV[1]=HV[2]=0;
	char GPSTM[5];
/////////////////////
/// for error_code
	Int_t error =0;
/////////////////////
/// count
	Int_t nlines = 0;
	Int_t count = 0;
	Int_t length_count = 0;
/////////////////////

	TFile *file = new TFile(filename.ReplaceAll(".dat", "v5.root"),"RECREATE");
	TTree *pulse = new TTree("pulse","pulse from ascii file");
	// Set branch addresses.
	pulse->Branch("time", &time, "time/D");
	pulse->Branch("counter", &counter, "counter/l");
	pulse->Branch("trigger", &trigger, "trigger/I");
	pulse->Branch("length", &length, "length/I");
	pulse->Branch("channel1", &channel1, "trace[12]/I:peak/I:total_charge/I:charge/I:get_baseline/F");
	pulse->Branch("channel2", &channel2, "trace[12]/I:peak/I:total_charge/I:charge/I:get_baseline/F");
	pulse->Branch("channel3", &channel3, "trace[12]/I:peak/I:total_charge/I:charge/I:get_baseline/F");
	//pulse->Branch("channe", channe[3], "trace[12]/I:peak/I:total_charge/I:charge/I:get_baseline/F");

	TTree *second_dat = new TTree("second_dat","second data from ascii file");
	// Set branch addresses.
	second_dat->Branch("frequency",&frequency,"frequency(Hz)/i");	//frequency  int
	second_dat->Branch("temperature",&temperature,"temperature(C)/F");	//temperature
	second_dat->Branch("pressure",&pressure,"pressure(hPa)/F");	//pressure
	second_dat->Branch("altitude",&altitude,"altitude(m)/F");	//altitude
	second_dat->Branch("gps_data",gps_data,"gps_data[3]/F");	//GPS data - latitude, longitude, altitude
	//second_dat->Branch("baselines", baselines,"baselines[3]/F")
	second_dat->Branch("D1", &D1,"D1/i");
	second_dat->Branch("D2", &D2,"D2/i");
	//second_dat->Branch("raw_sensor",raw_sensor,"raw_sensor[3]/F");	//raw temperature and pressure sensor value (NOT IMPLEMENTED IN LAGO)
//# #   # x r C1-DD <V> (raw_sensor[0]) : raw temperature and pressure sensor value
//# #   # x r D1 <V>    (raw_sensor[1]) : raw temperature/pressure value
//# #   # x r D2 <V>    (raw_sensor[2]) : raw temperature/pressure value
	second_dat->Branch("rate",&rate,"rate/i");
	//second_dat->Branch("pulse_second",&pulse_second,"pulse_second/i");
	second_dat->Branch("gps_time",&gps_time,"gps_time/i");

	TTree *Current_registers_setting = new TTree("Current_registers_setting","Current registers setting from ascii file");
	// Set branch addresses.
	Current_registers_setting->Branch("T",T,"T[3]/i");
	Current_registers_setting->Branch("ST",ST,"ST[3]/i");
	Current_registers_setting->Branch("HV",HV,"HV[3]/i");
	Current_registers_setting->Branch("GPSTM",&GPSTM,"GPSTM/C");

	TTree *error_code = new TTree("error_code","Error from ascii file");
	// Set branch addresses.
	error_code->Branch("error",&error,"error/i");
	error_code->Branch("gps_time",&gps_time,"gps_time/i");
	//	the error message in human readable format, where the number is the error code:
	// 1 : read timeout of 2 seconds
	// 2 : too many buffer reading tries
	// 3 : unknown word from FPGA

     
	//while(in_file.good()) {
	while (fgets(line,256,in_file)){
	
		if(line[0] =='#') {
			switch(line[2]) {
			case 'v':  // version check
				sscanf(line, "# v %f", &ver);
				//cout<<ver<<endl;
				if (ver != CODEVER) {
					cout << "La version de este archivo no es  " << CODEVER <<endl;
					exit(1);
				}
				break;
			case 'x':
				switch (line[4]) {
				case 'c':     // sensors
					switch (line[6]) {
					case 'T':     // 	T1 T2 T3
						switch (line[7]) {
						case '1':
							sscanf(line, "# x c T1 %d", &T[0]);
							break;
						case '2':
							sscanf(line, "# x c T2 %d", &T[1]);
							break;
						case '3':
							sscanf(line, "# x c T3 %d", &T[2]);
							break;
						//default:
						}
						break;
					case 'S':     // 	ST1 ST2 ST3
						switch (line[8]) {
						case '1':
							sscanf(line, "# x c ST1 %d", &ST[0]);
							break;
						case '2':
							sscanf(line, "# x c ST2 %d", &ST[1]);
							break;
						case '3':
							sscanf(line, "# x c ST3 %d", &ST[2]);
							break;
						//default:
						}
						break;
					case 'H':     // 	HV1 HV2 HV3
						switch (line[8]) {
						case '1':
							sscanf(line, "# x c HV1 %d", &HV[0]);
							break;
						case '2':
							sscanf(line, "# x c HV2 %d", &HV[1]);
							break;
						case '3':
							sscanf(line, "# x c HV3 %d", &HV[2]);
							break;
						//default:
						}
						break;
					case 'G':     // 	GPSTM UTC
						sscanf(line, "# x c GPSTM %s", GPSTM);
						Current_registers_setting->Fill();
						T[0]=T[1]=T[2]=ST[0]=ST[1]=ST[2]=HV[0]=HV[1]=HV[2]=0;
						break;
					//default:
					}
					break;
				case 'f':     // frequency
					sscanf(line, "# x f %d", &frequency);
					break;
				case 'r':     // sensors raw
					switch (line[6]) {
/*					case 'C':     // sensors raw
						sscanf(line, "# x r C1-DD %d", &C1_DD);
						break;
*/						
					case 'D':     // sensors raw
						switch (line[7]) {
						case '2':     // sensors raw
							sscanf(line, "# x r D2 %d", &D2);
							break;
						case '1':     // sensors raw
							sscanf(line, "# x r D1 %d", &D1);
							gps_time=gps;
							rate=count;
							//cout <<nlines<<"   "<<count<<"   "<<frequency<<"   "<<temperature<<"   "<<pressure<<"   "<<altitude<<"   "<< gps_data[0] <<"   "<< gps_data[1] <<"   "<< gps_data[2] <<"   "<< gps_time<<endl;
							//printf(" time = %f \n",time);
							second_dat->Fill();
							
							frequency = gps_time = gps = 0;
							temperature = pressure = altitude = 0;
							LAT=gps_data[0] = LON=gps_data[1] = ALT=gps_data[2] =0;
							D1 = D2 = C1_DD = rate =0;	
							count =0;						
							break;
						//default:
						}
						break;
					//default:
					}
					break;
				case 'h':     //  GPS time
					//cout <<nlines<<"   "<<count<<"   "<<line<< endl;
					sscanf(line, "# x h %s %s %d", hora, fecha, &gps);
					break;
				case 's':     // sensor
					sscanf(line, "# x s %f %s %f %s %f %s", &temperature, utemp, &pressure, upres, &altitude, ualti);
					break;
				case 'g':     // gps_data
					sscanf(line, "# x g %f %f %f", &gps_data[0], &gps_data[1], &gps_data[2]);
					break;
/*				case 'b':     // base_line
					sscanf(line, "# x b %f %f %f", &baselines[0] &baselines[1] &baselines[2]);
					break;
*/
				//default:
				}
				break;
			case 't': //channel trigger
				sscanf(line, "# t %d %d", &trigger, &clock_count_trigger);
				//trigger = ch_trigger;
				break;
			case 'c': //internal trigger counter
				//counter = in_trigr_count ;
				sscanf(line, "# c %d", &counter);
				count ++;
				time = gps + 25e-9*clock_count_trigger;

				channel1.charge = channel1.total_charge-length*kBaseline;
				channel2.charge = channel2.total_charge-length*kBaseline;
				channel3.charge = channel3.total_charge-length*kBaseline;

				channel1.get_baseline = channel1.baseline/4;
				channel2.get_baseline = channel2.baseline/4;
				channel3.get_baseline = channel3.baseline/4;

/*				printf(" time = %f \n",time);
				cout << length_count <<"  "<<channel1.trace[0]<<"   "<<channel1.trace[1]<<"   "<<channel1.trace[2]<<"   "<<channel1.trace[11]<<"   "<< time<<endl;
				cout <<count<<"     "<< channel1.charge<<"   "<<channel1.total_charge<<"   "<<length*kBaseline<<endl;
				cout << length_count <<"  "<<trigger<<"  "<<counter<<"  "<<channel1.total_charge<<"  "<<channel1.peak<<endl;
				cout << time <<"  "<<gps<<"  "<<clock_count_trigger<<"  "<<25e-9*clock_count_trigger<<"  "<<channel1.baseline<<"  "<<channel1.get_baseline<<endl;
*/
				pulse->Fill();

				length_count = 0;
				channel1.total_charge = 0;
				channel1.peak = 0;
				channel1.baseline = 0;
				channel2.total_charge = 0;
				channel2.peak = 0;
				channel2.baseline = 0;
				channel3.total_charge = 0;
				channel3.peak = 0;
				channel3.baseline = 0;
				counter = 0;
				ch_trigger = trigger = clock_count_trigger = 0;
				
				break;
			case 'E':
				//cout <<line<< endl;
				sscanf(line, "# E %d", &error);
				//gps_time=gps;
				error_code->Fill();
				error=0;
				break;

			//default:
			}

		}
		else {
			sscanf(line, "%d %d %d", &ch1, &ch2, &ch3);
			channel1.trace[length_count]=ch1;
			channel2.trace[length_count]=ch2;
			channel3.trace[length_count]=ch3;
			
			channel1.total_charge += ch1;
			channel2.total_charge += ch2;
			channel3.total_charge += ch3;
			
			if(ch1>channel1.peak)channel1.peak=ch1;
			if(ch2>channel2.peak)channel2.peak=ch2;
			if(ch3>channel3.peak)channel3.peak=ch3;
			
			if(length_count == 0 || length_count == 9 ||length_count == 10 ||length_count == 11) {
				channel1.baseline+=ch1;
				channel2.baseline+=ch2;
				channel3.baseline+=ch3;
			}
			length_count++;
			ch1 = ch2 = ch3 = 0;
		}
		nlines++;
	}
	printf(" found %d points\n",nlines);
	fclose(in_file);
    file->Write();
	//file->Write("", TObject::kWriteDelete);
    file->Close();
	if(nlines==0) {
		//remove("D:\CHA_2010_01_16.dat.gz\CHA_2011_02_16_18h01.root");
		cerr << "Error archivo sin datos" << endl;
		//break;
        exit(1);
        
	}
}


//-----------------------------------------------------------------
int main(int argc, char** argv)
{
//-----------------------------------------------------------------
    //Check for arguments
    if(argc != 2) {
        cout << "You need to supply a dat filename as an argument."<<endl;
        cout<<"Execute as: "<<argv[0]<<" "<<"<filename.dat>"<<endl;
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

    //Execute the converter
    lagodat2root(argv[1]);
    return 0;
}//---------------------------------------------------------main()    

