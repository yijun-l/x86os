BUILD := ./build
SOURCE := ./src
HD_IMG_NAME := "hd.img"

all: ${BUILD}/boot/boot_lba.o ${BUILD}/boot/os.o
	$(shell rm -rf $(HD_IMG_NAME))
	bximage -q -hd=16 -func=create -sectsize=512 -imgmode=flat $(HD_IMG_NAME)
	dd if=${BUILD}/boot/boot_lba.o of=hd.img bs=512 seek=0 count=1 conv=notrunc
	dd if=${BUILD}/boot/os.o of=hd.img bs=512 seek=1 count=2 conv=notrunc

${BUILD}/boot/%.o: ${SOURCE}/boot/%.asm
	$(shell mkdir -p ${BUILD}/boot)
	nasm $< -o $@

clean:
	$(shell rm -rf ${BUILD})
    $(shell rm -f ${HD_IMG_NAME}*)

bochs: all
	bochs -q -f bochsrc
