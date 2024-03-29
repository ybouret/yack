#include "yack/hashing/sha256.hpp"

#include <cstring>

namespace yack
{
	
	namespace hashing
	{
		
#define GET_ULONG_BE(n,b,i)                             \
{                                                       \
(n) = (uint32_t) ( \
        ( (unsigned long) (b)[(i)    ] << 24 )          \
|     ( (unsigned long) (b)[(i) + 1] << 16 )            \
|     ( (unsigned long) (b)[(i) + 2] <<  8 )            \
|     ( (unsigned long) (b)[(i) + 3]       ) );         \
}
		
#define PUT_ULONG_BE(n,b,i)                             \
{                                                       \
(b)[(i)    ] = (unsigned char) ( (n) >> 24 );       \
(b)[(i) + 1] = (unsigned char) ( (n) >> 16 );       \
(b)[(i) + 2] = (unsigned char) ( (n) >>  8 );       \
(b)[(i) + 3] = (unsigned char) ( (n)       );       \
}
		
		/*
		 * SHA-256 context setup
		 */
		static
		void sha2_starts( SHA256_CTX *ctx, const int is224 )
		{
			ctx->total[0] = 0;
			ctx->total[1] = 0;
			
			if( is224 == 0 )
			{
				/* SHA-256 */
				ctx->state[0] = 0x6A09E667;
				ctx->state[1] = 0xBB67AE85;
				ctx->state[2] = 0x3C6EF372;
				ctx->state[3] = 0xA54FF53A;
				ctx->state[4] = 0x510E527F;
				ctx->state[5] = 0x9B05688C;
				ctx->state[6] = 0x1F83D9AB;
				ctx->state[7] = 0x5BE0CD19;
			}
			else
			{
				/* SHA-224 */
				ctx->state[0] = 0xC1059ED8;
				ctx->state[1] = 0x367CD507;
				ctx->state[2] = 0x3070DD17;
				ctx->state[3] = 0xF70E5939;
				ctx->state[4] = 0xFFC00B31;
				ctx->state[5] = 0x68581511;
				ctx->state[6] = 0x64F98FA7;
				ctx->state[7] = 0xBEFA4FA4;
			}
			
			ctx->is224 = is224;
		}
		
		static void sha256_set( SHA256_CTX *ctx ) {
			sha2_starts(ctx,0);
		}
		
		static void sha224_set( SHA256_CTX *ctx ) {
			sha2_starts(ctx,1);
		}
		
		
		
