/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   testNEST.cpp
 * Author: brodsky3
 *
 * Created on August 1, 2017, 1:03 PM
 */

/* 
 * Original testNEST.cpp has been modified with python wrapper so that after compiling the NEST can be imported into python space as a module
 * Author for python wrpper: D.Q. Huang
 * Date: June 8, 2018
 * version 1.0


 * Parts has been added or modifed based on the original testNEST.cpp are indicated by 
 * "// this part to be added for python wrapper --->" and "// this part to be added for python wrapper <---"
 * where "--->" indicates the beginning of added part and "<----" indicates the end of the added part


 * The wrapper contains five parts:
 *  1. std_vector_to_py_list function, which is used to convert c++ vector to python-style list
 *  2. nestOP stuct, which is the output type when run NEST in python
 *  3. nest_py_wrapper,the wrapper class 
 *  4. testNEST function, which is a function of nest_py_wrapper class, and the original main function
 *  5. BOOST_PYTHON_MODULE(wrapper), at the very end of file, which wrappes up class and function defined to be callable by python


 * Follow "readme.txt" to learn how to set up boost python and compile the code and run the module in python


 * In case there is a new NEST output(e.g. nph, ne, etc.) that needed to be added, follow the following steps to modify the code (use nph as example)
 *  1. add "boost::python::list nph;" into "nestOP" struct
 *  2. add "vector<int> nph;" into "nestOP nest_py_wrapper::testNEST(boost::python::list command_input)" function
 *  3. add "nest_op.nph = (std_vector_to_py_list<int>(nph));" into "nestOP nest_py_wrapper::testNEST(boost::python::list command_input)" function
 *  4. add "nph" argument into "testNEST(argc, argv, nph, ne, s1_raw, s1_zCorr, s1c_spike, ne_extract, s2_raw, s2_zCorr);" which is in "nestOP nest_py_wrapper::testNEST(boost::python::list command_input)" function
 *  5. add "vector<int>& nph" argument into "int testNEST(int argc, char** argv, vector<int>& nph, vector<int>& ne, vector<double>& s1_raw, vector<double>& s1_zCorr, vector<double>& s1c_spike, vector<int>& ne_extract, vector<double>& s2_raw, vector<double>& s2_zCorr);"
 *  6. add "nph.push_back(quanta.photons);" inside the body of "int testNEST(int argc, char** argv, vector<int>& nph, vector<int>& ne, vector<double>& s1_raw, vector<double>& s1_zCorr, vector<double>& s1c_spike, vector<int>& ne_extract, vector<double>& s2_raw, vector<double>& s2_zCorr);"
 *  7. add ".def_readonly("nph",&nestOP::nph)" into "BOOST_PYTHON_MODULE(wrapper)"

 * In the output
    nph -> number of photons generated at site
    ne --> number of electros generated at site
    s1_raw --> s1 raw size [PE]
    s1_zCorr --> s1 size with z correction
    s1c_spike --> s1 spike count with correction
    ne_extract --> number of electrons extracted
    s2_raw --> s2 raw size
    s2_zCorr --> s2 size with drift correction
 */ 




#include "NEST.hh"
#include "TestSpectra.hh"
#include "analysis.hh"

#include "Detectors/DetectorExample_XENON10.hh"
#include "Detectors/LUX_Run3.hh"
#include "Detectors/LUX_Run4.hh"

// this part to be added for python wrapper --->
#include <iostream>
#include <string>
#include <vector>
#include <boost/python.hpp>
#include <boost/python/list.hpp>
#include <boost/python/extract.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
// this part to be added for python wrapper <---

using namespace std;
using namespace NEST;

/*
 * 
 */

double band[200][6], energies[3];
vector<vector<double>> GetBand ( vector<double> S1s, vector<double> S2s, bool resol );
void GetEnergyRes ( vector<double> Es );

// this part to be added for python wrapper --->
template<class T> boost::python::list std_vector_to_py_list(const std::vector<T>& v)
{
    boost::python::object get_iter = boost::python::iterator<std::vector<T> >();
    boost::python::object iter = get_iter(v);
    boost::python::list l(iter);
    return l;
}

