
#
# Lista dos arquivos fonte
#  Inclua seus arquivos aqui
#
FONTES = main.s

#
# Arquivos de saída 
#  Altere se quiser
#
EXEC = kernel.elf
MAP = kernel.map

PREFIXO = arm-none-eabi-
LDSCRIPT = kernel.ld
AS = ${PREFIXO}as
LD = ${PREFIXO}ld
OBJETOS = $(FONTES:.s=.o)

#
# Gerar executável
#
${EXEC}: ${OBJETOS}
	${LD} -T ${LDSCRIPT} -M=${MAP} -o $@ ${OBJETOS}

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

