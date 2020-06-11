//Weilei Zeng, Nov 26 2018
/*
This code produce two random quantum codes and construct concatenated codes and reduced code. Then check their distance.
 */
#include "weilei_lib/my_lib.h"
#include <itpp/itbase.h>
#include <ctime> //to get current time
using namespace itpp;
using namespace std;


//int simulate(string title_str, string note, int mode, int sub_mode_A, int sub_mode_B, int n_low, int n_high, int k_low, int k_high, int debug);

int simulate(string title_str, string note, int mode, int sub_mode_A, int sub_mode_B, 
	     int n_low, int n_high, int k_low, int k_high, int debug,
	     int na_input, int Gax_row_input, int id_Gax, int Gaz_row_input, int id_Gaz);


int main(int args, char ** argv){
  itpp::Parser parser;
  parser.init(args,argv);
  parser.set_silentmode(true);
  int num_cores=2; parser.get(num_cores,"num_cores");
  int mode = -1; parser.get(mode,"mode"); //default values are illegal, to make sure there is an input value
  int sub_mode_A = -1; parser.get(sub_mode_A,"sub_mode_A");
  int sub_mode_B = -1; parser.get(sub_mode_B,"sub_mode_B");
  std::string title_str="no-title";  parser.get(title_str,"title");
  std::string note="no-note"; parser.get(note,"note");
  int debug = 1; //default debug on
  parser.get(debug,"debug");
  int seed=1; parser.get(seed,"seed");
  //  std::cout<<"\t seed:"<<seed;
  if (debug) std::cout<<"input seed: "<<seed<<" --> ";
  seed = seed+get_time(3);
  itpp::RNG_reset(seed); 
  if (debug)   std::cout<<"converted seed:"<<seed;
  //std::cout<<std::endl;

  int na_input;  parser.get(na_input,"na_input");
  int n_low;  parser.get(n_low,"n_low");
  int n_high;  parser.get(n_high,"n_high");
  int k_low;  parser.get(k_low,"k_low");
  int k_high;  parser.get(k_high,"k_high");

  switch (mode){
  case 1:
    //actually I can run the random simulation here. and let openmp control the threads
  case 2:
    //allow mode = 1,2, sub_mode_A = 1; sub_mode_B=1,2,3
    simulate(title_str, note, mode, sub_mode_A, sub_mode_B, n_low, n_high, k_low, k_high, debug,0,0,0,0,0);
    break;
  case 3:
    {

  //mode need to be determined here.

  //parallel for loop
	/*
	  for loop for Gax_row
	  parallel for loop for Gax
	  for loop for Gaz
	  esitimate the distance
	 */
  
      int na=na_input;
      for ( int Gax_row = 1; Gax_row< na-1; Gax_row++){
	//	const int id_Gax_MAX = (int) pow(2,  Gax_row * (na-Gax_row) ) -2 ;
	const int id_Gax_MAX = (int) pow(2,  Gax_row * (na-Gax_row) ) -1 ; //maximun all one
#pragma omp parallel for schedule(dynamic, 1) num_threads(num_cores)
	for ( int id_Gax = 1; id_Gax < id_Gax_MAX+1 ; id_Gax++){
	  //	  cout<<"                                                                 start: id_Gax="<<id_Gax<<",\t"<<endl;
	  for ( int Gaz_row = 1; Gaz_row< min(Gax_row +1,na-Gax_row); Gaz_row ++){ //check
	    const int id_Gaz_MAX = (int) pow(2, Gaz_row*(na - Gax_row)) - 1; //maximun all one
	    //	    const int id_Gaz_MAX = (int) pow(2, Gaz_row*(na - Gax_row)) - 2;
	    for ( int id_Gaz = 1; id_Gaz < id_Gaz_MAX+1 ; id_Gaz++){
	    //run the program. symmetric, symmetric inverse.
	    // simulate(title_str, note, mode, sub_mode_A, sub_mode_B, n_low, n_high, k_low, k_high, debug);
	      if (debug) cout<<"Gax_row="<<Gax_row<<",Gaz_row="<<Gaz_row<<endl;
	      sub_mode_A=2;
	      sub_mode_B=2;
	      //	      cout<<title_str.c_str()<<endl;
	      string title_str_trial=title_str+"-na"+to_string(na)+"-Gax_row"+to_string(Gax_row)+"-id_Gax"+to_string(id_Gax)
		+"-Gaz_row"+to_string(Gaz_row)+"-id_Gaz"+to_string(id_Gaz);
	      if (simulate(title_str_trial, note, mode, sub_mode_A, sub_mode_B, 0, 0, 0, 0, debug, na, Gax_row, id_Gax, Gaz_row, id_Gaz)==2){
		//duplicated case, skip following calculation		
	      }else{
		//continue calculation
		cout<<"                                                                         id_Gax_MAX="<<id_Gax_MAX<<",id_Gaz_MAX="<<id_Gaz_MAX<<endl;
		sub_mode_B=3;
		simulate(title_str_trial, note, mode, sub_mode_A, sub_mode_B, 0, 0, 0, 0, debug, na, Gax_row, id_Gax, Gaz_row, id_Gaz);
	      }
	    }
	    //	    simulate(mode, sub_mode_A, sub_mode_B,
	    //	     na, Gax_row, id_Gax, id_Gaz)
	    //  generate_code(Gax, Gaz, na, Gax_row, id_Gax, id_Gaz);
	//n_low,n_high
	  }
	}
      }
    }
    break;
  default:
    cout<<"main(): illegal mode value"<<endl;
    throw 2;
  }
  return 0;
}

