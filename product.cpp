//Weilei Zeng, Nov 26 2018
/**
 *\file product.cpp
 *\author Weilei Zeng
 * This code produce two random quantum codes and construct concatenated codes and reduced code. Then check their distance.
 */
#include "weilei_lib/weilei_lib.h"
#include <itpp/itbase.h>
#include <ctime> //to get current time
using namespace itpp;
using namespace std;
using namespace common;

//int simulate(string title_str, string note, int mode, int sub_mode_A, int sub_mode_B, int n_low, int n_high, int k_low, int k_high, int debug);


/** from given input parameter, generate two CSS codes and check
 *  result for subsystem product codes, concatenated codes and
 *  higher-dimensional hypergraph product codes.
 *@param mode 1 for random, 2 for check, 3 for two identical code ( or reversed)
 *@param sub_mode_A random construction or enumerate all cases with give size
 *@param sub_mode_B random code, identical, or reverse identical
 * generate two codes to make a product code and estimate their distances
 * Now it can run mode=2 enumerate symmetric and reverse symmetric casee
 * and mode=3, enumerate all cases
 * how about case 1? enumerate all cases
 */
int simulate(string title_str, string note, int mode, int sub_mode_A, int sub_mode_B, 
	     int n_low, int n_high, int k_low, int k_high, int debug,
	     //	     int na_input, int Gax_row_input, int id_Gax, int Gaz_row_input, int id_Gaz,
	     SubsystemProductCSSCode code
	     );

