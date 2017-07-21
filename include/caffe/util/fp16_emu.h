/*
 * Copyright 1993-2014 NVIDIA Corporation.  All rights reserved.
 *
 * NOTICE TO LICENSEE:
 *
 * This source code and/or documentation ("Licensed Deliverables") are
 * subject to NVIDIA intellectual property rights under U.S. and
 * international Copyright laws.
 *
 * These Licensed Deliverables contained herein is PROPRIETARY and
 * CONFIDENTIAL to NVIDIA and is being provided under the terms and
 * conditions of a form of NVIDIA software license agreement by and
 * between NVIDIA and Licensee ("License Agreement") or electronically
 * accepted by Licensee.  Notwithstanding any terms or conditions to
 * the contrary in the License Agreement, reproduction or disclosure
 * of the Licensed Deliverables to any third party without the express
 * written consent of NVIDIA is prohibited.
 *
 * NOTWITHSTANDING ANY TERMS OR CONDITIONS TO THE CONTRARY IN THE
 * LICENSE AGREEMENT, NVIDIA MAKES NO REPRESENTATION ABOUT THE
 * SUITABILITY OF THESE LICENSED DELIVERABLES FOR ANY PURPOSE.  IT IS
 * PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY OF ANY KIND.
 * NVIDIA DISCLAIMS ALL WARRANTIES WITH REGARD TO THESE LICENSED
 * DELIVERABLES, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY,
 * NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE.
 * NOTWITHSTANDING ANY TERMS OR CONDITIONS TO THE CONTRARY IN THE
 * LICENSE AGREEMENT, IN NO EVENT SHALL NVIDIA BE LIABLE FOR ANY
 * SPECIAL, INDIRECT, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, OR ANY
 * DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THESE LICENSED DELIVERABLES.
 *
 * U.S. Government End Users.  These Licensed Deliverables are a
 * "commercial item" as that term is defined at 48 C.F.R. 2.101 (OCT
 * 1995), consisting of "commercial computer software" and "commercial
 * computer software documentation" as such terms are used in 48
 * C.F.R. 12.212 (SEPT 1995) and is provided to the U.S. Government
 * only as a commercial end item.  Consistent with 48 C.F.R.12.212 and
 * 48 C.F.R. 227.7202-1 through 227.7202-4 (JUNE 1995), all
 * U.S. Government End Users acquire the Licensed Deliverables with
 * only those rights set forth herein.
 *
 * Any use of the Licensed Deliverables in individual and commercial
 * software must include, in the user documentation and internal
 * comments to the code, the above Disclaimer and U.S. Government End
 * Users Notice.
 */

// Conversion from/to 16-bit floating point (half-precision).
#ifndef INCLUDE_CAFFE_UTIL_FP16_EMU_H_
#define INCLUDE_CAFFE_UTIL_FP16_EMU_H_

#include <driver_types.h>
#include <cuda_fp16.h>

#define HLF_EPSILON  4.887581E-04
#define HLF_MIN      6.103516E-05
#define HLF_MAX      6.550400E+04
#define HLF_TRUE_MIN 5.960464E-08

__inline__ __device__ __host__ __half habs(__half h) {
  h.x &= 0x7fffU;
  return h;
}

__inline__ __device__ __host__ __half hneg(__half h) {
  h.x ^= 0x8000U;
  return h;
}

__inline__ __device__ __host__ int ishnan(__half h) {
  // When input is NaN, exponent is all ones and mantissa is non-zero.
  return (h.x & 0x7c00U) == 0x7c00U && (h.x & 0x03ffU) != 0;
}

__inline__ __device__ __host__ int ishinf(__half h) {
  // When input is +/- inf, exponent is all ones and mantissa is zero.
  return (h.x & 0x7c00U) == 0x7c00U && (h.x & 0x03ffU) == 0;
}

__inline__ __device__ __host__ int ishequ(__half x, __half y) {
  return ishnan(x) == 0 && ishnan(y) == 0 && x.x == y.x;
}

// Returns 0.0000 in FP16 binary form
__inline__ __device__ __host__ __half hzero() {
  __half ret;
  ret.x = 0x0000U;
  return ret;
}

// Returns 1.0000 in FP16 binary form
__inline__ __device__ __host__ __half hone() {
  __half ret;
  ret.x = 0x3c00U;
  return ret;
}

// Returns quiet NaN, the most significant fraction bit #9 is set
__inline__ __device__ __host__ __half hnan() {
  __half ret;
  ret.x = 0x7e00U;
  return ret;
}

// Largest positive FP16 value, corresponds to 6.5504e+04
__inline__ __device__ __host__ __half hmax() {
  __half ret;
  // Exponent all ones except LSB (0x1e), mantissa is all ones (0x3ff)
  ret.x = 0x7bffU;
  return ret;
}

// Smallest positive (normalized) FP16 value, corresponds to 6.1035e-05
__inline__ __device__ __host__ __half hmin() {
  __half ret;
  // Exponent is 0x01 (5 bits), mantissa is all zeros (10 bits)
  ret.x = 0x0400U;
  return ret;
}

#endif  // INCLUDE_CAFFE_UTIL_FP16_EMU_H_
