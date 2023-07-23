
FONTES = main.c boot.s hex.c uart.c

# Dispositivo serial conectado ao rasperry (bootloader)
BOOTLOADR = /dev/ttyUSB0

#
# Arquivos de saída 
#
EXEC = kernel.elf
MAP = kernel.map
IMAGE = loader.img
LIST = kernel.list
HEXFILE = kernel.hex

PREFIXO = arm-none-eabi-
LDSCRIPT = kernel.ld
AS = ${PREFIXO}as
LD = ${PREFIXO}ld
GCC = ${PREFIXO}gcc
OBJCPY = ${PREFIXO}objcopy
OBJDMP = ${PREFIXO}objdump
OPTS = -march=armv7-a -mtune=cortex-a7 -g 
OBJ = $(FONTES:.s=.o)
OBJETOS = $(OBJ:.c=.o)

all: ${EXEC} ${IMAGE} ${LIST}

#
# Gerar executável
#
${EXEC}: ${OBJETOS}
	${LD} -T ${LDSCRIPT} -M=${MAP} -o $@ ${OBJETOS}

#
# Gerar imagem
#
${IMAGE}: ${EXEC}
	${OBJCPY} ${EXEC} -O binary ${IMAGE}

#
# Gerar arquivo HEX para bootloader 
#
${HEXFILE}: ${EXEC}
	${OBJCPY} ${EXEC} -O ihex ${HEXFILE}

#
# Gerar listagem
#
${LIST}: ${EXEC}
	${OBJDMP} -std ${EXEC} > ${LIST}

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
	rm -f *.o ${EXEC} ${MAP} ${LIST} ${IMAGE} ${HEXFILE}

#
# Iniciar openocd
#
ocd: 
	@if pgrep openocd >/dev/null ; then\
		echo "openocd já está executando"; \
	else openocd -f rpi2.cfg & \
	fi

#
# Iniciar gdb
#
gdb: ${EXEC}
	gdb-multiarch -ex "set architecture arm" \
		           -ex "target extended-remote :3333" \
					  -ex "load" \
					  ${EXEC}

#
# Enviar arquivo ao bootloader
#
load: ${HEXFILE}
	@if [ ! -e ${BOOTLOADR} ]; then \
		echo "Dispositivo USB não encontrado em ${HEXFILE}" ;\
	else stty -F ${BOOTLOADR} 115200 && cp ${HEXFILE} ${BOOTLOADR} ;\
	fi

stop:
	-pkill openocd
	-pkill gdb-multiarch

rebuild: clean all

