/*
 * PBKDF2_Optimization.c
 *
 * Application entry point, and test timing
 * 
 * Author: Howdy Pierce, howdy.pierce@colorado.edu
 */

#include <string.h>

#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"

#include "pbkdf2.h"
#include "pbkdf2_test.h"
#include "ticktime.h"


/*
 * Times a single call to the pbkdf2_hmac_isha function, and prints
 * the resulting duration
 */
static void time_pbkdf2_hmac_isha()
{
  const char *pass = "Boulder";
  const char *salt = "Buffaloes";
  int iterations = 4096;
  size_t dk_len = 48;
  int passlen, saltlen;
  uint8_t act_result[512];
  uint8_t exp_result[512];
  const char *exp_result_hex = "7577B5FFB058195DE3978773B472E92D0216873EE1A2"\
    "170157C2054EDC41E58D7F949050253F8CE1D55E6B86E62AED3F";
    
  ticktime_t duration = 0;

  assert(dk_len <= sizeof(act_result));

  hexstr_to_bytes(exp_result, exp_result_hex, dk_len);passlen = strlen(pass);
  saltlen = strlen(salt);

  reset_timer();
  pbkdf2_hmac_isha((const uint8_t *)pass, passlen, (const uint8_t *)salt, saltlen,
      iterations, dk_len, act_result);
  duration = get_timer();

  if (cmp_bin(act_result, exp_result, dk_len)) {
    printf("%s: %u iterations took %u msec\r\n", __FUNCTION__,
        iterations, duration/10);
  } else {
    printf("FAILURE on timed test\r\n");
  }
}


/*
 * Run all the validity checks; exit on failure
 */
static void run_tests()
{
  bool success = true;

  success &= test_isha();
  success &= test_hmac_isha();
  success &= test_pbkdf2_hmac_isha();

  if (success)
    return;

  printf("TEST FAILURES EXIST ... exiting\r\n");

  exit(-1);
}


/*
 * Application entry point.
 */
int main(void) {

  /* Init board hardware. */

  BOARD_InitBootPins();
  BOARD_InitBootClocks();
  BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
  /* Init FSL debug console. */
  BOARD_InitDebugConsole();
#endif

  init_ticktime();

  printf("Running validity tests...\r\n");
  run_tests();
  printf("All tests passed!\r\n");

  printf("Running timing test...\r\n");
  time_pbkdf2_hmac_isha();

  return 0 ;
}
