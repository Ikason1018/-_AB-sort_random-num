# expeII_AB-sort_random-num
本レポジトリ内の`randnum_gen.c`は電気情報工学実験Ⅱ後期A・Bソーティングアルゴリズムの効率でソートされる乱数値が記述されたファイル(以下、出力乱数ファイル)を生成するプログラムである。

# パラメータとファイル階層構造
`randnum<dr>`内の`randnum-0-[0-9].txt`は`randnum_gen.exe`に`n[0]=100`を, `randnum-1-[0-9].txt`は`n[1]=1000`を, `randnum-2-[0-9].txt`は`n[2]=100000`を渡した場合の出力乱数ファイルである。以下に`randnum_gen.c`に記載されている4つのパラメータの説明を示す。
​
- **`Elemcase`** : `randnum_gen.exe`実行時に渡す`n[i]`(0<`i`<`n`)の個数, `i+1`が取りうる最大値(初期値:`3`)
- **`pattern`** : `n[i]`個の乱数が記述された出力乱数ファイルの数(初期値:`10`)
- `fname_pass` : 出力乱数ファイルが置かれるディレクトリの`randnum_gen.c`に対する相対パス(初期値:`"randnum/"`)
- `fname_max` : `fname_pass`と出力乱数ファイル名を連結させた文字列の長さより大きな値(初期値:`50`)

ダウンロードしたZIPファイルを解凍したものは、以下の階層になっている。
​
```
expeII_AB-sort_random-num<dr> ──┬── .vscode<dr> ───── tasks.json
                                ├── randnum<dr> ───── randnum-[0-2]-[0-9].txt
                                │── 20221125計測結果.txt
                                │── config.txt
                                │── randnum_gen.c
                                │── randnum_gen.exe
                                │── randnum_template.c
                                │── randnum_template.exe
                                │── README.md
                                └── Sort_time_list.csv
```

初期状態で出力乱数ファイルは`randnum`フォルダ直下に生成される。

追記：2022/11/25に計測を行ったため、`20221125計測結果.txt`を追加

​
# 使用方法
1. <span style="color: green; ">Code</span>ボタンをクリックし、Download ZIPを選択する。
1. ダウンロードしたZIPファイルを解凍し、`randnum_template.c`の18行目以降に任意のソーティングアルゴリズムを関数化したものを記述する。なお、この関数は`void`型で、引数は`int *x, int n`(`*x`はソート対象の数列, `n`はその要素数)とするのが望ましい。
1. `main`関数計算部の`//任意のソーティングアルゴリズム関数:関数名(x, n[i]);`の次の行で先ほど記述した関数を呼び出す。ただし、前作業で記述した関数の引数に合うように呼び出すこと。
1. 計測条件に応じて`randnum_gen.c`を書き換える。これを[**☆の操作**](#☆の操作)として以下に示す。
1. gccコンパイラにより`randnum_gen.c`をコンパイルする[^1]。
    ```
    gcc randnum_gen.c -o randnum_gen.exe
    ```
1. `randnum`フォルダに計測条件に合致するフォルダがあることを確認する[^2]。
1. gccコンパイラにより、`randnum_template.c`をコンパイルする。
    ```
    gcc randnum_template.c -o randnum_template.exe
    ```
1. 生成した`randnum_template.exe`を実行して、得られた計測時間が`Sort_time_list.csv`に出力されていることを確認する。
​

# ☆の操作
[パラメータとファイル階層構造](#パラメータとファイル階層構造)に示したパラメータの内、太字になっている`Elemcase`, `pattern`は本レポジトリの`randnum_gen.c`でそれぞれ`3`, `10`となっている。そのため、実行ファイルが生成する出力乱数ファイルは合計で30個存在する。この2つのパラメータを計測条件に応じて書き換えることで、目的に応じた出力乱数ファイルが生成される。

## 例：ソート対象の乱数の個数を$1, 2, 4, 8, \ldots , 2^{30}$個と変化させた時のソート時間の変化を測定する時(1つの場合につき5回測定)

`randnum_gen.c`内の変数`n[]`は`int`型のため、これが32bitの場合は最大値が$2^{31}-1$になるが、今回は$2^{30}$個のためプログラム内の32bit整数型`int`を64bit整数型に変更しなくても心配ない[^2]。

次に、$1=2^0$から$2^{30}$まで変化させるときの場合は全部で31個ある。つまり、`Elemcase=30`となる。また、1つの場合につき5回計測とあるため、`pattern=5`となる。

出力乱数ファイルは`Elemcase`と`pattern`の積である$30×5=150$個存在するため、最も長い出力乱数ファイル名は`randnum-29-149.txt`となる。これは18文字のため、`fname_pass`の文字数との合計が`fname_max`の初期値`50`を超える場合は`fname_max`をより大きな数に書き換える。`fname_pass`の初期値は`"randnum/"`のため、`fname_pass`をより長いものに変更しない限りは$18+8=26$で50を超えない。


[^1]:実行環境にgccコンパイラが既にインストールされている前提である。
[^2]:理解されていると思うが、`fname_pass`を変更した場合は`randnum`フォルダでは無いこともある。
[^3]:詳細は[このサイト](https://marycore.jp/prog/c-lang/data-type-min-max-values/)を参照していただきたい。