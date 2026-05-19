// utf8_to_sjis.hpp
#pragma once
#if __cplusplus > 201703L // c++20
#include <array>
#include "unicode_to_sjis.hpp"

namespace utf8_sjis {

namespace detail {

/**
 * UTF-8 文字列リテラルを保持するための型.
 *
 * この型はコンパイル時に UTF-8 文字列リテラルを std::array<char, N>
 * に変換するために使用される.
 *
 * \tparam N 文字列リテラルのサイズ (終端含む).
 */
template <std::size_t N>
struct fixed_string {
    char data[N];
    static constexpr std::size_t size = N;
    constexpr fixed_string(const char (&str)[N]) {
        for (std::size_t i = 0; i < N; ++i) {
            data[i] = str[i];
        }
    }
};

/**
 * SJIS 変換の中間結果を保持する構造体.
 *
 * \tparam size バッファサイズ
 */
template <std::size_t capacity>
struct sjis_conversion_buffer {
    std::array<unsigned char, capacity> bytes; ///< 変換結果バッファ
    std::size_t size;                             ///< 有効バイト数
};

/**
 * UTF-8 文字列リテラルを SJIS に変換し、可変長バッファに格納する.
 *
 * \tparam src 変換対象の UTF-8 文字列リテラル
 * \tparam replace_unmappable 変換できない文字を '?' に置換するかどうか
 * \return 変換結果バッファと実際に使われたバイト数
 */
template <fixed_string src, bool replace_unmappable>
consteval auto convert_utf8_to_sjis()
{
    sjis_conversion_buffer<src.size> result{}; // 変換結果バッファ
    result.size = 0;
    std::size_t size = (src.data[src.size - 1] == '\0') ? src.size - 1 : src.size;

    // 不完全または不正な UTF-8 シーケンスを処理するラムダ関数
    auto handle_invalid = [&](std::size_t& pos) {
        if (replace_unmappable)
            result.bytes[result.size++] = 0x3F; // '?'
        pos += 1;
    };
    
    // 必要なバイト数が残っているかを確認するラムダ関数
    auto check_incomplete = [&](std::size_t pos, std::size_t required_bytes) {
        return pos + required_bytes >= size;
    };

    for (std::size_t pos = 0; pos < size;) {
        unsigned char c = src.data[pos];
        if (c <= 0x7F) {
            // ASCII 文字はそのままコピー
            result.bytes[result.size++] = c;
            pos += 1;
            continue;
        }
        uint32_t codepoint = 0;
        if ((c & 0xE0) == 0xC0) {
            // 2 バイトの UTF-8
            if (check_incomplete(pos, 1)) {
                handle_invalid(pos);
                continue;
            }
            codepoint = ((c & 0x1F) << 6) | (src.data[pos + 1] & 0x3F);
            pos += 2;
        } else if ((c & 0xF0) == 0xE0) {
            // 3 バイトの UTF-8
            if (check_incomplete(pos, 2)) {
                handle_invalid(pos);
                continue;
            }
            codepoint = ((c & 0x0F) << 12) |
                        ((src.data[pos + 1] & 0x3F) << 6) |
                        (src.data[pos + 2] & 0x3F);
            pos += 3;
        } else if ((c & 0xF8) == 0xF0) {
            // 4 バイトの UTF-8 (BMP 外の文字)
            if (check_incomplete(pos, 3)) {
                handle_invalid(pos);
                continue;
            }
            if (replace_unmappable)
                result.bytes[result.size++] = 0x3F; // '?'
            pos += 4;
            continue;
        } else {
            // 不正な UTF-8 シーケンス
            handle_invalid(pos);
            continue;
        }
        auto mapped = unicode_to_sjis(codepoint);
        if (replace_unmappable && mapped.num == 0) {
            // 変換できない文字は '?' に置換
            result.bytes[result.size++] = 0x3F; // '?'
            continue;
        }
        for (unsigned char j = 0; j < mapped.num; ++j) {
            result.bytes[result.size++] = mapped.byte[j];
        }
    }
    if (src.data[src.size - 1] == '\0') {
        // 終端文字がある場合はそれもコピー
        result.bytes[result.size++] = '\0';
    }
    return result;
}

/**
 * SJIS バイト列を固定長 std::array に格納して返す.
 *
 * \tparam src 変換対象の UTF-8 文字列リテラル
 * \tparam replace_unmappable 変換できない文字を '?' に置換するかどうか
 * \return 変換後の SJIS バイト配列
 */
template <fixed_string src, bool replace_unmappable>
consteval auto make_sjis_array()
{
    constexpr auto temporary = convert_utf8_to_sjis<src, replace_unmappable>();
    std::array<unsigned char, temporary.size> result{};

    for (std::size_t i = 0; i < temporary.size; i++) {
        result[i] = temporary.bytes[i];
    }

    return result;
}

} // namespace detail

/**
 * UTF-8 文字列リテラルをコンパイル時に SJIS バイト列へ変換する変数テンプレート.
 * 変換できない文字はオプションで '?' に置換可能.
 * \tparam src 変換対象の UTF-8 文字列リテラル
 * \tparam replace_unmappable 変換できない文字を '?' に置換するかどうか (デフォルト: false)
 * \return 変換後の SJIS バイト配列 (std::array<unsigned char, N>)
 * 使用例:
 *   constexpr auto s = utf8_sjis::utf8_to_sjis<"こんにちは">;
 *   constexpr auto s_with_question = utf8_sjis::utf8_to_sjis<"こんにちは🤔", true>;
 *   // s_with_question は '?' に置き換えられたバイト列になる
 */
template <detail::fixed_string src, bool replace_unmappable = false>
constexpr auto utf8_to_sjis = detail::make_sjis_array<src, replace_unmappable>();

} // namespace utf8_sjis
#else
#error This library requires C++20 or later.
#endif
