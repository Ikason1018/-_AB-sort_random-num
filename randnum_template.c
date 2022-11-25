#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define int_digit 12 // INT_MAX(32bit)の桁数+2('\n\0'の分)

//パラメータ(configファイルから読み込む)
int Elemcase;                              // 要素数の種類
int pattern;                               // 要素数ごとの種類
char *fname_pass;                          // fnameのpass
int fname_max;                             // fnameの文字数
char *config_fname = "config.txt";         // configファイル
char *export_fname = "Sort_time_list.csv"; // 測定結果出力ファイル

//任意のソーティングアルゴリズム関数:void 関数名(int x[], int n){}
void bubble_sort(int x[], int n) { //バブルソート関数
  int temp = 0;
  for (int i = 0; i < n - 1; i++) {
    for (int j = n - 1; j > i; j--) {
      if (x[j - 1] > x[j]) {
        temp = x[j - 1];
        x[j - 1] = x[j];
        x[j] = temp;
      }
    }
  }
}

int randnum_import(FILE *fp, int *x, const int n) {
  char str[n][int_digit];
  for (int i = 0; i < n; i++) {
    if (fgets(str[i], int_digit, fp) == NULL) {
      return 1;
    }
    x[i] = (int)strtol(str[n], NULL, 10); // 10進数に変換
  }
  return 0;
}

int main(void) {
  //変数宣言
  FILE *fp;
  int *n, *x;
  char *fname;
  clock_t start_sort, end_sort;
  double **sort_time, *sort_ave, *sort_SD;

  // configファイルの読み込み
  if ((fp = fopen(config_fname, "r")) == NULL) {
    printf("\"%s\" could not be opened.\n", config_fname);
    return 1;
  }
  char str[4][20];
  for(int i = 0;i < 4;i++){
    if(fgets(str[i], 20, fp) == NULL){
      return 1;
    }
  }
  // 出典:https://teratail.com/questions/281401
  char *p = strchr(str[3], '\n');  // 改行文字を探す
  if (p) *p = '\0';        // 改行文字あれば、上書きして消す
  Elemcase = (int)strtol(str[0], NULL, 10);
  pattern = (int)strtol(str[1], NULL, 10);
  fname_max = (int)strtol(str[2], NULL, 10);
  fname_pass = str[3];
  printf("Elemcase:%d\npattern:%d\nfname_pass:%s\nfname_max:%d\n", Elemcase, pattern, fname_pass, fname_max);

  //メモリ確保
  n = (int *)calloc(Elemcase, sizeof(int));
  fname = (char *)calloc(fname_max, sizeof(char));
  sort_time = (double **)calloc(Elemcase, sizeof(double *));
  for (int i = 0; i < Elemcase; i++) {
    sort_time[i] = (double *)calloc(pattern, sizeof(double));
  }
  sort_ave = (double *)calloc(Elemcase, sizeof(double));
  sort_SD = (double *)calloc(Elemcase, sizeof(double));

  printf("Please check the %d types of n\n", Elemcase);
  for (int i = 0; i < Elemcase; i++) {
    fscanf(fp, "%d ", &n[i]);
  }
  for (int i = 0; i < Elemcase; i++) {
    printf("  n[%d]:%d\n", i, n[i]);
  }
  fclose(fp); //configファイルを閉じる

  //計算部
  for (int i = 0; i < Elemcase; i++) {
    sort_ave[i] = 0.0;
    sort_SD[i] = 0.0;
    for (int j = 0; j < pattern; j++) {
      sprintf(fname, "%s%s%d%s%d%s", fname_pass, "randnum-", i, "-", j, ".txt");
      x = (int *)calloc(n[i], sizeof(int));
      if ((fp = fopen(fname, "r")) == NULL) {
        printf("\"%s\" could not be opened.\n", fname);
        return 1;
      }
      //出力乱数ファイルの読み込み
      if (randnum_import(fp, x, n[i]) == 1) {
        return 1;
      }

      //ソート時間の計測
      start_sort = clock();
      //任意のソーティングアルゴリズム関数:関数名(x, n[i]);
      bubble_sort(x, n[i]);
      end_sort = clock();
      sort_time[i][j] = (double)(end_sort - start_sort) / CLOCKS_PER_SEC;

      //後処理
      fclose(fp);
      free(x);
      sort_ave[i] += sort_time[i][j];
      sort_SD[i] += sort_time[i][j] * sort_time[i][j];
      printf("Sorting of \"%s\" has been completed.\n", fname);
    }
    //平均aveと標準偏差SDの計算
    sort_ave[i] = sort_ave[i] / (double)Elemcase;
    sort_SD[i] =
        sqrt(sort_SD[i] / (double)Elemcase - sort_ave[i] * sort_ave[i]);
  }

  //出力部
  int check = 0; // export_fnameが開けるか確認
  if ((fp = fopen(export_fname, "w")) == NULL) {
    printf("\"%s\" could not be opened.\n", export_fname);
    check = 1;
  }
  printf("Time taken to sort random numbers.\n");
  for (int i = 0; i < Elemcase; i++) {
    printf("%srandnum-%d-N.txt(n[%d]:%d):\n  Ave:%lf[s], SD:%lf[s]\n",
           fname_pass, i, i, n[i], sort_ave[i], sort_SD[i]);
    for (int j = 0; j < pattern; j++) {
      printf("  N=%*d:%lf[s]\n", (int)log10(pattern - 1) + 1, j,
             sort_time[i][j]);
    }
  }
  if (check == 0) {
    fprintf(fp, "Time taken to sort random numbers.(unit:[s])\nN=");
    for (int i = 0; i < Elemcase - 1; i++) {
      fprintf(fp, "%d,", i);
    }
    fprintf(fp, "%d,Ave,SD\n", Elemcase - 1);
    for (int i = 0; i < Elemcase; i++) {
      fprintf(fp, "rand_file:%srandnum-%d-N.txt(n[%d]:%d)\n", fname_pass, i, i,
              n[i]);
      for (int j = 0; j < pattern - 1; j++) {
        fprintf(fp, "%lf,", sort_time[i][j]);
      }
      fprintf(fp, "%lf,Ave:%lf,SD:%lf\n", sort_time[i][pattern - 1],
              sort_ave[i], sort_SD[i]);
    }
    fclose(fp);
    printf("Measurement results were output to %s.\n", export_fname);
  }

  //後処理
  free(n);
  free(fname);
  for (int i = 0; i < Elemcase; i++) {
    free(sort_time[i]);
  }
  free(sort_time);
  free(sort_ave);
  free(sort_SD);
  return 0;
}
