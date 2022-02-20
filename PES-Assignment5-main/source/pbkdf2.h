/*
 * pbkdf2.h
 *
 * A perfectly legitimate implementation of HMAC and PBKDF2, but based
 * on the "ISHA" insecure and bad hashing algorithm.
 * 
 * Author: Howdy Pierce, howdy.pierce@colorado.edu
 */

#ifndef _PBKDF2_H_
#define _PBKDF2_H_

//#include <stdint.h>
#include "isha.h"

/*
 * Computes the HMAC-ISHA for the given key and message. See RFC 2104
 * for details
 *
 * Parameters:
 *    key:      The secret key
 *    key_len:  Length of key
 *    msg:      The message to be hashed
 *    msg_len:  Length of msg
 *    digest:   Output area: the 20-byte digest will be written here
 * 
 * Returns:
 *    20-byte computed key is returned in digest
 */
void hmac_isha(const uint8_t *key, size_t key_len,
    const uint8_t *msg, size_t msg_len, uint8_t *digest);

/*
 * Implements PBKDF2-HMAC-ISHA as defined in RFC 8018 section
 * 5.2. Note that hlen from the RFC is ISHA_DIGESTLEN here, i.e., 20 bytes
 *
 * Parameters:
 *   pass      The password
 *   pass_len  length of pass
 *   salt      The salt
 *   salt_len  length of salt
 *   iter      The iteration count ("c" in RFC 8018)
 *   dkLen     intended length in bytes of the derived key
 *   DK        the output derived key, must be at least dkLen bytes long
 * 
 * Returns:
 *   The derived key of dkLen bytes is copied into DK
 */
void pbkdf2_hmac_isha(const uint8_t *pass, size_t pass_len,
    const uint8_t *salt, size_t salt_len, int iter, size_t dkLen, uint8_t *DK);

#endif  // _PBKDF2_H_
