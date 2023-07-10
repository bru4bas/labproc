
#include <stdint.h>
#include "kernel.h"

int main(void) {
   int i;
   for(;;) {
      for(i=0; i<5; i++) ;
      yield();
   }
}

int main2(void) {
   int i;
   for(;;) {
      for(i=0; i<7; i++) ;
      yield();
   }
}

