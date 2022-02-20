/*
 * isha.h
 *
 * A completely insecure and bad hashing algorithm, based loosely on
 * SHA-1 (which is itself no longer considered a good hashing
 * algorithm)
 *
 * Based on code for sha1 processing from Paul E. Jones, available at
 * https://www.packetizer.com/security/sha1/
 */

#ifndef _ISHA_H_
#define _ISHA_H_

#include <stdint.h>
#include <stdlib.h>

#define ISHA_BLOCKLEN  64  // length of an ISHA block, in bytes
#define ISHA_DIGESTLEN 20  // length of an ISHA digest, in bytes

typedef struct 
{
  uint32_t MD[5];        // Message Digest (output)

  uint32_t Length_Low;   // Message length in bits
  uint32_t Length_High;  // Message length in bits

  uint8_t MBlock[64];    // 512-bit message blocks
  int MB_Idx;            // Index into message block array

  int Computed;          // Is the digest computed?
  int Corrupted;         // Is the message digest corruped?
} ISHAContext;

void ISHA_loop(ISHAContext *ctx, int i, const uint8_t *message_array, size_t length, const uint8_t *message_array2, size_t length2, uint8_t *digest_out);
/*
 * Resets/initializes the given context back to its starting state, in
 * preparation for computing a new message digest
 * 
 * Parameters:
 *   ctx         The ISHAContext (in/out)
 */
void ISHAReset(ISHAContext *ctx);

/*
 * Computes the ISHA hash of the message, and returns the 20-byte hash
 * 
 * Parameters:
 *   ctx         The ISHAContext (in/out)
 *   digest_out  Upon return, the 20-byte message digest (out)
 * 
 * Returns:
 *   the message digest, in digest_out
 */
void ISHAResult(ISHAContext *ctx, uint8_t *digest_out);


/*
 * Accepts an array of bytes as the next portion of the running ISHA hash
 * 
 * Parameters:
 *   ctx     The ISHAContext (in/out)
 *   bytes   Pointer to the bytes to be processed (in)
 *   nbytes  Number of bytes to be processed (in)
 */
void ISHAInput(ISHAContext * ctx, const uint8_t *bytes, size_t nbytes);

#endif