int main(int args, char ** argv){
  itpp::Parser parser;
  parser.init(args,argv);
  parser.set_silentmode(true);
  int num_cores; parser.get(num_cores,"num_cores");
  int mode = -1; parser.get(mode,"mode"); //default values are illegal, to make sure there is an input value
  int sub_mode_A = -1; parser.get(sub_mode_A,"sub_mode_A");
  int sub_mode_B = -1; parser.get(sub_mode_B,"sub_mode_B");
  std::string title_str="no-title";  parser.get(title_str,"title");
  std::string note="no-note"; parser.get(note,"note");
  int debug; //default debug on
  parser.get(debug,"debug");
  int seed; parser.get(seed,"seed");
  //  std::cout<<"\t seed:"<<seed;
  if (debug) std::cout<<"input seed: "<<seed<<" --> ";
  seed = seed+get_time(3);
  itpp::RNG_reset(seed); 
  if (debug)   std::cout<<"converted seed:"<<seed;

  //parameters for random simulation
  int na_input;  parser.get(na_input,"na_input");
  int n_low;  parser.get(n_low,"n_low");
  int n_high;  parser.get(n_high,"n_high");
  int k_low;  parser.get(k_low,"k_low");
  int k_high;  parser.get(k_high,"k_high");
  int max_trial; parser.get(max_trial,"max_trial");

  Real_Timer timer;  timer.tic();

  //the maximum number of cases to be evaluated is 1 million. Duplicated cases and distance 1 cases are not saved here.
  const int id_list_max=100000; 
  int id_list[id_list_max][5];
  //  save five number for the input of CSSCode codeA(na,Gax_row,id_Gax,Gaz_row,id_Gaz);

  switch (mode){
  case 1:
    //actually I can run all random simulation here. and let openmp control the threads. Insted of running it one by one
      //run a random simulation
    {
      //allow mode = 1, sub_mode_A = 1; sub_mode_B=1,2,3
      cout<<"max_trial = "<<max_trial<<endl;
      int count=0;
#pragma omp parallel for schedule(guided) num_threads(num_cores)
      for ( int i =0; i< max_trial; i++){
	SubsystemProductCSSCode code_temp;
	//      simulate(title_str, note, mode, sub_mode_A, sub_mode_B, n_low, n_high, k_low, k_high, debug,0,0,0,0,0, code_temp);
	string title_str_trial = title_str +"-" +to_string(i);
	simulate(title_str_trial, note, mode, sub_mode_A, sub_mode_B, n_low, n_high, k_low, k_high, debug, code_temp);
#pragma omp critical
	{
	  count++;
	  if ( count % 10 == 0 ) 
	    cout<<"count="<<count<<endl;
	}
      }
    }
    break;    
  case 2:
    {
      //check a specific case
      //allow mode = 2, sub_mode_A = 1; sub_mode_B=1,2,3
      SubsystemProductCSSCode code_temp;
      //      simulate(title_str, note, mode, sub_mode_A, sub_mode_B, 0, 0,0,0, debug,0,0,0,0,0, code_temp);
      simulate(title_str, note, mode, sub_mode_A, sub_mode_B, 0, 0,0,0, debug, code_temp);
    }
    break;
  case 3:       //enumerate all cases with size na
    {
      //statistic counts
      int total_trials=0, calculated_trials=0;//, distance_2_trials=0;
      int na=na_input;
      for ( int Gax_row = 1; Gax_row< na-1; Gax_row++){ //loop through Gax_row
	const int id_Gax_MAX = (int) pow(2,  Gax_row * (na-Gax_row) ) -1 ; //maximun when all bits are one
#pragma omp parallel for schedule(guided) num_threads(num_cores)
	for ( int id_Gax = 1; id_Gax < id_Gax_MAX+1 ; id_Gax++){
	  if ( id_Gax % 1000 == 0 )
	    cout<<"start: id_Gax="<<id_Gax<<",\t id_Gax_MAX="<<id_Gax_MAX<<",\t Gax_row="<<Gax_row<<endl;
	  for ( int Gaz_row = 1; Gaz_row< min(Gax_row +1,na-Gax_row); Gaz_row ++){ //check
	    const int id_Gaz_MAX = (int) pow(2, Gaz_row*(na - Gax_row)) - 1; //maximum when all bits are 1
	    for ( int id_Gaz = 1; id_Gaz < id_Gaz_MAX+1 ; id_Gaz++){
	      total_trials++;

	    //run the program. symmetric, reverse symmetric.
	      if (debug) cout<<"Gax_row="<<Gax_row<<",Gaz_row="<<Gaz_row<<endl;

	      //use data wrapper class
	      CSSCode codeA(na,Gax_row,id_Gax,Gaz_row,id_Gaz);
	      SubsystemProductCSSCode code;

	      //note: to enumerate one CSS code, run it here. to simulate two codes, generate all code here, then run it later.
	      //	      if (sub_mode_B ==4){ //enumerate code B as well.
		//		if ( generate_code(Gax, Gaz, na, Gax_row, id_Gax, Gaz_row, id_Gaz, debug) ==2 ){

		if ( codeA.generate_by_id(debug) == 2){
		  if (debug) cout<<"duplicated case, return 2"<<endl;
		  //	    cout<<"*";
		  //		  return 2;
		}else{
		  //it is valid code, save it for running later
		  
		  int id[5] = {na,Gax_row,id_Gax,Gaz_row,id_Gaz};
		  //cout<<"save id when calculated_trials = "<<calculated_trials<<endl;
#pragma omp critical
		  {		    
		    for ( int i_id = 0; i_id < 5; i_id ++) id_list[calculated_trials][i_id] = id[i_id];
		    calculated_trials++;
		    //	    cout<<calculated_trials<<"/"<<( total_trials/1000000 )<<"M"<<endl;
		  }
		}
		//      }

		//	      sub_mode_A=2;//enumerate all cases
	      //	      sub_mode_B=2;//reverse identical code A and B
	      //	      cout<<title_str.c_str()<<endl;
	      string title_str_trial=title_str+"-na"+to_string(na)+"-Gax_row"+to_string(Gax_row)+"-id_Gax"+to_string(id_Gax)
		+"-Gaz_row"+to_string(Gaz_row)+"-id_Gaz"+to_string(id_Gaz);
	    }
	  }
	}
      }
   
      //      cout<<calculated_trials<<"/"<<( total_trials/1000000 )<<"M"<<endl;
      cout<<"start calculating for "<< calculated_trials<<" cases out of "<<total_trials<<" trials."<<endl;
      cout<<"time used: "<<timer.toc()<< " sec"<<endl;
      timer.tic();

      if ( sub_mode_A != 2)
	cout<<"wrong input or no input for sub_mode_A. It should be 2"<<endl;

      sub_mode_A=2;//enumerate all cases. must be 2 at this point
      //	      sub_mode_B=2;//reverse identical code A and B
      switch ( sub_mode_B ){
      case 2:

	{
	  int count=0;
#pragma omp parallel for schedule(guided) num_threads(num_cores)
	  for (int iAB = 0; iAB <calculated_trials; iAB++){
#pragma omp critical
	    { 
	      count ++;
	      if ( count % 10 == 0){		
		cout<<count*100/calculated_trials<<"% finished. total: "<<calculated_trials
		    <<", remaining time:"<<timer.toc()/count*(calculated_trials-count)/60<<" min"
		    <<endl;
	      }
	    }
	    int iA = iAB ;
	    //	    int iB = iAB ;
	    //	    if ( iA > iB ) continue; //remove duplicate	

	    CSSCode codeA(id_list[iA][0],id_list[iA][1],id_list[iA][2],id_list[iA][3],id_list[iA][4]);
	    //	    CSSCode codeB(id_list[iB][0],id_list[iB][1],id_list[iB][2],id_list[iB][3],id_list[iB][4]);
	    CSSCode codeB; //codeB will be the same as codeA, or reverse symmetric codeA
	    SubsystemProductCSSCode codeC(codeA,codeB);
	    //string title_str_trial=title_str+"-test";
	    string title_str_trial=title_str+"-na"+to_string(codeA.n)+"-Gax_row"+to_string(codeA.Gx_row)+"-id_Gax"+to_string(codeA.id_Gx)
	      +"-Gaz_row"+to_string(codeA.Gz_row)+"-id_Gaz"+to_string(codeA.id_Gz);
	    //	if (simulate(title_str_trial, note, mode, sub_mode_A, sub_mode_B, 0, 0, 0, 0, debug, na, Gax_row, id_Gax, Gaz_row, id_Gaz, codeC)==2){
	    //	    simulate(title_str_trial, note, mode, sub_mode_A, sub_mode_B, 0, 0, 0, 0, debug, 0,0,0,0,0, codeC);
	    //	    simulate(title_str_trial, note, mode, sub_mode_A, 3, 0, 0, 0, 0, debug, 0,0,0,0,0, codeC);

	    simulate(title_str_trial, note, mode, sub_mode_A, sub_mode_B, 0, 0, 0, 0, debug, codeC);
	    simulate(title_str_trial, note, mode, sub_mode_A, 3, 0, 0, 0, 0, debug,  codeC);
	  }  
	}
	break;// sub_mode_B = 2; // include 3
      case 4:
	{
	  int count=0;
	  const int total = calculated_trials*calculated_trials;
	  const int chunk_size_max = 10000;                         //chunk to display progress
	  const int chunk_size = ( total/100 < chunk_size_max ) ? total/100:chunk_size_max ;
	  //	  count = total/10*7; // start from some point, 50%, 70%, ...
	  //	  cout<<"start from count = "<<count<<endl;
	  //guided for better speed
#pragma omp parallel for schedule(guided) num_threads(num_cores)
	  //
	  //#pragma omp parallel for schedule(dynamical,1) num_threads(num_cores)
	  for (int iAB = count; iAB <total; iAB++){
#pragma omp critical
	    {
	      //display progress information
	      count++;
	      if (count % chunk_size == 0){
		cout<<count<<", "<<(int) (count*1.0/total*100)<<"% finished. total: "<<total
		    <<", remaining time:"<<timer.toc()/count*(total-count)/60<<" min"
		    <<endl;
	      }
	    }
	    int iA = iAB / calculated_trials;
	    int iB = iAB % calculated_trials;
	    if ( iA < iB ) continue; //remove duplicate	
	    //	    cout<<"iA="<<iA<<", iB = "<<iB<<endl;
	    CSSCode codeA(id_list[iA][0],id_list[iA][1],id_list[iA][2],id_list[iA][3],id_list[iA][4]);
	    CSSCode codeB(id_list[iB][0],id_list[iB][1],id_list[iB][2],id_list[iB][3],id_list[iB][4]);
	    SubsystemProductCSSCode codeC(codeA,codeB);
	    //	    string title_str_trial=title_str+"-test";
	    string title_str_trial=title_str
	      +"-A-"+to_string(codeA.n)+"-"+to_string(codeA.Gx_row)+"-"+to_string(codeA.id_Gx)
	      +"-"+to_string(codeA.Gz_row)+"-"+to_string(codeA.id_Gz)
	      +"-B-"+to_string(codeB.n)+"-"+to_string(codeB.Gx_row)+"-"+to_string(codeB.id_Gx)
	      +"-"+to_string(codeB.Gz_row)+"-"+to_string(codeB.id_Gz);

	    //	string title_str_trial=title_str+"-na"+to_string(na)+"-Gax_row"+to_string(Gax_row)+"-id_Gax"+to_string(id_Gax)
	    // +"-Gaz_row"+to_string(Gaz_row)+"-id_Gaz"+to_string(id_Gaz);
	    //	if (simulate(title_str_trial, note, mode, sub_mode_A, sub_mode_B, 0, 0, 0, 0, debug, na, Gax_row, id_Gax, Gaz_row, id_Gaz, codeC)==2){
	    //	    simulate(title_str_trial, note, mode, sub_mode_A, sub_mode_B, 0, 0, 0, 0, debug, 0,0,0,0,0, codeC)
	    simulate(title_str_trial, note, mode, sub_mode_A, sub_mode_B, 0, 0, 0, 0, debug, codeC);
	    
	  }
	}  
	break;//sub_mode_B=4
      default:
	cout<<"main(): illegal sub_mode_B value"<<sub_mode_B<<endl;
	throw 2;    
      }//switch (sub_mode_B)
}// mode=3

    break;
  default:
    cout<<"main(): illegal mode value"<<endl;
    throw 2;
  }
  return 0;
}

