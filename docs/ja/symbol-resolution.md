# シンボル解決について

## この文書について

* 便宜上、本文書が対象とする言語を `TsuQL` とよぶ
  * 言語仕様は SQL 1999 をベースにしてあるが、細部が異なる
* 本文書では、 TsuQL におけるシンボル解決の手順について定める

## コンセプト

* 名前空間の考え方を単純化するため、ANSI SQL 1999とは相容れない仕様が存在する
* シンボル解決を行う際、フィールドやメソッドを除き、型情報の補助なしでシンボルを特定できるようにする
* 識別子列 (identifier chain) は先頭から順に、決定的 (deterministic) にシンボル解決を行う
  * `a.b` のような列がある場合、まず `a` に対応するシンボルを解決し、以降は `a` を解決した結果の子要素として `b` を検出する
  * `a` に複数の解決方法が存在した場合、 `a.b` が一意であったとしてもあいまいな識別子として取り扱う
    * `SELECT X.X FROM T AS X(X)` の `X.X` は多くのRDBMSで相関名 `X` のカラム `X` として解決できるが、 TsuQL では常に カラム `X` のフィールド `X` として解決しようとし、そのようなフィールドがなければエラーとなる
* 識別子列の構造によってカタログやスキーマを判別することはしない
  * 代わりに深度という概念で名前の衝突を緩和する

## プライマリシンボル

「プライマリシンボル」とは、 `TsuQL` において単一の識別子からなる名前 (以下、単純名) が参照しうるシンボルである。
なお、 `a.b.c` は複数の識別子からなる名前（以下、限定名）ではあるが、先頭の `a` は単純名である。

### プライマリシンボルの一覧

以下はプライマリシンボルの一覧である。

* カタログ
* スキーマ
* テーブル定義
* ビュー定義
* 関数定義
* データ型定義
* セッション変数
* パラメータ変数
* クエリ式 (`WITH ...`)
* 相関名 (`AS ...`)
* カラム名

### プライマリシンボルの解決手順

プライマリシンボルを検索する際、以下の順番で行う。

1. 現在の問合せ式
   1. カラム名
   2. テーブル参照, 相関名
2. 親問合せ式のプライマリシンボル (再帰的に行う)
3. クエリ参照 (`WITH ...`)
4. パラメータ変数
5. セッション変数
6. スキーマ要素
   * 変数定義
   * テーブル定義
   * ビュー定義
   * 関数定義
   * データ型定義
7. スキーマ
8. カタログ

なお、実際には上記からシンボル探索コンテキストに応じて必要なものだけを探索することになる。
探索コンテキストについては後述する。

----
note:

* TsuQL では副問合せ内に `WITH` 句を含められない

## プライマリシンボルの探索

TsuQL におけるシンボルの探索は、主にプライマリシンボルの探索と、その再帰的な子要素の探索の2つのパートに分かれる。

以降ではプライマリシンボルの探索アルゴリズムについて解説する。

### 探索深度

プライマリシンボルを探索する際、対象の単純名がいずれかの名前の一部である場合、その単純名の「深度 (depth)」によって検索対象となるシンボルの種類が異なる。
この深度は以下のように定める。

* ある名前が他の限定名の一部 **でない** 場合、その名前の深度を `0` とする
* ある名前が `A.*` の形式で表される (e.g. `SELECT T0.* FROM T0 ...`) 場合、 `A` の深度を `1` とする
* ある名前が `A.b` の形式で表され、かつ `b` が単純名である場合、 `A` の深度は `A.b` の深度に `1` を加えたものとする

### 探索コンテキスト

名前は、どの文脈で要求されたかによってシンボルの探索方法が異なる。

以下は、存在する文脈の種類である。

* 値コンテキスト
* リレーションコンテキスト
* 関数コンテキスト
* 型コンテキスト

#### 値コンテキスト

値コンテキストは、任意の値が要求される場面に名前が出現した場合を表す。

この場合、名前全体は何らかの変数を表すシンボルに解決され、プライマリシンボルはそれらを包含しうる要素となる。

1. 現在の問合せ式
   1. カラム名
   2. テーブル参照, 相関名 (`depth >= 1`)
2. 親問合せ式のプライマリシンボル (再帰的に行う)
3. パラメータ変数
4. セッション変数
5. スキーマ要素
   * 変数定義
   * テーブル定義 (`depth >= 1`)
   * ビュー定義 (`depth >= 1`)
6. スキーマ (`depth >= 1`)
7. カタログ (`depth >= 2`)

#### リレーションコンテキスト

リレーションコンテキストは、任意のテーブル定義やそれに類するものが要求される場面に名前が出現した場合を表す。

この場合、名前全体はいずれかのテーブル、ビュー、相関、クエリのいずれかを表すシンボルに解決され、プライマリシンボルはそれらを包含しうる要素となる。

1. クエリ参照 (`WITH ...`)
2. スキーマ要素
   * テーブル定義 (`depth = 0`)
   * ビュー定義 (`depth = 0`)
3. スキーマ (`depth = 1`)
4. カタログ (`depth = 2`)

#### 関数コンテキスト

