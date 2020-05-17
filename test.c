#include "weilei_lib/my_lib.h"
#include <stdio.h>
#include <itpp/itbase.h>
#include <fstream>
int copy_test(){//fail
  cout<<"copy the text in a file to the end of another file"<<endl;
  ifstream infile("tmp/a.txt");
  ofstream outfile("tmp/b.txt");
  outfile << infile.rdbuf();
  infile.close();
  outfile.close();
  return 0;
}

int mm_test(int k){//test the limit of array length
  GF2mat G;
  //  int   k=750;//750, segmentation fault
  //  for ( int k=500;k<1000;k+=50){
  cout<<"k = "<<k<<endl;
  G.set_size(k,k,true);
  for (int i=0;i<k;i++){
    for ( int j=0;j<k;j++){
      G.set(i,j,1);
    }
  }
  GF2mat_to_MM(G,"data/tmp/bigG.mm");
    //}
  cout<<"done"<<endl;
  return 0;

}
int mm_test2(){
  GF2mat G;
  int   k=3;
  //  for ( int k=500;k<1000;k+=50){
  cout<<"k = "<<k<<endl;
  G.set_size(k,k,true);
  for (int i=0;i<k;i++){
    for ( int j=0;j<k;j++){
      G.set(i,i,1);
    }
  }
  GF2mat_to_MM(G,"data/tmp/smallG.mm");
    //}

  return 0;

}

int mm_test3(){
  GF2mat G;
  G = MM_to_GF2mat("data/tmp/bigG.mm7000");
  cout<<"got matrix G("<<G.rows()<<", "<<G.cols()<<")"<<endl;
  return 0;
}

int G_s_test(){  
  //check the rank of G_sx and G_sz for cubic repetition code
  //result:  G_sx.rows() = 1;
  char *filename_G_sx="data/repetition/cubic_size_5_G_sx.mm";
  char *filename_G_sz="data/repetition/cubic_size_5_G_sz.mm";
  cout<<filename_G_sx<<endl;
  GF2mat G_sx = MM_to_GF2mat(filename_G_sx);
  GF2mat G_sz = MM_to_GF2mat(filename_G_sz);
  cout<<G_sx.rows()<<" + "<<G_sx.cols()<<endl;
  cout<<G_sz.rows()<<" + "<<G_sz.cols()<<endl;
  return 0;
  
}

GF2mat * array_pointer(){
  GF2mat A=gf2dense_eye(2),B(3,3);
  static GF2mat  p[]={A,B};
  cout<<"A = "<<A<<endl;
  return &p[0];
}

int * array_pointer2(){
  const  int m=5;
  int d[m][m];
  d[0][0]=1;
  d[0][1]=2;
  d[1][1]=3;
  return &d[0][0];
}

int array_pointer_test(){
  GF2mat *p;
  p= array_pointer();
  cout<<p<<endl;
  cout<<*(p+1)<<endl;

  const  int m=5;
  int *d;//[m][m];
  d=array_pointer2();
  cout<<d<<endl;
  cout<<*(d+1)<<endl;
  return 0;
}

vector<vector<int> > vector_test(){
  vector<vector<int> > vec(10);
  vector<int> sub(10);
  sub[0]=1;
  vec[0]=sub;
  for ( int i=0;i<10;i++){
    cout<<vec[i][i]<<endl;
  }
  return vec;
}

int run_vector_test(){
  vector<vector<int> > vec;
  //  cout<<vec<<endl;
  //cout<<vec[1]<<endl;
  vec = vector_test();
  for ( int i=0;i<10;i++){
    cout<<vec[i][i]<<endl;
  }

  return 0;
}



int pointer_test(){
  int max=3;
  GF2mat *ptr[max];
  int var[]={1,3,5};
  GF2mat G(2,3);
  cout<<G<<endl;
  ptr[0]=&G;
  GF2mat Q=*ptr[0];
  cout<<Q<<endl;
  GF2mat H[]={G,G};
  cout<<sizeof(H)/sizeof(H[0])<<endl;

  GF2mat F[]={G};
  cout<<sizeof(F)/sizeof(F[0])<<endl;

  
  /*
  for (int i=0;i<max;i++){
    ptr[i] = &var[i];
  }
  for ( int i = 0; i < max; i++) {
    printf("Value of var[%d] = %d\n", i, *ptr[i] );
  }
  */
  
  return 0;
}

int char_test(){
  char a[]="left",b[]="right";

  if (a=="left"){
    cout<<'done'<<endl;
  }
  return 0;

}


int if_test(){

  int a= (3>2)? 1:0;
  cout<<a<<endl;
  int b= (3>3)? 1:0;
  cout<<b<<endl;
}

int pointer_array_test(){
  int **board = new int*[4];
  for (int i = 0; i < 4; i++) {
    board[i] = new int[10];
  }

}
int main(int args, char ** argvs){
  cout<<"begin test"<<endl;
  cout<<"args = "<<args<<endl;
  //int k=atof(argvs[1]);
  
  //  mm_test(k);//big G
  //mm_test2();//small G
  //mm_test3();//read
  //  copy_test();

  //G_s_test();
  //    pointer_test();
  //  array_pointer_test();
  //  char_test();
  //  run_vector_test();
  //  if_test();
  pointer_array_test();
  cout<<"finish test"<<endl;
  return 0;
}
