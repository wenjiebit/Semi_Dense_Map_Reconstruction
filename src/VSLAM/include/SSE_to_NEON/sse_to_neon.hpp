// =====================================================================================
// 
//       Filename:  sse2neon.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2016年10月14日 21时58分08秒
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Su Junjie (USTC), jjsu@email.ustc.edu.cn
//        Company:  
// 
// =====================================================================================

#ifndef SSE2NEON_H
#define SSE2NEON_H

//#define ENABLE_CPP_VERSION 0
#define GCC 1

#if GCC
#define INLINE		inline __attribute__((always_inline))
#else
#define INLINE		inline
#endif

#include <arm_neon.h>


typedef float32x4_t __m128;
typedef float32x2_t __m64;
typedef int32x4_t __m128i;

/*  expands to the following value */
#define _MM_SHUFFLE(z, y, x, w)    ( (z<<6) | (y<<4) | (x<<2) | w )




INLINE int32_t _mm_movemask_epi8(__m128i input)
{
    const int8_t __attribute__ ((aligned (16))) xr[8] = {-7,-6,-5,-4,-3,-2,-1,0};
    uint8x8_t mask_and = vdup_n_u8(0x80);
    int8x8_t mask_shift = vld1_s8(xr);

    uint8x8_t lo = vget_low_u8((uint8x16_t)input);
    uint8x8_t hi = vget_high_u8((uint8x16_t)input);

    lo = vand_u8(lo, mask_and);
    lo = vshl_u8(lo, mask_shift);

    hi = vand_u8(hi, mask_and);
    hi = vshl_u8(hi, mask_shift);

    lo = vpadd_u8(lo,lo);
    lo = vpadd_u8(lo,lo);
    lo = vpadd_u8(lo,lo);

    hi = vpadd_u8(hi,hi);
    hi = vpadd_u8(hi,hi);
    hi = vpadd_u8(hi,hi);

    return ((hi[0] << 8) | (lo[0] & 0xFF));
}
/***************************************************************************
 *                max and min
 ***************************************************************************/

INLINE __m128i _mm_max_epu8 (__m128i a, __m128i b)
{
	return (__m128i)vmaxq_u8((uint8x16_t) a, (uint8x16_t) b);
}

INLINE __m128i _mm_max_epi16 (__m128i a, __m128i b)
{
	return (__m128i)vmaxq_s16((int16x8_t) a, (int16x8_t) b);
}

/* Return value
 * A 128-bit parameter that can be defined with the following equations:
 * r0 := (a0 > b0) ? a0 : b0
 * r1 := (a1 > b1) ? a1 : b1
 * r2 := (a2 > b2) ? a2 : b2
 * r3 := (a3 > b3) ? a3 : b3
 * */
INLINE __m128i _mm_max_epi32(__m128i a, __m128i b)
{
	return vmaxq_s32(a,b);
}


/* todo: when the input data contain the NaN. => different behave
	BUT, in actual use, NaN ?
Need MORE tests?
 */
INLINE __m128 _mm_max_ps(__m128 a, __m128 b)
{
	return vmaxq_f32(a, b);
}

INLINE __m128i _mm_min_epu8 (__m128i a, __m128i b)
{
	return (__m128i)vminq_u8((uint8x16_t) a, (uint8x16_t) b);
}

INLINE __m128i _mm_min_epi16(__m128i a, __m128i b)
{
	return (__m128i)vminq_s16((int16x8_t)a, (int16x8_t)b);
}

/* Return value
 * A 128-bit parameter that can be defined with the following equations:
 * r0 := (a0 < b0) ? a0 : b0
 * r1 := (a1 < b1) ? a1 : b1
 * r2 := (a2 < b2) ? a2 : b2
 * r3 := (a3 < b3) ? a3 : b3
 * */
INLINE __m128i _mm_min_epi32(__m128i a, __m128i b)
{
	return vminq_s32(a,b);
}

/* todo: when the input data contain the NaN. => different behave
	BUT, in actual use, NaN ?
Need MORE tests?
 */
INLINE __m128 _mm_min_ps(__m128 a, __m128 b)
{
	return vminq_f32(a, b);
}

/***************************************************************************
 *                add and sub
 ***************************************************************************/
/* Subtracts the 16 unsigned 8-bit integers of b from the 16 unsigned 8-bit integers of a and saturates.
 * r0 := UnsignedSaturate(a0 - b0)
 * r1 := UnsignedSaturate(a1 - b1)
 * ...
 * r15 := UnsignedSaturate(a15 - b15)
 * */
INLINE __m128i _mm_subs_epu8 (__m128i a, __m128i b)
{
	return (__m128i)vqsubq_u8((uint8x16_t) a, (uint8x16_t) b);
}

