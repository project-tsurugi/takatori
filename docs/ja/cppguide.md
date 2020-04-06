# C++ プロジェクトガイドライン

## この文書について

* C++ プロジェクトを構成する際のメモ
  * https://github.com/project-tsurugi/takatori が現在のベース
  * ただし守れていないケースもある
* 既存プロジェクトは無理して追従しなくてよい
  * 新規プロジェクト向けのガイドライン
  * ただし、ビルド構成は共通化したほうが無難

## ビルド構成

* Git を利用する
  * プロジェクトのソースコードは Git で管理し、特定のバージョンにアクセスしやすい状態にしておく
* CMake を利用する
  * CMake のバージョンは、ターゲットプラットフォームの標準パッケージ管理ツール (e.g. `apt` / Ubuntu, `yum` CentOS) でインストール可能な物が望ましい
  * シェルスクリプト等の環境依存しやすいものを可能な限りビルド手順に含めない
* Docker を利用しやすい構成にする
  * Docker 化できない環境では CI を実施しにくい
  * 依存するライブラリは以下のいずれかが望ましい
    * ターゲットプラットフォームの標準パッケージ管理ツールで導入可能なもの
    * GitHub のパブリックリポジトリでソースコードが公開されており、 CMake でインストール可能なもの
    * アーカイブをプロジェクトにバンドル可能 (ライセンス、サイズ等) であり、特定の手順で非標準パスにインストール可能であるもの
  * 上記のいずれにも該当しない場合、個別に検討したほうがいい
* トップレベルの `README.md` に環境の要件、およびビルド手順を記載する
  * CMake のバージョン
  * C++ コンパイラのバージョン
  * 依存ライブラリの導入手順
  * ビルド手順
  * テスト実行手順
  * インストール手順
* 標準的なツールでコード品質を保つ
  * コンパイル時に `-Wall` 等ですべての警告を表示する
  * clang-tidy でソースコードをチェックする
  * CTest で単体テストを実行する
  * デバッグ構成時には address sanitizer などの動的なチェックを有効にする
  * Doxygen でAPIドキュメントを生成する
  * 上記のいずれかで警告やエラーが出たら、「ビルドが壊れている」とみなす

## リポジトリ構成

トップレベルに以下のファイル/ディレクトリを配置する。

```
./
|-- cmake/
|-- docs/
|   `-- ja/
|-- doxygen/
|   |-- CMakeLists.txt
|   `-- Doxyfile.in
|-- include/
|-- src/
|   `-- CMakeLists.txt
|-- test/
|   `-- CMakeLists.txt
|-- third_party
|   `-- CMakeLists.txt
|-- tools/
|   `-- bin/
|       `-- run-clang-tidy.py
|-- .clang-tidy
|-- CMakeLists.txt
|-- README.md
```

個々の内容については `takatori` などの既存プロジェクトを参照。

### ビルドディレクトリ

CMake を利用する際、かならず out-of-source ビルドを行う。

このとき、以下のディレクトリが利用されることを想定し、ソースコード管理から除外する。

* `./build` (`README.md` のビルド手順用)
* `./build-*` (複数のビルド構成用)
* `./cmake-*` (CLion規定値)

## コードスタイル

* 基本的には [C++ Core Guidelines](http://isocpp.github.io/CppCoreGuidelines/) に準拠

### 命名規約

* 公開シンボルは C++ 標準ライブラリと命名規約をそろえる
  * ファイル、クラス、関数、変数のいずれも `snake_case`
  * テンプレート引数のみ `CamelCase`
* プライベートフィールドには末尾に `_` を付与する
  * プライベート関数は自由に命名してよい
* 本質的に同じものを表す場合、同じ名前を付ける
  * static duck typing の上で重要
  * 逆に、本質的に異なるなら別の名前を付けると取違いが減る

### 公開ヘッダー

* 他のモジュールから利用される可能性のあるシンボルだけを、公開ヘッダーに配置する
  * 公開ヘッダーから参照されるヘッダーも公開ヘッダーでなければならないが、可能な限り削減することが望ましい
    * split declaration, pimpl, pure abstract などで公開ヘッダーから definition を取り除きやすくなる
    * ただし、性能や開発効率に多大な影響が出る場合には、ある程度妥協してもよい
  * 公開ヘッダーから参照する外部ライブラリを最低限にする
    * そうすることで、 CMake におけるライブラリ依存関係を `PRIVATE` にできる
    * ただし、これも性能や開発効率と相談
* 公開ヘッダーは `./include` ディレクトリ配下に、クラス限定名と同じ構造で配置する
  * IDE friendly
  * e.g. `class ::takatori::scalar::expression` -> `./include/takatori/scalar/expression.h`
* 非メンバ関数のみからなるヘッダーファイルは、名前空間と同じ構造のディレクトリに、以下を参考にファイル名を決める
  * 関数名
  * 取り扱う対象のコンセプト名
  * `algorithm.h`, `literals.h` など、名前空間に対する機能を提供するもの
* 1つの公開ヘッダーファイルには、直接利用するクラスの宣言を原則1つまでしか配置しない
  * 利用したいクラスから、公開ヘッダを選択する余地を減らす
  * メタ関数はクラスの形式で宣言するが、クラスとして扱わなくてよい
* 直接インクルードする価値がない公開ヘッダーは、 `*/details` に配置する
  * 名前空間自体も `*::details` にし、必要であれば `using` で別の名前空間に付け替える
* 公開ヘッダーは基本的にすべてのシンボルに対し Doxygen のコメントを付与する
  * 以下は除外
    * `private:` ブロックのシンボル
    * `namespace impl { ... }`
      * `@cond IMPL_DEFS ~ @endcond` で囲んで Doxygen の処理対象から外す
  * とにかく、 Doxygen の警告がなくなるまでコメントを書く
    * Doxygen に怒られたくなければ公開しない

### ソースファイル

* ソースファイルは `./src` ディレクトリ配下に、クラス限定名と同じ構造で配置する
  * ただし、名前空間の共通接頭辞は省略してよい (TBD)
  * e.g. `class ::takatori::scalar::expression` -> `./src/scalar/expression.cpp`
* ソースファイルに配置できる定義は、可能な限り配置する
  * 依存の削減・ビルド時間の短縮のため
  * 以下は宣言と定義の分離が明らかに行えない例
    * テンプレート関数
    * テンプレートクラスのメンバ関数
    * `constexpr` 関数
* ソースファイルの先頭には、対応するヘッダファイルへの `#include` を記載する

