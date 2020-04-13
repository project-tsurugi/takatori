# pImpl idiom について

## この文書について

* 本プロジェクトにおける pImpl イディオムの使いどころについて紹介

## pImpl イディオムとは

* クラスを公開部分 (interface) と実装部分 (implementation) に分離し、interface では implementation へのポインター (pointer to implementation) のみを所有するイディオム
  * interface は実装へのポインターのみをフィールドに持つ
  * implementation はパブリックAPIとせず、 interface を経由して利用する

## pImpl が達成するもの

* primary
  * 公開クラスに対する stable **ABI** の確立
    * 公開クラスの実態がポインター1つ分のみになるため、公開クラスのバイナリ互換性を担保しやすい
    * pure abstract class では vtable が ABI の一部になるため、これは実現できない
* secondary
  * 実装詳細をパブリックヘッダーから隠蔽
    * C++ では `private` であってもフィールドやメンバ関数宣言をヘッダに記述する必要があり、その部分がどうしても隠せない
    * パブリックヘッダーから参照するヘッダーは、同様に公開されていなければならないため、この依存の除去は重要
    * ただし、これは pure abstract class でも実現はできる

## pImpl の欠点

* グルーコードが増える
  * ヘッダからは実装クラスの宣言のみしか見えていないため、自動生成可能なコンストラクタも明示的に宣言し、実装クラスのメンバ宣言が見える位置で定義しなければならない
  * interface で宣言したメンバ関数を、 implementation でも同様に宣言することがたいていの場合求められる
* 性能が多少劣化する
  * implementation をヒープに配置する必要があるため、ヒープの割り当てと解放のコストが余計にかかる
  * interface から implementation へポインターを経由するため、多少のオーバーヘッドがある
* テンプレートが一通り使えなくなる
  * interface の宣言から implementation を参照できないので、公開クラスをテンプレートにしたり、メンバ関数をテンプレートにしたりが難しい
* クラス継承を利用するとメリットが失われる
  * 公開クラスに vtable を持たせると、それは ABI の一部となるため pImpl の主目的が達成できない

## pImpl の使いどころ

* エンドユーザーやサードパーティが直接利用するAPI
  * バイナリ互換性はあったほうがいいが、バージョン1.0からでよい
  * ただし、バイナリ互換性を実現するには、すべての公開クラスがそうなっている必要がある
* フィールドに公開したくないクラスを持つケース
  * ライセンスやビルド手順的に当該ライブラリのヘッダをインストールするのがためらわれるもの
  * 対象のヘッダをインクルードしただけでビルド時間が爆裂するやつ (Boost系にたまにある)
  * type erasure (`std::any`, `boost::type_erasure` 等) を使う手もあるが、たいていは pImpl のほうが品質・性能の面でマシになる
  * 書くコードの量では type erasure のほうが少なくなるかも

## pImpl イディオムの例

### 標準的な実装

* 標準的な実装では、 interface クラスに `std::unique_ptr` で実装クラスへのポインタを持たせる
* 以下の例では、 `object.h` を公開ヘッダとして提供すれば、残りは公開しなくてよい
  * `object_impl.h` を非公開ヘッダとして別途作成することで、このヘッダを利用できれば非公開APIを直接利用できるようになっている
  * 非公開部分を多用する場合、 implementation クラスはメンバクラスではなく、トップレベルにおいてもよい (`object::impl` -> `object_impl` など)

```cpp
// object.h
#pragma once

#include <memory>

/* NOTE:
 * interface の declaration 部分。
 * 公開ヘッダに配置する。
 */
class object {
public:
    /* NOTE:
     * 一般的には implementation の declaration は private に配置するが、
     * public に配置することによって実装からの取り回しが良くなる。
     */
    /// @private
    class impl;

    /* NOTE:
     * pImpl ではコンストラクタの自動生成を行えないため、自前で用意する必要がある。
     * また、ここから実装が見えないため、 declaration のみを行う。
     * コピー関連は不要なら = delete してしまってもよい。
     * ムーブ関連は簡単に実現できるため、特別な理由がなければ作っておく。
     */
    /**
     * @brief creates a new instance.
     */
    object();

    ~object();

    /**
     * @brief creates a new instance.
     * @param other the copy source
     */
    object(object const& other);

    /**
     * @brief assigns the given object into this.
     * @param other the copy source
     */
    object& operator=(object const& other);

    /**
     * @brief creates a new instance.
     * @param other the copy source
     */
    object(object&& other) noexcept;

    /**
     * @brief assigns the given object into this.
     * @param other the copy source
     */
    object& operator=(object&& other) noexcept;

    /* NOTE:
     * 公開メンバも、ここでは declaration のみを用意しておく。
     */
    int something();

private:
    /* NOTE:
     * フィールドに std::unique_ptr で implementation へのポインタのみを所有する。
     */
    std::unique_ptr<impl> impl_;

    /* NOTE:
     * implementation を friend 宣言しておくことで、
     * interface から implementation を安全に取り出す手段を用意できる。
     */
    friend impl;
};
```

