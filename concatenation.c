//Nov 21, 2018 Weilei Zeng,  copied from hypergraph.c
//construct quantum concatenated code from two quantum codes. Check their parameters
//there are several ways for concatenation, see hypergraph_product_code.pdf

//July 22, 2018 Weilei Zeng
//An integrated program to check all parameter in Table 1 in Leonid's construction of Hypergraph product codes.


//Weilei Zeng, July 19, 2018
//According to Leonid's note on construction of high dimension hypergraph product code, using small random Matrix P_j, j={1,2,3,4} to generate code in 2D, 3D, and 4D. Then get parameters of [n,k,d] for those codes.
//This program is modified from the hypercubic code project (dated Jul 19, 2018)

//if G_x.row_rank()+G_z.row_rank()=full rank, then it is not a quantum code.
//construct G_x and G_z for toric code, cubic code, and hypercubic code. Following the structure and notation in model.pdf
#include "weilei_lib/my_lib.h"
#include <itpp/itbase.h>
using namespace itpp;
using namespace std;

/*//moved to lib.h
bool is_quantum_code(GF2mat G_x, GF2mat G_z){
  //check if the code is quantum or not
  //not used here. originally designed for generator like [7,3,4] code at random size, which has been confirmed to be valid only for size L = 7n
  if (G_x.row_rank()+G_z.row_rank() == G_x.cols()){
    return false;
  }else{
    return true;
  }
  }*/

/*//moved to lib.h
GF2mat make_it_full_rank(GF2mat fat){
  //Can not use the functoin here. I can not remove the degeneracy in the check matrix, which is actually the syndrome code. By removing it, I am removing the syndrome code.
  //reduce a fat matrix with degenerate rows to a thin matrix with full rank; remove the dependent rows
  GF2mat thin, T,U;
  ivec P;
  int rank = fat.T_fact(T,U,P);
  thin = U.get_submatrix(0,0,rank-1,U.cols()-1);
  thin.permute_cols(P,true);
  return thin;
  }*/

GF2mat get_check_code734(int L){//L=7n
  //return check matrix code code [7,3,4], find definition in research note.pdf
  GF2mat G(L,L);
  for ( int i=0;i<L;i++){
    G.set(i,i,1);
    if (i+2>L-1) {
      G.set(i,i+2-L,1);
    } else {
      G.set(i,i+2,1);
    }
    if (i+3>L-1) {
      G.set(i,i+3-L,1);
    }else{
      G.set(i,i+3,1);
    }
  }
  return G;
}
GF2mat get_check_code743(int L){//L=7n
  //return check matrix code code [7,4,3], find definition in research note.pdf
  GF2mat G(L,L);
  for ( int i=0;i<L;i++){
    G.set(i,i,1);
    if (i+2>L-1) {
      G.set(i,i+2-L,1);
    } else {
      G.set(i,i+2,1);
    }
    if (i+3>L-1) {
      G.set(i,i+3-L,1);
    }else{
      G.set(i,i+3,1);
    }
    if (i+4>L-1) {
      G.set(i,i+4-L,1);
    }else{
      G.set(i,i+4,1);
    }    
  }
  return G;
}

GF2mat get_check_rept(int L){//return circulant check matrix for repetition code of length L
  GF2mat a(L,L);
  for (int i=0;i<L-1;i++){//construct a : circulant check matrix for repetition code
    a.set(i,i,1);
    a.set(i,i+1,1);
  }
  a.set(L-1,L-1,1);
  a.set(L-1,0,1);
  //cout<<"circulant check matrix for repetition code : a = "<<a<<endl;
  return a;
}
GF2mat get_check(int generator_flag, int L){
  //return check matric a for generating toric code, cubic code and hypercubic code.
  switch(generator_flag){
  case 1: return get_check_rept(L);break;
  case 2: return get_check_code734(L);break;
  case 3: return get_check_code743(L);break;
  }
  //  return get_check_734(L);//code [7,3,4]
  //  return get_check_743(L);//code [7,4,3]
  //  return get_check_rept(L); //circulant check matrix for repetition code.
}

