//Weilei Zeng, April 28
//return min weight of rows in C, which is the distance of the code
//random window method is applied with default number of permutation 10.

//#include "weilei_lib/my_lib.h"
#include "dist.h"
#include <itpp/itbase.h>
#include <itpp/itcomm.h>
#include <stdio.h>
#include "my_lib.h"
using namespace itpp;
using namespace std;


int min_wt_decoding(GF2mat C){
  //  cout<<C<<endl;
  //when C is not so large, we can apply the true min weight decoding
  //  int rowC=C.rows();
  //  cout<<"rowC = "<<rowC<<endl;
  bvec alpha(C.rows());//a binary vector for linear combination of codeword
  GF2mat alphaM(1,C.rows());
  int max = pow(2,C.rows());
  bvec codeword(C.cols());
  int wt, min_wt=C.cols();
  bvec zero = zeros_b(C.cols());
  //  cout<<"min_wt_decodnig"<<endl;
  //  cout<<"max = "<<max<<endl<<C.rows()<<endl;
  for ( int i =1;i<max;i++){
    //    dec2bin(i,alpha);
    alpha = dec2bin(C.rows(),i);
    alphaM.set_row(0,alpha);
    //cout<<i<<endl;
    //    cout<<"alpha = "<<alpha<<endl;
    codeword=(alphaM*C).get_row(0);
    wt = BERC::count_errors(zero,codeword);
    min_wt = (wt<min_wt)? wt : min_wt;
  }
  return min_wt;    
}

int save_dist(int d,char * filename){
  mat mat_d(1,1);
  mat_d.set(0,0,d);
  mat_to_MM(mat_d,filename);
  return 0;
}



int rand_dist(GF2mat C, int perm_try){//default perm_try=10
  //  return min wt of rows in C
  if (C.rows()<7){ //for small codes, use true min wt decoding
    return min_wt_decoding(C);
  }
  //use random window decoder to find min wt of C
  RNG_randomize();
  bvec row_vec,zero=zeros_b(C.cols());
  int wt,min_wt=C.cols();
  ivec perm;
  GF2mat T,U;
  ivec P;
  for (int j=0;j<perm_try;j++){
    perm = sort_index( randu(C.cols()) );
    C.permute_cols(perm,false);
    //no need to permute back; can also permute the rows
    //permute rows also
    perm = sort_index( randu(C.rows()) );
    C.permute_rows(perm,false);
    C.T_fact(T,U,P);
    for (int i = 0;i<C.rows();i++){
      row_vec = C.get_row(i);
      wt = BERC::count_errors(zero,row_vec);
      if (wt < min_wt){
	min_wt = wt;
	//	cout<<row_vec<<endl;
      }
    }
  }
  return min_wt;
}
int classical_dist(GF2mat G){
  //return distance of a classical code GH^t=0
  //G is parity check matrix
  GF2mat T,U;
  ivec P;
  int rank_of_G = G.transpose().T_fact(T,U,P);
  if ( rank_of_G == G.cols()  ){
    return 999;//999 for infinity
  }
  GF2mat H = T.get_submatrix(rank_of_G,0,G.cols()-1,G.cols()-1);
  if (H.rows()<7){//use true min wt decoding for small codes.
    return min_wt_decoding(H);
  }
  int min_wt = rand_dist(H);//default permutation = 10
  return min_wt;
}

GF2mat getC(GF2mat G_x,GF2mat G_z,int flip){
  //return C_x
  //flip=1 to get C_z
  if (flip==1){
    GF2mat temp=G_x;
    G_x=G_z;
    G_z=temp;
  }
  GF2mat T,U;
  ivec P;
  int rank_of_G_z =   G_z.transpose().T_fact(T,U,P);
  GF2mat Q=T.get_submatrix(rank_of_G_z,0,G_z.cols()-1,G_z.cols()-1);//Q include G_x and C_x/L_x
  GF2mat GQ=G_x.concatenate_vertical(Q);

  GQ.T_fact(T,U,P);
  int rank_of_G_x = G_x.row_rank();
  int rank_of_Q = Q.rows();
  if (rank_of_G_x == rank_of_Q){
    cout<<"getC(): It is not a quantum code:zero rank for codeword space"<<endl;
    //    return;
  }
  GF2mat C = U.get_submatrix(rank_of_G_x,0,rank_of_Q-1,G_x.cols()-1 );
  C.permute_cols(P,true);//codewords/logical group 
  //check  if ((G_z*C.transpose()).is_zero() ){    cout<<"GOOD C"<<endl;  }
  return C;
}


