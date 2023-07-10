
#
# Lista dos arquivos fonte
#
FONTES = main.c botao.c serial.c timer.c startup.s

#
# Arquivos de saída 
#
EXEC = kernel.elf
MAP = kernel.map

PREFIXO = arm-none-eabi-
LDSCRIPT = kernel.ld
AS = ${PREFIXO}as
LD = ${PREFIXO}ld
GCC = ${PREFIXO}gcc
OPTS = -march=armv4 -g 
OBJ = $(FONTES:.s=.o)
OBJETOS = $(OBJ:.c=.o)

#
# Gerar executável
#
${EXEC}: ${OBJETOS}
	${LD} -T ${LDSCRIPT} -M=${MAP} -o $@ ${OBJETOS}

#
# Compilar arquivos em C
#
.c.o:
	${GCC} ${OPTS} -c -o $@ $<

#
# Montar arquivos em assembler
#
.s.o:
	${AS} -g -o $@ $<

#
# Limpar tudo
#
clean:
	rm -f *.o ${EXEC} ${MAP}

#
# Iniciar openocd
#
ocd: 
	@if pgrep openocd >/dev/null ; then\
		echo "openocd já está executando"; \
	else openocd -f evaluator7t.cfg & \
	fi

#
# Iniciar gdb
#
gdb: ${EXEC}
	gdb-multiarch -ex "set architecture arm" \
		           -ex "target extended-remote :3333" \
					  -ex "load" \
					  ${EXEC}

