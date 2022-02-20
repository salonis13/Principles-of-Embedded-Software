/*
 * pbkdf2.c
 *
 * A perfectly legitimate implementation of HMAC and PBKDF2, but based
 * on the "ISHA" insecure and bad hashing algorithm.
 * 
 * Author: Howdy Pierce, howdy.pierce@colorado.edu
 * Edited By: Saloni Shah
 */
#include<stdio.h>
#include<string.h>
//#include <assert.h>

#include "pbkdf2.h"

/*
 * See function description in pbkdf2.h
 */
void hmac_isha(const uint8_t *key, size_t key_len,
    const uint8_t *msg, size_t msg_len,
    uint8_t *digest)
{
  uint8_t ipad[ISHA_BLOCKLEN];
  uint8_t opad[ISHA_BLOCKLEN];
  uint8_t keypad[ISHA_BLOCKLEN];
  uint8_t inner_digest[ISHA_DIGESTLEN];
  register unsigned int i=0;					//using register keyword for faster variable access
  ISHAContext ctx;

	 memcpy(keypad, key, key_len);					//using memcpy and memset instead of for loop
	 memset(keypad+key_len, 0x00, ISHA_BLOCKLEN-key_len);
	 memset(ipad, 0x36, ISHA_BLOCKLEN);
	 memset(opad, 0x5c, ISHA_BLOCKLEN);

  for(i=0; i<key_len; i+=2) {
  	  ipad[i] = keypad[i] ^ 0x36;
  	  opad[i] = keypad[i] ^ 0x5c;
  	  ipad[i+1] = keypad[i+1] ^ 0x36;
  	  opad[i+1] = keypad[i+1] ^ 0x5c;
  }
  // Perform inner ISHA
  ISHA_loop(&ctx, 2, ipad, ISHA_BLOCKLEN, msg, msg_len, inner_digest);		//implementing combined isha loop

  // perform outer ISHA
  ISHA_loop(&ctx, 2, opad, ISHA_BLOCKLEN, inner_digest, ISHA_DIGESTLEN, digest);
}


/*
 * Implements the F function as defined in RFC 8018 section 5.2
 *
 * Parameters:
 *   pass      The password
 *   pass_len  length of pass
 *   salt      The salt
 *   salt_len  length of salt
 *   iter      The iteration count ("c" in RFC 8018)
 *   blkidx    the block index ("i" in RFC 8018)
 *   result    The result, which is ISHA_DIGESTLEN bytes long
 *
 * Returns:
 *   The result of computing the F function, in result
 */
static void F(const uint8_t *pass, size_t pass_len,
    const uint8_t *salt, size_t salt_len,
    int iter, unsigned int blkidx, uint8_t *result)
{
  uint8_t temp[ISHA_DIGESTLEN];
  uint8_t saltplus[16];
  register unsigned int i;

  for (i=0; i<salt_len; ++i)
    saltplus[i] = salt[i];

  // append blkidx in 4 bytes big endian 
  saltplus[i] = blkidx>>24;
  saltplus[i+1] = blkidx>>16;
  saltplus[i+2] = blkidx>>8;
  saltplus[i+3] = blkidx;

  hmac_isha(pass, pass_len, saltplus, salt_len+4, temp);
  memcpy(result, temp, ISHA_DIGESTLEN);				//using memcpy instead of for loop

  for (register unsigned int j=1; j<iter; ++j) {
    hmac_isha(pass, pass_len, temp, ISHA_DIGESTLEN, temp);

      result[0] ^= temp[0];					//unrolling for loop
      result[1] ^= temp[1];
      result[2] ^= temp[2];
      result[3] ^= temp[3];
      result[4] ^= temp[4];
      result[5] ^= temp[5];
      result[6] ^= temp[6];
      result[7] ^= temp[7];
      result[8] ^= temp[8];
      result[9] ^= temp[9];
      result[10] ^= temp[10];
      result[11] ^= temp[11];
      result[12] ^= temp[12];
      result[13] ^= temp[13];
      result[14] ^= temp[14];
      result[15] ^= temp[15];
      result[16] ^= temp[16];
      result[17] ^= temp[17];
      result[18] ^= temp[18];
      result[19] ^= temp[19];
  }
}


/*
 * See function description in pbkdf2.h
 */
void pbkdf2_hmac_isha(const uint8_t *pass, size_t pass_len,
    const uint8_t *salt, size_t salt_len, int iter, size_t dkLen, uint8_t *DK)
{
  uint8_t accumulator[512];

  register unsigned int l = dkLen / ISHA_DIGESTLEN + 1;
  for (register int i=0; i<l; ++i) {
    F(pass, pass_len, salt, salt_len, iter, i+1, accumulator + i*ISHA_DIGESTLEN);
  }
  memcpy(DK, accumulator, dkLen);						//using memcpy instead of for loop
}



