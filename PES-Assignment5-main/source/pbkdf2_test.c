/*
 * pbkdf2_test.c
 *
 * Test functions for pbkdf2
 * 
 * Author: Howdy Pierce, howdy.pierce@colorado.edu
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "isha.h"
#include "pbkdf2.h"
#include "pbkdf2_test.h"


/* 
 * Compares two byte strings; returns true if they match, false
 * otherwise
 * 
 * Parameters:
 *   b1, b2    The strings to be compared
 *   len       The length of the strings
 */
bool cmp_bin(const uint8_t *b1, const uint8_t *b2, size_t len)
{
  for (size_t i=0; i<len; i++)
    if (b1[i] != b2[i])
      return false;
  return true;
}

/*
 * Returns the integer corresponding to the given hex character
 */
static unsigned char hexdigit_to_int(char c)
{
  if (c >= 'a' && c <= 'f')
    return c - 'a' + 10;
  if (c >= 'A' && c <= 'F')
    return c - 'A' + 10;
  if (c >= '0' && c <= '9')
    return c - '0';
  assert(0);
}


/*
 * Convert a hex string into a binary value
 *
 * Parameters:
 *   out         Output binary value
 *   in_str      Input hex string
 *   binary_len  The length of the binary output, in bytes. 
 *                 in_str must be 2x this length
 * 
 * Returns:
 *   The converted string, in out
 */
void hexstr_to_bytes(uint8_t *out, const char *in_str, size_t binary_len)
{
  for (size_t i=0; i<binary_len; i++) 
    out[i] = hexdigit_to_int(in_str[i*2]) << 4 | hexdigit_to_int(in_str[i*2+1]);
}


#define min(a,b)  ((a)<(b)?(a):(b))

/*
 * Tests the ISHA functions. Returns true if all tests pass, false
 * otherwise. Diagnostic information is printed via printf.
 */
bool test_isha()
{
  typedef struct {
    const char *msg;
    const char *hexdigest;
  } test_matrix_t;

  test_matrix_t tests[] =
    { {"abc", "C308057C568001FDFB9AB52EBA26A8EF1082DD03"},
      {"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
       "1E9276B16369822668226DA470782ECE565F7579"},
      {"Nobody inspects the spammish repetition",
       "0FD1120FD8E25E70EBB0084690C64817B9FFD1BE"},
      {"Now his life is full of wonder/But his heart still knows some fear/Of a simple thing, he cannot comprehend/Why they try to tear the mountains down/To bring in a couple more/More people, more scars upon the land", "6139833602C45E330A2FF5486C10BF2F2BE8FAC0"}
    };

  const int num_tests = sizeof(tests) / sizeof(test_matrix_t);
  int tests_passed = 0;
  uint8_t exp_digest[ISHA_DIGESTLEN];
  uint8_t act_digest[ISHA_DIGESTLEN];
  int msglen;
  ISHAContext ctx;
      
  // First time through: Single call to ISHAInput per test
  for (int i=0; i<num_tests; i++) {
    msglen = strlen(tests[i].msg);
    hexstr_to_bytes(exp_digest, tests[i].hexdigest, ISHA_DIGESTLEN);

    ISHAReset(&ctx);
    ISHAInput(&ctx, (const unsigned char *)tests[i].msg, msglen);
    ISHAResult(&ctx, act_digest);

    if (cmp_bin(act_digest, exp_digest, ISHA_DIGESTLEN)) {
      printf("%s test %d: success\r\n", __FUNCTION__, i);
      tests_passed++;
    } else {
      printf("%s test %d: FAILURE\r\n", __FUNCTION__, i);
    }
  }

  // Second time through: Deliver data in 10-byte chunks to ISHAInput
  for (int i=0; i<num_tests; i++) {
    const unsigned char *data = (const unsigned char *)tests[i].msg;
    msglen = strlen(tests[i].msg);
    hexstr_to_bytes(exp_digest, tests[i].hexdigest, ISHA_DIGESTLEN);

    ISHAReset(&ctx);
    while (msglen > 0) {
      int bytes_this_pass = min(10, msglen);
      ISHAInput(&ctx, data, bytes_this_pass);
      msglen -= bytes_this_pass;
      data += bytes_this_pass;
    }

    ISHAResult(&ctx, act_digest);

    if (cmp_bin(act_digest, exp_digest, ISHA_DIGESTLEN)) {
      printf("%s test %d: success\r\n", __FUNCTION__, i + num_tests);
      tests_passed++;
    } else {
      printf("%s test %d: FAILURE\r\n", __FUNCTION__, i + num_tests);
    }
  }

  return (num_tests*2 == tests_passed);
}


/*
 * Tests the hmac function. Returns true if all tests pass, false
 * otherwise. Diagnostic information is printed via printf.
 */