		static void sha2_process( SHA256_CTX *ctx, const uint8_t data[64] )
		{
			uint32_t temp1, temp2, W[64];
			uint32_t A, B, C, D, E, F, G, H;
			
			GET_ULONG_BE( W[ 0], data,  0 );
			GET_ULONG_BE( W[ 1], data,  4 );
			GET_ULONG_BE( W[ 2], data,  8 );
			GET_ULONG_BE( W[ 3], data, 12 );
			GET_ULONG_BE( W[ 4], data, 16 );
			GET_ULONG_BE( W[ 5], data, 20 );
			GET_ULONG_BE( W[ 6], data, 24 );
			GET_ULONG_BE( W[ 7], data, 28 );
			GET_ULONG_BE( W[ 8], data, 32 );
			GET_ULONG_BE( W[ 9], data, 36 );
			GET_ULONG_BE( W[10], data, 40 );
			GET_ULONG_BE( W[11], data, 44 );
			GET_ULONG_BE( W[12], data, 48 );
			GET_ULONG_BE( W[13], data, 52 );
			GET_ULONG_BE( W[14], data, 56 );
			GET_ULONG_BE( W[15], data, 60 );
			
#define  SHR(x,n) ((x & 0xFFFFFFFF) >> n)
#define ROTR(x,n) (SHR(x,n) | (x << (32 - n)))
			
#define S0(x) (ROTR(x, 7) ^ ROTR(x,18) ^  SHR(x, 3))
#define S1(x) (ROTR(x,17) ^ ROTR(x,19) ^  SHR(x,10))
			
#define S2(x) (ROTR(x, 2) ^ ROTR(x,13) ^ ROTR(x,22))
#define S3(x) (ROTR(x, 6) ^ ROTR(x,11) ^ ROTR(x,25))
			
#define F0(x,y,z) ((x & y) | (z & (x | y)))
#define F1(x,y,z) (z ^ (x & (y ^ z)))
			
#define R(t)                                    \
(                                               \
W[t] = S1(W[t -  2]) + W[t -  7] +          \
S0(W[t - 15]) + W[t - 16]            \
)
			
#define P(a,b,c,d,e,f,g,h,x,K)                  \
{                                               \
temp1 = h + S3(e) + F1(e,f,g) + K + x;      \
temp2 = S2(a) + F0(a,b,c);                  \
d += temp1; h = temp1 + temp2;              \
}
			
			A = ctx->state[0];
			B = ctx->state[1];
			C = ctx->state[2];
			D = ctx->state[3];
			E = ctx->state[4];
			F = ctx->state[5];
			G = ctx->state[6];
			H = ctx->state[7];
			
			P( A, B, C, D, E, F, G, H, W[ 0], 0x428A2F98 );
			P( H, A, B, C, D, E, F, G, W[ 1], 0x71374491 );
			P( G, H, A, B, C, D, E, F, W[ 2], 0xB5C0FBCF );
			P( F, G, H, A, B, C, D, E, W[ 3], 0xE9B5DBA5 );
			P( E, F, G, H, A, B, C, D, W[ 4], 0x3956C25B );
			P( D, E, F, G, H, A, B, C, W[ 5], 0x59F111F1 );
			P( C, D, E, F, G, H, A, B, W[ 6], 0x923F82A4 );
			P( B, C, D, E, F, G, H, A, W[ 7], 0xAB1C5ED5 );
			P( A, B, C, D, E, F, G, H, W[ 8], 0xD807AA98 );
			P( H, A, B, C, D, E, F, G, W[ 9], 0x12835B01 );
			P( G, H, A, B, C, D, E, F, W[10], 0x243185BE );
			P( F, G, H, A, B, C, D, E, W[11], 0x550C7DC3 );
			P( E, F, G, H, A, B, C, D, W[12], 0x72BE5D74 );
			P( D, E, F, G, H, A, B, C, W[13], 0x80DEB1FE );
			P( C, D, E, F, G, H, A, B, W[14], 0x9BDC06A7 );
			P( B, C, D, E, F, G, H, A, W[15], 0xC19BF174 );
			P( A, B, C, D, E, F, G, H, R(16), 0xE49B69C1 );
			P( H, A, B, C, D, E, F, G, R(17), 0xEFBE4786 );
			P( G, H, A, B, C, D, E, F, R(18), 0x0FC19DC6 );
			P( F, G, H, A, B, C, D, E, R(19), 0x240CA1CC );
			P( E, F, G, H, A, B, C, D, R(20), 0x2DE92C6F );
			P( D, E, F, G, H, A, B, C, R(21), 0x4A7484AA );
			P( C, D, E, F, G, H, A, B, R(22), 0x5CB0A9DC );
			P( B, C, D, E, F, G, H, A, R(23), 0x76F988DA );
			P( A, B, C, D, E, F, G, H, R(24), 0x983E5152 );
			P( H, A, B, C, D, E, F, G, R(25), 0xA831C66D );
			P( G, H, A, B, C, D, E, F, R(26), 0xB00327C8 );
			P( F, G, H, A, B, C, D, E, R(27), 0xBF597FC7 );
			P( E, F, G, H, A, B, C, D, R(28), 0xC6E00BF3 );
			P( D, E, F, G, H, A, B, C, R(29), 0xD5A79147 );
			P( C, D, E, F, G, H, A, B, R(30), 0x06CA6351 );
			P( B, C, D, E, F, G, H, A, R(31), 0x14292967 );
			P( A, B, C, D, E, F, G, H, R(32), 0x27B70A85 );
			P( H, A, B, C, D, E, F, G, R(33), 0x2E1B2138 );
			P( G, H, A, B, C, D, E, F, R(34), 0x4D2C6DFC );
			P( F, G, H, A, B, C, D, E, R(35), 0x53380D13 );
			P( E, F, G, H, A, B, C, D, R(36), 0x650A7354 );
			P( D, E, F, G, H, A, B, C, R(37), 0x766A0ABB );
			P( C, D, E, F, G, H, A, B, R(38), 0x81C2C92E );
			P( B, C, D, E, F, G, H, A, R(39), 0x92722C85 );
			P( A, B, C, D, E, F, G, H, R(40), 0xA2BFE8A1 );
			P( H, A, B, C, D, E, F, G, R(41), 0xA81A664B );
			P( G, H, A, B, C, D, E, F, R(42), 0xC24B8B70 );
			P( F, G, H, A, B, C, D, E, R(43), 0xC76C51A3 );
			P( E, F, G, H, A, B, C, D, R(44), 0xD192E819 );
			P( D, E, F, G, H, A, B, C, R(45), 0xD6990624 );
			P( C, D, E, F, G, H, A, B, R(46), 0xF40E3585 );
			P( B, C, D, E, F, G, H, A, R(47), 0x106AA070 );
			P( A, B, C, D, E, F, G, H, R(48), 0x19A4C116 );
			P( H, A, B, C, D, E, F, G, R(49), 0x1E376C08 );
			P( G, H, A, B, C, D, E, F, R(50), 0x2748774C );
			P( F, G, H, A, B, C, D, E, R(51), 0x34B0BCB5 );
			P( E, F, G, H, A, B, C, D, R(52), 0x391C0CB3 );
			P( D, E, F, G, H, A, B, C, R(53), 0x4ED8AA4A );
			P( C, D, E, F, G, H, A, B, R(54), 0x5B9CCA4F );
			P( B, C, D, E, F, G, H, A, R(55), 0x682E6FF3 );
			P( A, B, C, D, E, F, G, H, R(56), 0x748F82EE );
			P( H, A, B, C, D, E, F, G, R(57), 0x78A5636F );
			P( G, H, A, B, C, D, E, F, R(58), 0x84C87814 );
			P( F, G, H, A, B, C, D, E, R(59), 0x8CC70208 );
			P( E, F, G, H, A, B, C, D, R(60), 0x90BEFFFA );
			P( D, E, F, G, H, A, B, C, R(61), 0xA4506CEB );
			P( C, D, E, F, G, H, A, B, R(62), 0xBEF9A3F7 );
			P( B, C, D, E, F, G, H, A, R(63), 0xC67178F2 );
			
			ctx->state[0] += A;
			ctx->state[1] += B;
			ctx->state[2] += C;
			ctx->state[3] += D;
			ctx->state[4] += E;
			ctx->state[5] += F;
			ctx->state[6] += G;
			ctx->state[7] += H;
		}
		