//add by jio
INLINE __m128i _mm_sub_epi8 (__m128i a, __m128i b)
{
	return (__m128i)vqsubq_s8((int8x16_t) a, (int8x16_t) b);
}

INLINE __m128i _mm_add_epi16(__m128i a, __m128i b)
{
	return (__m128i)vaddq_s16((int16x8_t)a, (int16x8_t)b);
}

INLINE __m128i _mm_sub_epi16 (__m128i a, __m128i b)
{
	return (__m128i)vsubq_s16((int16x8_t) a, (int16x8_t) b);
}

//add by jio
INLINE __m128i _mm_adds_epu8 (__m128i a, __m128i b)
{
	return (__m128i)vqaddq_u8((uint8x16_t) a, (uint8x16_t) b);
}

INLINE __m128i _mm_adds_epu16 (__m128i a, __m128i b)
{
	return (__m128i)vqaddq_u16((uint16x8_t) a, (uint16x8_t) b);
}

INLINE __m128i _mm_subs_epu16 (__m128i a, __m128i b)
{
	return (__m128i)vqsubq_u16((uint16x8_t) a, (uint16x8_t) b);
}

/* Adds the 8 signed 16-bit integers in a to the 8 signed 16-bit integers in b and saturates.
 * r0 := SignedSaturate(a0 + b0)
 * r1 := SignedSaturate(a1 + b1)
 * ...
 * r7 := SignedSaturate(a7 + b7)
 * */
INLINE __m128i _mm_adds_epi16 (__m128i a, __m128i b)
{
	return (__m128i)vqaddq_s16((int16x8_t)a, (int16x8_t)b); 
}

/* Subtracts the 8 signed 16-bit integers of b from the 8 signed 16-bit integers of a and saturates.
 * r0 := SignedSaturate(a0 - b0)
 * r1 := SignedSaturate(a1 - b1)
 * ...
 * r7 := SignedSaturate(a7 - b7)
 * */
INLINE __m128i _mm_subs_epi16 (__m128i a, __m128i b)
{
	return (__m128i)vqsubq_s16((int16x8_t) a, (int16x8_t) b);
}
/* Adds the 4 signed or unsigned 32-bit integers in a to the 4 signed or unsigned 32-bit integers in b.
 * r0 := a0 + b0
 * r1 := a1 + b1
 * r2 := a2 + b2
 * r3 := a3 + b3
 * */
INLINE __m128i _mm_add_epi32(__m128i a, __m128i b)
{
	return vaddq_s32(a, b);
}

/* Subtracts the 4 signed or unsigned 32-bit integers of b from the 4 signed or unsigned 32-bit integers of a.
 * r0 := a0 - b0
 * r1 := a1 - b1
 * r2 := a2 - b2
 * r3 := a3 - b3
 * */
INLINE __m128i _mm_sub_epi32(__m128i a, __m128i b)
{
	return vsubq_s32(a, b);
}

/* Adds the four single-precision, floating-point values of a and b.
 * r0 := a0 + b0
 * r1 := a1 + b1
 * r2 := a2 + b2
 * r3 := a3 + b3
 * */
INLINE __m128 _mm_add_ps(__m128 a, __m128 b)
{
	return vaddq_f32(a, b);
}

/* Subtracts the four single-precision, floating-point values of a and b.
 * r0 := a0 - b0
 * r1 := a1 - b1
 * r2 := a2 - b2
 * r3 := a3 - b3
 * */
INLINE __m128 _mm_sub_ps(__m128 a, __m128 b)
{
	return vsubq_f32(a, b);
}

/* The haddps instruction performs a horizontal add, meaning that adjacent elements in the same operand are added together. Each 128-bit argument is considered as four 32-bit floating-point elements, numbered from 0 to 3, with 3 being the high-order element. The result of the operation on operand a (A3, A2, A1, A0) and operand b (B3, B2, B1, B0) is (B3 + B2, B1 + B0, A3 + A2, A1 + A0).
 * This routine is only available as an intrinsic*/
INLINE __m128 _mm_hadd_ps(__m128 a, __m128 b)
{
	return vcombine_f32(vpadd_f32(vget_low_f32(a), vget_high_f32(a)), vpadd_f32(vget_low_f32(b), vget_high_f32(b)));
}

/***************************************************************************
 *                Multiply
 ***************************************************************************/

/* Multiplies the 8 signed 16-bit integers from a by the 8 signed 16-bit integers from b.
 * r0 := (a0 * b0)[31:16]
 * r1 := (a1 * b1)[31:16]
 * ...
 * r7 := (a7 * b7)[31:16]
 * */
