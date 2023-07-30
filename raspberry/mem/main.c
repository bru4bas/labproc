
#include <stdint.h>

extern void mmu_start(void*);

/**
 * Tabela de tradução de páginas.
 */
typedef struct {
   uint32_t descriptor[4096];
} ttb_l1_t;

ttb_l1_t *ttb = (ttb_l1_t*)0x4000;

/**
 * Cria um mapeamento de uma seção (1 MiB)
 */
void 
map_section(uint32_t virt, uint32_t fis, uint32_t flags) {
   uint32_t indice = (virt & 0xfff00000) >> 20;
   uint32_t descr = (fis & 0xfff00000) | 0xc002 | flags;
   ttb->descriptor[indice] = descr;
}

/**
 * Marca a entrada de um endereço virtual de seção como inválida.
 */
void 
map_invalida(uint32_t virt) {
   uint32_t indice = (virt & 0xfff00000) >> 20;
   ttb->descriptor[indice] = 0;
}

/**
 * Cria um mapeamento "flat" da memória virtual:
 * Os endereços virtuais e físicos são os mesmos.
 */
void
mmu_flat(void) {
   uint32_t i;
   for(i=0; i<4096; i++) {
      map_section(i << 20, i << 20, 0);
   }
}

int main(void) {
   char *ptr = (char *)0x00100000;
   mmu_flat();
   map_section(0xfff00000, 0, 0);      // mapeia a última seção (quase 4 GiB) na primeira, onde está o programa
   map_invalida(0x00100000);           // invalida o segundo MiB da memória virtual
   *ptr = 'x';                         // aqui funciona
   mmu_start(ttb);                     // liga o bicho
   *ptr = 'x';                         // aqui deve dar pau.
   for(;;);
}

