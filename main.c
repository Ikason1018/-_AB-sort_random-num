#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int Elemcase = 3; // 要素数の種類
int pattern = 10; // 要素数ごとの種類

//乱数計算関数
int rand_create(FILE *fp, int n, int init) {
  srand((unsigned int)time(NULL) + init);
  for(int i = 0;i < n;i++){
    fprintf(fp,"%d\n",rand());
  }
  return 0;
}

int main(void) {
  //変数宣言
  int n[Elemcase]; //要素数を格納
  char fname[Elemcase * pattern][20];
  FILE *fp[Elemcase * pattern];

  //初期設定
  printf("Please enter the number of the %dth types of elements in order.\n", Elemcase);
  for (int i = 0; i < Elemcase; i++) {
    printf("  n[%d]:", i);
    scanf("%d[^\n]%*c", &n[i]);
    //出力ファイル名の指定
    for (int j = 0; j < pattern; j++) {
      sprintf(fname[i * pattern + j], "%s%d%s%d%s", "randnum-", i, "-", j,".txt");
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
