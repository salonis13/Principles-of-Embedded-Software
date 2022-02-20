/*
 * assignment2_tester.c - automated test cases for grading the
 * student's llfifo and cbfifo implementations
 * 
 * Author: Howdy Pierce, howdy.pierce@colorado.edu
 * 
 * To use: You'll need to modify the student's assignment to isolate
 * out the student's llfifo and cbfifo test cases; hopefully this is
 * as simple as just isolating their llfifo.c and cbfifo.c files. Then
 * do:
 * 
 * gcc -Wall -Werror assignment2_tester.c cbfifo.c llfifo.c -o run-(studentname)
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "cbfifo.h"
#include "llfifo.h"

#define max(x,y) ((x) > (y) ? (x) : (y))

int g_tests_passed = 0;
int g_tests_total = 0;

#define test_assert(value) {                                            \
  g_tests_total++;                                                      \
  if (value) {                                                          \
    g_tests_passed++;                                                   \
  } else {                                                              \
    printf("ERROR: test failure at line %d\n", __LINE__);               \
  }                                                                     \
}

#define test_equal(value1, value2) {                                    \
  g_tests_total++;                                                      \
  long res1 = (long)(value1);                                           \
  long res2 = (long)(value2);                                           \
  if (res1 == res2) {                                                   \
    g_tests_passed++;                                                   \
  } else {                                                              \
    printf("ERROR: test failure at line %d: %ld != %ld\n", __LINE__, res1, res2); \
  }                                                                     \
}


static void
test_llfifo_one_iteration(int capacity)
{
  char *strs[] =
    { "To be, or not to be: that is the question:",
      "Whether 'tis nobler in the mind to suffer",
      "The slings and arrows of outrageous fortune,",
      "Or to take arms against a sea of troubles,",
      "And by opposing end them? To die, to sleep—",
      "No more—and by a sleep to say we end",
      "The heart-ache and the thousand natural shocks",
      "That flesh is heir to, 'tis a consummation",
      "Devoutly to be wish'd. To die, to sleep;",
      "To sleep: perchance to dream: ay, there's the rub;",
      "For in that sleep of death what dreams may come",
      "When we have shuffled off this mortal coil,",
      "Must give us pause."
    };

  const int strs_len = sizeof(strs) / sizeof(const char *);
  llfifo_t *fifo;

  fifo = llfifo_create(capacity);
  test_assert(fifo != NULL);

  test_equal(llfifo_capacity(fifo), capacity);
  test_equal(llfifo_length(fifo), 0);
  test_equal(llfifo_dequeue(fifo), NULL);

  // enqueue one element, then dequeue it, make sure it all matches
  test_equal(llfifo_enqueue(fifo, strs[0]), 1);
  test_equal(llfifo_capacity(fifo), max(capacity, 1));
  test_equal(llfifo_length(fifo), 1);
  test_equal(llfifo_dequeue(fifo), strs[0]);
  test_equal(llfifo_capacity(fifo), max(capacity, 1));
  test_equal(llfifo_length(fifo), 0);

  // enqueue all the elements, then dequeue all
  for (int i=0; i<strs_len; i++) {
    test_equal(llfifo_enqueue(fifo, strs[i]), i+1);
    test_equal(llfifo_capacity(fifo), max(capacity, i+1));
    test_equal(llfifo_length(fifo), i+1);
  }
  for (int i=0; i<strs_len; i++) {
    test_equal(llfifo_dequeue(fifo), strs[i]);
    test_equal(llfifo_length(fifo), strs_len - i - 1);
    test_equal(llfifo_capacity(fifo), max(capacity, strs_len));
  }

  // should be empty now
  test_equal(llfifo_length(fifo), 0);
  test_equal(llfifo_dequeue(fifo), NULL);
  test_equal(llfifo_capacity(fifo), max(capacity, strs_len));

  // enqueue one, then enqueue one, dequeue one, etc, through the whole list
  test_equal(llfifo_enqueue(fifo, strs[0]), 1);
  for (int i=1; i<strs_len; i++) {
    test_equal(llfifo_enqueue(fifo, strs[i]), 2);
    test_equal(llfifo_length(fifo), 2);
    test_equal(llfifo_dequeue(fifo), strs[i-1]);
    test_equal(llfifo_length(fifo), 1);
    test_equal(llfifo_capacity(fifo), max(capacity, strs_len));
  }
  test_equal(llfifo_dequeue(fifo), strs[strs_len-1]);
  
  // should be empty now
  test_equal(llfifo_length(fifo), 0);
  test_equal(llfifo_dequeue(fifo), NULL);
  test_equal(llfifo_capacity(fifo), max(capacity, strs_len));

  // create a second fifo
  const int capacity2 = 3;
  llfifo_t *fifo2;

  fifo2 = llfifo_create(capacity2);
  test_assert(fifo2 != NULL);
  test_equal(llfifo_capacity(fifo2), capacity2);
  test_equal(llfifo_length(fifo2), 0);
  test_equal(llfifo_dequeue(fifo2), NULL);

  // enqueuing the even numbered strings onto the second fifo, and the
  // odd numbered strings onto the original fifo
  for (int i=0; i<strs_len; i++) {
    llfifo_t * this_fifo = (i & 0x1) ? fifo : fifo2;
    test_equal(llfifo_enqueue(this_fifo, strs[i]), (i/2)+1);
    test_equal(llfifo_length(this_fifo), (i/2)+1);
  }
  test_equal(llfifo_capacity(fifo), max(capacity, strs_len));
  test_equal(llfifo_capacity(fifo2), max(capacity2, strs_len/2 + 1));

  // now dequeue and make sure everything comes out correctly
  for (int i=0; i<strs_len; i++) {
    llfifo_t * this_fifo = (i & 0x1) ? fifo : fifo2;
    test_equal(llfifo_dequeue(this_fifo), strs[i]);
  }
  test_equal(llfifo_length(fifo), 0);
  test_equal(llfifo_length(fifo2), 0);
  test_equal(llfifo_dequeue(fifo), NULL);
  test_equal(llfifo_dequeue(fifo2), NULL);

  test_equal(llfifo_capacity(fifo), max(capacity, strs_len));
  test_equal(llfifo_capacity(fifo2), max(capacity2, strs_len/2 + 1));

  llfifo_destroy(fifo);
  llfifo_destroy(fifo2);
}


void
test_llfifo()
{
  g_tests_passed = 0;
  g_tests_total = 0;
  
  test_llfifo_one_iteration(0);
  test_llfifo_one_iteration(1);
  test_llfifo_one_iteration(5);
  test_llfifo_one_iteration(20);

  printf("%s: passed %d/%d test cases (%2.1f%%)\n", __FUNCTION__,
      g_tests_passed, g_tests_total,
      100.0*g_tests_passed/g_tests_total);
}


static void
test_cbfifo_one_iteration()
{
  char *str ="To be, or not to be: that is the question:\n"
    "Whether 'tis nobler in the mind to suffer\n"
    "The slings and arrows of outrageous fortune,\n"
    "Or to take arms against a sea of troubles,\n"
    "And by opposing end them? To die, to sleep--\n"
    "No more--and by a sleep to say we end\n"
    "The heart-ache and the thousand natural shocks\n"
    "That flesh is heir to, 'tis a consummation\n"
    "Devoutly to be wish'd. To die, to sleep;\n"
    "To sleep: perchance to dream: ay, there's the rub;\n"
    "For in that sleep of death what dreams may come\n"
    "When we have shuffled off this mortal coil,\n"
    "Must give us pause.";

  char buf[1024];
  const int cap = cbfifo_capacity();
  size_t ret = 0;

  // the asserts in the following 2 lines are there because they're
  // not testing the student, they're validating that the test is
  // correct
  assert(strlen(str) >= cap*2);
  assert(sizeof(buf) > cap);
  test_assert(cap == 128 || cap == 127);

  test_equal(cbfifo_length(), 0);
  test_equal(cbfifo_dequeue(buf, cap), 0);
  test_equal(cbfifo_dequeue(buf, 2), 0);

  // enqueue 10 bytes, then dequeue same amt 
  test_equal(cbfifo_enqueue(str, 10), 10);
  test_equal(cbfifo_length(), 10);
  test_equal(cbfifo_dequeue(buf, 10), 10);
  test_equal(strncmp(buf, str, 10), 0);
  test_equal(cbfifo_length(), 0);

  // enqueue 20 bytes;  dequeue 5, then another 20
  test_equal(cbfifo_enqueue(str, 20), 20);
  test_equal(cbfifo_length(), 20);
  test_equal(cbfifo_dequeue(buf, 5), 5);
  test_equal(cbfifo_length(), 15);
  test_equal(cbfifo_dequeue(buf+5, 20), 15);
  test_equal(cbfifo_length(), 0);
  test_equal(strncmp(buf, str, 20), 0);

  // fill buffer and then read it back out
  test_equal(cbfifo_enqueue(str, cap), cap);
  test_equal(cbfifo_length(), cap);
  ret = cbfifo_enqueue(str, 1);
  test_assert(ret == 0 || (int)ret == -1);
  test_equal(cbfifo_dequeue(buf, cap), cap);
  test_equal(cbfifo_length(), 0);
  test_equal(strncmp(buf, str, cap), 0);

  // Add 20 bytes and pull out 18
  test_equal(cbfifo_enqueue(str, 20), 20);
  test_equal(cbfifo_length(), 20);
  test_equal(cbfifo_dequeue(buf, 18), 18);
  test_equal(cbfifo_length(), 2);
  test_equal(strncmp(buf, str, 18), 0);

  // Now add a bunch of data in 4 chunks
  int chunk_size = (cap-2) / 4;
  for (int i=0; i<4; i++) {
    test_equal(cbfifo_enqueue(str+i*chunk_size, chunk_size), chunk_size);
    test_equal(cbfifo_length(), (i+1)*chunk_size + 2);
  }
  test_equal(cbfifo_length(), 4*chunk_size + 2);

  // Take out the 2 remaining bytes from above
  test_equal(cbfifo_dequeue(buf, 2), 2);
  test_equal(strncmp(buf, str+18, 2), 0);

  // now read those chunks out a byte at a time
  for (int i=0; i<chunk_size*4; i++) {
    test_equal(cbfifo_dequeue(buf+i, 1), 1);
    test_equal(cbfifo_length(), chunk_size*4 - i - 1);
  }
  test_equal(strncmp(buf, str, chunk_size*4), 0);
  
  // write more than capacity
  test_equal(cbfifo_enqueue(str, 65), 65);
  test_equal(cbfifo_enqueue(str+65, cap), cap-65);
  test_equal(cbfifo_length(), cap);
  test_equal(cbfifo_dequeue(buf, cap), cap);
  test_equal(cbfifo_length(), 0);
  test_equal(strncmp(buf, str, cap), 0);

  // write zero bytes
  test_equal(cbfifo_enqueue(str, 0), 0);
  test_equal(cbfifo_length(), 0);

  // Exercise the following conditions:
  //    enqueue when read < write:
  //        bytes < CAP-write  (1)
  //        bytes exactly CAP-write  (2)
  //        bytes > CAP-write but < space available (3)
  //        bytes exactly the space available (4)
  //        bytes > space available (5)
  test_equal(cbfifo_enqueue(str, 32), 32);  // advance so that read < write
  test_equal(cbfifo_length(), 32);
  test_equal(cbfifo_dequeue(buf, 16), 16);
  test_equal(cbfifo_length(), 16);
  test_equal(strncmp(buf, str, 16), 0);

  test_equal(cbfifo_enqueue(str+32, 32), 32);  // (1)
  test_equal(cbfifo_length(), 48);
  test_equal(cbfifo_enqueue(str+64, cap-64), cap-64);  // (2)
  test_equal(cbfifo_length(), cap-16);
  test_equal(cbfifo_dequeue(buf+16, cap-16), cap-16);
  test_equal(strncmp(buf, str, cap), 0);
  
  test_equal(cbfifo_enqueue(str, 32), 32);  // advance so that read < write
  test_equal(cbfifo_length(), 32);
  test_equal(cbfifo_dequeue(buf, 16), 16);
  test_equal(cbfifo_length(), 16);
  test_equal(strncmp(buf, str, 16), 0);

  test_equal(cbfifo_enqueue(str+32, cap-20), cap-20);  // (3)
  test_equal(cbfifo_length(), cap-4);
  test_equal(cbfifo_dequeue(buf, cap-8), cap-8);
  test_equal(strncmp(buf, str+16, cap-8), 0);
  test_equal(cbfifo_length(), 4);
  test_equal(cbfifo_dequeue(buf, 8), 4);
  test_equal(strncmp(buf, str+16+cap-8, 4), 0);
  test_equal(cbfifo_length(), 0);

  test_equal(cbfifo_enqueue(str, 49), 49);  // advance so that read < write
  test_equal(cbfifo_length(), 49);
  test_equal(cbfifo_dequeue(buf, 16), 16);
  test_equal(cbfifo_length(), 33);
  test_equal(strncmp(buf, str, 16), 0);

  test_equal(cbfifo_enqueue(str+49, cap-33), cap-33);  // (4)
  test_equal(cbfifo_length(), cap);
  test_equal(cbfifo_dequeue(buf, cap), cap);
  test_equal(cbfifo_length(), 0);
  test_equal(strncmp(buf, str+16, cap), 0);

  test_equal(cbfifo_enqueue(str, 32), 32);  // advance so that read < write
  test_equal(cbfifo_length(), 32);
  test_equal(cbfifo_dequeue(buf, 16), 16);
  test_equal(cbfifo_length(), 16);
  test_equal(strncmp(buf, str, 16), 0);

  test_equal(cbfifo_enqueue(str+32, cap), cap-16);  // (5)
  test_equal(cbfifo_dequeue(buf, 1), 1);
  test_equal(cbfifo_length(), cap-1);
  test_equal(cbfifo_dequeue(buf+1, cap-1), cap-1);
  test_equal(cbfifo_length(), 0);
  test_equal(strncmp(buf, str+16, cap), 0);

  //    enqueue when write < read:
  //        bytes < read-write (6)
  //        bytes exactly read-write (= the space available) (7)
  //        bytes > space available (8)

  int wpos=0, rpos=0;
  test_equal(cbfifo_enqueue(str, cap-4), cap-4);
  wpos += cap-4;
  test_equal(cbfifo_length(), cap-4);
  test_equal(cbfifo_dequeue(buf, 32), 32);
  rpos += 32;
  test_equal(cbfifo_length(), cap-36);
  test_equal(strncmp(buf, str, 32), 0);
  test_equal(cbfifo_enqueue(str+wpos, 12), 12);
  wpos += 12;
  test_equal(cbfifo_length(), cap-24);

  test_equal(cbfifo_enqueue(str+wpos, 16), 16);  // (6)
  test_equal(cbfifo_length(), cap-8);
  test_equal(cbfifo_dequeue(buf, cap), cap-8);
  test_equal(cbfifo_length(), 0);
  test_equal(strncmp(buf, str+rpos, cap-8), 0);

  // reset
  wpos=0;
  rpos=0;
  test_equal(cbfifo_enqueue(str, cap-4), cap-4);
  wpos += cap-4;
  test_equal(cbfifo_length(), cap-4);
  test_equal(cbfifo_dequeue(buf, 32), 32);
  rpos += 32;
  test_equal(cbfifo_length(), cap-36);
  test_equal(strncmp(buf, str, 32), 0);
  test_equal(cbfifo_enqueue(str+wpos, 12), 12);
  wpos += 12;
  test_equal(cbfifo_length(), cap-24);

  test_equal(cbfifo_enqueue(str+wpos, 24), 24);  // (7)
  test_equal(cbfifo_length(), cap);
  test_equal(cbfifo_dequeue(buf, cap), cap);
  test_equal(cbfifo_length(), 0);
  test_equal(strncmp(buf, str+rpos, cap), 0);

  // reset
  wpos=0;
  rpos=0;
  test_equal(cbfifo_enqueue(str, cap-4), cap-4);
  wpos += cap-4;
  test_equal(cbfifo_length(), cap-4);
  test_equal(cbfifo_dequeue(buf, 32), 32);
  rpos += 32;
  test_equal(cbfifo_length(), cap-36);
  test_equal(strncmp(buf, str, 32), 0);
  test_equal(cbfifo_enqueue(str+wpos, 12), 12);
  wpos += 12;
  test_equal(cbfifo_length(), cap-24);

  test_equal(cbfifo_enqueue(str+wpos, 64), 24);  // (8)
  test_equal(cbfifo_length(), cap);
  test_equal(cbfifo_dequeue(buf, cap), cap);
  test_equal(cbfifo_length(), 0);
  test_equal(strncmp(buf, str+rpos, cap), 0);
}

void
test_cbfifo()
{
  g_tests_passed = 0;
  g_tests_total = 0;
  
  test_cbfifo_one_iteration();

  printf("%s: passed %d/%d test cases (%2.1f%%)\n", __FUNCTION__, 
      g_tests_passed, g_tests_total,
      100.0*g_tests_passed/g_tests_total);
}

int main(int argc, char **argv)
{
  test_cbfifo();
  test_llfifo();
  printf("all tests complete\n");
}
