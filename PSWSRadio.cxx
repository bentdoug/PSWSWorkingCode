//------------------------------------------------------------------
//Stand alone version of FLDIGI analysis.cxx file for HAMSci's PSWS
//project.
//
//
//
//------------------------------------------------------------------
#include <fstream>
#include <unistd.h>
#include <stdio.h>
#include <cstdio>
#include <string>
using namespace std;





// global variables 
        string analysisFileName;  //to hold name of current analysis file
        bool write_to_csv; //says to begin writing csv
        char FileDate[20];
        char FileData[20]; //
        struct tm File_Start_Date;
        struct tm File_Start_Data;
        string OpenAnalalysisFile;

void createfilename()
{
// Function to find the working directory
// also creates the filename that should be open for today's date [w and w/o full path]

        // String defs
        string DateTimeZulu;

	// path names to working directories
        string HomePath;
        string PSWSBaseDir;
        string AnalysisDir;

       // File locations for metadata info
        HomePath = getenv("HOME");
        PSWSBaseDir = HomePath + "/PSWS/";
        AnalysisDir = PSWSBaseDir + "Srawdata/";

// Create embedded ISODateTime YYYY-MM-DDTHHMMSS for file creation naming
	time_t now = time(NULL);
	gmtime_r(&now, &File_Start_Date);

// Create embedded ISODateTime data image as YYYY-MM-DDTHH:MM:SS for file header info
	gmtime_r(&now, &File_Start_Data);

// create the embedded filename date&time image as YYMMDD (original date format)
	strftime((char*)FileDate,sizeof(FileDate),"%y%m%d", &File_Start_Date);

// create the embedded date data image as YYYY-MM-DDTHH:MM:SSZ (Full ISO Date format)
//	strftime((char*)FileData,sizeof(FileData),"%Y-%m-%dT%H:%M:%S", &File_Start_Data);
	strftime((char*)FileData,sizeof(FileData),"%FT%T", &File_Start_Data);


// check for ~/PSWS/ directory and if not there create entire file structure
        if (access(PSWSBaseDir.c_str(),F_OK) == -1)
        //base directory does not exist, so create it and all subdirectoriew
        {
	        //put_status("~/PSWS/Srawdata/ does NOT EXIST!");
		printf("Cannot find %s\n", PSWSBaseDir.c_str());
		write_to_csv = false;  //say NO  writes to file
        }
        else
	        printf("\n\nPSWS Main Directory %s DOES EXIST and will use it!\n", PSWSBaseDir.c_str() ); // diag line

// now create the analysis file name structure for today's data
	OpenAnalalysisFile.assign("analysis").append(FileDate).append(".csv");

// Full name with path
// Added new file naming and storage by N8OBJ 4-24-20
	analysisFileName.assign(AnalysisDir).append(OpenAnalalysisFile);

// diag print out full path name
        printf("\nFull pathname is: %s\n", analysisFileName.c_str());


}