struct nestOP{
    boost::python::list nph;
    boost::python::list ne;
    boost::python::list s1_raw;
    boost::python::list s1_zCorr;
    boost::python::list s1c_spike;
    boost::python::list ne_extract;
    boost::python::list s2_raw;
    boost::python::list s2_zCorr;
};

class nest_py_wrapper{
private:
    int argc;
    char** argv;
    nestOP nest_op;
    int print_result = 1;
    //  vector<int> nph;
    //  vector<int> ne;
    //  vector<double> s1_raw;
    //  vector<double> s1_zCorr;
    //  vector<double> s1c_spike;
    //  vector<int> ne_extract;
    //  vector<double> s2_raw;
    //  vector<double> s2_zCorr;
    int testNEST(int argc, char** argv, int print_result, vector<int>& nph, vector<int>& ne, vector<double>& s1_raw, vector<double>& s1_zCorr, vector<double>& s1c_spike, vector<int>& ne_extract, vector<double>& s2_raw, vector<double>& s2_zCorr);
public:
    nestOP testNEST(boost::python::list command_input);
    void print_result_or_not(int a){print_result = a;}
};

nestOP nest_py_wrapper::testNEST(boost::python::list command_input){
    //int argc;
    //char** argv;
    vector<int> nph;
    vector<int> ne;
    vector<double> s1_raw;
    vector<double> s1_zCorr;
    vector<double> s1c_spike;
    vector<int> ne_extract;
    vector<double> s2_raw;
    vector<double> s2_zCorr;
    vector<boost::python::list> result;
    int l = len(command_input);
    argc = l+1;
    argv = (char**)std::malloc((argc)*sizeof(char *));
    std::string str_[argc];
    for(int i = 0; i < argc; i++){
    	if(i==0){argv[i] = "ignore";}
    	else{
        str_[i] = boost::python::extract<std::string>(command_input[i-1]);
        argv[i] = const_cast<char *>(str_[i].c_str());
        }
    }
    testNEST(argc, argv, print_result, nph, ne, s1_raw, s1_zCorr, s1c_spike, ne_extract, s2_raw, s2_zCorr);
    free(argv);
    
    //  std::cout<<endl<<endl<<endl;
    
    //  for(int i = 0; i < nph.size();i++){
    //    cout<<nph[i]<<endl;
    //  }
    boost::python::object print =
    boost::python::import("__main__").attr("__builtins__").attr("print");
    
    nest_op.nph = (std_vector_to_py_list<int>(nph));
    nest_op.ne = (std_vector_to_py_list<int>(ne));
    nest_op.s1_raw = (std_vector_to_py_list<double>(s1_raw));
    nest_op.s1_zCorr = (std_vector_to_py_list<double>(s1_zCorr));
    nest_op.s1c_spike = (std_vector_to_py_list<double>(s1c_spike));
    nest_op.ne_extract = (std_vector_to_py_list<int>(ne_extract));
    nest_op.s2_raw = (std_vector_to_py_list<double>(s2_raw));
    nest_op.s2_zCorr  = (std_vector_to_py_list<double>(s2_zCorr));
    
    if(0)
    {
        for(int i = 0; i < len(nest_op.nph);i++){
            print(boost::python::object(nest_op.nph[i]));
        }
    }
    return nest_op;
}

// this part to be added for python wrapper <-------