int simulate(string title_str, string note, int mode, int sub_mode_A, int sub_mode_B,     //general info
	     int n_low, int n_high, int k_low, int k_high, int debug,                     //for random simulation
	     //int na_input, int Gax_row_input, int id_Gax, int Gaz_row_input, int id_Gaz,   //for enumarating all cases
	     SubsystemProductCSSCode code
	     ){
  //return 2 when the code is duplicate, or either dax = 1 or daz = 1
  if ( mode == 3) mode =1; //mode 3 do some preprocessing in main()

  Real_Timer timer;  timer.tic();
  GF2mat Gax,Gaz,Cax,Caz;
  GF2mat Gbx,Gbz,Cbx,Cbz;
  int na,ka, Gax_row,Gaz_row;//k is not necessary number of qubits
  int nb,kb, Gbx_row,Gbz_row;
  int id_Gbx, id_Gbz;
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
      case 1: //generate a random CSS code with distance > 1
	na=randi(n_low,n_high); ka = randi(k_low,k_high);Gax_row=randi(1,na-ka-1); Gaz_row=na-ka-Gax_row;
	getGoodQuantumCode(na,Gax_row,Gaz_row,Gax,Gaz,Cax,Caz,debug);
      //getRandomQuantumCode(na,Gax_row,Gaz_row,Gax,Gaz,Cax,Caz);
	break;
      case 2://enumerate all codes with size na
	//input: na, Gax_row, ...
	{
	  //	  na=na_input; Gax_row=Gax_row_input; Gaz_row=Gaz_row_input;


	  //	  cout<<"debug"<<endl;
	  if ( code.codeA.is_defined == 0 )
	      cout<<"code A is not defined,"<<endl;

	  na = code.codeA.n;
	  Gax_row = code.codeA.Gx_row;
	  Gaz_row = code.codeA.Gz_row;
	  
	  //	  cout<<"code.codeA.n = "<<code.codeA.n<<endl;
	  //debug=1;
	  if ( code.codeA.generate_by_id(debug) == 2){
	    if (debug) cout<<"duplicated case, return 2"<<endl;
	    return 2;
	  }
	  Gax = code.codeA.Gx;
	  Gaz = code.codeA.Gz;
	  //	  cout<<Gax<<endl;

	  /*	  if ( generate_code(Gax, Gaz, na, Gax_row, id_Gax, Gaz_row, id_Gaz, debug) ==2 ){
	    if (debug) cout<<"duplicated case, return 2"<<endl;
	    return 2;
	    }*/	
	  Cax=getC(Gax,Gaz);
	  Caz=getC(Gax,Gaz,1);
	}
	break;
      default:
	cout<<"simulate(): illegal sub_mode_A value"<<endl;
	throw 2;
	break;
      }

      switch ( sub_mode_B ){
        case 1: //two random codes
      
          //random code B
          nb=randi(n_low,n_high); kb = randi(k_low,k_high);Gbx_row=randi(1,nb-kb-1); Gbz_row=nb-kb-Gbx_row;
          getGoodQuantumCode(nb,Gbx_row,Gbz_row,Gbx,Gbz,Cbx,Cbz,debug);
          break;
        case 2: // use identical reverse code A for code B  
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
        case 3: //use identical code A for code B
          kb=ka;        
          nb=na;
          Gbx_row=Gax_row;
          Gbz_row=Gaz_row;
          Gbx=Gax;
          Gbz=Gaz;
          Cbx=Cax;
          Cbz=Caz;
          break;
      case 4: //enumerate all codes with size nb=na
	//input: na, Gax_row, ...
	{
	  //	  cout<<"running sub_mode_B =4"<<endl;
	  if ( code.codeB.generate_by_id(debug) == 2){
	    if (debug) cout<<"duplicated case, return 2"<<endl;
	    return 2;
	  }
	  //	  na=na_input; Gax_row=Gax_row_input; Gaz_row=Gaz_row_input;
	  nb=code.codeB.n; Gbx_row=code.codeB.Gx_row; Gbz_row=code.codeB.Gz_row;
	  id_Gbx=code.codeB.id_Gx; id_Gbz=code.codeB.id_Gz;
	  /*
	  if ( generate_code(Gbx, Gbz, nb, Gbx_row, id_Gbx, Gbz_row, id_Gbz, debug) ==2 ){
	    if (debug) cout<<"duplicated case, return 2"<<endl;
	    //	    cout<<"*";
	    return 2;
	    }*/
	  Gbx=code.codeB.Gx;
	  Gbz=code.codeB.Gz;

	  Cbx=getC(Gbx,Gbz);
	  Cbz=getC(Gbx,Gbz,1);
	}
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

      //manually change code B for a quick check, because submode_B=2 is not saved in file, onl submode_B=3
      if (false){
  	  kb=ka;        
	  nb=na;
          Gbx_row=Gaz_row;
          Gbz_row=Gax_row;
          Gbx=Gaz;
          Gbz=Gax;
          Cbx=Caz;
          Cbz=Cax;
      }


      if (debug)  {cout<<"Gax "<<Gax<<endl;  cout<<"Gaz "<<Gaz<<endl;  cout<<"Cax "<<Cax<<endl;  cout<<"Caz "<<Caz<<endl;}
      if (debug) { cout<<"Gbx "<<Gbx<<endl;  cout<<"Gbz "<<Gbz<<endl;  cout<<"Cbx "<<Cbx<<endl;  cout<<"Cbz "<<Cbz<<endl;}
      //      cout<<"debug"<<endl;
      break;
  }


  
  
  //  cout<<"check code A"<<endl;
  if (! is_quantum_code(Gax,Gaz,Cax,Caz)) throw "invalid code";

  int dax = quantum_dist_v2(Gax,Gaz);
  int daz = quantum_dist_v2(Gax,Gaz,1);

  //there is no valid reason to discard this case. For test purpose, this make the program much faster and should not miss a real case
  /*  if ( dax ==2 && daz == 2){
    //    cout<<"=";
    return 2;
    }*/

  //  cout<<"sub_mode_B="<<sub_mode_B<<",na="<<na<<",Gax_row="<<Gax_row<<",id_Gax="<<id_Gax<<",Gaz_row="<<Gaz_row<<", id_Gaz="<<id_Gaz;
  //  cout<<",dax="<<dax<<",daz="<<daz;
  //  cout<<endl;

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
  //  flag_chain_complex=1; //force estimate the chain complex code
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

    //    if ( mode == 1 && sub_mode_A == 2 && sub_mode_B == 4){ // this cannot be used because it was removed when generating the code by saying Gx_row <= Gz_row
      //pass
      //no need to calculate it because all codes will be enumerated, hence include a symmetry between X and Z

    if  (product(Gax,Gaz,Gbx,Gbz,dax,daz,dbx,dbz,debug,4) == 2)  
      std::cout<<title<<","<<note<<", time:"<<timer.toc()<<std::endl;


    //check rank
    /*
    if (Gax.row_rank() < Gax.rows())
      cout<<common::red_text("Gax not full rank")<<endl;
    if (Gaz.row_rank() < Gaz.rows())
      cout<<common::red_text("Gaz not full rank")<<endl;
    if (Gbx.row_rank() < Gbx.rows())
      cout<<common::red_text("Gbx not full rank")<<endl;
    if (Gbz.row_rank() < Gbz.rows())
      cout<<common::red_text("Gbz not full rank")<<endl;
    */

  }

  if ( debug )  timer.toc_print();
  return 0;
}