void start_csv()
{
	printf("\nEntering start_csv routine\n\n"); //diag pring

	double CalcFreq;
	unsigned int CalcFrqNum;

        // path names to working directories
        string HomePath;
        string PSWSBaseDir;
        string AnalysisDir;
        string PSWSinfoPath;
        string PSWSstatPath;
        string PSWScmdPath;
        string PSWScodePath;
        string PSWStempPath;
        string PSWSdataPath;
	string PSWSdataPathNew;

        // metadata file paths
        string CityStatePath;
        string CallSignPath;
        string LatLonElvPath;
        string FreqStdPath;
        string NodeNumPath;
        string GridSqrPath;
        string Radio1Path;
	string Radio1IDPath;
        string AntennaPath;
        string MetadataPath;
        string Beacon1Path;
	string SystemPath;

        // variables to hold metadata read from files
        string CityState;
        string CallSign;
        string LatLonElv;
        string FreqStd;
        string NodeNum;
        string GridSqr;
        string Radio1;
	string Radio1ID;
        string Antenna;
	string System;
        string Metadata;
        string DataTyp;
	string SavedBeacon1;
        string Beacon1;

       // File locations for metadata info
        HomePath = getenv("HOME");
        PSWSBaseDir = HomePath + "/PSWS/";
        AnalysisDir = PSWSBaseDir + "Srawdata/";
        PSWSinfoPath = PSWSBaseDir + "Sinfo/";
        PSWSstatPath = PSWSBaseDir + "Sstat/";
        PSWScmdPath = PSWSBaseDir + "Scmd/";
        PSWScodePath = PSWSBaseDir + "Scode/";
        PSWStempPath = PSWSBaseDir + "Stemp/";
        PSWSdataPath = PSWSBaseDir + "Sdata/";

        CityStatePath = PSWSinfoPath + "CityState.txt";
        CallSignPath = PSWSinfoPath + "CallSign.txt";
        LatLonElvPath = PSWSinfoPath + "LatLonElv.txt";
        FreqStdPath = PSWSinfoPath + "FreqStd.txt";
        NodeNumPath = PSWSinfoPath + "NodeNum.txt";
        GridSqrPath = PSWSinfoPath + "GridSqr";
        Radio1Path = PSWSinfoPath + "Radio1.txt";
        Radio1IDPath = PSWSinfoPath + "Radio1ID.txt";
        AntennaPath = PSWSinfoPath + "Antenna.txt";
        SystemPath = PSWSinfoPath + "System.txt";
        MetadataPath = PSWSinfoPath + "Metadata.txt";
        Beacon1Path = PSWSinfoPath + "Beacon1";

        //read in the user info for the 1st line of this days data collection
        //fprintf -> (ISODate, NodeNum, GridSqr, LatLonElv, CityState, Radio1, Antenna, FrqStd);

        //read in the Node Assigned to this station - if not found indicate so in name returned
        ifstream NodeNumFile (NodeNumPath);
        if (NodeNumFile.is_open())
        {
                getline (NodeNumFile,NodeNum);
                NodeNumFile.close();
        }
        else
                NodeNum = "N0000000";  // no node file found - make test Node by default


        //read in the CallSign - if not found indicate so in CallSign returned
        ifstream CallSignFile (CallSignPath);
        if (CallSignFile.is_open())
        {
                getline (CallSignFile,CallSign);
                CallSignFile.close();
        }
        else
        {
	         CallSign = "NOCall";
	}

        //read in the LatLongAlt - if not found indicate so in name returned
        ifstream LatLonElvFile (LatLonElvPath);
        if (LatLonElvFile.is_open())
        {
                getline (LatLonElvFile,LatLonElv);
                LatLonElvFile.close();
        }
        else
        {
	        LatLonElv = "00.00000, -00.00000, 0000";
	}

        //read in the GridSqr - if not found indicate so
        ifstream GridSqrFile (GridSqrPath);
        if (GridSqrFile.is_open())
        {
                getline (GridSqrFile,GridSqr);
                GridSqrFile.close();
        }
        else
                GridSqr = "JJ00aa";


       //read in the City State Names - if not found indicate so in name returned
        ifstream CityStateFile (CityStatePath);
        if (CityStateFile.is_open())
        {
                getline (CityStateFile,CityState);
                CityStateFile.close();
        }
        else
        {
                 CityState = "NOCity NOState";
        }

        //read in the Radio1 Make and Model being used - if not found indicate so in name returned
        ifstream Radio1File (Radio1Path);
        if (Radio1File.is_open())
        {
                getline (Radio1File,Radio1);
                Radio1File.close();
        }
        else
                Radio1 = "Mystery Radio1";


        //read in the System Info - if not found indicate so in name returned
        ifstream SystemFile (SystemPath);
        if (SystemFile.is_open())
        {
                getline (SystemFile,System);
                SystemFile.close();
        }
        else
                System = "A Computer running Linux";


        //read in the Radio1 Make and Model being used - if not found indicate so in name returned
         ifstream Radio1IDFile (Radio1IDPath);
         if (Radio1IDFile.is_open())
         {
                 getline (Radio1IDFile,Radio1ID);
                 Radio1IDFile.close();
         }
         else
                 Radio1ID = "No_Radio1_ID";



        //read in the Antenna Make and Model being used - if not found indicate so in name returned
        ifstream AntennaFile (AntennaPath);
        if (AntennaFile.is_open())
        {
                getline (AntennaFile,Antenna);
                AntennaFile.close();
        }
        else
                Antenna = "50 Ohm Dummy Load";


        //read in the frequency standard being used - if not found indicate so in name returned
        ifstream FreqStdFile (FreqStdPath);
        if (FreqStdFile.is_open())
        {
                getline (FreqStdFile,FreqStd);
                FreqStdFile.close();
        }
        else
	        FreqStd = "Unknown FreqStd";


        //read in the Beacon previously being used - if not found indicate so in name returned
        ifstream Beacon1File (Beacon1Path);
        if (Beacon1File.is_open())
        {
                getline (Beacon1File,SavedBeacon1);
                Beacon1File.close();
        }
        else
                SavedBeacon1 = "Unknown Beacon";



        // see if frequency is available here - diag print
       // CalcFreq = (wf->rfcarrier() + (wf->USB() ? 1.0 : -1.0) * (frequency + fout));
//        printf("\nCalc Frequency at file creation is = %13.3f\n",CalcFreq);

	// Create integer rep for analysis (add offset to get correct rounding)
	CalcFrqNum = int((CalcFreq+100)/10000);
//        printf("\nCalc Freq Num /10000 string at file creation is = %u\n",CalcFrqNum);

	// Calculate the Beacon1 indicator to establish file storge data type (as frequency data)

	switch (CalcFrqNum)
	{
     		case 250:
			{
	     		Beacon1 = "WWV2p5";
			break;
			};

     		case 500:
			{
     			Beacon1 = "WWV5";
			break;
			};

                case 1000:
			{
                	Beacon1 = "WWV10";
			break;
			};

                case 1500:
			{
                	Beacon1 = "WWV15";
			break;
			};

                case 2000:
			{
	               	Beacon1 = "WWV20";
			break;
			};

                case 2500:
			{
                	Beacon1 = "WWV25";
			break;
			};

                case 333:
			{
                	Beacon1 = "CHU3";
			break;
			};

                case 785:
			{
                	Beacon1 = "CHU7";
			break;
			};

                case 1467:
			{
                	Beacon1 = "CHU14";
			break;
			};

      		default:
 	    		Beacon1 = "Unknown";
	}

	printf("\nDecoded Beacon1 for Frequency %8.0f MHz is %s \n",CalcFreq, Beacon1.c_str());


	// diag to force freq of Beacon1
	//        Beacon1 = "WWV2p5";
	//        Beacon1 = "WWV5";
	//        Beacon1 = "WWV10";
	//        Beacon1 = "WWV15";
	//        Beacon1 = "WWV20";
	//        Beacon1 = "WWV25";
	//        Beacon1 = "CHU3";
	//        Beacon1 = "CHU7";
	//        Beacon1 = "CHU14";

	// Create and write the Beacon1 file with new days beacon1 descriptor


	// see if beacon1 file exists
//

	// check if beacon1 freq has changed - save if it did
	if (SavedBeacon1 != Beacon1)
	{
        
	FILE *out = fopen(Beacon1Path.c_str(), "w"); //create [write] for new data file
		/**if (unlikely(!out)) {
			LOG_PERROR("fl_fopen");
			return;
		}**/
        
	// file opened - write out Beacon1 Name
	fprintf(out, "%s",Beacon1.c_str());
	fclose(out);

        printf("\nWriting New Beacon1 File with Beacon1 = %s\n",Beacon1.c_str());  //diag line

	}
	else

	printf("\nBeacon1 %s has not changed", Beacon1.c_str());


	// read in the Metadata File

	// Establish file storge data type as frequency data
        DataTyp = "FRQ"; // or MAG or TMP

	// open the output file
	createfilename();

	//Open the data file for creation (write) operation
	//first check to see if already created

	if (fopen(analysisFileName.c_str(), "r+") != nullptr)
	{
	// file exists! - use it and keep adding to it
	// indicate in status line that file write to existing file
	        printf("Using Existing Analysis File");
		write_to_csv = true;  //say to do writes to file
	}
	else
	{
	        printf("Creating New Analysis File");
		FILE *out = fopen(analysisFileName.c_str(), "w"); //create new data file
		/**if (unlikely(!out)){
			LOG_PERROR("fl_fopen");
			return;
			}**/

        // file opened - write out header info
        printf("Creating analysis file");

        // Write out the initial header info line to the new .csv file

        // use for initial header containing full ISO date & time  YYYY-MM-DDTHH:MM:SSZ
       fprintf(out, "#,%sZ,%s,%s,%s,%s,%s,%s\n",FileData,NodeNum.c_str(),GridSqr.c_str(),LatLonElv.c_str(),CityState.c_str(),Radio1ID.c_str(),Beacon1.c_str());
        //Diag printout
//        printf("Initial Hdr>#,%sZ,%s,%s,%s,%s,%s,%s\n",FileData,NodeNum.c_str(),GridSqr.c_str(),LatLonElv.c_str(),CityState.c_str(),Radio1ID.c_str(),Beacon1.c_str());

	// Print out saved Metadat contained in ~/PSWS/Sinfo/ directory (alreadd read in)
        // CityState; CallSign; LatLonElv; FreqStd; NodeNum; GridSqr; Radio1; Radio1ID; Antenna; Station;

        fprintf(out, "#######################################\n");
        fprintf(out, "# MetaData for Grape Gen 1 Station\n");
        fprintf(out, "#\n");
        fprintf(out, "# Station Node Number      %s\n", NodeNum.c_str());
        fprintf(out, "# Callsign                 %s\n", CallSign.c_str());
        fprintf(out, "# Grid Square              %s\n", GridSqr.c_str());
        fprintf(out, "# Lat, Long, Elv           %s\n", LatLonElv.c_str());
        fprintf(out, "# City State               %s\n", CityState.c_str());
        fprintf(out, "# Radio1                   %s\n", Radio1.c_str());
        fprintf(out, "# Radio1ID                 %s\n", Radio1ID.c_str());
        fprintf(out, "# Antenna                  %s\n", Antenna.c_str());
        fprintf(out, "# Frequency Standard       %s\n", FreqStd.c_str());
        fprintf(out, "# System Info              %s\n", System.c_str());
        fprintf(out, "#\n");
        fprintf(out, "# Beacon Now Decoded       %s\n", Beacon1.c_str());
        fprintf(out, "#\n");
        fprintf(out, "#######################################\n");

        // read in the Metadata File

        ifstream MetadataFile (MetadataPath);
	        if (MetadataFile.is_open())
       	 	{
                        fprintf(out, "# --- Extra Metadata File ---\n");
                	while (getline(MetadataFile,Metadata))
				{
					fprintf(out,"# %s\n", Metadata.c_str());
//			        	printf("Metadata = %s\n",Metadata.c_str());  //diag line
				}
			fprintf(out,"#\n"); // Make the file end look nice
                	MetadataFile.close();
        	}
        	else
                Metadata = "No Metadata file";

//        printf("\n1st Metadata line = %s\n",Metadata.c_str());  //diag line

        // Create the headers for the columns in the .CSV file usage
        fprintf(out, "UTC,Freq,Freq Err,Vpk,dBV(Vpk)\n");

        fclose(out);

        //printf("Creating analysis file %s \n\n", analysisFilename.c_str());  //diag output

        write_to_csv = true;  //say to do writes to file
        }

	printf("\nExiting start_csv routine\n\n"); //diag pring
}


int main()
{
    printf("Starting");
    start_csv();
    printf("Done");
}