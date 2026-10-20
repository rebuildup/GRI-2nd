V2 仕様書作成に伴う担当作業について

今回の作業目的は、チームで V2 仕様書を作成すること

V1 までの資料、ソースコード、図、Doxygen 設定などが入った 2nd ディレクトリは、ZIP 形式ですでに共有しています。各担当者は、共有済みの 2nd ディレクトリを展開し、自分の担当部分について V2 版を作成してください。

共通ルール
既存の V0・V1 ファイルは削除・上書きしない
V1 を基に V2 を新しく作る
編集したファイルと書き出したファイルの両方を提出する
最終版の内容と図が一致しているか確認する
完成したファイルはチャットへ添付する

1. ユースケース図担当

最初に V1 を V2 としてコピーします。

```sh
cp Usecase/usecaseV1.pu Usecase/usecaseV2.pu
```

Usecase/usecaseV2.pu を編集した後、PNG を書き出します。

```sh
plantuml -tpng Usecase/usecaseV2.pu
```

提出するファイル：

```
Usecase/usecaseV2.pu
Usecase/usecaseV2.png
```

2. シーケンス図担当

V1 を V2 としてコピーします。

```sh
cp SequenceDiagram/sequenceV1.pu \
 SequenceDiagram/sequenceV2.pu
```

SequenceDiagram/sequenceV2.pu を編集した後、PNG を書き出します。

```sh
plantuml -tpng SequenceDiagram/sequenceV2.pu
```

提出するファイル：

```
SequenceDiagram/sequenceV2.pu
SequenceDiagram/sequenceV2.png
```

3. モジュール関連図担当

概要図と詳細図を V2 としてコピーします。

```sh
cp AssociationChart/GaiyouModuleV1.pu \
 AssociationChart/GaiyouModuleV2.pu

cp AssociationChart/DetailModuleV1.pu \
 AssociationChart/DetailModuleV2.pu
```

編集後、PNG を書き出します。

```sh
plantuml -tpng AssociationChart/GaiyouModuleV2.pu
plantuml -tpng AssociationChart/DetailModuleV2.pu
```

提出するファイル：

```
AssociationChart/GaiyouModuleV2.pu
AssociationChart/GaiyouModuleV2.png
AssociationChart/DetailModuleV2.pu
AssociationChart/DetailModuleV2.png
```

4. PAD 図担当

V1 の PAD 用 C ファイルを V2 としてコピーします。

```sh
cp PAD/GaiyouPadV1.c PAD/GaiyouPadV2.c
cp PAD/DetailPadV1.c PAD/DetailPadV2.c
```

GaiyouPadV2.c と DetailPadV2.c を、最終版の処理に合わせて編集してください。

PAD 図を生成します。

```sh
pad2ps PAD/GaiyouPadV2.c
pad2ps PAD/DetailPadV2.c
```

PDF へ変換します。

```sh
ps2pdf PAD/GaiyouPadV2.ps \
 PAD/GaiyouPadV2PAD.pdf

ps2pdf PAD/DetailPadV2.ps \
 PAD/DetailPadV2PAD.pdf
```

提出するファイル：

```
PAD/GaiyouPadV2.c
PAD/GaiyouPadV2PAD.pdf
PAD/DetailPadV2.c
PAD/DetailPadV2PAD.pdf
```

可能であれば、Markdown 確認用の PNG も作成してください。

```sh
pdftoppm -png -r 200 \
 PAD/GaiyouPadV2PAD.pdf \
 PAD/GaiyouPadV2PAD

pdftoppm -png -r 200 \
 PAD/DetailPadV2PAD.pdf \
 PAD/DetailPadV2PAD
```

生成されるファイル：

```
PAD/GaiyouPadV2PAD-1.png
PAD/DetailPadV2PAD-1.png
```

5. Doxygen 担当

最終版のソースコードがあるディレクトリを対象に、Doxygen 資料を作成してください。

```sh
cd 'srcV2'
doxygen Doxyfile > doxygen.log 2>&1
```

生成確認：

```sh
test -f html/index.html \
 && echo 'Doxygen 生成成功' \
 || echo 'html/index.html がありません'
```

警告・エラー確認：

```
grep -iE 'warning|error' doxygen.log
```

提出するファイル：

```
srcV ２/Doxyfile
srcV ２/doxygen.log
```

仕様書に掲載する必要がある場合は、Doxygen が生成した関数呼び出し関係図などの PNG も提出してください。

```
find html -type f \( \
 -name '_\_cgraph.png' -o \
 -name '_\_icgraph.png' -o \
 -name '_\_\_incl.png' -o \
 -name '_**dep**incl.png' \
\) -print
```

html ディレクトリ全体は、仕様書担当から求められた場合のみ共有してください。

6. 仕様書担当

各担当者から受け取った V2 成果物を、仕様書用 Markdown へ反映します。

主に使用するファイル：

```
Usecase/usecaseV2.png
SequenceDiagram/sequenceV2.png
AssociationChart/GaiyouModuleV2.png
AssociationChart/DetailModuleV2.png
PAD/GaiyouPadV2PAD.pdf
PAD/DetailPadV2PAD.pdf
Doxygen から選定した PNG
```

図を貼るだけではなく、各図について以下も記述します。

V1 から V2 へ変更した内容
チャットでの提出方法

添付できない場合は、ファイル名を明記してコードブロックで貼ってください。

ファイル名：Usecase/usecaseV2.pu

```plantuml
ここにファイル内容を貼る
```

PNG と PDF は cat で表示せず、ファイルとして添付してください。

提出時には、次の形式でメッセージを付けてください。

担当：
作成したファイル：
V1 から変更した内容：
確認してほしい点(あれば)：