関数コンテキストは、任意の関数が要求される場面に名前が出現した場合を表す。

この場合、名前全体はいずれかの関数やメソッドを表すシンボルに解決され、プライマリシンボルはそれらを包含しうる要素となる。

1. 現在の問合せ式
   1. カラム名 (`depth >= 1`)
   2. テーブル参照, 相関名 (`depth >= 2`)
2. 親問合せ式のプライマリシンボル (再帰的に行う)
3. パラメータ変数 (`depth >= 1`)
4. セッション変数 (`depth >= 1`)
5. スキーマ要素
   * 変数定義 (`depth >= 1`)
   * 関数定義 (`depth = 0`)
6. スキーマ (`depth >= 1`)
7. カタログ (`depth >= 2`)

#### 型コンテキスト

型コンテキストは、任意の型が要求される場面に名前が出現した場合を表す。

この場合、名前全体は何らかの型を表すシンボルに解決され、プライマリシンボルはそれらを包含しうる要素となる。

1. スキーマ要素
   * データ型定義 (`depth = 0`)
2. スキーマ (`depth = 1`)
3. カタログ (`depth = 2`)

----
note:

* TBD: nested type があるなら depth の制約を変える

### スキーマ探索パス

スキーマ要素のうち、スキーマ探索パスに記載されたスキーマのものはプライマリシンボルとして探索が可能である。

スキーマ探索パスは一次元のスキーマの列で、先頭に配置されたスキーマほど探索の優先順位が高くなる。
また、複数のスキーマをまたいで解決先のシンボルが複数存在した場合、優先順位の高いもののみが採用される。

TsuQLは言語仕様として特別なスキーマを規定したりはせず、ビルトインシンボルの探索ルールもこのスキーマ探索パスを利用して実現する。

## 子要素の探索

限定名に含まれるプライマリシンボルを解決した場合、後続する識別子を「プライマリシンボルの子要素」として解決する。
この操作は限定名全体を単一のシンボルとして解決するまで再帰的に行う。

子要素をとりうるシンボルと、その子要素は以下のとおりである。

| シンボル | 子要素 |
|:-:|:-:|
| カタログ | スキーマ |
| スキーマ | スキーマ要素 |
| テーブル, 相関名 | カラム |
| 値として解決されるもの (e.g. カラム) | フィールド, メソッド |

ただし、上記のうち実際に発見できるシンボルは現在の探索コンテキストに準ずる。

----
note:

* SQL/TsuQL において、スタティックメソッドは構文規則が異なる (`T::m(...)`) ため、限定名から直接解決されることはない

## あいまいさの回避

プライマリシンボルや子要素を探索した際に、複数の候補が発見された際、以下の手順であいまいさを回避する。

* プライマリシンボル探索時に、異なる優先順位で複数のシンボルを発見した場合、最も優先順位が高いもののみを採用する
  * なお、スキーマ探索パスはエントリごとに優先順位が定められており、異なるエントリで重複があれば、より先頭のスキーマで発見されたもののみを採用する
* 対象の要素が関数またはメソッドである場合、以下の手順で対象を絞り込む
  * 実引数のリストを `A1, ..., An` (`n >= 0`) とする
  * 対象の仮引数のリストが実引数のリストと異なる個数であった場合、その要素を除去する
  * 対象の仮引数リストを `U1, ... Un` とした場合、 `i=1..n` のいずれかについて `Ai` から `Ui` への代入変換が **行えない場合**, その要素を除去する
  * 任意の対象 `f1`, `f2` (`f1 <> f2`) について、それぞれの仮引数リストを `U1, ..., Un`, `V1, ..., Vn` とした場合、 `i=1..n` にすべてついて `Ui` を型に持つ任意の式 `Ei` から `Vi` への代入変換を行え、かつ逆が成り立たない場合、 `f1` を除去する
* 上記の結果、複数の候補が残った場合、あいまいなシンボルとしてコンパイルに失敗する

## 識別子と内部表現

TsuQLにおける識別子には、以下の2種類が存在する。

* regular identifier
  * アルファベット (`A-Z`, `a-z`)またはアンダースコア (`_`)から始まり、以降に0文字以上のアルファベット、アンダースコア、数字 (`0-9`) が後続するトークン
    * locale の取扱い等が面倒なので上記以外の文字は単語構成文字としない
* delimited identifier
  * ダブルクウォート (`"`) で始まり、ダブルクウォート以外の文字、または2連のダブルクウォートのいずれかが1回以上繰り返され、末尾にダブルクウォートが来るトークン

それぞれ、内部的には以下の文字列から構成された識別子として取り扱う。

* regular identifier
  * 元の文字列のうち、大文字のアルファベット (`A-Z`) をそれぞれ対応する小文字のアルファベット (`a-z`) に置き換えたもの
  * つまり、 regular identifier は本質的に case insensitive である
* delimited identifier
  * 元の文字列から先頭と末尾のダブルクウォート (`"`) を除去し、二連のダブルクウォート (`""`) を単一のダブルクウォート (`"`) に置き換えたもの
  * regular identifier とは逆に、 delimited identifier は case sensitive である

## ビルトイン関数

