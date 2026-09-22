#include "types.h"
#include "io.h"

extern uint32_t HEAP_START;
extern uint32_t HEAP_SIZE;
extern uint32_t TEXT_START;
extern uint32_t TEXT_END;
extern uint32_t DATA_START;
extern uint32_t DATA_END;
extern uint32_t RODATA_START;
extern uint32_t RODATA_END;
extern uint32_t BSS_START;
extern uint32_t BSS_END;

static uint32_t _heap_start_aligned;
static uint32_t _alloc_start;
static uint32_t _alloc_end;
static uint32_t _number_of_meta_pages;
static uint32_t _number_of_usable_pages;

#define PAGE_SIZE = 4096


/*
第0位：页是否使用
第1位：是否为某段连续内存的结尾
第2位：是否为某段连续内存的开始（暂不实现）
*/
struct PageDescriptor
{
    uint8_t flags;
};

/*
地址4k对齐
*/
static inline uint32_t _align_page(uint32_t address)
{
    uint32_t mask = (1 << 12) - 1;
    return (address + mask) & (~mask);
}

static inline uint32_t ceil(uint32_t number, uint32_t div)
{
    return number % div == 0 ? number / div : number / div + 1;
}

static inline uint32_t floor(uint32_t number, uint32_t div)
{
    return number % div == 0 ? number / div : number / div;
}

static inline void init_meta()
{
    // 初始化meta区，全都置为0
    for (int i = _heap_start_aligned; i < _alloc_start; i++)
    {
        *(uint8_t *)i = 0;
        // printf("addr=%p v=%d", i, *(uint8 *)i);
    }
}

/*
需要完成以下事情：
1，4k对齐，
2，计算出meta区和usable区的起始
2，初始化meta区（usable区的大小无需在意）
各种参数的具体计算过程见笔记：https://github.com/youth7/myblog/blob/master/mds/C/write_os_with_c_in_days/day-04.md
*/
void page_init()
{
    printf("==============heap init start===========================\n");
    // 堆按照4096对齐
    _heap_start_aligned = _align_page(HEAP_START);
    _number_of_meta_pages = floor(HEAP_SIZE - (_heap_start_aligned - HEAP_START), 4096 * 4097);
    _number_of_usable_pages = _number_of_meta_pages * 4096;
    _alloc_start = _heap_start_aligned + _number_of_meta_pages * 4096;
    _alloc_end = _alloc_start + _number_of_usable_pages * 4096;
    init_meta();
    struct PageDescriptor t = {1};
    printf("size of descripter %d\n", sizeof(t));
    printf("HEAP_START: %x\n", HEAP_START);
    printf("HEAP_SIZE: %x\n", HEAP_SIZE);
    printf("_heap_start_aligned: %p\n", _heap_start_aligned);
    printf("_number_of_meta_pages: %x\n", _number_of_meta_pages);
    printf("_number_of_usable_pages: %x\n", _number_of_usable_pages);
    printf("_alloc_start: %x\n", _alloc_start);
    printf("_alloc_end: %x\n", _alloc_end);
    printf("TEXT:   %p -> %p\n", TEXT_START, TEXT_END);
    printf("RODATA: %p -> %p\n", RODATA_START, RODATA_END);
    printf("DATA:   %p -> %p\n", DATA_START, DATA_END);
    printf("BSS:    %p -> %p\n", BSS_START, BSS_END);
    printf("HEAP:   %p -> %p\n", _heap_start_aligned, _alloc_end);
}

void *page_alloc(int number_of_page)
{
}

void page_free(void *addr)
{
}