INLINE __m128i _mm_mulhi_epi16(__m128i a, __m128i b)
{
	return (__m128i)vshrq_n_s16(vqdmulhq_s16((int16x8_t)a, (int16x8_t)b), 1);
}

/* Multiplies the 8 signed or unsigned 16-bit integers from a by the 8 signed or unsigned 16-bit integers from b.
 * r0 := (a0 * b0)[15:0]
 * r1 := (a1 * b1)[15:0]
 * ...
 * r7 := (a7 * b7)[15:0]
 * */
INLINE __m128i _mm_mullo_epi16(__m128i a, __m128i b)
{
	return (__m128i)vmulq_s16((int16x8_t)a, (int16x8_t)b);
}

/* Multiplies the four single-precision, floating-point values of a and b.
 * r0 := a0 * b0
 * r1 := a1 * b1
 * r2 := a2 * b2
 * r3 := a3 * b3
 * */
INLINE __m128 _mm_mul_ps(__m128 a, __m128 b)
{
	//todo:
	//NEON:(-2.33512e-28) * (-2.13992e-13)=0
	//SSE: (-2.33512e-28) * (-2.13992e-13)=4.99689e-41
//	return vmulq_f32(a, b);
	__m128 ret;
	ret[0] = a[0]*b[0];
	ret[1] = a[1]*b[1];
	ret[2] = a[2]*b[2];
	ret[3] = a[3]*b[3];
	return ret;
}

/* Multiplies the 8 signed 16-bit integers from a by the 8 signed 16-bit integers from b.
 * __m128i _mm_madd_epi16 (__m128i a, __m128i b);
 * PMADDWD
 * Return Value
 * Adds the signed 32-bit integer results pairwise and packs the 4 signed 32-bit integer results.
 * r0 := (a0 * b0) + (a1 * b1)
 * r1 := (a2 * b2) + (a3 * b3)
 * r2 := (a4 * b4) + (a5 * b5)
 * r3 := (a6 * b6) + (a7 * b7)
 * */
INLINE __m128i _mm_madd_epi16 (__m128i a, __m128i b)
{
	int32x4_t r_l = vmull_s16(vget_low_s16((int16x8_t)a), vget_low_s16((int16x8_t)b));
	int32x4_t r_h = vmull_s16(vget_high_s16((int16x8_t)a), vget_high_s16((int16x8_t)b));
	return vcombine_s32(vpadd_s32(vget_low_s32(r_l), vget_high_s32(r_l)), vpadd_s32(vget_low_s32(r_h), vget_high_s32(r_h)));
}

/***************************************************************************
 *                absdiff
 ***************************************************************************/
//#define _mm_absdiff_epu16(a,b) _mm_adds_epu16(_mm_subs_epu16(a, b), _mm_subs_epu16(b, a))

/* Computes the absolute difference of the 16 unsigned 8-bit integers from a and the 16 unsigned 8-bit integers from b.
 * __m128i _mm_sad_epu8 (__m128i a, __m128i b);
 * PSADBW
 * Return Value
 * Sums the upper 8 differences and lower 8 differences and packs the resulting 2 unsigned 16-bit integers into the upper and lower 64-bit elements.
 * r0 := abs(a0 - b0) + abs(a1 - b1) +...+ abs(a7 - b7)
 * r1 := 0x0 ; r2 := 0x0 ; r3 := 0x0
 * r4 := abs(a8 - b8) + abs(a9 - b9) +...+ abs(a15 - b15)
 * r5 := 0x0 ; r6 := 0x0 ; r7 := 0x0
 * */
INLINE __m128i _mm_sad_epu8 (__m128i a, __m128i b)
{
	uint16x8_t t = vpaddlq_u8(vabdq_u8((uint8x16_t) a, (uint8x16_t) b));
	uint16_t r0 = t[0]+t[1]+t[2]+t[3];
	uint16_t r4 = t[4]+t[5]+t[6]+t[7];
	uint16x8_t r = vsetq_lane_u16(r0,vdupq_n_u16(0),0);
	return (__m128i)vsetq_lane_u16(r4,r,4);
}
/***************************************************************************
 *                divides
 ***************************************************************************/
/* r0 := a0 / b0
 * r1 := a1 / b1
 * r2 := a2 / b2
 * r3 := a3 / b3
 * */
