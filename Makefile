OVMF_DIR := /usr/share/OVMF

VM 	:= qemu-system-x86_64
VMFLAGS := -bios $(OVMF_DIR)/OVMF_CODE.fd # -pflash $(OVMF_DIR)/OVMF_CODE.fd

BIN 	:= ./bin
BUILD 	:= ./build
SRC 	:= ./src
INCLUDE := ./include

EDK2 := ./edk2
EDK2_INCLUDE := $(EDK2)/MdePkg/Include
EDK2_INCLUDE_ARCH := $(EDK2_INCLUDE)/X64

CC 	:= clang
CFLAGS 	:= -target x86_64-unknown-windows 	\
		-ffreestanding			\
		-fshort-wchar			\
		-mno-red-zone 			\
		-I$(INCLUDE) -I$(EDK2_INCLUDE) -I$(EDK2_INCLUDE_ARCH)

BOOT_LDFLAGS := -target x86_64-unknown-windows 	\
		-nostdlib 			\
		-Wl,-entry:efi_main 		\
		-Wl,-subsystem:efi_application 	\
		-fuse-ld=lld

BOOT_SRCS := $(shell find $(SRC)/boot -name '*.c')
BOOT_OBJS := $(BOOT_SRCS:.c=.o)
BOOT_DEPS := $(shell find $(INCLUDE)/boot -name '*.h')

KERNEL_SRCS := $(shell find $(SRC)/kernel -name '*.c')
KERNEL_OBJS := $(KERNEL_SRCS:.c=.o)
KERNEL_DEPS := $(shell find $(INCLUDE)/kernel -name '*.h')

boot/%.o: %.c $(BOOT_DEPS)
	$(CC) $(CFLAGS) -c -o $@ $^

kernel/%o: %.c $(KERNEL_DEPS)
	$(CC) $(CFLAGS) -c -o $@ $^

BOOTX64.efi: $(BOOT_OBJS)
	$(CC) $(BOOT_LDFLAGS) -o $@ $^

kernel3.exe: $(KERNEL_OBJS)
	$(CC) $(KERNEL_LDFLAGS) -o $@ $^

kernel3.img: BOOTX64.efi # kernel3.exe
	dd if=/dev/zero of=$@ bs=1k count=1440
	mformat -i $@ -f 1440 ::
	mmd -i $@ ::/EFI
	mmd -i $@ ::/EFI/BOOT
	mcopy -i $@ $< ::/EFI/BOOT

all: kernel3.img 
	@echo "[+] kernel3.img built"

clean:
	rm -rf $(BOOT_OBJS) $(KERNEL_OBJS) kernel3.img BOOTX64.efi

run: all
	$(VM) $(VMFLAGS) -hda kernel3.img