GF2mat   extensionBj(GF2mat Aj,GF2mat Ajless, GF2mat P){
  //  GF2mat Aj=*Aj_p,Ajless=*Ajless_p,P=*P_p;
  //cout<<"extensionBj"<<endl;
  GF2mat Bj=kron(Aj,gf2dense_eye(P.rows()))
      .concatenate_horizontal(kron(gf2dense_eye(Aj.rows()),P))
      .concatenate_vertical(
			    GF2mat(Ajless.rows()*P.cols(),Aj.cols()*P.rows()).concatenate_horizontal(
      	    kron(Ajless,gf2dense_eye(P.cols()))
												     ));
  return Bj;
}

GF2mat * extension(GF2mat *A1_p,GF2mat P,int m){
  //from an (m-1) chain complex A = {A1,A2,A3,...A_(m-1)}, and an r x c binary matrix P, construct an extended m chain complex B={B1,B2,...,Bm}.
  GF2mat A[m-1]; //construct A from pointer A1_p
  for (int i=0;i<m-1;i++){
    A[i]=*(A1_p+i);
  }
  const int MAX_m=6;//maximum length of chain complex
  //  cout<<"extension: m = "<<m<<endl; 
  //GF2mat B[MAX_m];
  static GF2mat B[MAX_m];//have to be static to be used outside this function. It is better to use vector<> here
  GF2mat A1=A[0],B1;
  B1=kron(A1,gf2dense_eye(P.rows()))
    .concatenate_horizontal(kron(gf2dense_eye(A1.rows()),P));
  B[0]=B1;
  if (m==2){
    GF2mat B2=kron(gf2dense_eye( A1.cols() ),P)
      .concatenate_vertical(kron(A1,gf2dense_eye(P.cols())));
    B[1]=B2;
      return B;
  }
  //if m>2
  for (int j=2;j<m;j++){//run m-1 loop
    GF2mat Bj = extensionBj(A[j-1],A[j-2],P);
    B[j-1]=Bj;
  }
  GF2mat Amless = A[m-2];
  GF2mat Bm=kron(gf2dense_eye( Amless.cols() ),P)
      .concatenate_vertical(kron(Amless,gf2dense_eye(P.cols())));
  B[m-1] = Bm;
  return B;
}