INLINE __m128 _mm_div_ps(__m128 a, __m128 b )
{
	// get an initial estimate of 1/b.
	float32x4_t reciprocal = vrecpeq_f32(b);

	// use a couple Newton-Raphson steps to refine the estimate.  Depending on your
	// application's accuracy requirements, you may be able to get away with only
	// one refinement (instead of the two used here).  Be sure to test!
	reciprocal = vmulq_f32(vrecpsq_f32(b, reciprocal), reciprocal);
//	reciprocal = vmulq_f32(vrecpsq_f32(b, reciprocal), reciprocal);

	// and finally, compute a/b = a*(1/b)
	float32x4_t result = vmulq_f32(a,reciprocal);
	return result;
}


/* Computes the square roots of the four single-precision, floating-point values of a.
 * r0 := sqrt(a0)
 * r1 := sqrt(a1)
 * r2 := sqrt(a2)
 * r3 := sqrt(a3)
 * */
INLINE __m128 _mm_sqrt_ps(__m128 in)
{
	__m128 recipsq = vrsqrteq_f32(in);
	__m128 sq = vrecpeq_f32(recipsq);
	// ??? use step versions of both sqrt and recip for better accuracy?
	//precision loss
//	__m128 recipsq = vrsqrtsq_f32(in,vdupq_n_f32(1.0));
//	__m128 sq = vrecpsq_f32(recipsq,vdupq_n_f32(1.0));
	return sq;
}
/***************************************************************************
 *                logic
 ***************************************************************************/

/* Computes the bitwise OR of the 128-bit value in a and the 128-bit value in b.
 * r := a | b
 * */
INLINE __m128i _mm_or_si128(__m128i a, __m128i b)
{
	return (__m128i)vorrq_s32(a, b);
}

/* Computes the bitwise XOR of the 128-bit value in a and the 128-bit value in b. 
 * r := a ^ b
 * */
INLINE __m128i _mm_xor_si128(__m128i a, __m128i b)
{
	return veorq_s32(a, b);
}

/* Computes the bitwise AND of the 128-bit value in b and the bitwise NOT of the 128-bit value in a. 
 * r := (~a) & b
 * */
INLINE __m128i _mm_andnot_si128(__m128i a, __m128i b)
{
	return vbicq_s32(b, a); // *NOTE* argument swap
}

/* Computes the bitwise AND-NOT of the four single-precision, floating-point values of a and b.
 * r0 := ~a0 & b0
 * r1 := ~a1 & b1
 * r2 := ~a2 & b2
 * r3 := ~a3 & b3
 * */
INLINE __m128 _mm_andnot_ps(__m128 a, __m128 b)
{
	return (__m128)vbicq_s32((__m128i)b, (__m128i)a); // *NOTE* argument swap
}

/* Computes the bitwise AND of the 128-bit value in a and the 128-bit value in b.
 * r := a & b
 * */
INLINE __m128i _mm_and_si128(__m128i a, __m128i b)
{
	return vandq_s32(a, b);
}

/* Computes the bitwise AND of the four single-precision, floating-point values of a and b.
 * r0 := a0 & b0
 * r1 := a1 & b1
 * r2 := a2 & b2
 * r3 := a3 & b3
 * */
INLINE __m128 _mm_and_ps(__m128 a, __m128 b)
{
	return (__m128)vandq_s32((__m128i)a, (__m128i)b);
}

/* Compares the 16 signed 8-bit integers in a and the 16 signed 8-bit integers in b for greater than. 
 * r0 := (a0 > b0) ? 0xff : 0x0
 * r1 := (a1 > b1) ? 0xff : 0x0
 * ...
 * r15 := (a15 > b15) ? 0xff : 0x0
 */
INLINE __m128i _mm_cmpgt_epi8 (__m128i a, __m128i b)
{
	return (__m128i)vcgtq_s8((int8x16_t) a,( int8x16_t) b);
}

/* Compares the 8 signed 16-bit integers in a and the 8 signed 16-bit integers in b for greater than.
 * r0 := (a0 > b0) ? 0xffff : 0x0
 * r1 := (a1 > b1) ? 0xffff : 0x0
 * ...
 * r7 := (a7 > b7) ? 0xffff : 0x0
 * */
INLINE __m128i _mm_cmpgt_epi16 (__m128i a, __m128i b)
{
	return (__m128i)vcgtq_s16((int16x8_t) a,( int16x8_t) b);
}

/* Compares for greater than.
 * r0 := (a0 > b0) ? 0xffffffff : 0x0
 * r1 := (a1 > b1) ? 0xffffffff : 0x0
 * r2 := (a2 > b2) ? 0xffffffff : 0x0
 * r3 := (a3 > b3) ? 0xffffffff : 0x0
 * */
INLINE __m128 _mm_cmpgt_ps(__m128 a, __m128 b)
{
	return (__m128)vcgtq_f32(a, b);
}

