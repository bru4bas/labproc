
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static int 
from_hex(char c) {
   if((c >= '0') && (c <= '9')) return c - '0';
   if((c >= 'a') && (c <= 'f')) return c - 'a' + 10;
   if((c >= 'A') && (c <= 'F')) return c - 'A' + 10;
   return 0;
}

static uint32_t 
le_byte(char *p) {
   return (from_hex(p[0]) << 4) + from_hex(p[1]);
}

uint32_t base = 0;
uint32_t run = 0;
bool flag_run = false;

/**
 * Processa uma linha de um arquivo Intel HEX (sem os ':').
 * @param buffer Buffer com os caracteres.
 * @param size Quantidade de caracteres no buffer.
 */
bool 
processa_hex(uint8_t *buffer, uint8_t size) {
   if(size & 0x01) return false;      // tamanho deve ser par
   int sz = le_byte(buffer);
   if(size < 10+2*sz) return false;   // confere o tamanho

   /*
    * Converte os caracteres hexadecimais para números binários e
    * calcula o checksum.
    */
   uint8_t *i = buffer;
   int j = 0;
   uint8_t chk = 0;
   while (*i) {
      uint8_t b = le_byte(i);
      chk += b;
      buffer[j] = b;
      i += 2;
      j++;
   }
   if(chk) return false;               // checksum inválido

   /*
    * Processa o comando.
    */
   uint8_t cmd = buffer[3];
   uint32_t addr = buffer[1];
   addr = (addr << 8) | buffer[2];
   addr = base + addr;
   switch(cmd) {
      /*
       * Comando 0: dados.
       * Copia na memória.
       */
      case 0: {
            uint8_t *dst = (uint8_t*)addr;
            uint8_t *src = buffer + 4;
            while(sz) {
               *dst = *src;
               dst++;
               src++;
               sz--;
            }
         } break;

      /*
       * Comando 1: final de arquivo
       * tenta iniciar o programa.
       */
      case 1: {
            if(!flag_run) return false;
            void (*jump)(void) = (void(*)(void))run;
            jump();
         }
         break;

      /*
       * Comando 2: extented segment address.
       * Atualiza endereço base com um segmento de 64k.
       */
      case 2:
         base = buffer[4];
         base = (base << 8) | buffer[5];
         base = base << 4;
         break;

      /*
       * Comando 3: start segment address.
       * Atualiza endereço de início do programa (formato CS:PC).
       */
      case 3: {
            flag_run = true;
            uint32_t seg = buffer[4];
            seg = (seg << 8) | buffer[5];
            run = buffer[6];
            run = (run << 8) | buffer[7];
            run = run + (seg << 4);
         }
         break;

      /*
       * Comando 4: Extended linear address.
       * Atualiza endereço base.
       */
      case 4:
         base = buffer[4];
         base = (base << 8) | buffer[5];
         base = base << 16;
         break;

      /*
       * Comando 5: Start linear address.
       * Atualiza endereço de início do programa.
       */
      case 5:
         flag_run = true;
         run = buffer[4];
         run = (run << 8) | buffer[5];
         run = (run << 8) | buffer[6];
         run = (run << 8) | buffer[7];
         break;
   }
}
