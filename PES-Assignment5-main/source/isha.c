/*
 * isha.c
 *
 * A completely insecure and bad hashing algorithm, based loosely on
 * SHA-1 (which is itself no longer considered a good hashing
 * algorithm)
 *
 * Based on code for sha1 processing from Paul E. Jones, available at
 * https://www.packetizer.com/security/sha1/
 * Edited By: Saloni Shah
 */

#include "isha.h"
#include<string.h>


/*
 * circular shift macro
 */
#define ISHACircularShift(bits,word) \
  (((word) << (bits)) | ((word) >> (32-(bits))))

/*  
 * Processes the next 512 bits of the message stored in the MBlock
 * array.
 *
 * Parameters:
 *   ctx         The ISHAContext (in/out)
 */
static void ISHAProcessMessageBlock(ISHAContext *ctx)
{
  register uint32_t temp;
  register uint32_t A=ctx->MD[0], B=ctx->MD[1], C=ctx->MD[2], D=ctx->MD[3], E=ctx->MD[4];

  for(register unsigned int t = 0; t < 16; t++)                                //combined both for loops
  {
    temp = (ISHACircularShift(5,A) + ((B & C) | ((~B) & D)) + E + (__builtin_bswap32(*(uint32_t *)(&ctx->MBlock[t<<2])))); //elimination of w[t] and computing the chunk in a single line
    E=D;
    D=C;
    C=ISHACircularShift(30,B);
    B=A;
    A=temp;
  }

  ctx->MD[0] += A;
  ctx->MD[1] += B;
  ctx->MD[2] += C;
  ctx->MD[3] += D;
  ctx->MD[4] += E;

  ctx->MB_Idx = 0;
}


/*  
 * The message must be padded to an even 512 bits.  The first padding
 * bit must be a '1'.  The last 64 bits represent the length of the
 * original message.  All bits in between should be 0. This function
 * will pad the message according to those rules by filling the MBlock
 * array accordingly. It will also call ISHAProcessMessageBlock()
 * appropriately. When it returns, it can be assumed that the message
 * digest has been computed.
 *
 * Parameters:
 *   ctx         The ISHAContext (in/out)
 */
static void ISHAPadMessage(ISHAContext *ctx)
{
  /*
   *  Check to see if the current message block is too small to hold
   *  the initial padding bits and length.  If so, we will pad the
   *  block, process it, and then continue padding into a second
   *  block.
   */
	ctx->MBlock[ctx->MB_Idx++] = 0x80;
  if (ctx->MB_Idx > 55)
  {
	  memset(ctx->MBlock+ctx->MB_Idx, 0, 64-ctx->MB_Idx);		//using memset instead of while loop
    ISHAProcessMessageBlock(ctx);
  }
  if(ctx->MB_Idx < 56)
     {
	  memset(ctx->MBlock+ctx->MB_Idx, 0, 64-ctx->MB_Idx);		//using memset instead of while loop
     }

  /*
   *  Store the message length as the last 8 octets
   */
  ctx->MBlock[56] = (ctx->Length_High >> 24);
  ctx->MBlock[57] = (ctx->Length_High >> 16);
  ctx->MBlock[58] = (ctx->Length_High >> 8);
  ctx->MBlock[59] = (ctx->Length_High);
  ctx->MBlock[60] = (ctx->Length_Low >> 24);
  ctx->MBlock[61] = (ctx->Length_Low >> 16);
  ctx->MBlock[62] = (ctx->Length_Low >> 8);
  ctx->MBlock[63] = (ctx->Length_Low);

  ISHAProcessMessageBlock(ctx);
}

void ISHA_loop(ISHAContext *ctx, int i, const uint8_t *message_array, size_t length, \
		const uint8_t *message_array2, size_t length2, uint8_t *digest_out) {		//combining isha reset, isha result and isha input functions in same loop

  ctx->Length_Low  = 0;
  ctx->Length_High = 0;
  ctx->MB_Idx      = 0;

  ctx->MD[0]       = 0x67452301;
  ctx->MD[1]       = 0xEFCDAB89;
  ctx->MD[2]       = 0x98BADCFE;
  ctx->MD[3]       = 0x10325476;
  ctx->MD[4]       = 0xC3D2E1F0;

  ctx->Computed    = 0;
  ctx->Corrupted   = 0;

  ctx->Length_Low += length<<3;						//computing length low outside while loop
  while(length--)							//processing message block for inner isha
  {
    ctx->MBlock[ctx->MB_Idx++] = (*message_array++);
    if (ctx->MB_Idx == 64)
      ISHAProcessMessageBlock(ctx);
  }

	  ctx->Length_Low += length2<<3;
	  while(length2--)						//processing message block for outer isha
	  {
	    ctx->MBlock[ctx->MB_Idx++] = (*message_array2++);
	    if (ctx->MB_Idx == 64)
	      ISHAProcessMessageBlock(ctx);
	  }

  if (!ctx->Computed)
  {
    ISHAPadMessage(ctx);
    ctx->Computed = 1;
  }
  *((uint32_t *)(digest_out)) = __builtin_bswap32(ctx->MD[0]);		//eliminating for loop 
  *((uint32_t *)(digest_out+4)) = __builtin_bswap32(ctx->MD[1]);
  *((uint32_t *)(digest_out+8)) = __builtin_bswap32(ctx->MD[2]);
  *((uint32_t *)(digest_out+12)) = __builtin_bswap32(ctx->MD[3]);
  *((uint32_t *)(digest_out+16)) = __builtin_bswap32(ctx->MD[4]);
}

  void ISHAReset(ISHAContext *ctx)
{
  ctx->Length_Low  = 0;
  ctx->Length_High = 0;
  ctx->MB_Idx      = 0;

  ctx->MD[0]       = 0x67452301;
  ctx->MD[1]       = 0xEFCDAB89;
  ctx->MD[2]       = 0x98BADCFE;
  ctx->MD[3]       = 0x10325476;
  ctx->MD[4]       = 0xC3D2E1F0;

  ctx->Computed    = 0;
  ctx->Corrupted   = 0;
}


void ISHAResult(ISHAContext *ctx, uint8_t *digest_out)
{
  if (!ctx->Computed)
  {
    ISHAPadMessage(ctx);
    ctx->Computed = 1;
  }

  *((uint32_t *)(digest_out)) = __builtin_bswap32(ctx->MD[0]);
  *((uint32_t *)(digest_out+4)) = __builtin_bswap32(ctx->MD[1]);
  *((uint32_t *)(digest_out+8)) = __builtin_bswap32(ctx->MD[2]);
  *((uint32_t *)(digest_out+12)) = __builtin_bswap32(ctx->MD[3]);
  *((uint32_t *)(digest_out+16)) = __builtin_bswap32(ctx->MD[4]);
  return;
}


void ISHAInput(ISHAContext *ctx, const uint8_t *message_array, size_t length)
{
  ctx->Length_Low += length<<3;
  length--;
  do
  {
    ctx->MBlock[ctx->MB_Idx++] = (*message_array++);
    if (ctx->MB_Idx == 64)
      ISHAProcessMessageBlock(ctx);
  }while(length--);
}