//this method declaration used to be that of the main method. Simply change the name (and arguments if needed)
// this part to be modified for python wrapper --->
int nest_py_wrapper::testNEST (int argc, char** argv, int print_result, vector<int>& nph, vector<int>& ne, vector<double>& s1_raw, vector<double>& s1_zCorr, vector<double>& s1c_spike, vector<int>& ne_extract, vector<double>& s2_raw, vector<double>& s2_zCorr) {
    // this part to be modified for python wrapper <---
    
  // Instantiate your own VDetector class here, then load into NEST class constructor
	//DetectorExample_XENON10* detector = new DetectorExample_XENON10();
	LUX_Run3* detector = new LUX_Run3();
	//LUX_Run4* detector = new LUX_Run4();
	NEST::NESTcalc n(detector);

	vector<double> signal1,signal2,signalE, vTable, NuisParam={1.,1.}; int index;
  string position, delimiter, token; size_t loc;
  double pos_x,pos_y,pos_z,r,phi,driftTime, field, vD,vD_middle, atomNum=0,massNum=0, keVee=0.0;
  
  if (argc < 7)
    {
      cout << "This program takes 6 (or 7) inputs, with Z position in mm from bottom of detector:" << endl;
      cout << "\t./testNEST numEvts type_interaction E_min[keV] E_max[keV] field_drift[V/cm] x,y,z-position[mm] {optional:seed}" << endl << endl;
      cout << "For 8B or WIMPs, numEvts is kg-days of exposure:" << endl;
      cout << "\t./testNEST exposure[kg-days] {WIMP} m[GeV] x-sect[cm^2] field_drift[V/cm] x,y,z-position[mm] {optional:seed}" << endl << endl;
      cout << "For cosmic-ray muons or other similar particles with elongated track lengths:" << endl;
      cout << "\t./testNEST numEvts {MIP} LET[MeV*cm^2/gram] step_size[cm] field_drift[V/cm] x,y,z-position[mm] {optional:seed}" << endl << endl;
      return 0;
    }
  unsigned long int numEvts = atoi(argv[1]);
  
  string type = argv[2];
  INTERACTION_TYPE type_num;
	TestSpectra spec;
  if ( type == "NR" || type == "neutron" ) type_num = NR;
  else if (type == "WIMP")
    {
      if (atof(argv[3])<0.44) { cerr << "WIMP mass too low, you're crazy!" << endl; return 0; }
      type_num = WIMP;
      spec.wimp_spectrum_prep= spec.WIMP_prep_spectrum(atof(argv[3]), E_step);
      numEvts = RandomGen::rndm()->poisson_draw(spec.wimp_spectrum_prep.integral * 1.0 * atof(argv[1]) * atof(argv[4]) / 1e-36);
    }
  else if ( type == "B8" || type == "Boron8" || type == "8Boron" || type == "8B" || type == "Boron-8" )
    {
      type_num = B8;
      numEvts = RandomGen::rndm()->poisson_draw(0.0026 * atof(argv[1]));
    } else if ( type == "DD" || type == "D-D" ) type_num = DD;
  else if ( type == "AmBe" ) type_num = AmBe;
  else if ( type == "Cf" || type == "Cf252" || type == "252Cf" || type == "Cf-252" ) type_num = Cf;
  else if ( type == "ion" || type == "nucleus" || type == "alpha" ) {
    type_num = ion;
    if ( type == "alpha" ) {
      atomNum = 2; massNum = 4;
    }
    else {
      cerr << "Atomic Number: "; cin >> atomNum;
      cerr << "Mass Number: "; cin >> massNum;
    } if ( atomNum == ATOM_NUM ) type_num = NR;
  }
  else if ( type == "gamma" || type == "gammaRay" ||
	    type == "x-ray" || type == "xray" || type == "xRay" || type == "X-ray" || type == "Xray" || type == "XRay" )
    type_num = gammaRay; //includes photo-absorption and electron capture
  else if ( type == "Kr83m" || type == "83mKr" || type == "Kr83" ) type_num = Kr83m;
  else if ( type == "CH3T" || type == "tritium" ) type_num = CH3T;
  else if ( type == "beta" || type == "ER" || type == "Compton" || type == "compton" || type == "electron" || type == "e-" ||
	    type == "muon" || type == "MIP" || type == "LIP" || type == "mu" || type == "mu-" )
    type_num = beta; //default electron recoil model
  else {
    cerr << "UNRECOGNIZED PARTICLE TYPE!! VALID OPTIONS ARE:" << endl;
    cerr << "NR or neutron," << endl;
    cerr << "WIMP," << endl;
    cerr << "B8 or Boron8 or 8Boron or 8B or Boron-8," << endl;
    cerr << "DD or D-D," << endl;
    cerr << "AmBe," << endl;
    cerr << "Cf or Cf252 or 252Cf or Cf-252," << endl;
    cerr << "ion or nucleus," << endl;
    cerr << "alpha," << endl;
    cerr << "gamma or gammaRay," << endl;
    cerr << "x-ray or xray or xRay or X-ray or Xray or XRay," << endl;
    cerr << "Kr83m or 83mKr or Kr83," << endl;
    cerr << "CH3T or tritium, and" << endl;
    cerr << "beta or ER or Compton or compton or electron or e-" << endl;
    cerr << "muon or MIP or LIP or mu or mu-" << endl;
    return 0;
  }
  
  double eMin = atof(argv[3]);
  double eMax = atof(argv[4]);

  double rho = n.SetDensity ( detector->get_T_Kelvin(), detector->get_p_bar() ); //cout.precision(12);
  if ( rho < 1. ) detector->set_inGas(true);
  
	// Print g1 and g2 values, requiring S2 calculation for 1 SE and x,y,d = 0,0,0
  vector<double> secondary = n.GetS2 ( 1, 0., 0., 0., 1. );
  
	if ( atof(argv[5]) == -1. ) {
    vTable = n.SetDriftVelocity_NonUniform(rho, z_step);
    vD_middle = vTable[int(floor(.5*detector->get_TopDrift()/z_step))];
  }
  else vD_middle = n.SetDriftVelocity(detector->get_T_Kelvin(), rho, atof(argv[5]));
  cout << "Density = " << rho << " g/mL" << "\t";
  cout << "central vDrift = " << vD_middle << " mm/us\n";
  cout << "\t\t\t\t\t\t\t\t\t\tNegative numbers are flagging things below threshold!\n";
  
  if ( type_num == Kr83m && eMin == 9.4 && eMax == 9.4 )
    fprintf(stdout, "t [ns]\t\tE [keV]\t\tfield [V/cm]\ttDrift [us]\tX,Y,Z [mm]\tNph\tNe-\tS1_raw [PE]\tS1_Zcorr\tS1c_spike\tNe-Extr\tS2_rawArea\tS2_Zcorr [phd]\n");
  else
    fprintf(stdout, "E [keV]\t\tfield [V/cm]\ttDrift [us]\tX,Y,Z [mm]\tNph\tNe-\tS1_raw [PE]\tS1_Zcorr\tS1c_spike\tNe-Extr\tS2_rawArea\tS2_Zcorr [phd]\n");
  
  if (argc >= 8) RandomGen::rndm()->SetSeed(atoi(argv[7]));
  
  if ( type_num != WIMP && type_num != B8 ) {
    NEST::YieldResult yieldsMax = n.GetYields(type_num, eMax, rho, detector->FitEF(0., 0., detector->get_TopDrift()/2.),
					      double(massNum), double(atomNum), NuisParam);
    if ( (0.1*yieldsMax.PhotonYield) > (2.*maxS1) && eMin != eMax )
      cerr << "\nWARNING: Your energy maximum may be too high given your maxS1.\n";
  }
  
  double keV = -999;
  for (unsigned long int j = 0; j < numEvts; j++) {
    
    if (eMin == eMax) {
      keV = eMin;
    } else {
      switch (type_num) {
      case CH3T:
	keV = spec.CH3T_spectrum(eMin, eMax);
	break;
      case B8: //normalize this to ~3500 / 10-ton / year, for E-threshold of 0.5 keVnr, OR 180 evts/t/yr/keV at 1 keV
	keV = spec.B8_spectrum(eMin, eMax);
	break;
      case AmBe: //for ZEPLIN-III FSR from HA (Pal '98)
	keV = spec.AmBe_spectrum(eMin, eMax);
	break;
      case Cf:
	keV = spec.Cf_spectrum(eMin, eMax);
	break;
      case DD:
	keV = spec.DD_spectrum(eMin, eMax);
	break;
      case WIMP:
	{
          keV = spec.WIMP_spectrum(spec.wimp_spectrum_prep, atof(argv[3]));
	}
	break;
      default:
	keV = eMin + (eMax - eMin) * RandomGen::rndm()->rand_uniform();
	break;
      }
    }
    
    if ( type_num != WIMP ) {
      if (keV > eMax) keV = eMax;
      if (keV < eMin) keV = eMin;
    }
    
  Z_NEW:
    if ( atof(argv[6]) == -1. ) { // -1 means default, random location mode
      pos_z = 0. + ( detector->get_TopDrift() - 0. ) * RandomGen::rndm()->rand_uniform(); // initial guess
      r = detector->get_radius() * sqrt ( RandomGen::rndm()->rand_uniform() );
      phi = 2.*M_PI*RandomGen::rndm()->rand_uniform();
      pos_x = r * cos(phi); pos_y = r * sin(phi);
    }
    else {
      position = argv[6];
      delimiter = ",";
      loc = 0; int i = 0;
      while ( (loc = position.find(delimiter)) != string::npos ) {
	token = position.substr(0,loc);
	if ( i == 0 ) pos_x = stof(token);
	else pos_y = stof(token);
	position.erase(0,loc+delimiter.length());
	i++;
      }
      pos_z = stof(position);
      if ( stof(position) == -1. )
	pos_z = 0. + ( detector->get_TopDrift() - 0. ) * RandomGen::rndm()->rand_uniform();
      if ( stof(token) == -999. ) {
	r = detector->get_radius() * sqrt ( RandomGen::rndm()->rand_uniform() );
	phi = 2.*M_PI*RandomGen::rndm()->rand_uniform();
	pos_x = r * cos(phi); pos_y = r * sin(phi); }
    }
    
    if ( atof(argv[5]) == -1. ) { // -1 means use poly position dependence
			field = detector->FitEF(pos_x, pos_y, pos_z);
    }
    else field = atof(argv[5]);
    
    if ( field <= 0. )
      cerr << "\nWARNING: A LITERAL ZERO FIELD MAY YIELD WEIRD RESULTS. USE A SMALL VALUE INSTEAD.\n";
    
    if ( atof(argv[5]) == -1. ) {
      //for ( int jj = 0; jj < vTable.size(); jj++ ) //DEBUG
      //cerr << double(jj)*z_step << "\t" << vTable[jj] << endl;
      index = int(floor(pos_z/z_step));
      vD = vTable[index];
    }
    else
      vD = n.SetDriftVelocity(detector->get_T_Kelvin(),rho,field);
    driftTime = ( detector->get_TopDrift() - pos_z ) / vD; // (mm - mm) / (mm / us) = us
    if ( atof(argv[5]) != -1. && detector->get_dt_min() > ( detector->get_TopDrift() - 0. ) / vD )
      { cerr << "ERROR: dt_min is too restrictive (too large)" << endl; return 0; }
    if ( (driftTime > detector->get_dt_max() || driftTime < detector->get_dt_min()) && (atof(argv[6]) == -1. || stof(position) == -1.) )
      goto Z_NEW;
    if ( detector->get_dt_max() > (detector->get_TopDrift()-0.)/vD && !j )
      { cerr << "WARNING: dt_max is greater than max possible" << endl; }
    
    NEST::YieldResult yields; NEST::QuantaResult quanta;
    double zStep = eMax;
    if ( type == "muon" || type == "MIP" || type == "LIP" || type == "mu" || type == "mu-" ) {
      double dEOdx = eMin, eStep = dEOdx * rho * zStep * 1e3, refEnergy = 1e6; keV = 0.;
      int Nph = 0, Ne = 0;
      for ( double zz = detector->get_TopDrift(); zz > 0; zz -= zStep*10. ) { //only VERTICAL-going muons work right now
	yields = n.GetYields ( beta, refEnergy, rho, detector->FitEF(pos_x, pos_y, zz), double(massNum), double(atomNum), NuisParam );
	quanta = n.GetQuanta ( yields, rho );
	Nph+= quanta.photons * (eStep/refEnergy);
	Ne += quanta.electrons*(eStep/refEnergy);
	keV+= eStep;
			}
      quanta.photons = Nph; quanta.electrons = Ne;
      pos_z = detector->get_TopDrift() / 2.; //approximate everything as middle of detector since muon goes through whole detector
      driftTime = ( detector->get_TopDrift() - pos_z ) / vD_middle;
    	field = detector->FitEF(pos_x, pos_y, pos_z);
		}
    else {
      yields = n.GetYields(type_num,keV,rho,field,
			   double(massNum),double(atomNum),NuisParam);
      quanta = n.GetQuanta(yields,rho);
    }
    
    vector<double> scint = n.GetS1(quanta.photons,pos_x,pos_y,pos_z,
				   vD,vD_middle,type_num);
    if ( usePE == 0 && fabs(scint[3]) > minS1 && scint[3] < maxS1 )
      signal1.push_back(scint[3]);
    else if ( usePE == 1 && fabs(scint[5]) > minS1 && scint[5] < maxS1 )
      signal1.push_back(scint[5]);
    else if ( usePE >= 2 && fabs(scint[7]) > minS1 && scint[7] < maxS1 )
      signal1.push_back(scint[7]);
    else signal1.push_back(0.);
    
    vector<double> scint2= n.GetS2(quanta.electrons, pos_x, pos_y, driftTime, vD);
    if ( usePE == 0 && fabs(scint2[5]) > minS2 && scint2[5] < maxS2 )
      signal2.push_back(scint2[5]);
    else if ( usePE >= 1 && fabs(scint2[7]) > minS2 && scint2[7] < maxS2 )
      signal2.push_back(scint2[7]); //no spike option for S2
    else signal2.push_back(0.);
    
    if ( !MCtruthE ) {
      double Nph, g1 = fabs(scint[8]), Ne, g2 = fabs(scint2[8]);
      if ( usePE == 0 )
	Nph= fabs(scint[3]) / (g1*fabs(scint[3]/scint[5]));
      else if ( usePE == 1 ) Nph = fabs(scint[5]) / g1;
      else Nph = fabs(scint[7]) / g1;
      if ( usePE == 0 )
	Ne = fabs(scint2[5]) / (g2*fabs(scint2[5]/scint2[7]));
      else Ne = fabs(scint2[7]) / g2;
      if ( signal1.back() <= 0. )
	Nph= 0.;
      if ( signal2.back() <= 0. )
	Ne = 0.;
      if ( yields.Lindhard > DBL_MIN && Nph > 0. && Ne > 0. ) {
	keV = ( Nph + Ne ) * W_DEFAULT * 1e-3 / yields.Lindhard;
	keVee+=( Nph + Ne ) * W_DEFAULT * 1e-3;
      }
      else
	keV = 0.;
    }
    if ( signal1.back() <= 0. || signal2.back() <= 0. )
      signalE.push_back(0.);
    else
      signalE.push_back(keV);
    
    if ( !MCtruthPos && fabs(scint2[6]) > DBL_MIN ) {
      vector<double> xySmeared(2); xySmeared = n.xyResolution ( pos_x, pos_y, fabs(scint2[6]) );
      pos_x = xySmeared[0];
      pos_y = xySmeared[1];
    }
// this part to be added for python wrapper --->  (print_result was set to be 0 in original NEST code)
    if ( print_result ) { //fabs(scint[7]) > DBL_MIN && fabs(scint2[7]) > DBL_MIN ) { //if you want to skip specific below-threshold events, then please comment in this if statement
    // this part to be added for python wrapper <----
      printf("%.6f\t%.6f\t%.6f\t%.0f, %.0f, %.0f\t%d\t%d\t",keV,field,driftTime,pos_x,pos_y,pos_z,quanta.photons,quanta.electrons); //comment this out when below line in
      //printf("%.6f\t%.6f\t%.6f\t%.0f, %.0f, %.0f\t%lf\t%lf\t",keV,field,driftTime,pos_x,pos_y,pos_z,yields.PhotonYield,yields.ElectronYield); //for when you want means
      if ( keV > 1000. ) { //switch to exponential notation to make output more readable, if energy is too high (>1 MeV)
	printf("%e\t%e\t%e\t", scint[2], scint[5], scint[7]);
	printf("%li\t%e\t%e\n", (long)scint2[0], scint2[4], scint2[7]);
      }
      else {
	printf("%.6f\t%.6f\t%.6f\t", scint[2], scint[5], scint[7]); //see GetS1 inside of NEST.cpp for full explanation of all 8 scint return vector elements. Sample 3 most common
	printf("%i\t%.6f\t%.6f\n", (int)scint2[0], scint2[4], scint2[7]); //see GetS2 inside of NEST.cpp for full explanation of all 8 scint2 vector elements. Change as you desire
      }
    }
    // this part to be added for python wrapper --->
    nph.push_back(quanta.photons);
    ne.push_back(quanta.electrons);
    s1_raw.push_back(scint[2]);
    s1_zCorr.push_back(scint[5]);
    s1c_spike.push_back(scint[7]);
    ne_extract.push_back(scint2[0]);
    s2_raw.push_back(scint2[4]);
    s2_zCorr.push_back(scint2[7]);
    // this part to be added for python wrapper <----
  }
  
  if ( eMin != eMax ) {
    if ( useS2 == 2 )
      GetBand ( signal2, signal1, false );
    else
      GetBand ( signal1, signal2, false );
    fprintf(stderr,"Bin Center\tBin Actual\tHist Mean\tMean Error\tHist Sigma\t\tEff[%%>thr]\n");
    for ( int j = 0; j < numBins; j++ ) {
      fprintf(stderr,"%lf\t%lf\t%lf\t%lf\t%lf\t\t%lf\n",band[j][0],band[j][1],band[j][2],band[j][4],band[j][3],band[j][5]*100.);
      if ( band[j][0] <= 0.0 || band[j][1] <= 0.0 || band[j][2] <= 0.0 || band[j][3] <= 0.0 || band[j][4] <= 0.0 || band[j][5] <= 0.0 ||
           std::isnan(band[j][0]) || std::isnan(band[j][1]) || std::isnan(band[j][2]) || std::isnan(band[j][3]) || std::isnan(band[j][4]) || std::isnan(band[j][5]) )
	{ if ( eMax != -999. ) cerr << "WARNING: Insufficient number of high-energy events to populate highest bins is likely.\n"; eMax = -999.; }
    }
  }
  else {
    GetBand ( signal1, signal2, true );
    GetEnergyRes ( signalE );
    if ( type_num == NR ) {
      fprintf(stderr,"S1 Mean\t\tS1 Res [%%]\tS2 Mean\t\tS2 Res [%%]\tEc [keVnr]\tEc Res[%%]\tEff[%%>thr]\tEc [keVee]\n");
      keVee /= numEvts;
    }
    else
      fprintf(stderr,"S1 Mean\t\tS1 Res [%%]\tS2 Mean\t\tS2 Res [%%]\tEc Mean\t\tEc Res[%%]\tEff[%%>thr]\n"); //the C here refers to the combined (S1+S2) energy scale
    for ( int j = 0; j < numBins; j++ ) {
      fprintf(stderr,"%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t",band[j][0],band[j][1]/band[j][0]*100.,
	      band[j][2],band[j][3]/band[j][2]*100.,energies[0],energies[1]/energies[0]*100.,energies[2]*100.);
      if ( type_num == NR ) fprintf(stderr,"%lf\n",keVee/energies[2]); else fprintf(stderr,"\n");
      if ( band[j][0] <= 0.0 || band[j][1] <= 0.0 || band[j][2] <= 0.0 || band[j][3] <= 0.0 ||
	   std::isnan(band[j][0]) || std::isnan(band[j][1]) || std::isnan(band[j][2]) || std::isnan(band[j][3]) )
	cerr << "CAUTION: YOUR S1 and/or S2 MIN and/or MAX may be set to be too restrictive, please check.\n";
      else if ( energies[0] == eMin || energies[0] == eMax || energies[1] <= 0.0 )
	cerr << "If your energy resolution is 0% then you probably still have MC truth energy on." << endl;
      else ;
    }
  }
  
  return 1;
  
}

