#include "io.h"
#include "types.h"

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

#define PAGE_SIZE 4096
#define PAGE_USED ((uint32_t)1 << 0)
#define PAGE_LAST ((uint32_t)1 << 1)

/*
第0位：页是否使用
第1位：是否为某段连续内存的结尾
第2位：是否为某段连续内存的开始（暂不实现）
*/
struct PageDescriptor {
  uint8_t flags;
};

/*
地址4k对齐
*/
static inline uint32_t _align_page(uint32_t address) {
  uint32_t mask = (1 << 12) - 1;
  return (address + mask) & (~mask);
}

static inline uint32_t ceil(uint32_t number, uint32_t div) { return number % div == 0 ? number / div : number / div + 1; }

static inline uint32_t floor(uint32_t number, uint32_t div) { return number % div == 0 ? number / div : number / div; }

static inline void init_meta() {
  // 初始化meta区，全都置为0
  for (int i = _heap_start_aligned; i < _alloc_start; i++) {
    *(uint8_t *)i = 0;
  }
}

static int is_used(struct PageDescriptor *p) {
  // 判断第0位是否为1,
  return p->flags & (1 << 0);
}

static int is_last(struct PageDescriptor *p) {
  // 判断第1位是否为1
  return p->flags & (1 << 1);
}

static inline uint32_t find_Continuous_mem_block_from_here(uint32_t first_avaiable, uint32_t n) {
  struct PageDescriptor *p = (struct PageDescriptor *)(_heap_start_aligned + first_avaiable);
  uint32_t seq = 0;
  for (; seq < n; seq++) {
    // printf("seq=%x p=%p \n", seq, p);
    if (is_used(p))
      return -1;
    p++;
  }
  return first_avaiable;
}

static inline void set_flags(struct PageDescriptor *p, uint8_t flags) {
  //   printf("p=%p flags=%x\n", p, flags);
  p->flags |= flags;
  //   printf("p=%p flags=%x\n", p, flags);
}

static inline void set_descriptors_flags(uint32_t start_addr, uint32_t size, uint8_t flags) {
  struct PageDescriptor *p = (struct PageDescriptor *)start_addr;
  for (int i = 0; i < size; i++) {
    set_flags(p, flags);
    p++;
  }
}

static inline void mark_used(uint32_t start_addr, uint32_t size) {
  // 将meta区中连续的内存标记为已使用
  set_descriptors_flags(start_addr, size, 1 << 0);
  // 最后一个描述符要设置为last
  struct PageDescriptor *p = (struct PageDescriptor *)start_addr;
  p += size - 1;
  set_flags(p, PAGE_LAST);
}

void *page_alloc(int required_page) {
  // 这是一个非常低效的内存分配算法，每次都要扫描meta区，但我们这里只是为了演示，
  printf("start to alloc memory for %d------------------------------------\n", required_page);
  for (int i = 0; i < _number_of_usable_pages - required_page; i++) {
    //   printf("i=%d _number_of_usable_pages=%d required_page = %d \n", i, _number_of_usable_pages, required_page);
    struct PageDescriptor *p = (struct PageDescriptor *)(_heap_start_aligned + i);
    if (is_used(p)) {
      // printf("first is used for index =%x\n", i);
      continue;
    } else {
      printf("first is ok for index =%x\n", i);
    }
    uint32_t found = find_Continuous_mem_block_from_here(i, required_page);
    if (found == -1) {
      printf("no available memory for address i=%x\n", i);
      continue;;
    } else {
      mark_used(found + _heap_start_aligned, required_page);
      uint32_t addr = _alloc_start + found * PAGE_SIZE;
      printf("found available memory , meta index is %d, heap addr is %p\n", found, addr);
      return (void *)addr;
    }
  }
}


void page_free(void *_addr) {
  uint32_t addr = (uint32_t)_addr;
  // 理论上来说，传入的地址必须是某段内存的首地址我们才能对它进行释放，但这里只是演示因此没有做严谨。
  if (addr < _alloc_start || addr > _alloc_end) {
    printf("addr=%p is invalid, can not free it\n", addr);
    return;
  }
  printf("start to free addr %p ~~~~~~~~~~~~~~~~~~~~~~~~\n", addr);
  uint32_t seq_of_meta = (addr - _alloc_start) / PAGE_SIZE;
  struct PageDescriptor *p = (struct PageDescriptor *)(_heap_start_aligned + seq_of_meta);
  printf("seq_of_meta = %d , addr = %p ~~~~~~~~~~~~~~~~~~~~~~~~\n", seq_of_meta, p);
  int i = 0;
  while (1) {
    if (is_last(p)) {
      p->flags = 0;
      //   printf("found last, addr=%p, i=%d\n", p, i);
      return;
    } else {
      //   printf("not last, addr=%p, i=%d\n", p, i);
      p->flags = 0;
      p++;
    }
  }
  return;
}


void page_test(void) {
  void *p = page_alloc(3);
  page_alloc(14);
  page_alloc(15);
  page_free(p);
  page_alloc(2);
}


/*
需要完成以下事情：
1，4k对齐，
2，计算出meta区和usable区的起始
2，初始化meta区
各种参数的具体计算过程见笔记：https://github.com/youth7/myblog/blob/master/mds/C/write_os_with_c_in_days/day-04.md
*/
void page_init() {
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
  page_test();
}