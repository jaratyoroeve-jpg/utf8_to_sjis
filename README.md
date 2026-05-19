# utf8_sjis

[![License: 0BSD](https://img.shields.io/badge/license-0BSD-blue.svg)](LICENSE)

UTF-8文字列リテラルをShift-JIS バイト列へコンパイル時に変換します。

## ざっくり特徴

- ヘッダーオンリー
- コンパイル時に UTF-8 を Shift-JIS に変換
- 実行時オーバーヘッドなし

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
| `unicode_limited_normalization.hpp` | 濁点・半濁点の正規化 |
| `unicode_to_sjis.hpp` | Unicode コードポイント → SJIS 変換テーブル（自動生成） |
| `generate_unicode_to_sjis.py` | `unicode_to_sjis.hpp` を生成する Python スクリプト |

## ライセンス

このプロジェクトは [0BSD](LICENSE) ライセンスのもとで公開されています。