/* Compares for less than or equal.
 * r0 := (a0 <= b0) ? 0xffffffff : 0x0
 * r1 := (a1 <= b1) ? 0xffffffff : 0x0
 * r2 := (a2 <= b2) ? 0xffffffff : 0x0
 * r3 := (a3 <= b3) ? 0xffffffff : 0x0
 * */
INLINE __m128 _mm_cmple_ps(__m128 a, __m128 b)
{
	return (__m128)vcleq_f32(a, b);
}
/***************************************************************************
 *                load and store
 ***************************************************************************/

INLINE __m128i _mm_load_si128(const __m128i *p)
{
	return vld1q_s32((int32_t *)p);
}
INLINE void _mm_store_si128(__m128i *p, __m128i a ) 
{
	vst1q_s32((int32_t*) p,a);
}

INLINE __m128 _mm_load_ps(const float * p)
{
	return vld1q_f32(p);
}

INLINE void _mm_store_ps(float *p, __m128 a)
{
	vst1q_f32(p, a);
}

#define _mm_loadu_si128 _mm_load_si128
#define _mm_storeu_si128 _mm_store_si128

#define _mm_loadu_ps _mm_load_ps
#define _mm_storeu_ps _mm_store_ps

INLINE __m128i _mm_loadl_epi64(__m128i const*p)
{
	/* Load the lower 64 bits of the value pointed to by p into the lower 64 bits of the result, zeroing the upper 64 bits of the result. */
	return vcombine_s32(vld1_s32((int32_t const *)p),vcreate_s32(0));
}

INLINE void _mm_storel_epi64(__m128i* a, __m128i b)
{
	/* Reads the lower 64 bits of b and stores them into the lower 64 bits of a. */
	//
	//*a = (__m128i)vsetq_lane_s64((int64_t)vget_low_s32(b), *(int64x2_t*)a, 0);
	//vst1_s64( (int64_t *) a, vget_low_s64((int64x2_t)b));
	vst1_s32( (int32_t *) a, vget_low_s32((int32x4_t)b));
}

/* Sets the lower two single-precision, floating-point values with 64 bits of data loaded from the address p; the upper two values are passed through from a.
 * __m128 _mm_loadl_pi( __m128 a , __m64 * p );
 * MOVLPS reg, mem
 * Return Value
 *  r0 := *p0
 *  r1 := *p1
 *  r2 := a2
 *  r3 := a3
 *  */
INLINE __m128 _mm_loadl_pi( __m128 a , __m64 const * p )
{
	return vcombine_f32(vld1_f32((float32_t const *)p),vget_high_f32(a));
}

/* Stores the lower two single-precision, floating-point values of a to the address p.
 * *p0 := b0
 * *p1 := b1
 * */
INLINE void _mm_storel_pi( __m64 * p , __m128 a )
{
	vst1_f32((float32_t *)p, vget_low_f32((float32x4_t)a));
}

INLINE __m128 _mm_load_ss(const float * p)
{
	/* Loads an single-precision, floating-point value into the low word and clears the upper three words. */
	__m128 result = vdupq_n_f32(0);
	return vsetq_lane_f32(*p, result, 0);
}

INLINE void _mm_store_ss(float *p, __m128 a)
{
	/* Stores the lower single-precision, floating-point value. */
	vst1q_lane_f32(p, a, 0);
}
/***************************************************************************
 *                SET 
 ***************************************************************************/
/* Moves 32-bit integer a to the least significant 32 bits of an __m128 object, zero extending the upper bits.
 * r0 := a
 * r1 := 0x0 ; r2 := 0x0 ; r3 := 0x0
 * */
INLINE __m128i _mm_cvtsi32_si128(int a)
{
	__m128i result = vdupq_n_s32(0);
	return vsetq_lane_s32(a, result, 0);
}

/* Sets the 16 signed 8-bit integer values to b.
 * r0 := b
 * r1 := b
 * ...
 * r15 := b
 * */
INLINE __m128i _mm_set1_epi8 (char b)
{
	return (__m128i)vdupq_n_s8((int8_t)b);
}

/* Sets the 8 signed 16-bit integer values to w.
 * r0 := w
 * r1 := w
 * ...
 * r7 := w
 * */
INLINE __m128i _mm_set1_epi16 (short w)
{
	return (__m128i)vdupq_n_s16((int16_t)w);
}

/* Sets the 4 signed 32-bit integer values to i.
 * r0 := i
 * r1 := i
 * r2 := i
 * r3 := I
 * */
INLINE __m128i _mm_set1_epi32(int i)
{
	return vdupq_n_s32(i);
}
/* Sets the four single-precision, floating-point values to w
 * r0 := r1 := r2 := r3 := w 
 * */
