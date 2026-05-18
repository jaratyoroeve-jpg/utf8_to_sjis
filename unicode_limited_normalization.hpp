// unicode_limited_normalization.hpp
#pragma once
#include <cstdint>

namespace utf8_sjis {

namespace detail {

/**
 * Unicode の特定のコードポイントを、SJIS で濁点・半濁点が結合された文字に正規化する関数
 * 
 * \param codepoint 正規化対象の Unicode コードポイント
 * \return 正規化された Unicode コードポイント。正規化できない場合は 0x0000 を返す
 */
consteval uint32_t unicode_limited_normalization(uint32_t mark_codepoint, uint32_t base_codepoint)
{
    if (mark_codepoint == 0x3099) {
        // 濁点 (゛)
        switch (base_codepoint)
        {
        case 0x304B: return 0x304C; // か -> が
        case 0x304D: return 0x304E; // き -> ぎ
        case 0x304F: return 0x3050; // く -> ぐ
        case 0x3051: return 0x3052; // け -> げ
        case 0x3053: return 0x3054; // こ -> ご
        case 0x3055: return 0x3056; // さ -> ざ
        case 0x3057: return 0x3058; // し -> じ
        case 0x3059: return 0x305A; // す -> ず
        case 0x305B: return 0x305C; // せ -> ぜ
        case 0x305D: return 0x305E; // そ -> ぞ
        case 0x305F: return 0x3060; // た -> だ
        case 0x3061: return 0x3062; // ち -> ぢ
        case 0x3064: return 0x3065; // つ -> づ
        case 0x3066: return 0x3067; // て -> で
        case 0x3068: return 0x3069; // と -> ど
        case 0x306F: return 0x3070; // は -> ば
        case 0x3072: return 0x3073; // ひ -> び
        case 0x3075: return 0x3076; // ふ -> ぶ
        case 0x3078: return 0x3079; // へ -> べ
        case 0x307B: return 0x307C; // ほ -> ぼ
        case 0x30AB: return 0x30AC; // カ -> ガ
        case 0x30AD: return 0x30AE; // キ -> ギ
        case 0x30AF: return 0x30B0; // ク -> グ
        case 0x30B1: return 0x30B2; // ケ -> ゲ
        case 0x30B3: return 0x30B4; // コ -> ゴ
        case 0x30B5: return 0x30B6; // サ -> ザ
        case 0x30B7: return 0x30B8; // シ -> ジ
        case 0x30B9: return 0x30BA; // ス -> ザ
        case 0x30BB: return 0x30BC; // セ -> ゼ
        case 0x30BD: return 0x30BE; // ソ -> ゾ
        case 0x30BF: return 0x30C0; // タ -> ダ
        case 0x30C1: return 0x30C2; // チ -> ヂ
        case 0x30C4: return 0x30C5; // ツ -> ヅ
        case 0x30C6: return 0x30C7; // テ -> デ
        case 0x30C8: return 0x30C9; // ト -> ド
        case 0x30CF: return 0x30D0; // ハ -> バ
        case 0x30D2: return 0x30D3; // ヒ -> ビ
        case 0x30D5: return 0x30D6; // フ -> ブ
        case 0x30D8: return 0x30D9; // ヘ -> ベ
        case 0x30DB: return 0x30DC; // ホ -> ボ
        case 0x30A6: return 0x30F4; // ウ -> ヴ
        default: return 0x0000; // 正規化できない場合は 0x0000 を返す
        }
    } else if (mark_codepoint == 0x309A) {
        // 半濁点 (゜)
        switch (base_codepoint)
        {
        case 0x306F: return 0x3071; // は -> ぱ
        case 0x3072: return 0x3074; // ひ -> ぴ
        case 0x3075: return 0x3077; // ふ -> ぷ
        case 0x3078: return 0x307A; // へ -> ぺ
        case 0x307B: return 0x307D; // ほ -> ぽ
        case 0x30CF: return 0x30D1; // ハ -> パ
        case 0x30D2: return 0x30D4; // ヒ -> ピ
        case 0x30D5: return 0x30D7; // フ -> プ
        case 0x30D8: return 0x30DA; // ヘ -> ペ
        case 0x30DB: return 0x30DD; // ホ -> ポ
        default: return 0x0000; // 正規化できない場合は 0x0000 を返す
        }
    } else {
        // 濁点・半濁点以外のコードポイントは正規化できない
        return 0x0000;
    }
}

} // namespace detail

} // namespace utf8_sjis