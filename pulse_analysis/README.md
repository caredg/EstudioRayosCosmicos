This is some code to analyze data from our lago tank

DESCRIPTION:
============

* PulseAnalysis.C and PulseAnalysis.h:  are the meat and potatoes of the pulses analysis.  It was created from one of the root files for the "pulse" tree.  The header (.h file) contains most of the objects and the right pointing to appropriate branches and leafs.  The .C file is where the analysis is done.  In particular, the "Process" method happens for each event.  There is where you have to put all your creativity.  Currently, it creates a ROOT file called "eqhistos.root" where all the produced stuff, histograms, etc. (after analyzing the whole batch of data) is stored for later plotting or reviewing.

* runPulseAnalysis.C: this is a simple C++ code to run the analysis a la ROOT TSelector way.  It is meant to steer the analysis.  Note that it maybe expanded later to incorporate other analyzers (for other trees in the files).
It basically takes a text file (anyfile.lst) with a list of the ".root" files to be analyzed (the full path needs to be given), it creates an instance of the Pulse analysis (or any other analysis that in the future one might consider doing), chain all the trees from all the root files and process them.

* compileme: this is a small script with a set of shell commands that compile everything needed aginst ROOT libraries.


INSTRUCTIONS:
=============

1. Download the code to your favorite area:

> git clone https://github.com/caredg/EstudioRayosCosmicos

> cd pulse_analysis

2. Make whatever changes to the code (if needed)

3. Make sure the compile script is executable:

> chmod +x compileme

4. Compile everything:

> ./compileme

5. Run the analysis with the steering program:

> ./runPulseAnalysis mylist.lst


NOTE: The file mylist.lst is any file with a list of files like:

/your/own/full/path/location/ESPOCH_nogps_2016_04_18_19h00v5.root
/your/own/full/path/location/ESPOCH_nogps_2016_04_18_20h00v5.root
/your/own/full/path/location/ESPOCH_nogps_2016_04_18_21h00v5.root
/your/own/full/path/location/ESPOCH_nogps_2016_04_18_22h00v5.root
/your/own/full/path/location/ESPOCH_nogps_2016_04_18_23h00v5.root