### テストコード

* ヘッダーファイルに対してテストフィクスチャをそれぞれ作成する
  * `*.h` に対して `*_test.cpp`
  * フィクスチャの名前はファイル名 (`*_test`) と揃える
  * CMake や CLion で不便にならないように、単純名が衝突する場合には名前空間の一部をファイル名に含める
    * `a/x.h`, `b/x.h` -> `a_x_test.cpp` -> `b_x_test.cpp`
  * あまりに長くなりすぎたらファイルを分割してもよい
    * 5000行を超えたあたりでIDEの挙動が不安定になった
* フィクスチャごとに異なる実行バイナリを生成する
  * ビルド時間の短縮のため
  * 別のフィクスチャの副作用に影響されないため
  * アボート時に別のテストを巻き込まないため
* テストコードの先頭には、対応するヘッダファイルへの `#include` を記載する
* 非公開ヘッダーに対してもテストを書いてよい

### include

* `#include` は基本的に絶対パスを指定し、 `#include <...>` の形式を利用する
  * 例外的に、以下の場合のみ `#include "..."` を利用してもよい
    * 公開ヘッダーから、カレントディレクトリまたはサブディレクトリを参照する場合
    * 非公開ヘッダーを参照する場合
* `#include` は以下の順序で記述し、読みやすい単位で空行を挟む
  * 対応する公開ヘッダー (ソースファイルの場合)
  * 標準ライブラリ
  * 外部ライブラリ
  * ファーストパーティライブラリ (e.g. `yugawara` から `takatori` への参照)
  * 自モジュールの公開ヘッダー (絶対パス)
  * 相対パス

## クラス

### オブジェクトの分類

クラスを設計する前に、そのクラスのオブジェクトが、以下のどの分類に当てはまるのかを検討する。

* 値
  * 属性が同値であれば同一とみなして問題ないモノ
* エンティティ
  * 本質的に同一の実体が二つと存在しないモノ
* リソース
  * システム上の有限なものを貸し出し、管理するモノ
* コンテナ
  * 値やエンティティなどを格納するモノ
* サービス
  * モノではなく、モノを受け取って何らかの処理を行うオブジェクト

上記は厳密な分類ではなく、オブジェクトは複数の分類に当てはまる場合もある。

上記の分類をもとに、クラスが備えるべき以下の基本的な性質について検討する。

* `==` comparable
  * 比較不可能でない限り、 `operator==` は定義しておく
    * 「値」は必ず属性の比較によって同値性を判定する
    * 「エンティティ」は識別子の比較か、または複製がないことが明らかであれば同一性の判定でよい
    * 「コンテナ」はコンポーネントの特性を引き継ぐ
    * 「リソース」はエンティティ同様に扱うか、そもそも比較不可能にしておく