```cpp
// object_impl.h
#pragma once

#include <object.h>

/* NOTE:
 * 前方宣言した implementation クラスを実際に宣言する。
 * 例では各メンバの definition も同時に行っているが、別ソースファイルに分離してもよい。
 * いずれにしても、 implementation クラスを非公開ヘッダとして用意すると取り回しがよい。
 */
class object::impl {
public:

    /* NOTE:
     * interface で定義したメンバ関数と同じものを implementation でも用意する。
     */
    int something() {
        return ++field_;
    }

    /* NOTE:
     * 非公開メンバを定義することもできる。
     */
    int implementation_only_something() {
        return --field_;
    }

    /* NOTE:
     * interface から implementation を取得するためのバックドア.
     * implementation の非公開メンバを利用するために必要。
     */
    static impl& extract(object& interface) noexcept {
        return *interface.impl_;
    }

private:
    /* NOTE:
     * implementation では自由にフィールドを宣言できる。
     */
    int field_ {};
};
```

```cpp
// object.cpp
#include <object.h>
#include <object_impl.h>

/* NOTE:
 * implementation は std::make_unique で作成する。
 * コンストラクタ引数がある場合、 implementation 側にも同様のコンストラクタを作成しておけば、
 * 引数を転送するだけで実現できる。
 */
object::object()
    : impl_(std::make_unique<impl>())
{}

/* NOTE:
 * デストラクタは std::unique_ptr のデフォルト処理でいいので = default.
 */
object::~object() = default;

/* NOTE:
 * コピー関連は implementation に移譲する必要がある。
 */
object::object(object const& other)
    : impl_(std::make_unique<impl>(*other.impl_))
{}

object& object::operator=(object const& other) {
    *impl_ = *other.impl_;
    return *this;
}

/* NOTE:
 * ムーブ関連は std::unique_ptr のデフォルト処理でいいので = default.
 */
object::object(object&& other) noexcept = default;
object& object::operator=(object&& other) noexcept = default;

/* NOTE:
 * interface のメンバ関数は、 implementation の同名のメンバ関数に移譲する。
 */
int object::something() {
    return impl_->something();
}
```

### `std::shared_ptr` を利用した実装

* interface クラスに `std::shared_ptr` で実装クラスへのポインタを持たせると便利なケースがある
  * `std::shared_ptr` がデリーター部分を型消去してくれるため、デストラクタ等を自動生成できる
  * 変更不可能な「値」クラスに適している
* `takatori::datetime::time_zone` はこのパターン

```cpp
// object.h
#pragma once

#include <memory>

class object {
public:
    /// @private
    class impl;

    /**
     * @brief creates a new instance.
     */
    object();

    /* NOTE:
     * std::shared_ptr 版ではデストラクタ等は自動生成できる。
     */

    int something();

private:
    std::shared_ptr<impl> impl_;
    friend impl;
};
```

```cpp
// object_impl.h
// std::unique_ptr 版と同様のため省略
```

```cpp
// object.cpp
#include <object.h>
#include <object_impl.h>

/* NOTE:
 * implementation は std::allocate_shared で作成するとよい。
 */
object::object()
    : impl_(std::allocate_shared<impl, std::allocator<impl>>({}))
{}

/* NOTE:
 * std::unique_ptr 版と違い、デストラクタ等はヘッダ側で暗黙に定義済み。
 */

int object::something() {
    return impl_->something();
}
```
