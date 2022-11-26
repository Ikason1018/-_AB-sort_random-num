#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//パラメータ
const int Elemcase = 3;                  // 要素数の種類
const int pattern = 10;                  // 要素数ごとの種類
const char *fname_pass = "randnum/";     // fnameのpass
const int fname_max = 50;                // fnameの文字数
const char *config_fname = "config.txt"; // configファイル

//乱数計算関数
void rand_create(FILE *fp, int n, int init) {
  srand((unsigned int)time(NULL) + init);
  for (int i = 0; i < n; i++) {
    fprintf(fp, "%d\n", rand());
  }
}

int main(void) {
  //変数宣言
  FILE *fp;
  int n[Elemcase]; //要素数を格納
  char fname[Elemcase][pattern][fname_max];

  //初期設定
  printf("RAND_MAX:%d\nPlease enter the number of the %dth types of elements "
         "in order.\n",RAND_MAX, Elemcase);
  for (int i = 0; i < Elemcase; i++) {
    printf("  n[%d]:", i);
    scanf("%d[^\n]%*c", &n[i]);
    //出力ファイル名の指定
    for (int j = 0; j < pattern; j++) {
      sprintf(fname[i][j], "%s%s%d%s%d%s", fname_pass, "randnum-", i, "-", j, ".txt");
      // printf("fname[%d]:%s%s%d%s%d%s", i * pattern + j, fname_pass, "randnum-", i, "-", j, ".txt\n");
    }
  }

  //乱数計算
  for (int i = 0; i < Elemcase; i++) {
    for (int j = 0; j < pattern; j++) {
      if ((fp = fopen(fname[i][j], "w")) == NULL) {
        printf("\"%s\" could not be opened.\n", fname[i][j]);
        return 1;
      }
      //出力ファイルに乱数を格納
      rand_create(fp, n[i], i + j);
      fclose(fp);
    }
  }
  printf("File writing has been completed.\n");

  // configファイルへの書込み
  if ((fp = fopen(config_fname, "w")) == NULL) {
    printf("\"%s\" could not be opened.\n", config_fname);
    return 1;
  }
  fprintf(fp, "%d\n%d\n%d\n%s\n", Elemcase, pattern, fname_max, fname_pass);
  for(int i = 0;i < Elemcase;i++){
    fprintf(fp, "%d ", n[i]);
  }

  return 0;
}
