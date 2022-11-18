#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//パラメータ
const int Elemcase = 3; // 要素数の種類
const int pattern = 10; // 要素数ごとの種類
const char* fname_pass = "randnum/"; //fnameのpass
const int fname_max = 50; //fnameの文字数

//乱数計算関数
void rand_create(FILE *fp, int n, int init) {
  srand((unsigned int)time(NULL) + init);
  for(int i = 0;i < n;i++){
    fprintf(fp,"%d\n",rand());
  }
}

int main(void) {
  //変数宣言
  FILE *fp[Elemcase * pattern];
  int n[Elemcase]; //要素数を格納
  char fname[Elemcase * pattern][fname_max];

  //初期設定
  printf("Please enter the number of the %dth types of elements in order.\n", Elemcase);
  for (int i = 0; i < Elemcase; i++) {
    printf("  n[%d]:", i);
    scanf("%d[^\n]%*c", &n[i]);
    //出力ファイル名の指定
    for (int j = 0; j < pattern; j++) {
      sprintf(fname[i * pattern + j], "%s%s%d%s%d%s", fname_pass, "randnum-", i, "-", j,".txt");
      printf("fname[%d]:%s%s%d%s%d%s", i * pattern + j, fname_pass, "randnum-", i, "-", j,".txt\n");
    }
  }

  //乱数計算
  for (int i = 0; i < Elemcase; i++) {
    for (int j = 0; j < pattern; j++) {
      if ((fp[i * pattern + j] = fopen(fname[i * pattern + j], "w")) == NULL){
        printf("File could not be opened.\n");
        return 1;
      }
      //出力ファイルに乱数を格納
      rand_create(fp[i * pattern + j], n[i], i + j);
      fclose(fp[i * pattern + j]);
    }
  }
  printf("File writing has been completed.\n");
  return 0;
}