int printNKD(int nkd[][MAX_M], int m){//print n k d in table format
  //5 is the maximum I will try, but just put 10 here since it is small
  for ( int i=1;i<m+1;i++){
        for ( int j=0;j<i+1;j++){
	  cout<<nkd[i][j]<<"\t";
	}
	cout<<endl;
  }
}
vector<int>  parameterP(GF2mat P[], int m){//m is the size of P
  //print parameters of matrix P, and derived parameter of Cm including
  //P: r,c,kappa,\tilde\kappa,\delta,\tilde\kappa,rank
  //C:n,k,d_left,d_right
  cout<<"#Expected value calculated from Table 1:"<<endl;
  int n[m+1][MAX_M];
  n[1][0]=P[0].rows();
  n[1][1]=P[0].cols();
  for ( int i=2;i<m+1;i++){
    n[i][0]=n[i-1][0]*P[i-1].rows();
    n[i][i]=n[i-1][i-1]*P[i-1].cols();
    for ( int j=1;j<i;j++){
      n[i][j]=n[i-1][j]*P[i-1].rows()+n[i-1][j-1]*P[i-1].cols();
    }
  }
  cout<<"n0\tn1\tn2\tn3\tn4"<<endl;
  printNKD(n,m);
    
  int k[m],kt[m],r;//\kappa, \tilde\kappa, rank
  cout<<"-----------------------------------"<<endl;
  for ( int i=0;i<m;i++){
    r=P[i].row_rank();
    //cout<<r<<"\t";
    k[i]=P[i].cols()-r;
    kt[i]=P[i].rows()-r;
  }
  int kC[m+1][MAX_M];//kC[m][j];k value of (Aj,Ajplus^T)
  kC[1][0]=kt[0];
  kC[1][1]=k[0];
  for ( int i=2;i<m+1;i++){
    kC[i][0]=kC[i-1][0]*kt[i-1];
    kC[i][i]=kC[i-1][i-1]*k[i-1]; 
    for ( int j=1;j<i;j++){
      kC[i][j] = kC[i-1][j-1]*k[i-1]+kC[i-1][j]*kt[i-1];
    }
  }
  cout<<"k0\tk1\tk2\tk3\tk4"<<endl;
  printNKD(kC,m);
  //distance
  int delta[m],delta_tilde[m];//distance of P and P^T
  int dl1,dl2; //temperary distance of P and P^T
  
  cout<<"-----------------------------------parameters of P:"<<endl;
  cout<<"row\tcol\trank\tkappa,delta,kappa_tilde,delta_tilde"<<endl;
  for ( int i=0;i<m;i++){
    dl1 =   classical_dist(P[i]);
    dl2 =   classical_dist(P[i].transpose());
    //    cout<<"dist(P["<<i<<"]) = "
    cout<<P[i].rows()<<"\t"<<P[i].cols()<<"\t"<<P[i].row_rank()<<"\t";
    cout<<k[i]<<"\t"<<dl1<<"\t"<<kt[i]<<"\t"<<dl2<<"\t"<<endl;
    delta[i]=dl1;
    delta_tilde[i]=dl2;
  }
  cout<<"-----------------------------------"<<endl;
  //  const int INF = 999;
  int dl[m+1][MAX_M];
  dl[1][1]=delta[0];
  dl[1][0]=  (kt[0]>0)? 1:INF;//delta_tilde[0];
  for ( int i=2;i<m+1;i++){
    dl[i][i]=dl[i-1][i-1]*delta[i-1];//not sure what to put here
    dl[i][0]=(kt[i-1]>0)? dl[i-1][0]:INF;//delta_tilde[i-1];//last element
    for ( int j=1;j<i;j++){//elements in between
      dl[i][j]= (kt[i-1]>0)? min(dl[i-1][j-1]*delta[i-1],dl[i-1][j]) : dl[i-1][j-1]*delta[i-1];
    }
  }
  
  //  cout<<"d0\td1\td2\td3\td4\td(left)"<<endl;
  for (int i=1;i<m+1;i++){
    for (int j=0;j<i+1;j++){
      if (dl[i][j]>999){//normalize infinity
	dl[i][j]=999;
      }
    }
  }
  //  printNKD(dl,m);
  vector<int> dist(m*(m+1)*2);
  int index=0;
  for (int i=1;i<m+1;i++){
    for (int j=0;j<i+1;j++){
      dist[index]=dl[i][j];
      index++;
    }
  }

  //right distance
  int dr[m+1][MAX_M];
  dr[1][0]=delta_tilde[0];
  dr[1][1]=  (k[0]>0)? 1:INF;//delta_tilde[0];
  for ( int i=2;i<m+1;i++){
    dr[i][0]=dr[i-1][0]*delta_tilde[i-1];//not sure what to put here
    dr[i][i]=(k[i-1]>0)? dr[i-1][i-1]:INF;//delta_tilde[i-1];//last element
    for ( int j=1;j<i;j++){//elements in between
      dr[i][j]= (k[i-1]>0)? min(dr[i-1][j]*delta_tilde[i-1],dr[i-1][j-1]) : dr[i-1][j]*delta_tilde[i-1];
    }
  }
  //  cout<<"d0\td1\td2\td3\td4\td(right)"<<endl;
  for (int i=1;i<m+1;i++){
    for (int j=0;j<i+1;j++){
      if (dr[i][j]>999){//normalize infinity for output
	dr[i][j]=999;
      }
    }
  }
  //  printNKD(dr,m);

  //add to vector dist
  index=m*(m+1);
  for (int i=1;i<m+1;i++){
    for (int j=0;j<i+1;j++){
      dist[index]=dr[i][j];
      index++;
    }
  }

  //print distance

  int d_min[m+1][MAX_M];
  for (int i=1;i<m+1;i++){
    for (int j=0;j<i+1;j++){
      d_min[i][j]=min(dl[i][j],dr[i][j]);
    }
  }
  cout<<"d0\td1\td2\td3\td4\td_min=min(d_left,d_right)"<<endl;
  printNKD(d_min,m);
  cout<<"d0\td1\td2\td3\td4\td_left"<<endl;
  printNKD(dl,m);
  cout<<"d0\td1\td2\td3\td4\td_right"<<endl;
  printNKD(dr,m);

  
  //  cout<<"\# Numerical results on [n,k,d] value of code Q (Aj,A(j+1)^T)"<<endl;
  return dist;
}