INLINE __m128 _mm_set1_ps(float w)
{
	return vdupq_n_f32(w);
}
#define _mm_set_ps1 _mm_set1_ps

INLINE __m128 _mm_set_ps(float w, float z, float y, float x)
{
	float __attribute__((aligned(16))) data[4] = {x,y,z,w};
	return vld1q_f32(data);
}

/* Sets the 8 signed 16-bit integer values in reverse order.
 * __m128i _mm_setr_epi16 (short w0, short w1,    short w2, short w3,   short w4, short w5,   short w6, short w7);
 * (composite)
 * Return Value
 *  r0 := w0
 *  r1 := w1
 *  ...
 *  r7 := w7
 *  */
INLINE __m128i _mm_setr_epi16 (short w0, short w1,    short w2, short w3,   short w4, short w5,   short w6, short w7)
{
	short __attribute__ ((aligned (16))) data[8] = { w0, w1, w2, w3, w4, w5, w6, w7 };
	return (__m128i)vld1q_s16((int16_t*)data);
}

//todo ~~~~~~~~~~~~~~~~~~~~~~~~~
/* Snhuffles the 4 signed or unsigned 32-bit integers in a as specified by imm. */
INLINE __m128i _mm_shuffle_epi32 (__m128i a, int imm)
{
	switch (imm)
	{
		case 0 : 
			return vdupq_n_s32(vgetq_lane_s32(a, 0)); 
			break;
		default: 

			__m128i ret;
			ret[0] = a[imm & 0x3];
			ret[1] = a[(imm >> 2) & 0x3];
			ret[2] = a[(imm >> 4) & 0x03];
			ret[3] = a[(imm >> 6) & 0x03];
			return ret;
	}
}

//todo ~~~~~~~~~~~~~~~~~~~~~~~~~
/* Selects four specific single-precision, floating-point values from a and b, based on the mask i. */
INLINE __m128 _mm_shuffle_ps(__m128 a , __m128 b , int i )
{
	switch (i)
	{
//		case 0 : 
//			return 
//			break;
		default: 
			__m128 ret;                  
			ret[0] = a[i & 0x3];         
			ret[1] = a[(i >> 2) & 0x3];  
			ret[2] = b[(i >> 4) & 0x03]; 
			ret[3] = b[(i >> 6) & 0x03]; 
			return ret;
	}
}
/***************************************************************************
 *                GET 
 ***************************************************************************/
INLINE int _mm_cvtsi128_si32(__m128i a)
{
	/* Moves the least significant 32 bits of a to a 32-bit integer. */
	return vgetq_lane_s32(a, 0);
}

/* Sets the 128-bit value to zero.
 * r := 0x0
 * */
INLINE __m128i _mm_setzero_si128()
{
	return vdupq_n_s32(0);
}

/* Clears the four single-precision, floating-point values.
 * r0 := r1 := r2 := r3 := 0.0 
 * */
INLINE __m128 _mm_setzero_ps(void)
{
	return vdupq_n_f32(0);
}
/***************************************************************************
 *                convert 
 ***************************************************************************/

/* Packs the 8 signed 32-bit integers from a and b into signed 16-bit integers and saturates.
 * r0 := SignedSaturate(a0)
 * r1 := SignedSaturate(a1)
 * r2 := SignedSaturate(a2)
 * r3 := SignedSaturate(a3)
 * r4 := SignedSaturate(b0)
 * r5 := SignedSaturate(b1)
 * r6 := SignedSaturate(b2)
 * r7 := SignedSaturate(b3)
 * */
INLINE __m128i _mm_packs_epi32(__m128i a, __m128i b)
{
	return (__m128i)vcombine_s16(vqmovn_s32(a), vqmovn_s32(b));
}

/* Interleaves the upper 8 signed or unsigned 8-bit integers in a with the upper 8 signed or unsigned 8-bit integers in b.
 * r0 := a8 ; r1 := b8
 * r2 := a9 ; r3 := b9
 * ...
 * r14 := a15 ; r15 := b15
 * */
INLINE __m128i _mm_unpackhi_epi8(__m128i a, __m128i b)
{
	int8x8_t a_h = vget_high_s8((int8x16_t)a);
	int8x8_t b_h = vget_high_s8((int8x16_t)b);
	int8x8x2_t r = vzip_s8(a_h, b_h);
	return (__m128i)vcombine_s8(r.val[0],r.val[1]);
}

/* Interleaves the lower 8 signed or unsigned 8-bit integers in a with the lower 8 signed or unsigned 8-bit integers in b.
 * r0 := a0 ; r1 := b0
 * r2 := a1 ; r3 := b1
 * ...
 * r14 := a7 ; r15 := b7
 * */
