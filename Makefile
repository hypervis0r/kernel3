OVMF_DIR := /usr/share/OVMF

VM 	:= qemu-system-x86_64
VMFLAGS := -bios $(OVMF_DIR)/OVMF_CODE.fd -hda kernel3.img # -pflash $(OVMF_DIR)/OVMF_CODE.fd

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

LDFLAGS := -target x86_64-unknown-windows 	\
		-nostdlib 			\
		-Wl,-entry:efi_main 		\
		-Wl,-subsystem:efi_application 	\
		-fuse-ld=lld

SRCS = $(shell find $(SRC) -name '*.c')
OBJS = $(SRCS:.c=.o)
DEPS = $(shell find $(INCLUDE) -name '*.h')

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $^

BOOTX64.efi: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

kernel3.img: BOOTX64.efi
	dd if=/dev/zero of=$@ bs=1k count=1440
	mformat -i $@ -f 1440 ::
	mmd -i $@ ::/EFI
	mmd -i $@ ::/EFI/BOOT
	mcopy -i $@ $< ::/EFI/BOOT

all: kernel3.img 
	@echo "[+] kernel3.img built"

clean:
	rm -rf $(OBJS) kernel3.img BOOTX64.efi

run: all
	$(VM) $(VMFLAGS)