int simulate(string title_str, string note, int mode, int sub_mode_A, int sub_mode_B, 
	     int n_low, int n_high, int k_low, int k_high, int debug,
	     int na_input, int Gax_row_input, int id_Gax, int Gaz_row_input, int id_Gaz){
  //return 2 when the code is duplicate, or either dax = 1 or daz = 1
  if ( mode == 3) mode =1;

  Real_Timer timer;  timer.tic();
  GF2mat Gax,Gaz,Cax,Caz;
  GF2mat Gbx,Gbz,Cbx,Cbz;
  int na,ka, Gax_row,Gaz_row;//k is not necessary number of qubits
  int nb,kb, Gbx_row,Gbz_row;
  //  int flag_save_matrices=0; // use flag_chain_complex

  //only save for cases
  const char * title = title_str.c_str();
  char filename_Gax[256];char filename_Gaz[256];char filename_Gbx[256];char filename_Gbz[256];
  sprintf(filename_Gax,"%sGax.mm",title);  sprintf(filename_Gaz,"%sGaz.mm",title);      sprintf(filename_Gbx,"%sGbx.mm",title);  sprintf(filename_Gbz,"%sGbz.mm",title);

  //  int sub_mode_A=1;
  if (debug)   cout<<mode<<endl<<title<<endl;
  switch( mode ){
    case 1://generate random codes and save
      switch ( sub_mode_A ){//sub mode for code A
      case 1:
	na=randi(n_low,n_high); ka = randi(k_low,k_high);Gax_row=randi(1,na-ka-1); Gaz_row=na-ka-Gax_row;
	getGoodQuantumCode(na,Gax_row,Gaz_row,Gax,Gaz,Cax,Caz,debug);
      //getRandomQuantumCode(na,Gax_row,Gaz_row,Gax,Gaz,Cax,Caz);
	break;
      case 2://enumerate all codes with size na
	//input: na, Gax_row, 
	{
	  na=na_input; Gax_row=Gax_row_input; Gaz_row=Gaz_row_input;
	  if ( generate_code(Gax, Gaz, na, Gax_row, id_Gax, Gaz_row, id_Gaz, debug) ==2 ){
	    if (debug) cout<<"duplicated case, return 2"<<endl;
	    //	    cout<<"*";
	    return 2;
	  }	
	  Cax=getC(Gax,Gaz);
	  Caz=getC(Gax,Gaz,1);
	  //estimate distance here, and discard cases with distance 1
	  int dax_temp = quantum_dist_v2(Gax,Gaz);
	  if (dax_temp ==1 ) {
	    if (debug) cout<<"sub_mode_B="<<sub_mode_B<<",na="<<na<<",Gax_row="<<Gax_row<<",id_Gax="<<id_Gax<<",Gaz_row="<<Gaz_row<<", id_Gaz="<<id_Gaz<<",";
	    if (debug) cout<<"discard when dax = 1"<<endl;
	    return 2 ;
	  }
	  int daz_temp = quantum_dist_v2(Gax,Gaz,1);
	  if (daz_temp ==1 ) {
	    if (debug) cout<<"sub_mode_B="<<sub_mode_B<<",na="<<na<<",Gax_row="<<Gax_row<<",id_Gax="<<id_Gax<<",Gaz_row="<<Gaz_row<<", id_Gaz="<<id_Gaz<<",";
	    //	  cout<<"na="<<na<<",Gax_row="<<Gax_row<<",sub_mode_B="<<sub_mode_B<<",id_Gax="<<id_Gax<<", id_Gaz="<<id_Gaz<<",";
	    if (debug) cout<<"discard when daz = 1"<<endl;
	    return 2 ;
	  }
	  cout<<"sub_mode_B="<<sub_mode_B<<",na="<<na<<",Gax_row="<<Gax_row<<",id_Gax="<<id_Gax<<",Gaz_row="<<Gaz_row<<", id_Gaz="<<id_Gaz<<",dax="<<dax_temp<<",daz="<<daz_temp<<endl;
	  //	  cout<<"Gax"<<Gax<<endl;
	  //	  cout<<"Gaz"<<Gaz<<endl;

	}
	break;
      default:
	cout<<"simulate(): illegal sub_mode_A value"<<endl;
	throw 2;
	break;
      }

      switch ( sub_mode_B){
        case 1: //two random codes
      
          //random code B
          nb=randi(n_low,n_high); kb = randi(k_low,k_high);Gbx_row=randi(1,nb-kb-1); Gbz_row=nb-kb-Gbx_row;
          getGoodQuantumCode(nb,Gbx_row,Gbz_row,Gbx,Gbz,Cbx,Cbz,debug);
          break;
        case 2: // identical reverse code A for code B  
          //reverse X and Z of code A for code B
          kb=ka;        
          nb=na;
          Gbx_row=Gaz_row;
          Gbz_row=Gax_row;
          Gbx=Gaz;
          Gbz=Gax;
          Cbx=Caz;
          Cbz=Cax;
          break;
        case 3: //identical code A for code B
          kb=ka;        
          nb=na;
          Gbx_row=Gax_row;
          Gbz_row=Gaz_row;
          Gbx=Gax;
          Gbz=Gaz;
          Cbx=Cax;
          Cbz=Caz;
          break;
      default:
	cout<<"simulate(): illegal sub_mode_A value"<<endl;
	throw 2;
	break;

      }
      //      cout<<"got two good quantum code"<<endl;
      break;
    case 2:      //from given input. This is to manually double check some result
      if (debug)  cout<<"check "<<title<<endl;
      Gax=MM_to_GF2mat(filename_Gax); Gaz=MM_to_GF2mat(filename_Gaz);
      Gbx=MM_to_GF2mat(filename_Gbx); Gbz=MM_to_GF2mat(filename_Gbz);
      na=Gax.cols();
      nb=Gbx.cols();

      Cax=getC(Gax,Gaz);
      Caz=getC(Gax,Gaz,1);
      Cbx=getC(Gbx,Gbz);
      Cbz=getC(Gbx,Gbz,1);


      if (debug)  {cout<<"Gax "<<Gax<<endl;  cout<<"Gaz "<<Gaz<<endl;  cout<<"Cax "<<Cax<<endl;  cout<<"Caz "<<Caz<<endl;}
      if (debug) { cout<<"Gbx "<<Gbx<<endl;  cout<<"Gbz "<<Gbz<<endl;  cout<<"Cbx "<<Cbx<<endl;  cout<<"Cbz "<<Cbz<<endl;}
      //      cout<<"debug"<<endl;
      break;
  }


  
  
  //  cout<<"check code A"<<endl;
  if (! is_quantum_code(Gax,Gaz,Cax,Caz)) throw "invalid code";

  int dax = quantum_dist_v2(Gax,Gaz);
  int daz = quantum_dist_v2(Gax,Gaz,1);

  //  cout<<"check code B"<<endl;
  if ( ! is_quantum_code(Gbx,Gbz,Cbx,Cbz)) throw "invalid code";
  //  cout<<"Gbx "<<Gbx<<endl;  cout<<"Gbz "<<Gbz<<endl;  cout<<"Cbx "<<Cbx<<endl;  cout<<"Cbz "<<Cbz<<endl;
  int dbx = quantum_dist_v2(Gbx,Gbz);
  int dbz = quantum_dist_v2(Gbx,Gbz,1);//1 to flip X and Z


  /*if ( debug) {
    cout<<"[Code A] na = "<<na<<", ";
    cout<<"dax = "<<dax<<", daz = "<<daz<<endl;
    cout<<"[Code B] nb = "<<nb<<", ";
    cout<<"dbx = "<<dbx<<", dbz = "<<dbz<<endl;
  }*/

  //  reduce(Gax,Gaz,Gbx,Gbz,dax,daz,dbx,dbz);
  //concatenate(Gax,Gaz,Gbx,Gbz,dax,daz,dbx,dbz);
  int flag_chain_complex=0; //run if 1
  //0 for reduce/subsystem, distance x
  if (product(Gax,Gaz,Gbx,Gbz,dax,daz,dbx,dbz,debug,0) == 2)   {
    std::cout<<title<<","<<note<<", time:"<<timer.toc()<<std::endl;
    flag_chain_complex=1;
  }
  //1 for reduce/subsystem, distance z
  if (product(Gax,Gaz,Gbx,Gbz,dax,daz,dbx,dbz,debug,1) == 2){
    std::cout<<title<<","<<note<<", time:"<<timer.toc()<<std::endl;
    //    std::cout<<title<<","<<note<<std::endl;
    //    std::cout<<title<<","<<note<<", sub_mode:"<<sub_mode<<std::endl;
    //    std::cout<<title<<std::endl;
    flag_chain_complex=1;
  }
  //2 for concatenate
  //  if ( product(Gax,Gaz,Gbx,Gbz,dax,daz,dbx,dbz,debug,2) == 2)    std::cout<<title<<std::endl;
  flag_chain_complex=1;
  if (     flag_chain_complex ){
    //save code A B
    if ( mode ==1 ){
      GF2mat_to_MM(Gax,filename_Gax);      GF2mat_to_MM(Gaz,filename_Gaz);
      GF2mat_to_MM(Gbx,filename_Gbx);      GF2mat_to_MM(Gbz,filename_Gbz);
    }


    // 3 for chain complex with two length-3, z distance
    // 4 for chain complex with two length-3, x distance
    //    cout<<"mode 3 4"<<endl;
    if  (product(Gax,Gaz,Gbx,Gbz,dax,daz,dbx,dbz,debug,3) == 2)  
          std::cout<<title<<","<<note<<", time:"<<timer.toc()<<std::endl;
    if  (product(Gax,Gaz,Gbx,Gbz,dax,daz,dbx,dbz,debug,4) == 2)  
          std::cout<<title<<","<<note<<", time:"<<timer.toc()<<std::endl;
  }

  if ( debug )  timer.toc_print();
  return 0;
}