		/*
		 * SHA-256 process buffer
		 */
		static
		void sha256_run( SHA256_CTX *ctx, const void *buf, const size_t buflen )
		{
			const uint8_t *input = (const uint8_t *)buf;
			size_t         ilen  = buflen;
			uint32_t  fill;
			uint32_t  left;
			
			if( ilen <= 0 )
				return;
			
			left = ctx->total[0] & 0x3F;
			fill = 64 - left;
			
			ctx->total[0] += uint32_t(ilen);
			ctx->total[0] &= 0xFFFFFFFF;
			
			if( ctx->total[0] < ilen )
				ctx->total[1]++;
			
			if( left && ilen >= fill )
			{
				memcpy( (void *) (ctx->buffer + left),
					   (void *) input, fill );
				sha2_process( ctx, ctx->buffer );
				input += fill;
				ilen  -= fill;
				left = 0;
			}
			
			while( ilen >= 64 )
			{
				sha2_process( ctx, input );
				input += 64;
				ilen  -= 64;
			}
			
			if( ilen > 0 )
			{
				memcpy( (void *) (ctx->buffer + left),
					   (void *) input, ilen );
			}
		}
		
		static const uint8_t sha2_padding[64] =
		{
			0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		};
		
		/*
		 * SHA-256 final digest
		 */
		static void sha256_compute( SHA256_CTX *ctx, uint8_t output[32] )
		{
			uint32_t last, padn;
			uint32_t high, low;
			uint8_t  msglen[8];
			
			high = ( ctx->total[0] >> 29 )
			| ( ctx->total[1] <<  3 );
			low  = ( ctx->total[0] <<  3 );
			
			PUT_ULONG_BE( high, msglen, 0 );
			PUT_ULONG_BE( low,  msglen, 4 );
			
			last = ctx->total[0] & 0x3F;
			padn = ( last < 56 ) ? ( 56 - last ) : ( 120 - last );
			
			sha256_run( ctx, sha2_padding, padn );
			sha256_run( ctx, msglen, 8 );
			
			PUT_ULONG_BE( ctx->state[0], output,  0 );
			PUT_ULONG_BE( ctx->state[1], output,  4 );
			PUT_ULONG_BE( ctx->state[2], output,  8 );
			PUT_ULONG_BE( ctx->state[3], output, 12 );
			PUT_ULONG_BE( ctx->state[4], output, 16 );
			PUT_ULONG_BE( ctx->state[5], output, 20 );
			PUT_ULONG_BE( ctx->state[6], output, 24 );
			
			if( ctx->is224 == 0 )
				PUT_ULONG_BE( ctx->state[7], output, 28 );
		}
		
		
		sha256::sha256() noexcept : function(__length, __window), ctx()
		{
		}
		