int parameterC(GF2mat *B1, int m, int m_max, vector<int> dist){//*B1=Cm[0]
  //input: pointer of first matrix, length of the chain, max chain length, expected distance
  //print rank and size of Cm

  int r[m];//rank
  cout<<"                                                     n= ";
  for (int i=0;i<m;i++){
    r[i]=(B1+i)->row_rank();
    cout<<(B1+i)->rows()<<"\t";
    //    cout<<"m="<<m<<",i="<<i<<", rank="<<r[i]<<", size("<<(B1+i)->rows()<<","<<(B1+i)->cols()<<")"<<endl;
  }
  cout<<(B1+m-1)->cols()<<endl;
  cout<<"                                                                                         k= ";
  for (int i=0;i<m-1;i++){
    int k = (B1+i)->cols() - r[i] -r[i+1];
    cout<<k<<"\t";
  }
  cout<<endl;
  //check distance

  
  /*  for (int i=0;i<30;i++){//14 for m_max=4
    cout<<"["<<""<<dist[i]<<"]";
    }*/
  //left distance
  cout<<"d_left=\t";
  for (int i=0;i<m-1;i++){
    int index = m*(m+1)/2+i;
    int d_left = hypergraph_dist(*(B1+i),*(B1+i+1),dist[index] );
      cout<<d_left<<"\t";
  }
  cout<<endl;

  cout<<"                          d_right=";
  //  int m_max=5;
  for (int i=0;i<m-1;i++){
    int index = m_max*(m_max+1)+m*(m+1)/2+i;
    int d_right = hypergraph_dist(*(B1+i),*(B1+i+1),dist[index] ,1);//1 for flipping to right distance
      cout<<d_right<<"\t";
  }
  cout<<endl;
  return 0;
}

int saveC(GF2mat *B1,int m, char * folder_Cm){//*B1=C[0]
  //save or print the result
  for (int i=0;i<m;i++){
    char filename[256];
    //    char folder[]="data/random/C";
    sprintf(filename,"%s/C%d_%d.mm",folder_Cm,m,i);
    GF2mat_to_MM(*(B1+i),filename);
    
    /* char matrixname[256];
    sprintf(matrixname,"m=%d C[%d]",m,i);
    GF2matPrint(*(B1+i),matrixname);*/
  }  
  return 0;
}

GF2mat generate_random_P(double p, int x,int y,char * filename){
  //probability p to get 1 for each element
  //return P with density p
  GF2mat P(x,y);
  for (int i=0;i<x;i++){
    for ( int j=0;j<y;j++){
      if (randu()<p) P.set(i,j,1);
    }
  }
  //  cout<<"Pj"<<P<<endl;
  //  GF2mat_to_MM(P,filename);
  return P;
}

//int check(GF2mat  Aj,GF2mat  Ajplus, GF2mat Bj,GF2mat Bjplus){
int check(){
  GF2mat Aj=MM_to_GF2mat("data/random/temp/C2_0.mm");
  GF2mat Ajplus=MM_to_GF2mat("data/random/temp/C2_1.mm");
  GF2mat Bj=MM_to_GF2mat("data/random/temp/C3_0.mm");
  GF2mat Bjplus=MM_to_GF2mat("data/random/temp/C3_1.mm");
  
  cout<<Aj.rows()<<"x"<<Aj.cols()<<endl;
  cout<<Ajplus.rows()<<"x"<<Ajplus.cols()<<endl;
  cout<<Bj.rows()<<"x"<<Bj.cols()<<endl;
  cout<<Bjplus.rows()<<"x"<<Bjplus.cols()<<endl;
  cout<<"check"<<endl;

  GF2mat T,U;
  ivec P;
  int rank_of_Aj =Aj.transpose().T_fact(T,U,P);
  GF2mat C = T.get_submatrix(rank_of_Aj,0,Aj.cols()-1,Aj.cols()-1);//rank_of_T = Aj.cols()-rank_of_Aj
  int dAj=2;
  bvec codeword=zeros_b(Aj.cols());
  bvec zero=zeros_b(Aj.cols());
  for ( int i=0;i<C.rows();i++){
    codeword = C.get_row(i);
    int wt = BERC::count_errors(zero,codeword);
    if (wt==dAj){
      cout<<"codeword : "<<codeword<<endl;
      break;
    }
  }
  cout<<Aj*codeword<<endl;
  bvec codewordB;
  int r=4;
  GF2mat y(1,r);
  GF2mat x(1,Aj.cols());
  x.set_row(0,codeword);
  y.set(0,1,1);
  GF2mat z=kron(x,y);
  z.set_size(1,Bj.cols(),true);
  //bvec e=z.get_row(0);
  if (  (Bj*z.transpose()).is_zero()  ){
    cout<<"Bj*z=0"<<endl;
  }
  GF2mat BjplusT=Bjplus.transpose();
  GF2mat  Bz=BjplusT.concatenate_vertical(z);
  cout<<Bjplus.row_rank()<<"<>"<<Bz.rows()<<"<>"<<Bz.row_rank()<<endl;
  return 0;
}

