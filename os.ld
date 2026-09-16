

OUTPUT_ARCH("riscv")

/**
_start定义在start.c文件中，因为链接脚本看到的都是符号，所以可以直接使用各个文件中定义的各种变量
**/
ENTRY( _start )

/*
定义内存区域
*/
MEMORY
{
	ram   (wxa!ri) : ORIGIN = 0x80000000, LENGTH = 128*1024*1024
}


/*
定义各种节，所有节都加载到名为ram的内存区域中
*/
SECTIONS
{
    .text : {
        PROVIDE(_text_start = .);/* 暴露.text的起始地址给mem.S汇编使用 */
        *(.text .text.*) /* 所有目标文件中，名为.text或者名为.text.xxx这样的节，全都聚拢到这里  */
        PROVIDE(_text_end = .);/* 暴露.text的结束地址给mem.S汇编使用 */
    }>ram

    .rodata : {
        PROVIDE(_rodata_start = .);
        *(.rodata .rodata.*)
        PROVIDE(_rodata_end = .);
    }>ram


    .data : {
        . = ALIGN(4096);
        PROVIDE(_data_start = .);
        *(.data .data.*)
        *(.sdata .sdata.*)
        PROVIDE(_data_end = .);
    }>ram

    .bss : {
        PROVIDE(_bss_start = .);
        *(.bss .bss.*)
        *(.sbss .sbss.*)
        *(COMMON)
        PROVIDE(_bss_end = .);
    } 
}

/*
暴露各种变量给后续用C语言编写的代码使用
*/
PROVIDE(_memory_start = ORIGIN(ram));
PROVIDE(_memory_end = ORIGIN(ram) + LENGTH(ram));
PROVIDE(_heap_start = _bss_end);
PROVIDE(_heap_size = _memory_end - _heap_start);