//Weilei Zeng, Nov 26 2018
/*
This code produce two random quantum codes and construct concatenated codes and reduced code. Then check their distance.
 */
#include "weilei_lib/my_lib.h"
#include <itpp/itbase.h>
using namespace itpp;
using namespace std;


int test(GF2mat &G){//test how to change a matrix in a function
  G.set(1,2,1);
  cout<<dec2bin(7,10)<<endl;
  cout<<dec2bin(7,8)<<endl;
  return 0;
}

int main(int args, char ** argv){
  itpp::Parser parser;
  parser.init(args,argv);
  parser.set_silentmode(true);
  int mode = 1; parser.get(mode,"mode");
  std::string title_str;
  parser.get(title_str,"title");
  const char * title = title_str.c_str();
  int debug = 1; //default debug on
  parser.get(debug,"debug");
  
  RNG_randomize();  Real_Timer timer;  timer.tic();

  GF2mat Gax,Gaz,Cax,Caz;
  GF2mat Gbx,Gbz,Cbx,Cbz;
  int na,ka, Gax_row,Gaz_row;//k is not necessary number of qubits
  int nb,kb, Gbx_row,Gbz_row;

  //int mode=atof(argv[1]);  
  //char * title = argv[2];
  char filename_Gax[256];char filename_Gaz[256];char filename_Gbx[256];char filename_Gbz[256];
  sprintf(filename_Gax,"%sGax.mm",title);  sprintf(filename_Gaz,"%sGaz.mm",title);      sprintf(filename_Gbx,"%sGbx.mm",title);  sprintf(filename_Gbz,"%sGbz.mm",title);

  //  cout<<mode<<endl<<title<<endl;
  switch( mode ){
    case 1://generate random codes and save
      na=randi(7,17); ka = randi(1,1);Gax_row=randi(1,na-ka-1); Gaz_row=na-ka-Gax_row;
      getGoodQuantumCode(na,Gax_row,Gaz_row,Gax,Gaz,Cax,Caz,debug);
      //getRandomQuantumCode(na,Gax_row,Gaz_row,Gax,Gaz,Cax,Caz);
      //      nb=randi(7,7); kb = randi(1,1);Gbx_row=randi(1,nb-kb-1); Gbz_row=nb-kb-Gbx_row;
      // the parameter might change after removing singleton
      na=Gax.cols();
      Gax_row=Gax.rows();
      Gaz_row=Gaz.rows();
      //kb=ka;      
      nb=na;
      Gbx_row=Gaz_row;
      Gbz_row=Gax_row;
      Gbx=Gaz;
      Gbz=Gax;
      Cbx=Caz;
      Cbz=Cax;   
	//getRandomQuantumCode(nb,Gbx_row,Gbz_row,Gbx,Gbz,Cbx,Cbz);
      GF2mat_to_MM(Gax,filename_Gax);      GF2mat_to_MM(Gaz,filename_Gaz);
      GF2mat_to_MM(Gbx,filename_Gbx);      GF2mat_to_MM(Gbz,filename_Gbz);
      break;
    case 2:      //from given input. This is to manually double check some result
      cout<<"check "<<title<<endl;
      Gax=MM_to_GF2mat(filename_Gax); Gaz=MM_to_GF2mat(filename_Gaz);
      Gbx=MM_to_GF2mat(filename_Gbx); Gbz=MM_to_GF2mat(filename_Gbz);
      na=Gax.cols();
      nb=Gbx.cols();

      Cax=getC(Gax,Gaz);
      Caz=getC(Gax,Gaz,1);
      Cbx=getC(Gbx,Gbz);
      Cbz=getC(Gbx,Gbz,1);
      break;
  }

  //  cout<<"check code A"<<endl;
  if (! is_quantum_code(Gax,Gaz,Cax,Caz)) throw "invalid code";
  //  cout<<"Gax "<<Gax<<endl;  cout<<"Gaz "<<Gaz<<endl;  cout<<"Cax "<<Cax<<endl;  cout<<"Caz "<<Caz<<endl;
  int dax = quantum_dist_v2(Gax,Gaz);
  int daz = quantum_dist_v2(Gax,Gaz,1);
  //  cout<<"[Code A] na = "<<na<<", ";
  //  cout<<"dax = "<<dax<<", daz = "<<daz<<endl;
  //  cout<<"check code B"<<endl;
  if ( ! is_quantum_code(Gbx,Gbz,Cbx,Cbz)) throw "invalid code";
  //  cout<<"Gbx "<<Gbx<<endl;  cout<<"Gbz "<<Gbz<<endl;  cout<<"Cbx "<<Cbx<<endl;  cout<<"Cbz "<<Cbz<<endl;
  int dbx = quantum_dist_v2(Gbx,Gbz);
  int dbz = quantum_dist_v2(Gbx,Gbz,1);//1 to flip X and Z
  //  cout<<"[Code B] nb = "<<nb<<", ";
  //  cout<<"dbx = "<<dbx<<", dbz = "<<dbz<<endl;
  //  reduce(Gax,Gaz,Gbx,Gbz,dax,daz,dbx,dbz);
  //concatenate(Gax,Gaz,Gbx,Gbz,dax,daz,dbx,dbz);
  //1 for reduce
  if (product(Gax,Gaz,Gbx,Gbz,dax,daz,dbx,dbz,debug,1) == 2)
    std::cout<<title<<std::endl;
  //2 for concatenate
  if ( product(Gax,Gaz,Gbx,Gbz,dax,daz,dbx,dbz,debug,2) == 2)
    std::cout<<title<<std::endl;
  if ( debug )  timer.toc_print();
  return 0;
}