int check2(){//check if the codeword with smaller distance is inside Bjplus
  GF2mat Aj=MM_to_GF2mat("data/random/temp/C3_1.mm");
  GF2mat Ajplus=MM_to_GF2mat("data/random/temp/C3_2.mm");
  GF2mat Bj=MM_to_GF2mat("data/random/temp/C4_2.mm");
  GF2mat Bjplus=MM_to_GF2mat("data/random/temp/C4_3.mm");
  
  cout<<Aj.rows()<<"x"<<Aj.cols()<<endl;
  cout<<Ajplus.rows()<<"x"<<Ajplus.cols()<<endl;
  cout<<Bj.rows()<<"x"<<Bj.cols()<<endl;
  cout<<Bjplus.rows()<<"x"<<Bjplus.cols()<<endl;
  cout<<"check2"<<endl;

  GF2mat T,U;
  ivec P;
  int rank_of_Aj =Aj.transpose().T_fact(T,U,P);
  GF2mat C = T.get_submatrix(rank_of_Aj,0,Aj.cols()-1,Aj.cols()-1);//rank_of_T = Aj.cols()-rank_of_Aj
  int dAj=4;
  bvec codeword=zeros_b(Aj.cols());
  bvec zero=zeros_b(Aj.cols());
  for ( int i=0;i<C.rows();i++){
    codeword = C.get_row(i);
    int wt = BERC::count_errors(zero,codeword);
    if (wt==dAj){
      cout<<"codeword : "<<codeword<<endl;
      break;
    }
  }
  cout<<Aj*codeword<<endl;

  GF2mat AjplusT=Ajplus.transpose();
  GF2mat z(1,AjplusT.cols());
  z.set_row(0,codeword);
  GF2mat  Az=AjplusT.concatenate_vertical(z);
  cout<<Ajplus.row_rank()<<"<>"<<Az.rows()<<"<>"<<Az.row_rank()<<endl;
  if (  (Aj*Az.transpose()).is_zero() ){
    cout<<"Aj*AzT=0"<<endl;
  }
  /*
  GF2mat BjplusT=Bjplus.transpose();
  GF2mat  Bz=BjplusT.concatenate_vertical(z);
  cout<<Bjplus.row_rank()<<"<>"<<Bz.rows()<<"<>"<<Bz.row_rank()<<endl;
  */  
  return 0;
}

int generate_concatenation(GF2mat P[],int max_m,vector<int> dist, char * folder_Cm){//folder_Cm: the folder to save all C matrix
    GF2mat *Cm1,*Cm2;
  Cm1=&P[0];Cm2=&P[2];
    Cm1=extension(Cm1,P[1],2);
  GF2mat Gax=*Cm1,Gaz=*(Cm1+1);
  Cm2=extension(Cm2,P[3],2);  
  GF2mat Gbx=*Cm2,Gbz=*(Cm2+1);
  Gaz=Gaz.transpose();  Gbz=Gbz.transpose();
  //quantum_dist(Gax,Gaz,5,1);
  //following is an upper bound of distance, need to be checked
  int daz=min(classical_dist(P[0]),classical_dist(P[1]) );
  int dax=min(classical_dist(P[0].transpose()),classical_dist(P[1].transpose()));
  int dbz=min(classical_dist(P[2]),classical_dist(P[3]));
  int dbx=min(classical_dist(P[2].transpose()),classical_dist(P[3].transpose()));
  //  concatenate(Gax,Gaz,Gbx,Gbz,dax,daz,dbx,dbz);
  reduce(Gax,Gaz,Gbx,Gbz,dax,daz,dbx,dbz);
  cout<<"finish generate_concatenation"<<endl;
  return 0;
}