INLINE __m128i _mm_unpacklo_epi8(__m128i a, __m128i b)
{
	int8x8_t a_l = vget_low_s8((int8x16_t)a);
	int8x8_t b_l = vget_low_s8((int8x16_t)b);
	int8x8x2_t r = vzip_s8(a_l, b_l);
	return (__m128i)vcombine_s8(r.val[0],r.val[1]);
}

/* Interleaves the lower 2 signed or unsigned 32-bit integers in a with the lower 2 signed or unsigned 32-bit integers in b.
 * r0 := a0 ; r1 := b0
 * r2 := a1 ; r3 := b1
 * */
INLINE __m128i _mm_unpacklo_epi32(__m128i a, __m128i b)
{
	int32x2_t a_l = vget_low_s32((int32x4_t)a);
	int32x2_t b_l = vget_low_s32((int32x4_t)b);
	int32x2x2_t r = vzip_s32(a_l, b_l);
	return (__m128i)vcombine_s32(r.val[0],r.val[1]);
}

INLINE __m128i _mm_unpacklo_epi64(__m128i a, __m128i b)
{
	int64x1_t a_l = vget_low_s64((int64x2_t)a);
	int64x1_t b_l = vget_low_s64((int64x2_t)b);
	return (__m128i)vcombine_s64(a_l, b_l);
}

/* Interleaves the upper signed or unsigned 64-bit integer in a with the upper signed or unsigned 64-bit integer in b.
 * __m128i _mm_unpackhi_epi64 (__m128i a, __m128i b);
 * PUNPCKHQDQ
 * Return Value
 *  r0 := a1 ; r1 := b1
 *  */
INLINE __m128i _mm_unpackhi_epi64(__m128i a, __m128i b)
{
	int64x1_t a_h = vget_high_s64((int64x2_t)a);
	int64x1_t b_h = vget_high_s64((int64x2_t)b);
	return (__m128i)vcombine_s64(a_h, b_h);
}

/* Interleaves the upper 4 signed or unsigned 16-bit integers in a with the upper 4 signed or unsigned 16-bit integers in b.
 * r0 := a4 ; r1 := b4
 * r2 := a5 ; r3 := b5
 * r4 := a6 ; r5 := b6
 * r6 := a7 ; r7 := b7
 * */
INLINE __m128i _mm_unpackhi_epi16(__m128i a, __m128i b)
{
	int16x4_t a_h = vget_high_s16((int16x8_t)a);
	int16x4_t b_h = vget_high_s16((int16x8_t)b);
	int16x4x2_t result = vzip_s16(a_h, b_h);
	return (__m128i)vcombine_s16(result.val[0], result.val[1]);
}

/* Interleaves the lower 4 signed or unsigned 16-bit integers in a with the lower 4 signed or unsigned 16-bit integers in b.
 * r0 := a0 ; r1 := b0
 * r2 := a1 ; r3 := b1
 * r4 := a2 ; r5 := b2
 * r6 := a3 ; r7 := b3
 * */
INLINE __m128i _mm_unpacklo_epi16(__m128i a, __m128i b)
{
	int16x4_t a_l = vget_low_s16((int16x8_t)a);
	int16x4_t b_l = vget_low_s16((int16x8_t)b);
	int16x4x2_t result = vzip_s16(a_l, b_l);
	return (__m128i)vcombine_s16(result.val[0], result.val[1]);
}

INLINE __m128i _mm_unpackhi_epi32(__m128i a, __m128i b)
{
	int32x2_t a1 = vget_high_s32(a);
	int32x2_t b1 = vget_high_s32(b);

	int32x2x2_t result = vzip_s32(a1, b1);

	return vcombine_s32(result.val[0], result.val[1]);
}

/* Selects and interleaves the lower two single-precision, floating-point values from a and b.
 * r0 := a0
 * r1 := b0
 * r2 := a1
 * r3 := b1
 * */
INLINE __m128 _mm_unpacklo_ps(__m128 a, __m128 b)
{
	float32x2x2_t result = vzip_f32(vget_low_f32(a), vget_low_f32(b));
	return vcombine_f32(result.val[0], result.val[1]);
}

/* Selects and interleaves the upper two single-precision, floating-point values from a and b.
 * r0 := a2
 * r1 := b2
 * r2 := a3
 * r3 := b3
 * */
INLINE __m128 _mm_unpackhi_ps(__m128 a, __m128 b)
{
	float32x2x2_t result = vzip_f32(vget_high_f32(a), vget_high_f32(b));
	return vcombine_f32(result.val[0], result.val[1]);
}