ビルトイン関数は TsuQL のキーワードを利用して参照されるが、内部的には通常の関数と同様にスキーマ要素として定義する。

多くのビルトイン関数は、関数の名称を表すキーワードを単なる識別子として読み替えたものとなる。
例えば、 `POSITION(A IN B)` は `position` という単純名の関数を、引数 `A`, `B` を伴って起動することと等しい。
それに伴い、実行基盤は対応する関数定義をスキーマパス上のスキーマ要素としてあらかじめ登録しておく必要がある。

いくつかのビルトイン関数は通常の式ではない引数をとるものが存在する (e.g. `TRIM(LEADING ...)`)。
このような関数は、通常の関数名に特定の接尾辞を付与したものを関数の単純名とする。

以下はビルトイン関数のうち、通常の関数呼び出しと異なる構文を有するものと、その対応付けの一覧である。
ただし、 `a, b, c, ...` はそれぞれ引数を表す式とし、 `id`, `'id'` を任意の名前とそれを文字列リテラルで表現したものとする。

下記に含まれないビルトイン関数の単純名や、それぞれの引数のデータ型などは `20.70 Definition of SQL built-in functions` の定義に準ずるものとする。
ただし、同書では関数名を delimited identifier として定義しているが、 TsuQL においては regular identifier として定義している (組み込み関数が本質的に case insensitive であるため)。

| 構文規則 | 対応する呼び出し |
|:-:|:-:|
| `POSITION(a IN b)` | `position(a, b)` |
| `EXTRACT(YEAR FROM a)` | `extract$year(a)` |
| `EXTRACT(MONTH FROM a)` | `extract$month(a)` |
| `EXTRACT(DAY FROM a)` | `extract$day(a)` |
| `EXTRACT(HOUR FROM a)` | `extract$hour(a)` |
| `EXTRACT(MINUTE FROM a)` | `extract$minute(a)` |
| `EXTRACT(SECOND FROM a)` | `extract$second(a)` |
| `EXTRACT(TIMEZONE_HOUR FROM a)` | `extract$timezone_hour(a)` |
| `EXTRACT(TIMEZONE_MINUTE FROM a)` | `extract$timezone_minute(a)` |
| `SUBSTRING(a FROM b)` | `substring(a, b)` |
| `SUBSTRING(a FROM b FOR c)` | `substring(a, b, c)` |
| `CONVERT(a USING id)` | `convert(a, 'id')` |
| `TRANSLATE(a USING id)` | `translate(a, 'id')` |
| `TRIM(FROM a)` | `trim(a)` |
| `TRIM(BOTH FROM a)` | `trim(a)` |
| `TRIM(LEADING FROM a)` | `trim$leading(a)` |
| `TRIM(TRAILING FROM a)` | `trim$trailing(a)` |
| `TRIM(b FROM a)` | `trim(a, b)` |
| `TRIM(BOTH b FROM a)` | `trim(a, b)` |
| `TRIM(LEADING b FROM a)` | `trim$leading(a, b)` |
| `TRIM(TRAILING b FROM a)` | `trim$trailing(a, b)` |
| `OVERLAY(a PLACING b FROM c)` | `overlay(a, b, c)` |
| `OVERLAY(a PLACING b FROM c FOR d)` | `overlay(a, b, c, d)` |
| `CURRENT_DATE` | `current_date()` |
| `CURRENT_TIME` | `current_time()` |
| `LOCALTIME` | `localtime()` |
| `CURRENT_TIMESTAMP` | `current_timestamp()` |
| `LOCALTIMESTAMP` | `localtimestamp()` |

同様に、集約関数についても以下のように定める。

| 構文規則 | 対応する呼び出し |
|:-:|:-:|
| `COUNT(*)` | `count()` |
| `COUNT(ALL a)` | `count(a)` |
| `COUNT(DISTINCT a)` | `count$distinct(a)` |
| `AVG(ALL a)` | `avg(a)` |
| `AVG(DISTINCT a)` | `avg$distinct(a)` |
| `MAX(ALL a)` | `max(a)` |
| `MAX(DISTINCT a)` | `max$distinct(a)` |
| `MIN(ALL a)` | `min(a)` |
| `MIN(DISTINCT a)` | `min$distinct(a)` |
| `SUM(ALL a)` | `sum(a)` |
| `SUM(DISTINCT a)` | `sum$distinct(a)` |
| `EVERY(ALL a)` | `every(a)` |
| `EVERY(DISTINCT a)` | `every$distinct(a)` |
| `BIT_AND(ALL a)` | `bit_and(a)` |
| `BIT_AND(DISTINCT a)` | `bit_and$distinct(a)` |
| `BIT_OR(ALL a)` | `bit_or(a)` |
| `BIT_OR(DISTINCT a)` | `bit_or$distinct(a)` |
| `BOOL_AND(ALL a)` | `bool_and(a)` |
| `BOOL_AND(DISTINCT a)` | `bool_and$distinct(a)` |
| `BOOL_OR(ALL a)` | `bool_or(a)` |
| `BOOL_OR(DISTINCT a)` | `bool_or$distinct(a)` |