int quantum_dist_v2(GF2mat G_x, GF2mat G_z, int flip){//without expected value
  //right or x  distance of (G_x,G_z)
  //flip left and right if flip = 1;
  int trialQ=100;//100 is good for not so big codes;permute GQ this max amount of time
  //  int trialQflag=1;//a falg to control the max amout of permutation
  
  if (flip==1){//flip G_x and G_z
    GF2mat temp=G_x;    G_x=G_z;    G_z=temp;
  }

  GF2mat T,U;  ivec P;
  int rank_of_G_z =   G_z.transpose().T_fact(T,U,P);
  GF2mat Q=T.get_submatrix(rank_of_G_z,0,G_z.cols()-1,G_z.cols()-1);//Q include G_x and C_x/L_x

  GF2mat GQ=G_x.concatenate_vertical(Q);
  int min_wt=GQ.cols(),wt;

  for ( int iq=0;iq<trialQ;iq++){
    ivec perm = sort_index(  randu( GQ.cols()  ));//random permutation
    GQ.permute_cols(perm,false);
    GQ.T_fact(T,U,P);
    int rank_of_G_x = G_x.row_rank();
    int rank_of_Q = Q.rows();

    if (rank_of_G_x == rank_of_Q){
      return INF;//999 for infinity
    }
    GF2mat C = U.get_submatrix(rank_of_G_x,0,rank_of_Q-1,G_x.cols()-1 );
    C.permute_cols(P,true);//codewords/logical group //not necessary to permute it back here
  
    wt = rand_dist(C);//defauylt permutation = 10
    trialQ=(wt<min_wt)? max(4*iq,trialQ):trialQ;//make sure this is the true min weight
    min_wt=(wt<min_wt)? wt:min_wt;

    //    cout<<"iq = "<<iq<<", [wt="<<wt<<"] "<<endl;;
    //  cout<<"got min wt of logical operator C  = "<<min_wt<<endl;
    //save_dist(min_wt,filename_dist);
  }
  return min_wt;
}
int quantum_dist(GF2mat G_x, GF2mat G_z, int dist_expected, int flip){
  //right or x  distance of (G_x,G_z)
  //flip left and right if flip = 1;
  int trialQ=50000;//1000;permute GQ this max amount of time
  int trialQflag=1;//a falg to control the max amout of permutation
  
  if (flip==1){//flip G_x and G_z
    GF2mat temp=G_x;    G_x=G_z;    G_z=temp;
  }

  GF2mat T,U;  ivec P;
  int rank_of_G_z =   G_z.transpose().T_fact(T,U,P);
  GF2mat Q=T.get_submatrix(rank_of_G_z,0,G_z.cols()-1,G_z.cols()-1);//Q include G_x and C_x/L_x

  //  cout<<Q.row_rank()<<","<<G_z.row_rank()<<","<<G_z.cols()<<endl;

  GF2mat GQ=G_x.concatenate_vertical(Q);
  int min_wt=GQ.cols(),wt;

  for ( int iq=0;iq<trialQ;iq++){
    ivec perm = sort_index(  randu( GQ.cols()  ));//random permutation
    GQ.permute_cols(perm,false);
    GQ.T_fact(T,U,P);
    int rank_of_G_x = G_x.row_rank();
    int rank_of_Q = Q.rows();

    if (rank_of_G_x == rank_of_Q){
      return INF;//999 for infinity
    }
    GF2mat C = U.get_submatrix(rank_of_G_x,0,rank_of_Q-1,G_x.cols()-1 );
    C.permute_cols(P,true);//codewords/logical group //not necessary to permute it back here
    //Question 1: does the row in C include some stabilizer generators which may increase its weight?
  
    wt = rand_dist(C);//defauylt permutation = 10
    min_wt=(wt<min_wt)? wt:min_wt;
    //cout<<"iq = "<<iq<<", [wt="<<wt<<"] "<<endl;;
    //  cout<<"got min wt of logical operator C  = "<<min_wt<<endl;
    //save_dist(min_wt,filename_dist);
    int max_trial = 0;//no need to run C again, always get the same result
    for (int i =0;i<max_trial;i++){
	  wt=rand_dist(C);
	  min_wt=(wt<min_wt)? wt:min_wt;
    }
    if (trialQflag) {//adjust the max number of permutation, only do this once
        if (min_wt <= dist_expected){
	  cout<<"reach min distance when iq = "<<iq<<endl;
	  trialQflag=0;
	  if (iq<3) {
	    //  cout<<"wt: "<<min_wt<<","<<dist_expected<<endl;
	    trialQ=4;
	  }	  else {trialQ = 4*iq;}
	}
    }
  }
  cout<<"--------trialQ = "<<trialQ<<"--------";
  return min_wt;
}

