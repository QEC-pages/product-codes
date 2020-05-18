//Weilei Zeng, Nov 26 2018
/*
This code produce two random quantum codes and construct concatenated codes and reduced code. Then check their distance.
 */
#include "weilei_lib/my_lib.h"
#include <itpp/itbase.h>
using namespace itpp;
using namespace std;
/*
bool is_quantum_code(GF2mat &Gx,GF2mat &Gz, GF2mat &Cx,GF2mat &Cz){
  if (!(Gx*Gz.transpose()).is_zero()){
    cout<<"(Gx*Gz.transpose()) is not zero"<<endl;return false;
  }
  if (!(Gx*Cz.transpose()).is_zero()){
    cout<<"(Gx*Cz.transpose()) is not zero"<<endl;return false;
  }
  if (!(Gz*Cx.transpose()).is_zero()){
    cout<<"(Gz*Cx.transpose()) is not zero"<<endl;return false;
  }
  int rank_of_Gx=Gx.row_rank();
  int rank_of_Gz=Gz.row_rank();
  int rank_of_Cx=Cx.row_rank();
  int rank_of_Cz=Cz.row_rank();
  int n=Gx.cols();
  if (rank_of_Gx+rank_of_Gz+rank_of_Cx != n){
    cout<<"(rank_of_Gx+rank_of_Gz+rank_of_Cx != n)"<<endl;
    return false;
  }
  if(rank_of_Cx != rank_of_Cz){
    cout<<"(rank_of_Cx != rank_of_Cz)"<<endl;return false;
  }
  //  cout<<"is_quantum_code(): It is a quantum code!"<<endl;
  return true;
}

/ *int getRandomQuantumCode(GF2mat &Gx,GF2mat &Gz, GF2mat &Cx,GF2mat &Cz){
  int n=21;//sample input
  int Gx_row=8;
  int Gz_row=8;
  getRandomQuantumCode(n,Gx_row,Gz_row,Gx,Gz,Cx,Cz);
  return 0;
  }* /

int getRandomQuantumCode(int n,int Gx_row,int Gz_row, GF2mat &Gx,GF2mat &Gz, GF2mat &Cx,GF2mat &Cz){

  Gx = GF2mat(Gx_row,n);
  Gz = GF2mat(Gz_row,n);
  for ( int i =0;i<Gx_row;i++){//random G_x
    Gx.set_row(i,randb(n));//equally 0 and 1s
  }
  GF2mat T,U; ivec P;
  int rank_of_Gx = Gx.transpose().T_fact(T,U,P);
  //  GF2matPrint(T,"T");
  GF2mat Q=T.get_submatrix(rank_of_Gx,0,n-1,n-1);
  //  Q.permute_cols(P,true); no need for T, only need for U which is not used here
  //  GF2matPrint(Q,"Q");
  GF2mat alpha(Gz_row,Q.rows()); //a random binary matrix to select G_z
  for ( int i=0;i<Gz_row;i++){
    alpha.set_row(i,randb(Q.rows()));
  }
  Gz=alpha*Q;
  //  Gz=Q.get_submatrix(0,0,Gz_row-1,n-1);
  //Cz=Q.get_submatrix(Gz_row,0,Q.rows()-1,n-1);
  //  GF2matPrint(Gz,"Gz");
  //  GF2matPrint(Cz,"Cz");
  Cx=getC(Gx,Gz);
  Cz=getC(Gx,Gz,1);
  return 0;
}
*/

int test(GF2mat &G){//test how to change a matrix in a function
  G.set(1,2,1);
  cout<<dec2bin(7,10)<<endl;
  cout<<dec2bin(7,8)<<endl;
  return 0;
}

int main(int args, char ** argv){
  RNG_randomize();  Real_Timer timer;  timer.tic();

  GF2mat Gax,Gaz,Cax,Caz;
  GF2mat Gbx,Gbz,Cbx,Cbz;
  int na,ka, Gax_row,Gaz_row;//k is not necessary number of qubits
  int nb,kb, Gbx_row,Gbz_row;

  int mode=atof(argv[1]);
  char * title = argv[2];
  char filename_Gax[256];char filename_Gaz[256];char filename_Gbx[256];char filename_Gbz[256];
  sprintf(filename_Gax,"%sGax.mm",title);  sprintf(filename_Gaz,"%sGaz.mm",title);      sprintf(filename_Gbx,"%sGbx.mm",title);  sprintf(filename_Gbz,"%sGbz.mm",title);

  //  cout<<mode<<endl<<title<<endl;
  switch( mode ){
    case 1://generate random codes and save
      na=randi(7,10); ka = randi(1,2);Gax_row=randi(1,na-ka-1); Gaz_row=na-ka-Gax_row;
      getRandomQuantumCode(na,Gax_row,Gaz_row,Gax,Gaz,Cax,Caz);
      nb=randi(7,10); kb = randi(1,2);Gbx_row=randi(1,nb-kb-1); Gbz_row=nb-kb-Gbx_row;
      getRandomQuantumCode(nb,Gbx_row,Gbz_row,Gbx,Gbz,Cbx,Cbz);
      GF2mat_to_MM(Gax,filename_Gax);      GF2mat_to_MM(Gaz,filename_Gaz);
      GF2mat_to_MM(Gbx,filename_Gbx);      GF2mat_to_MM(Gbz,filename_Gbz);
      break;
    case 2:      //from given input. This is to manually double check some result
      cout<<"check "<<title<<endl;
      Gax=MM_to_GF2mat(filename_Gax); Gaz=MM_to_GF2mat(filename_Gaz);
      Gbx=MM_to_GF2mat(filename_Gbx); Gbz=MM_to_GF2mat(filename_Gbz);
      na=Gax.cols();
      nb=Gbx.cols();

      cout<<"Gax "<<Gax<<endl;  cout<<"Gaz "<<Gaz<<endl;
      cout<<"Gbx "<<Gbx<<endl;  cout<<"Gbz "<<Gbz<<endl;
      //cout<<"Cax "<<Cax<<endl;  cout<<"Caz "<<Caz<<endl;
      break;
  }
  
  //  int nb=21; int Gbx_row=8;  int Gbz_row=8;

  //  is_quantum_code(Gax,Gaz,Cax,Caz);
  //  cout<<"Gax "<<Gax<<endl;  cout<<"Gaz "<<Gaz<<endl;  cout<<"Cax "<<Cax<<endl;  cout<<"Caz "<<Caz<<endl;
  int dax = quantum_dist_v2(Gax,Gaz);
  int daz = quantum_dist_v2(Gax,Gaz,1);
  cout<<"[Code A] na = "<<na<<", ";
  cout<<"dax = "<<dax<<", daz = "<<daz<<endl;

  //  is_quantum_code(Gax,Gaz,Cax,Caz);
  //  cout<<"Gbx "<<Gbx<<endl;  cout<<"Gbz "<<Gbz<<endl;  cout<<"Cbx "<<Cbx<<endl;  cout<<"Cbz "<<Cbz<<endl;
  int dbx = quantum_dist_v2(Gbx,Gbz);
  int dbz = quantum_dist_v2(Gbx,Gbz,1);
  cout<<"[Code B] nb = "<<nb<<", ";
  cout<<"dbx = "<<dbx<<", dbz = "<<dbz<<endl;
  reduce(Gax,Gaz,Gbx,Gbz,dax,daz,dbx,dbz);
  concatenate(Gax,Gaz,Gbx,Gbz,dax,daz,dbx,dbz);

  cout<<"finish "<<title<<endl;
  timer.toc_print();
  return 0;
}