		const char sha256:: clid[] ="sha256";

		
		void sha256:: set() noexcept
		{
			sha256_set( &ctx );
		}
		
		void sha256:: run( const void *buffer, size_t buflen ) noexcept
		{
			assert( !(buflen>0&&NULL==buffer) );
			sha256_run( &ctx, (const uint8_t *)buffer, buflen );
		}
		
		void sha256::get(void *output, size_t outlen ) noexcept
		{
			assert( !(output==NULL&&outlen>0) );
			uint8_t  digest[32];
			sha256_compute( &ctx, digest);
			fill( output, outlen, digest, sizeof(digest) );
		}
		
		sha256:: ~sha256() noexcept
		{
			memset( &ctx, 0 , sizeof(ctx) );
		}


		
		////////////////////////////////////////////////////////////////////////
		
		sha224::sha224() noexcept : function(__length, __window), ctx()
		{
		}
		
		const char sha224:: clid[] = "sha224";
        
		
		
		void sha224:: set() noexcept
		{
			sha224_set( &ctx );
		}
		
		void sha224:: run( const void *buffer, size_t buflen ) noexcept
		{
			assert( !(buflen>0&&NULL==buffer) );
			sha256_run( &ctx, (const uint8_t *)buffer, buflen );
		}
		
		void sha224::get(void *output, size_t outlen ) noexcept
		{
			assert( !(output==NULL&&outlen>0) );
			uint8_t  digest[28];
			sha256_compute( &ctx, digest);
			fill( output, outlen, digest, sizeof(digest) );
		}
		
		sha224:: ~sha224() noexcept
		{
			memset( &ctx, 0 , sizeof(ctx) );
		}
		
		
	}
	
}
#include "yack/hashing/testing.hpp"


namespace yack
{

    namespace hashing
    {
        static const testing sha256_vec[] =
        {
            { "", "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855" },
            { "The quick brown fox jumps over the lazy dog", "d7a8fbb307d7809469ca9abcb0082e4f8d5651e46d3cdb762d02d0bf37c9e592" }
        };

        void sha256:: tests()
        {
            YACK_HASHING_TEST(sha256,sha256_vec);
        }

        static const testing sha224_vec[] =
        {
            { "", "d14a028c2a3a2bc9476102bb288234c415a2b01f828ea62ac5b3e42f" },
            { "The quick brown fox jumps over the lazy dog",
                "730e109bd7a8a32b1cb9d9a09aa2325d2430587ddbc0c38bad911525"},
            { "The quick brown fox jumps over the lazy dog.",
                "619cba8e8e05826e9b8c519c0a5c68f4fb653e8a3d8aa04bb2c8cd4c"}
        };

        void sha224:: tests()
        {
            YACK_HASHING_TEST(sha224,sha224_vec);
        }


    }
}