int hypergraph_dist(GF2mat Aj, GF2mat Ajplus,int dist_expected,int flip){
  //left distance of (Aj,Ajplus^T)
  //right distance of (G_x,G_z)
  //flip left and right if flip = 1
  
  GF2mat G_z = Aj;
  GF2mat G_x = Ajplus.transpose();
  //check commutation
  /*  if ( (Aj*Ajplus).is_zero()){
    //pass
  }else{
    cout<<"It is not a CSS code!"<<endl;
    }*/
  
  if (flip==1){
    GF2mat temp=G_x;
    G_x=G_z;
    G_z=temp;
  }
  
  /* char * filename_G_x = argv[1];
  char * filename_G_z = argv[2];
  char * filename_C_x = argv[3];
  char * filename_dist = argv[4];
  */
  //  GF2mat G_z = MM_to_GF2mat(filename_G_z);
  GF2mat T,U;
  ivec P;
  int rank_of_G_z =   G_z.transpose().T_fact(T,U,P);
  GF2mat Q=T.get_submatrix(rank_of_G_z,0,G_z.cols()-1,G_z.cols()-1);//Q include G_x and C_x/L_x

  //  GF2mat G_x=MM_to_GF2mat(filename_G_x);


  GF2mat GQ=G_x.concatenate_vertical(Q);
  int min_wt=GQ.cols(),wt;
  int trialQ=1000;//1000;
  for ( int iq=0;iq<trialQ;iq++){
    ivec perm = sort_index(  randu( GQ.cols()  ));
    GQ.permute_cols(perm,false);
    GQ.T_fact(T,U,P);
    int rank_of_G_x = G_x.row_rank();
    int rank_of_Q = Q.rows();

    if (rank_of_G_x == rank_of_Q){
      //      cout<<"It is not a quantum code!"<<endl;
      if (dist_expected != 999){
	cout<<endl<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!NOTICE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!NOTICE!!!!!!!!!!!!!"<<endl;
	cout<<"("<<dist_expected<<")";
      }
      return 999;//999 for infinity
    }
    GF2mat C = U.get_submatrix(rank_of_G_x,0,rank_of_Q-1,G_x.cols()-1 );
    C.permute_cols(P,true);//codewords/logical group //not necessary to permute it back here

    //Question 1: does the row in C include some stabilizer generators which may increase its weight?
  
    //GF2mat_to_MM(C,filename_C_x);
    wt = rand_dist(C);//defauylt permutation = 10
    min_wt=(wt<min_wt)? wt:min_wt;
    //  cout<<"got min wt of logical operator C  = "<<min_wt<<endl;
    //save_dist(min_wt,filename_dist);
    //    cout<<"<"<<wt<<">";      
    int max_trial = 0;//no need to run C again, always get the same result
    for (int i =0;i<max_trial;i++){
        if (min_wt == dist_expected){
	  //great job! return the result
	  break;
	}else if (min_wt < dist_expected){
	  //      cout<<"Damn! how could this happen!"<<endl;
	  //    cout<<"("<<dist_expected<<")";
	  break;
	}else{
	  //continue another run
	  //      cout<<"#RUN AGAIN#"<<min_wt;
	  wt=rand_dist(C);
	  min_wt=(wt<min_wt)? wt:min_wt;
	  //	  cout<<"<"<<wt<<">";
	}
    }

    if (min_wt == dist_expected){
      break;
    }
  }

  //final check
  if (min_wt != dist_expected){
    cout<<endl<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!NOTICE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!NOTICE!!!!!!!!!!!!!"<<endl;
    cout<<"("<<dist_expected<<")";
    
  }


  return min_wt;
}