INLINE __m128 _mm_cvtepi32_ps(__m128i a)
{
	return vcvtq_f32_s32(a);
}
/* Converts the four single-precision, floating-point values of a to signed 32-bit integer values.
 * r0 := (int) a0
 * r1 := (int) a1
 * r2 := (int) a2
 * r3 := (int) a3
 * */
INLINE __m128i _mm_cvtps_epi32(__m128 a)
{
	//todo:precision
	//NaN -0
	//
	return vcvtq_s32_f32(a);
//	__m128i ret;
//	ret[0] = (int64_t)a[0];
//	ret[1] = (int64_t)a[1];
//	ret[2] = (int64_t)a[2];
//	ret[3] = (int64_t)a[3];
//	return ret;
}

/* Packs the 16 signed 16-bit integers from a and b into 8-bit unsigned integers and saturates.
 *
 * r0 := UnsignedSaturate(a0)
 * r1 := UnsignedSaturate(a1)
 * ...
 * r7 := UnsignedSaturate(a7)
 * r8 := UnsignedSaturate(b0)
 * r9 := UnsignedSaturate(b1)
 * ...
 * r15 := UnsignedSaturate(b7)
 * */
INLINE __m128i _mm_packus_epi16(const __m128i a, const __m128i b)
{
	return (__m128i)vcombine_u8(vqmovun_s16((int16x8_t)a), vqmovun_s16((int16x8_t)b));
}

/* Moves the lower two single-precision, floating-point values of b to the upper two single-precision, floating-point values of the result. 
 * r3 := b1
 * r2 := b0
 * r1 := a1
 * r0 := a0
 * */
INLINE __m128 _mm_movelh_ps( __m128 a, __m128 b )
{
	return vcombine_f32(vget_low_f32(a),vget_low_f32(b));
}

/* The upper two single-precision, floating-point values of a are passed through to the result.
 * r3 := a3
 * r2 := a2
 * r1 := b3
 * r0 := b2
 * */
INLINE __m128 _mm_movehl_ps( __m128 a, __m128 b )
{
	return vcombine_f32(vget_high_f32(b),vget_high_f32(a));
}

/***************************************************************************
 *                shift 
 ***************************************************************************/
/* Shifts the 4 signed 32-bit integers in a right by count bits while shifting in the sign bit.
* r0 := a0 >> count
* r1 := a1 >> count
* r2 := a2 >> count
* r3 := a3 >> count
* immediate ,use  #define _mm_srai_epi32(a, imm) vshrq_n_s32(a, imm)
* */
INLINE __m128i _mm_srai_epi32 (__m128i a, int count)
{
//	return vshrq_n_s32(a, count);
//	todo :
//	if immediate
	return vshlq_s32(a, vdupq_n_s32(-count));
}

/* Shifts the 8 signed 16-bit integers in a right by count bits while shifting in the sign bit.
 *  r0 := a0 >> count
 *  r1 := a1 >> count
 *  ...
 *  r7 := a7 >> count
 *  */
INLINE __m128i _mm_srai_epi16 (__m128i a, int count)
{
//	return vshrq_n_s16(a, count);
//	todo :
//	if immediate
	return (__m128i)vshlq_s16((int16x8_t)a, vdupq_n_s16(-count));
}

/* Shifts the 8 signed or unsigned 16-bit integers in a left by count bits while shifting in zeros.
 * r0 := a0 << count
 * r1 := a1 << count
 * ...
 * r7 := a7 << count
 * */
INLINE __m128i _mm_slli_epi16(__m128i a, int count)
{
//	todo :
//	if immediate
	return (__m128i)vshlq_s16((int16x8_t)a, vdupq_n_s16(count));
}

/* Shifts the 8 signed or unsigned 16-bit integers in a right by count bits while shifting in zeros.
 * r0 := srl(a0, count)
 * r1 := srl(a1, count)
 * ...
 * r7 := srl(a7, count)
 * */
INLINE __m128i _mm_srli_epi16(__m128i a, int count)
{
//	todo :
//	if immediate
	return (__m128i)vshlq_u16((uint16x8_t)a, vdupq_n_s16(-count));
}

/* Shifts the 128-bit value in a right by imm bytes while shifting in zeros. imm must be an immediate.
 * r := srl(a, imm*8)
 * */
#define _mm_srli_si128( a, imm ) (__m128i)vextq_s8((int8x16_t)a, vdupq_n_s8(0), (imm))

/* Shifts the 128-bit value in a left by imm bytes while shifting in zeros. imm must be an immediate.
 * r := a << (imm * 8)*/
//todo ::imm =0, compile error
#define _mm_slli_si128( a, imm ) (__m128i)vextq_s8(vdupq_n_s8(0), (int8x16_t)a, 16 - (imm))

#endif
