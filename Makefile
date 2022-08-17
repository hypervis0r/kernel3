OVMF_DIR := /usr/share/OVMF

VM 	:= qemu-system-x86_64
VMFLAGS := -smp 1 -bios $(OVMF_DIR)/OVMF_CODE.fd -s -d cpu_reset# -pflash $(OVMF_DIR)/OVMF_CODE.fd

BIN 	:= ./bin
BUILD 	:= ./build
SRC 	:= ./src
INCLUDE := ./include

EDK2 := ./edk2
EDK2_INCLUDE := $(EDK2)/MdePkg/Include
EDK2_INCLUDE_ARCH := $(EDK2_INCLUDE)/X64

CC 	:= clang
CFLAGS 	:= -target x86_64-unknown-windows-gnu 	\
		-ffreestanding			\
		-fshort-wchar			\
		-mno-red-zone 			\
		-I$(INCLUDE) -I$(EDK2_INCLUDE) -I$(EDK2_INCLUDE_ARCH) \
		-g \
		-O0

KERNEL_CFLAGS := -target x86_64-unknown-windows-gnu \
		-m64 \
		-fpic \
		-ffreestanding \
		-fno-stack-protector \
		-fno-stack-check \
		-fshort-wchar \
		-mno-red-zone -maccumulate-outgoing-args -mgeneral-regs-only \
		-I$(INCLUDE) -I$(EDK2_INCLUDE) -I$(EDK2_INCLUDE_ARCH) \
		-g \
		-O0


BOOT_LDFLAGS := -target x86_64-unknown-windows 	\
		-nostdlib 			\
		-Wl,-entry:efi_main 		\
		-Wl,-subsystem:efi_application 	\
		-fuse-ld=lld -g

KERNEL_LDFLAGS := \
		-target x86_64-unknown-windows-gnu \
		-nostdlib \
		-shared \
		-fuse-ld=lld \
		-Wl,-e,KeMain \
		-g \
		-Wl,--image-base,0xC000000000000000

BOOT_SRCS := $(shell find $(SRC)/boot -name '*.c')
BOOT_OBJS := $(BOOT_SRCS:.c=.o)
BOOT_DEPS := $(shell find $(INCLUDE)/boot -name '*.h')

KERNEL_SRCS := $(shell find $(SRC)/kernel -name '*.c')
KERNEL_ASM 	:= $(shell find $(SRC)/kernel -name '*.asm')

KERNEL_OBJS := $(KERNEL_SRCS:.c=.o)
KERNEL_OBJS += $(KERNEL_ASM:.asm=.o)

KERNEL_DEPS := $(shell find $(INCLUDE)/kernel -name '*.h')

boot/%.o: boot/%.c $(BOOT_DEPS)
	$(CC) $(CFLAGS) -c -o $@ $^

kernel/%.o: kernel/%.c $(KERNEL_DEPS)
	@echo "BBBB"
	$(CC) $(KERNEL_CFLAGS) -c -o $@ $^

%.o: %.asm
	@echo "CCCC"
	nasm -f win64 -o $@ $<

BOOTX64.efi: $(BOOT_OBJS)
	$(CC) $(BOOT_LDFLAGS) -o $@ $^

kernel3.exe: $(KERNEL_OBJS)
	@echo $(KERNEL_SRCS)
	$(CC) $(KERNEL_LDFLAGS) -o $@ $^

kernel3.img: BOOTX64.efi kernel3.exe
	#@echo $(word 2,$^)
	mkdir -p ./build_fs
	dd if=/dev/zero of=$@ bs=1024 count=65525
	mkfs.fat -F 32 $@
	sudo mount -o loop $@ ./build_fs
	sudo mkdir ./build_fs/system/
	sudo mkdir ./build_fs/EFI
	sudo mkdir ./build_fs/EFI/BOOT
	sudo cp $< ./build_fs/EFI/BOOT/
	sudo cp $(word 2,$^) ./build_fs/system
	sudo cp kernel_tty.psf ./build_fs/system
	sudo umount -l ./build_fs

all: kernel3.img 
	@echo "[+] kernel3.img built"

clean:
	rm -rf $(BOOT_OBJS) $(KERNEL_OBJS) kernel3.img BOOTX64.efi

run: all
	$(VM) $(VMFLAGS) -hda kernel3.img
