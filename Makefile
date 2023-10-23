BUILD := build
SOURCE := src
HD_IMG_NAME := hd.img

CFLAGS := -m32					# generate code for 32-bit ABI
CFLAGS += -masm=intel			# output assembly instructions using "intel" (default is "att")
CFLAGS += -fno-builtin			# disable GCC built-in functions (for performance purpose)
CFLAGS += -nostdinc				# do not search the standard system directories for header files
CFLAGS += -fno-pic				# generate non position independent code (pic) which is the code can be loaded and executed at any memory address, which is essential for shared libraries
CFLAGS += -fno-stack-protector	# disable the stack protector feature
CFLAGS := $(strip ${CFLAGS})
ASM_FLAGS := -f elf32			# specify the output file format is ELF32 (i386) object files
DEBUG := -g
LD_FLAGS := -m elf_i386			# specify produce an ELF binary file for Intel x86 architecture (32-bit).
OC_FLAGS := -O binary			# specify the output is raw binary data (flat binary) without metadata info in object file



all: ${BUILD}/boot/boot_lba.o ${BUILD}/boot/loader.o ${BUILD}/kernel.elf ${BUILD}/kernel.bin
	$(shell rm -f $(HD_IMG_NAME))
	bximage -q -hd=16 -func=create -sectsize=512 -imgmode=flat $(HD_IMG_NAME)
	dd if=${BUILD}/boot/boot_lba.o of=hd.img bs=512 seek=0 count=1 conv=notrunc
	dd if=${BUILD}/boot/loader.o of=hd.img bs=512 seek=1 count=2 conv=notrunc
	dd if=${BUILD}/kernel.bin of=hd.img bs=512 seek=3 count=30 conv=notrunc

${BUILD}/boot/%.o: ${SOURCE}/boot/%.asm
	$(shell mkdir -p ${BUILD}/boot)
	nasm $< -o $@

${BUILD}/kernel.bin: ${BUILD}/kernel.elf
	objcopy ${OC_FLAGS} $< $@

# compile C and ASM code to object file, and link them to ELF32 file
${BUILD}/kernel.elf: ${BUILD}/init/main32.o ${BUILD}/boot/head.o
	ld ${LD_FLAGS} $^ -o $@

${BUILD}/boot/head.o: ${SOURCE}/boot/head.asm
	nasm ${ASM_FLAGS} ${DEBUG} $< -o $@

${BUILD}/init/main32.o: ${SOURCE}/init/main32.c
	$(shell mkdir -p ${BUILD}/init)
	gcc ${CFLAGS} ${DEBUG} -c $< -o $@

clean:
	$(shell rm -rf ${BUILD})
    $(shell rm -f ${HD_IMG_NAME}*)

bochs: all
	bochs -q -f bochsrc
