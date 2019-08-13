// Copyright 2019 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_REGEXP_SPECIAL_CASE_H_
#define V8_REGEXP_SPECIAL_CASE_H_

#ifdef V8_INTL_SUPPORT
#include "unicode/uversion.h"
namespace U_ICU_NAMESPACE {
class UnicodeSet;
}  //  namespace U_ICU_NAMESPACE

namespace v8 {
namespace internal {

// Functions to build special sets of Unicode characters that need special
// handling under "i" mode that cannot use closeOver(USET_CASE_INSENSITIVE).
//
// For the characters in the "ignore set", the process should not treat other
// characters in the result of closeOver(USET_CASE_INSENSITIVE) as case
// equivlant under the ECMA262 RegExp "i" mode because these characters are
// uppercase themselves that no other characters in the set uppercase to.
//
// For the characters in the "special add set", the proecess should add only
// those characters in the result of closeOver(USET_CASE_INSENSITIVE) which is
// not uppercase characters as case equivlant under the ECMA262 RegExp "i" mode
// and also that ONE uppercase character that other non uppercase character
// uppercase into to the set. Other uppercase characters in the result of
// closeOver(USET_CASE_INSENSITIVE) should not be considered because ECMA262
// RegExp "i" mode consider two characters as "case equivlant" if both
// characters uppercase to the same character.
//
// For example, consider the following case equivalent set defined by Unicode
// standard. Notice there are more than one uppercase characters in this set:
//  U+212B Å Angstrom Sign - an uppercase character.
//  U+00C5 Å Latin Capital Letter A with Ring Above - an uppercase character.
//  U+00E5 å Latin Small Letter A with Ring Above - a lowercase character which
//    uppercase to U+00C5.
// In this case equivlant set is a special set and need special handling while
// considering "case equivlant" under the ECMA262 RegExp "i" mode which is
// different than Unicode Standard:
//  * U+212B should be included into the "ignore" set because there are no other
//    characters, under the ECMA262 "i" mode, are considered as "case equivlant"
//    to it because U+212B is itself an uppercase but neither U+00C5 nor U+00E5
//    uppercase to U+212B.
//  * U+00C5 and U+00E5 will both be included into the "special add" set. While
//    calculate the "equivlant set" under ECMA262 "i" mode, the process will
//    add U+00E5, because it is not an uppercase character in the set. The
//    process will also add U+00C5, because it is the uppercase character which
//    other non uppercase character, U+00C5, uppercase into.
//
// For characters not included in "ignore set" and "special add set", the
// process will just use closeOver(USET_CASE_INSENSITIVE) to calcualte, which is
// much faster.
//
// Under Unicode 12.0, there are only 7 characters in the "special add set" and
// 4 characters in "ignore set" so even the special add process is slower, it is
// limited to a small set of cases only.
//
// The implementation of these two function will be generated by calling ICU
// icu::UnicodeSet during the build time into gen/src/regexp/special-case.cc by
// the code in src/regexp/gen-regexp-special-case.cc.
//
// These two function will be used with LazyInstance<> template to generate
// global sharable set to reduce memory usage and speed up performance.

// Function to build and return the Ignore set.
icu::UnicodeSet BuildIgnoreSet();

// Function to build and return the Special Add set.
icu::UnicodeSet BuildSpecialAddSet();

}  // namespace internal
}  // namespace v8

#endif  // V8_INTL_SUPPORT

#endif  // V8_REGEXP_SPECIAL_CASE_H_