int generate(GF2mat P[],int max_m,vector<int> dist, char * folder_Cm){//folder_Cm: the folder to save all C matrix
  //from P[] create m chain complex for m=2,3,4, and save it in folder_Cm
  //  int L=6;//15 min for size 6
  //  GF2mat P=get_check_rept(L);
  //  check(); check2();return 0;
  //  const int max_m=4;
  
  GF2mat *Cm1;//the first matrix in m-chain complex C
  Cm1=&P[0];//initialize


  GF2mat Cms[max_m*(max_m)];//save all Cm matrices for test
  Cms[0]=P[0];
  for ( int i=2;i<max_m+1;i++){
     Cm1 = extension(Cm1,P[i-1],i);
    //    GF2mat *temp= & Cm1;
    for ( int j=0;j<i;j++){
      Cms[(i-1)*max_m+j] =  *(Cm1+j);
    }
    //saveC(Cm1,i,folder_Cm);
    parameterC(Cm1,i,max_m,dist);
  }
  //  check(Cms[4],Cms[5],Cms[8],Cms[9] );
  return 0;
}

int main(int args, char ** argv){

  
  RNG_randomize();
  Real_Timer timer;
  timer.tic();
  int max_m=4;
  GF2mat P[max_m];
  char * folder_Cm = "null";//argv[5];
  
  if (args == 2) {//generate random P, and save with title
    char * title = argv[1];      
    int px,py; //size of P[i]
    double density = 0.5; //density of P[i]
    for ( int i =0;i<max_m;i++){
      //  P[i] = MM_to_GF2mat(argv[i+1]);
      char pname[256];
      sprintf(pname,"%s-P%d.mm",title,i+1);//different input title to save the matrix in file
      px=randi(4,7);      py=randi(4,7);
      //px=5;py=px;
      density=0.75;//3.5/py;
      P[i]=generate_random_P(density,px,py,pname);//p,x,y,filename
            //check some condition to be satisfied. maximum number should be controled
      int condition_trials=500;
      for (int j=0;j<condition_trials;j++){
	// * check full rank, typical max run 2 to remain some full rank matrix
	//if (P[i].row_rank()==P[i].rows()){//reduce prob of full rank	
	if ( P[i].row_rank() == min(P[i].rows(),P[i].cols()) ){ //no full rank
	  P[i]=generate_random_P(density,px,py,pname);//p,x,y,filename
	}else{
	  GF2mat_to_MM(P[i],pname);//save file disabled in generate_random_P()
	  break;
	  }  // * // 
	/* //check condition for TZ's code, Alexey 2018
	int kappa=P[i].cols()-P[i].row_rank();
	int delta=classical_dist(P[i]);
	int delta_tilde=classical_dist(P[i].transpose());
	int delta_min = min(delta,delta_tilde);
	//	if (delta<INF &&delta_tilde<INF && kappa*delta_min >P[i].cols()){
	if (delta<INF &&delta_tilde<INF){//this condition is the same as P[i].row_rank()<min(P[i].rows(),P[i].cols())
	  cout<<"condition satidfied when j = "<<j<<endl;
	  GF2mat_to_MM(P[i],pname);//save file disabled in generate_random_P()
	  break;
	}else{
	  P[i]=generate_random_P(density,px,py,pname);//p,x,y,filename
	  }*/
      }
      //    P[i] = get_check_rept(4);
      //P[i]=get_check_code743(7);
      //      cout<<"P["<<i+1<<"] "<<P[i]<<endl;
    }
  }else{//from given P
        cout<<"Check result for given P. "<<argv[1]<<endl;
        for ( int i =0;i<max_m;i++){
	  char * filename_Pi = argv[i+1];
	  P[i]=MM_to_GF2mat(filename_Pi);
	  cout<<"P["<<i+1<<"] "<<P[i]<<endl;
	}
	/*	char * filename_P1 = argv[1];
	char * filename_P2 = argv[2];
	char * filename_P3 = argv[3];
	char * filename_P4 = argv[4];*/
	folder_Cm = argv[5]; //not in use

  }
  vector<int>  dist;  
  dist =   parameterP(P,max_m);//not necessary for concatenation right now

  generate_concatenation(P,max_m,dist,folder_Cm);
  //    generate(P,max_m,dist,folder_Cm);
  cout<<"Finish "<<argv[1]<<". Time needed is ";
  timer.toc_print();
  return 0;
}