vector<vector<double>> GetBand ( vector<double> S1s,
				 vector<double> S2s, bool resol ) {
  
  vector<vector<double>> signals;
  signals.resize(200,vector<double>(1,-999.));
  double binWidth, border;
  if ( useS2 == 2 ) {
    binWidth = ( maxS2 - minS2 ) / double(numBins);
    border = minS2;
  }
  else {
    binWidth = ( maxS1 - minS1 ) / double(numBins);
    border = minS1;
  }
  int i = 0, j = 0; double s1c, numPts;
  unsigned long reject[200] = {0};
  
  if ( resol ) {
    numBins = 1;
    binWidth = DBL_MAX;
  }
  
  for ( i = 0; i < S1s.size(); i++ ) {
    for ( j = 0; j < numBins; j++ ) {
      s1c = border + binWidth/2. + double(j) * binWidth;
      if ( i == 0 && !resol ) band[j][0] = s1c;
      if ( fabs(S1s[i]) > (s1c-binWidth/2.) && fabs(S1s[i]) < (s1c+binWidth/2.) ) {
	if ( S1s[i] >= 0. && S2s[i] >= 0. ) {
	  if ( resol ) {
	    signals[j].push_back(S2s[i]);
	  }
	  else {
	    if ( useS2 == 0 )
	      { if ( S1s[i] && S2s[i] ) signals[j].push_back(log10(S2s[i]/S1s[i])); else signals[j].push_back(0.); }
	    else if ( useS2 == 1 )
	      { if ( S1s[i] && S2s[i] ) signals[j].push_back(log10(S2s[i])); else signals[j].push_back(0.); }
	    else
	      { if ( S1s[i] && S2s[i] ) signals[j].push_back(log10(S1s[i]/S2s[i])); else signals[j].push_back(0.); }
	  }
	  band[j][2] += signals[j].back();
	  if ( resol )
	    band[j][0] += S1s[i];
	  else
	    band[j][1] += S1s[i];
	}
	else
	  reject[j]++;
	break; }
    }
  }
  
  for ( j = 0; j < numBins; j++ ) {
    if ( band[j][0] <= 0. && !resol ) band[j][0] = border + binWidth/2. + double(j) * binWidth;
    signals[j].erase(signals[j].begin());
    numPts = (double)signals[j].size();
    if (resol)
      band[j][0] /= numPts;
    band[j][1] /= numPts;
    band[j][2] /= numPts;
    for ( i = 0; i < (int)numPts; i++ ) {
      if ( signals[j][i] != -999. ) band[j][3] += pow(signals[j][i]-band[j][2],2.);
      if ( resol && S1s[i] >= 0.0 ) band[j][1] += pow(S1s[i]-band[j][0],2.); //std dev calc
    }
    band[j][3] /= numPts - 1.;
    band[j][3] = sqrt(band[j][3]);
    if ( resol ) {
      band[j][1] /= numPts - 1.;
      band[j][1] = sqrt(band[j][1]);
    }
    band[j][4] = band[j][3] / sqrt ( numPts );
    band[j][5] = numPts/
      (numPts+double(reject[j]));
  }
  
  return signals;
  
}

