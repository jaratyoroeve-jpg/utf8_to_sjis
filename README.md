# utf8_sjis

[![License: 0BSD](https://img.shields.io/badge/license-0BSD-blue.svg)](LICENSE)

`utf8_sjis` は個人が試しに作った実験的なヘッダーオンリーライブラリです。
C++20 の `consteval` を使って、UTF-8 文字列リテラルを Shift-JIS バイト列へコンパイル時に変換します。

## ざっくり特徴

- ヘッダーオンリー
- コンパイル時に UTF-8 を Shift-JIS に変換
- 実行時オーバーヘッドなし
- 変換不可能な文字を `?` に置換可能

## 要件

- C++20 以上

## 使い方

```cpp
#include "utf8_to_sjis.hpp"

constexpr auto s = utf8_sjis::utf8_to_sjis<"こんにちは世界">;
// s の型: std::array<unsigned char, N>

some_legacy_api(s.data(), s.size());
```

### 変換できない文字の扱い

2番目のテンプレートパラメータに `true` を渡すと、SJIS に変換できない文字や無効な UTF-8 シーケンスを `?` (`0x3F`) に置換します。

```cpp
constexpr auto s = utf8_sjis::utf8_to_sjis<"こんにちは🤔世界", true>;
```

## サポートファイル

| ファイル | 説明 |
|---|---|
| `utf8_to_sjis.hpp` | メインヘッダ。UTF-8 → SJIS 変換ロジック |
| `unicode_to_sjis.hpp` | Unicode コードポイント → SJIS 変換テーブル（自動生成） |
| `generate_unicode_to_sjis.py` | `unicode_to_sjis.hpp` を生成する Python スクリプト |
| `test.cpp` | 動作確認用テストコード |
| `CMakeLists.txt` | CMake プロジェクト設定 |
| `cmake/utf8_sjisConfig.cmake.in` | CMake パッケージ設定テンプレート |

## 開発

`unicode_to_sjis.hpp` を再生成する場合:

```sh
python generate_unicode_to_sjis.py > unicode_to_sjis.hpp
```

## ライセンス

このプロジェクトは [0BSD](LICENSE) ライセンスのもとで公開されています。
