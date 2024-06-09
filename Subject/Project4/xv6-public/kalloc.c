// Physical memory allocator, intended to allocate
// memory for user processes, kernel stacks, page table pages,
// and pipe buffers. Allocates 4096-byte pages.

#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "spinlock.h"

void freerange(void *vstart, void *vend);
extern char end[]; // first address after kernel loaded from ELF file
                   // defined by the kernel linker script in kernel.ld

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  int use_lock;
  struct run *freelist;
  uint num_free_pages; //store free page number
  uint page_refc[PHYSTOP >> PGSHIFT]; //page reference count
} kmem;

// Initialization happens in two phases.
// 1. main() calls kinit1() while still using entrypgdir to place just
// the pages mapped by entrypgdir on free list.
// 2. main() calls kinit2() with the rest of the physical pages
// after installing a full page table that maps them on all cores.
void
kinit1(void *vstart, void *vend)
{
  initlock(&kmem.lock, "kmem");
  kmem.use_lock = 0;
  freerange(vstart, vend);
}

void
kinit2(void *vstart, void *vend)
{
  freerange(vstart, vend);
  kmem.use_lock = 1;
}

void
freerange(void *vstart, void *vend)
{
  char *p;
  p = (char*)PGROUNDUP((uint)vstart);
  for(; p + PGSIZE <= (char*)vend; p += PGSIZE) {
    kmem.page_refc[V2P(p) >> PGSHIFT] = 0;
    kfree(p);
  }
}
//PAGEBREAK: 21
// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(char *v)
{
  struct run *r;

  if((uint)v % PGSIZE || v < end || V2P(v) >= PHYSTOP)
    panic("kfree");

  // Fill with junk to catch dangling refs.
  if(kmem.use_lock)
    acquire(&kmem.lock);
  r = (struct run*)v;

  // decrease reference count of page
  if(kmem.page_refc[V2P(v) >> PGSHIFT] > 0)         
    kmem.page_refc[V2P(v) >> PGSHIFT] = kmem.page_refc[V2P(v) >> PGSHIFT] - 1;

  // if no reference -> free page
  if(kmem.page_refc[V2P(v) >> PGSHIFT] == 0){
    // Fill garbage to catch dangling refs.  
    memset(v, 1, PGSIZE);     
    r->next = kmem.freelist;
    kmem.num_free_pages = kmem.num_free_pages + 1;
    kmem.freelist = r;
  }

  if(kmem.use_lock)
    release(&kmem.lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
char*
kalloc(void)
{
  struct run *r;

  if(kmem.use_lock)
    acquire(&kmem.lock);
  r = kmem.freelist;
  if(r) {
    kmem.freelist = r->next;
    kmem.page_refc[V2P((char*)r) >> PGSHIFT] = 1;
    kmem.num_free_pages = kmem.num_free_pages - 1;
  }

  if(kmem.use_lock)
    release(&kmem.lock);
  return (char*)r;
}

void incr_refc(uint pa) {
  //locking
  acquire(&kmem.lock);

  kmem.page_refc[pa >> PGSHIFT] = kmem.page_refc[pa >> PGSHIFT] + 1;

  //unlocking
  release(&kmem.lock);

}

void decr_refc(uint pa) {
  //locking
  acquire(&kmem.lock);

  kmem.page_refc[pa >> PGSHIFT] = kmem.page_refc[pa >> PGSHIFT] - 1;
  
  //unlocing
  release(&kmem.lock);

}


uint get_refc(uint pa) {

  uint count;
  //locking
  acquire(&kmem.lock);

  count = kmem.page_refc[pa >> PGSHIFT];
  
  //unlocking
  release(&kmem.lock);

  return count;
} 


int countfp(void) {

  acquire(&kmem.lock);

  uint num_free_pages = kmem.num_free_pages;
  
  release(&kmem.lock);
  
  return num_free_pages;
}