* default constructible
  * デフォルト構築したオブジェクトは、規定値、初期状態、無効値のいずれかを表す
    * どれを表しているのかは、かならず doc comment に記載する
    * ただし、他のコンストラクタが存在しない場合、初期状態であることが明らかであり、そもそもデフォルトコンストラクタを明示的に宣言しないほうがいい
* copy constructible/assignable
  * 本質的にコピー可能であれば copyable として定義する
    * 上記「値」に分類される場合、積極的に copyable として設計する
    * 「リソース」は微妙なライン
    * 「コンテナ」はコンポーネントの特性を引き継ぐ
  * そうでない場合、 copyable にすると誤操作で問題が起こる
    * 「エンティティ」は本質的に増えてはいけない
    * 「リソース」は状況次第
  * 仮想関数を利用した場合、この限りでない
    * この場合、必要であれば polymorphic clone を利用可能にしておく
  * `std::initializer_list` を利用する場合、 copyable でないと使いにくい
* move constructible/assignable
  * 一部の例外を除き常に movable にする
    * 何らかの理由で他のオブジェクトが自身のポインタを保持する場合、 movable だと壊れる
  * movable でない場合、STLコンテナの多くが利用不可能になる
* conversion constructor
  * ある値を別の値に暗黙に変換する場合、 conversion constructor を用意する
  * conversion operator と conversion constructor を相互に作成すると、 ambiguous になりがち
    * 自分が定義するクラスに変換する場合、 conversion constructor を定義
    * 変換先に利用可能な conversion constructor を定義できない場合、 conversion operator を定義
  * 「値」と「コンテナ」には conversion constructor を定義しておくとなにかと便利
    * e.g. `operator==` ですべての組み合わせを記述するのではなく、一部は conversion constructor 経由にする
    * ただし、 implicit conversion constructor は clang-tidy に怒られるので、ambiguity を理解したうえで `// NOLINT` してやる
  * `std::initializer_list` をとる conversion constructor は優先度が高くなるので考えて作る
  * widening と narrowing を明確に定め、 widening のみへ implicit に変換可能にする
    * monotonic にしておかないと ambiguous になりがち

### 定数関連

* クラスの先頭では `public:` ブロックに型や定数を定義する
  * 型 (`using ... = ...;`)
    * テンプレートパラメータ
    * `std::size_t` など、具体性に欠けるもの
    * イテレータなどの実装クラス名に意味がないもの
    * `type_traits` によって計算され、様々な箇所で利用するもの
    * その他、 traits から参照されるもの
  * 定数 (`static constexpr ... = ...;`)
    * テンプレートパラメータ
    * 環境によって決まる値
    * 他からも参照されうるマジックナンバー
* static duck typing を意識し、本質的に同じものには同じ名前を付ける

### コンストラクタ

* 原則的にファクトリ関数を定義しない
  * コンストラクタを直接利用したほうが利便性が高い
    * initializer list style の初期化
    * implicit conversion
    * `new` operator
  * 以下はファクトリ関数を利用してもよいケース
    * abstract class を公開して実装クラスを隠す場合
    * conversion constructor が overload hell になっている場合
* テスト用に便利なコンストラクタを分けて定義する
  * テストの書きやすさ、読みやすさはメンテナンス性に大きく影響する
  * テスト用のコンストラクタは、本番用のコンストラクタに移譲する以外の処理を記述してはいけない
* フィールド初期化子を利用する
  * ほとんどのケースで、フィールド初期化のみで本体ブロックは空にできる
* 単一の引数をとるコンストラクタには `explicit` を付与する
  * conversion constructor と勘違いされて事故が起こりやすい
    * 意図的に conversion constructor を定義する場合、その限りではない
    * 引数が `std::initializer_list` のみである場合、つけると逆に不便になる
  * 呼び出し時に単一の引数になることが条件なので、デフォルト引数があると引っかかりやすい
* 可能であれば `noexcept` を付与する
  * nothrow constructible であることでいろいろ単純にできるか所がある
  * ただし、例外を利用することが適切な場合もあるので、そこまでこだわらない
* 可能であれば `constexpr` を付与する
  * `constexpr` コンストラクタがあればリテラルクラスとなり、 `static constexpr` で定数として利用可能になる
  * 標準ライブラリが未対応だったりと意外と困難なので無理はしない

### メンバ関数

* 受け取った引数を関数内で参照のみ行う場合、 `T const&` で受ける
  * 呼び出し元が lvalue, rvalue のいずれであってもオーバーロードなしで受けられる
  * ただし、 `std::string` に対する `std::string_view` のような軽量なビューがある場合、そちらで受ける
  * ただし、 `T` が trivially copyable であり、かつ十分に小さい場合、 `T` で受けたほうが速い