void GetEnergyRes ( vector<double> Es ) {
  
  int i, numPts = Es.size();
  double numerator = 0.;
  
  for ( i = 0; i < numPts; i++ ) {
    if ( Es[i] > 0. )
      { energies[0] += Es[i]; numerator++; }
  }
  
  energies[0] /= numerator;
  
  for ( i = 0; i < numPts; i++ ) {
    if ( Es[i] > 0. )
      energies[1] += pow(energies[0]-Es[i],2.);
  }
  
  energies[1] /= numerator - 1.;
  energies[1] = sqrt(energies[1]);
  
  energies[2] = numerator / double ( numPts );
  return;
  
}

// this part to be added for python wrapper --->
using namespace boost::python;
BOOST_PYTHON_MODULE(wrapper)
{
    class_<std::vector<boost::python::list> >("double_vector1")
    .def(vector_indexing_suite<std::vector<boost::python::list> >());
    
    class_<std::vector<int> >("double_vector2")
    .def(vector_indexing_suite<std::vector<int> >());
    
    class_<std::vector<double> >("double_vector3")
    .def(vector_indexing_suite<std::vector<double> >());
    
    nestOP (nest_py_wrapper::*testNEST)(boost::python::list) = &nest_py_wrapper::testNEST;
    
    class_<nest_py_wrapper>("nest_py_wrapper")
    .def("print_result_or_not", &nest_py_wrapper::print_result_or_not)
    .def("testNEST", testNEST);
    
    class_<nestOP>("nestOP")
    .def_readonly("nph",&nestOP::nph)
    .def_readonly("ne",&nestOP::ne)
    .def_readonly("s1_raw",&nestOP::s1_raw)
    .def_readonly("s1_zCorr",&nestOP::s1_zCorr)
    .def_readonly("s1c_spike",&nestOP::s1c_spike)
    .def_readonly("ne_extract",&nestOP::ne_extract)
    .def_readonly("s2_raw",&nestOP::s2_raw)
    .def_readonly("s2_zCorr",&nestOP::s2_zCorr);
}
// this part to be added for python wrapper <---