bool test_hmac_isha()
{
  typedef struct {
    const char *key;
    const char *msg;
    const char *hexdigest;
  } test_matrix_t;

  test_matrix_t tests[] =
    { {"Colorado", "This is a test", "D8E23168EBE0C25D7F5E52FCD4C9FDB5211FF6C9"},
      {"Jefe", "what do ya want for nothin?", "D75D590CDDB00249FE313DA5112073A5FB9E5692"},
      {"RFC 2104", "HMAC: Keyed-Hashing for Message Authentication",
       "A61C0A33F264625D1255664EF960E2540D3E8D1C"}
    };
  const int num_tests = sizeof(tests) / sizeof(test_matrix_t);
  int tests_passed = 0;
  uint8_t exp_digest[ISHA_DIGESTLEN];
  uint8_t act_digest[ISHA_DIGESTLEN];
  int keylen, msglen;
      
  for (int i=0; i<num_tests; i++) {

    keylen = strlen(tests[i].key);
    msglen = strlen(tests[i].msg);

    hexstr_to_bytes(exp_digest, tests[i].hexdigest, ISHA_DIGESTLEN);
    hmac_isha((uint8_t*)tests[i].key, keylen, (uint8_t*)tests[i].msg, msglen, act_digest);
    
    if (cmp_bin(act_digest, exp_digest, ISHA_DIGESTLEN)) {
      printf("%s test %d: success\r\n", __FUNCTION__, i);
      tests_passed++;
    } else {
      printf("%s test %d: FAILURE\r\n", __FUNCTION__, i);
    }
  }

  return (num_tests == tests_passed);
}


/*
 * Tests the pbkdf2_hmac_isha function. Returns true if all tests pass, false
 * otherwise. Diagnostic information is printed via printf.
 */
bool test_pbkdf2_hmac_isha()
{
  typedef struct {
    const char *pass;
    const char *salt;
    int iterations;
    size_t dk_len;
    const char *hex_result;  // expected result, as a hex string
  } test_matrix_t;

  test_matrix_t tests[] =
    { {"password", "mysalt", 1, 20, "0B6D5AEC4AB01BE20B6B7945D570BBBE7B0BA639"},
      {"password", "mysalt", 1, 21, "0B6D5AEC4AB01BE20B6B7945D570BBBE7B0BA63918"},
      {"password", "mysalt", 1, 32, "0B6D5AEC4AB01BE20B6B7945D570BBBE7B0BA63918E9F018490044B6C0B7C117"},
      {"password", "mysalt", 1, 64, "0B6D5AEC4AB01BE20B6B7945D570BBBE7B0BA63918E9F018490044B6C0B7C117185D3F8C24439E7CE71C9E135F285DAC6223D44DB53AE2C83281C88DA0FBE5BE"},
      {"password", "mysalt", 1, 200, "0B6D5AEC4AB01BE20B6B7945D570BBBE7B0BA63918E9F018490044B6C0B7C117185D3F8C24439E7CE71C9E135F285DAC6223D44DB53AE2C83281C88DA0FBE5BE1F651646D63841410D2EDD5EC34CCB9BB5E2E2283C025EC282932FCC5D54BEC673990CF1BBFE759D01D7AB8F8BD7EB1ACDAE532789C2202D05C3B23E3E95960BE23B013B4687C55F63469AECE8E426355A40B6DC5ED81BD8F37C9D87197939484DB2B7DB8A69A8FE518DA0D3EEEFDD0E75D3D2F8F5525E5FF64C177FF19224CD2C8575599EA22F2E"},
      {"password", "mysalt", 2, 20, "06BEA36A00C1D136F562DAC67D2AC538E10B28EB"},
      {"password", "mysalt", 2, 64, "06BEA36A00C1D136F562DAC67D2AC538E10B28EB2BEECFECB760FAC855E9A1ED17B5D5753835698764B603CA5A2D2B94F9C06813ACF9BFA52E2C581CCF732E42"},
      {"password", "mysalt", 3, 20, "E38B24E76B7055D9D601BC0D13BACF2AB676116F"},
      {"password", "mysalt", 100, 20, "6EB7ED4636430AAB3537FC2EBF95FFD779E10C00"},
      {"password", "mysalt", 100, 64, "6EB7ED4636430AAB3537FC2EBF95FFD779E10C001111312874135502E92915B501D3F042838CED31FAF76AF662C1D6FB21AB2901DFBFA7BA2C4198C14A44A502"},
      {"Boulder", "Buffaloes", 100, 64, "DA4D98F2C1479415265BBCD77E7DE03A42A0147C62661D4FA40B454E02D2911BB61CB781479BA940D7D5514D9BA826AC39BAAB4FBB4B0FC1053073F2B9AC1A66"}
    };
  const int num_tests = sizeof(tests) / sizeof(test_matrix_t);
  int tests_passed = 0;
  uint8_t exp_result[512];
  uint8_t act_result[512];
  int passlen, saltlen;
      
  for (int i=0; i<num_tests; i++) {

    passlen = strlen(tests[i].pass);
    saltlen = strlen(tests[i].salt);

    assert(tests[i].dk_len <= sizeof(exp_result));
    hexstr_to_bytes(exp_result, tests[i].hex_result, tests[i].dk_len);

    pbkdf2_hmac_isha((const uint8_t *)tests[i].pass, passlen,
        (const uint8_t *)tests[i].salt, saltlen, tests[i].iterations, tests[i].dk_len,
        act_result);

    if (cmp_bin(act_result, exp_result, tests[i].dk_len)) {
      printf("%s test %d: success\r\n", __FUNCTION__, i);
      tests_passed++;
    } else {
      printf("%s test %d: FAILURE\r\n", __FUNCTION__, i);
    }
  }

  return (num_tests == tests_passed);
}