* 受け取った引数を関数内でコピーする場合、 `T` で受ける
  * 呼び出し元が lvalue, rvalue のいずれであってもオーバーロードなしで受けられる
  * ただし、ムーブが禁止または非常に遅い場合は `T const&`, `T&&` のオーバーロードを両方用意する

### オブジェクト管理

* naked new を直接利用しない
  * 基本的には `std::unique_ptr` と `std::shared_ptr` でほとんど事足りる
  * 利用したい場合、コンテナ内に隠蔽し、コンテナを通して管理する
* raw pointer を引数や戻り値に利用しない
  * 基本的には reference を利用する
    * `==` のセマンティクスが pointer と reference で違うので混在を避ける
  * 無効値を表したい場合、無効値を表す値の参照か、またはコンテナを利用する
    * e.g. [takatori::util::optional_ptr](https://github.com/project-tsurugi/takatori/blob/master/include/takatori/util/optional_ptr.h)
* フィールドに raw pointer を保持する場合、オーナーシップを持っていてはならない
  * raw pointer は borrowed object として統一する
  * `gsl::owner<>` は allocator の情報を持てないので使うべきでない

### friend

* 基本的に friend 宣言を行わない
  * メタプログラミング時に予想外の個所から参照される場合があり、カバーしきれないことが多い
* friend 定義を利用してもよいケース
  * クラステンプレートのテンプレート引数違い
    * 特に conversion constructor の実装に必要になる場合がある
  * `operator<<(std::ostream&, ...)`
    * 本質的にデバッグ用途であり、内部状態を表示したい場合が多々ある
  * `operator+` や `operator==` 等は friend で実装してもよいが、多くの場合は不要

### 演算子オーバーロード

* 非メンバ関数として演算子オーバーロードを定義する
  * メンバ関数では左項が固定され、メンバと非メンバが混ざることになり面倒
  * 非メンバ関数として定義できない場合はもちろん除外
  * 例外的に、 ambiguity を回避するためにメンバ関数として定義する場合がある
    * ただし、これはそれ以前の設計の問題ではある
* テンプレートクラスの比較演算子を定義する場合、左右で異なるテンプレート引数を指定可能にする
  * `T` と `U` が比較可能であるとき、それらのコンテナも比較可能にすると取り回しが良い
* 低コストで implicit conversion を行える場合、オーバーロードを削減できる
  * `U -> T` が可能である場合、 `(T, T)` だけでなんとかなる

### 非メンバ関数

* クラス内に `static` 関数を定義するのではなく、非メンバ関数として定義する
  * 前者は `using` でシンボルをインポートできない
    * Javaでは `T::newInstance()` 等が多かったが、 C++ では `make_T()` のほうが便利なことが多い

### 列挙

* 常に scoped enum を使う
  * unscoped enum は型が弱いので余計なバグを生み出しやすい
  * unqualified access が行いたい場合、別途 `constexpr` で定義する
  * ビットマスク的に使いたい場合、 [`takatori::util::enum_set`](https://github.com/project-tsurugi/takatori/blob/master/include/takatori/util/enum_set.h) のようなコンテナを別途用意する
    * ただし、上記は `std::bitset` ベースなので `constexpr` にできていない
* `constexpr std::string_view to_string_view(enum ...)` を定義する
  * 列挙から名前を取得する方法が存在しないため、ボイラープレートとして常に用意する
  * ついでに `operator<<(std::ostream&, ...)` のオーバーロードも定義する

### polymorphic

* polymorphic 以外の道があればそうしたほうがいい
  * 非 polymorphic class と作り方も使い方も違いすぎる
  * template, virtual を同時に指定できないなどの制限も多い
* 基底クラスを作る場合、 abstract にしたうえで virtual destructor を定義する
  * concrete polymorphic base class はおそらく設計ミス
  * non-virtual destructor にする場合、 `protected` に配置する
* sealed abstract class である場合、自身のクラスを表すタグを提供する
  * 作るのは多少面倒だが、実装クラスごとに `switch` するときに使いやすい
  * `static constexpr tag_type tag = ...;` で静的に取得可能
  * `virtual tag_type kind()` で動的に取得可能
* 潜在的にコピー可能である場合、 polymorphic clone を提供する
  * `T* T::clone()`
    * raw pointer が暴露するが、co-variant return type のために致し方なし
    * [`takatori::util::clonable`](https://github.com/project-tsurugi/takatori/blob/master/include/takatori/util/clonable.h) のようなヘルパ経由でコピーする

### グローバル変数

* グローバル変数は `constexpr` のみ
  * そうできないものについては、関数内に `static const` で定義する