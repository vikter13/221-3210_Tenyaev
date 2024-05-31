/*
 *   Copyright(C) 2019 Intel Corporation All Rights Reserved.
 *
 *   The source code, information  and  material ("Material") contained herein is
 *   owned  by Intel Corporation or its suppliers or licensors, and title to such
 *   Material remains  with Intel Corporation  or its suppliers or licensors. The
 *   Material  contains proprietary information  of  Intel or  its  suppliers and
 *   licensors. The  Material is protected by worldwide copyright laws and treaty
 *   provisions. No  part  of  the  Material  may  be  used,  copied, reproduced,
 *   modified, published, uploaded, posted, transmitted, distributed or disclosed
 *   in any way  without Intel's  prior  express written  permission. No  license
 *   under  any patent, copyright  or  other intellectual property rights  in the
 *   Material  is  granted  to  or  conferred  upon  you,  either  expressly,  by
 *   implication, inducement,  estoppel or  otherwise.  Any  license  under  such
 *   intellectual  property  rights must  be express  and  approved  by  Intel in
 *   writing.
 *
 *   *Third Party trademarks are the property of their respective owners.
 *
 *   Unless otherwise  agreed  by Intel  in writing, you may not remove  or alter
 *   this  notice or  any other notice embedded  in Materials by Intel or Intel's
 *   suppliers or licensors in any way.
 *
 */

#ifndef _SGX_SECURE_ALIGN_H_
#define _SGX_SECURE_ALIGN_H_

#include <cstdint>
#include <utility>

namespace sgx {

    template <class T>
    constexpr T __rol(T v, std::size_t c, std::size_t _m)
    {
        return (v << (c & _m)) | (v >> ((0 - c) & _m));
    }

    template <class T>
    constexpr T rol(T v, std::size_t c)
    {
        return __rol(typename std::make_unsigned<T>::type(v), c, sizeof(v) * 8 - 1);
    }

    template <class T>
    constexpr T ror(T v, std::size_t c)
    {
        return rol(v, 0 - c);
    }

    namespace __custom_alignment_internal {

        template <std::int64_t B, std::size_t... OLs>
        struct secret_bmp;

        template <std::int64_t B, std::size_t O, std::size_t L, std::size_t... OLs>
        struct secret_bmp<B, O, L, OLs...> :
            secret_bmp<B | rol((1ll << L) - 1, O), OLs...>
        {};

        template <std::int64_t B>
        struct secret_bmp<B>
        {
            enum : std::int64_t
            {
                value = B
            };
        };

        constexpr std::int64_t __gen_alignmask(
            std::size_t al,
            std::size_t a = sizeof(std::uint64_t) * 8,
            std::uint64_t m = 1ull << (sizeof(std::uint64_t) * 8 - 1))
        {
            return a > al ? __gen_alignmask(al, a >> 1, m | (m >> (a >> 1))) : m;
        }

        /* count leading zero bits */
        template <class T>
        constexpr int count_lzb(T bmp)
        {
            return bmp == 0 ? -1 :
                typename std::make_signed<T>::type(bmp) < 0 ? 0 : count_lzb(bmp << 1) + 1;
        }

        /* calculate leading spaces
         * returns negative value if no viable solution could be found
		*/
        constexpr int calc_lspc(std::size_t al, std::int64_t bmp)
        {
            return (al & (al - 1)) != 0 ? -2 : count_lzb(
                ~(ror(bmp | ror(bmp, 1) | ror(bmp, 2) | ror(bmp, 3), 5) | ror(bmp, 1)) & __gen_alignmask(al));
        }

        constexpr std::size_t __calc_algn(std::size_t size, std::size_t a = sizeof(std::uint64_t) * 8)
        {
            return a > 8 && size <= a / 2 ? __calc_algn(size, a / 2) : a;
        }

        /* calculate alignment for a structure */
        constexpr std::size_t calc_algn(std::size_t al, std::size_t size)
        {
            return al > 64 ? al : __calc_algn(size);
        }

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4324)
#endif

        /*
         * without this pragma, we need a destructor, which
         * would want to pull in C++ lib, which we want to avoid
        */

        template <class T, std::size_t A, int LZ>
        struct alignas(calc_algn(A, sizeof(T) + LZ)) custom_alignment
        {
            static_assert(LZ > 0, "No viable offset");

            char __no_secret_allowed_in_here[LZ];
            T v;

            template <class... Us>
            custom_alignment(Us&&... args) : v(std::forward<Us>(args)...) {}
        };

        template <class T, std::size_t A>
        struct alignas(calc_algn(A, sizeof(T))) custom_alignment<T, A, 0>
        {
            T v;

            template <class... Us>
            custom_alignment(Us&&... args) : v(std::forward<Us>(args)...) {}
        };

#ifdef _MSC_VER
#pragma warning(pop)
#endif


    } /* namespace __custom_alignment_internal */

    template <class T, std::size_t A, std::size_t... OLs>
    using custom_alignment_aligned = __custom_alignment_internal::custom_alignment<T, A,
        __custom_alignment_internal::calc_lspc(A, __custom_alignment_internal::secret_bmp<0, OLs...>::value)>;

    template <class T, std::size_t... OLs>
    using custom_alignment = custom_alignment_aligned<T, alignof(T), OLs...>;

} /* namespace sgx */

#endif