
.section .rodata #将下面所有符号都存到.rodata中
/*
将符号设置为global类型，使得外部可见
标签HEAP_START代表一个内存地址，在目标文件中就是一个符号
在这个地址地方开辟一个内存空间，长度4个字节，初始值为符号_heap_start（os.ld中定义）的值，即HEAP_START存的是一个地址的值
在C语言侧使用的时候，得到的HEAP_START的值是一个地址，这个地址中存储了_heap_start的值
*/
.global HEAP_START
HEAP_START: .word  _heap_start
.global HEAP_SIZE
HEAP_SIZE: .word  _heap_size


.global TEXT_START
TEXT_START: .word  _text_start
.global TEXT_END
TEXT_END: .word  _text_end

.global DATA_START
DATA_START: .word  _data_start
.global DATA_END
DATA_END: .word  _data_end


.global RODATA_START
RODATA_START: .word  _rodata_start
.global RODATA_END
RODATA_END: .word  _rodata_end


.global BSS_START
BSS_START: .word  _bss_start 
.global BSS_END
BSS_END: .word  _bss_end 
