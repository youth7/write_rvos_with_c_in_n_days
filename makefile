gcc_flag = -nostdlib -fno-builtin -g -Wall -march=rv32g -mabi=ilp32
os_elf = os.elf

compile: start.s kernel.c uart.c
	@echo "start to compile..."
	@riscv64-unknown-elf-gcc  ${gcc_flag} -c -o start.o start.s
	@riscv64-unknown-elf-gcc  ${gcc_flag} -c -o kernel.o kernel.c 
	@riscv64-unknown-elf-gcc  ${gcc_flag} -c -o uart.o uart.c 
	@echo "compile done"

link: compile
	@echo "start to link..."
	@riscv64-unknown-elf-gcc ${gcc_flag} -Ttext=0x80000000 -o ${os_elf} start.o kernel.o uart.o
	@echo "link done..."	


debug: link
	@echo "start to debug..."
	@qemu-system-riscv32 -nographic -smp 1 -machine virt -bios none -kernel ${os_elf} -s -S & 
	@riscv64-unknown-elf-gdb -q -ex 'target remote localhost:1234' -ex 'b _start' -ex 'b start_kernel' ${os_elf}
	@echo "debug done"


clean:
	@echo "start to clean..."
	@rm -rf *.o *.exe *.elf *.bin
	@echo "clean done"