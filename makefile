
compile: loop.s
	@echo "start to compile..."
	@riscv64-unknown-elf-gcc -nostdlib -fno-builtin -march=rv32g -mabi=ilp32 -g -Wall -Ttext=0x80000000 loop.s  -o start.elf 
	@riscv64-unknown-elf-objcopy -O binary start.elf start.bin	
	@echo "compile done"


debug: compile
	@echo "start to debug..."
	@qemu-system-riscv32 -nographic -smp 1 -machine virt -bios none -kernel start.elf -s -S & 
	@riscv64-unknown-elf-gdb -q -ex 'target remote localhost:1234' -ex 'b _start'  -ex 'display/z $$a0'   start.elf
	@echo "debug done"

run: compile
	@echo "start to run..."
	@qemu-system-riscv32 -nographic -smp 1 -machine virt -bios none -kernel start.elf 
	@echo "run done"

clean:
	@echo "start to clean..."
	@rm -rf start.*
	@echo "clean done"