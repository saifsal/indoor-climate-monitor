#include "../src/app/climate_evaluator.c"
#include <stdio.h>
#include <math.h>

void test_bsqrt(void);
void test_func(void);

void test_icq(void);

int main(void) {
  test_bsqrt();
  test_func();
  test_icq();
}

void test_bsqrt(void) {
  printf("Testing bsqrt...\n");
  for (unsigned long i = 0; i < (1 << 16); ++i) {
    const unsigned long i2 = i * i;
    if (i != bsqrt(i2)) {
      printf("[%li] %li\nTest failed!\n", i, i2);
      return;
    }
  }
  printf("Test succeeded!\n");
}

void test_func(void) {
  printf("Testing slope_i \t slope_f \t plateau\n");
  scale_t s = {0, {10, 20, 30, 40}, co2};
  for (unsigned short i = 0; i < 50; ++i) {
    printf("[%i]\t%i\t\t%i\t\t%i\n", i, slope_i(&s, i), slope_f(&s,(float)i), plateau(&s,(float)i));
  }
}

void test_icq(void) {
  printf("Testing compute_icq...\n");
  scale_t s[4];
  s[0].value = 50;
  s[1].value = 100;
  s[2].value = 150;
  s[3].value = 200;
  printf("ICQ: %i [%.6f]\n", compute_icq(s), 110.668192);
}
