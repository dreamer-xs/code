#include <config.h>

void l1_init(u32 *ttb)
{
	u32 i;
	//虚拟地址等于物理地址: 平板映射;
	//把整个内存地址空间映射了; (程序运行在内存中)
	for(i = 0x40000000; i < 0x80000000; i += 0x100000)
		ttb[i >> 20] = i | 2;

	//把sfr 地址空间映射了;  (printf访问uart的寄存器)
	for(i = 0x10000000; i < 0x14000000; i += 0x100000)
		ttb[i >> 20] = i | 2;

	//0x0 地址也映射了;
	ttb[0x0] = 0x0 | 2;
	ttb[0x20] = (0x20<<20) | 2;
}

void l1_init_cache(u32 *ttb)
{
	u32 i;
	//虚拟地址等于物理地址: 平板映射;
	//把整个内存地址空间映射了; (程序运行在内存中)
	for(i = 0x40000000; i < 0x80000000; i += 0x100000)
		ttb[i >> 20] = i | (3<<2) | 2;

	//把sfr 地址空间映射了;  (printf访问uart的寄存器)
	for(i = 0x10000000; i < 0x14000000; i += 0x100000)
		ttb[i >> 20] = i | 2;

	//0x0 地址也映射了;
	ttb[0x0] = 0x0 | 2;
	ttb[0x20] = (0x20<<20) | 2;
}

void l1_mmap(u32 *ttb, u32 vaddr, u32 paddr)
{
	ttb[vaddr >> 20] = (paddr & 0xfff00000) | 2;
}

void l2_mmap(u32 *ttb, u32 *ttb_l2, u32 vaddr, u32 paddr)
{
	ttb[vaddr >> 20] = (u32)ttb_l2 | 1;
	ttb_l2[(vaddr & 0xff000) >> 12] = (paddr & 0xfffff000) | 2;
}

void mmu_enable(u32 *ttb)
{

	asm __volatile__(
		"mcr p15, 0, r0, c8, c7, 0\n\t"			//clear tlb
		"mcr p15, 0, %0, c3, c0, 0\n\t"			//设置domain身份为Manager;
		"mcr p15, 0, %1, c2, c0, 0\n\t"			//设置ttb首地址给TTBR0
		"mrc p15, 0, r0, c1, c0, 0\n\t"
		"orr r0, r0, #1\n\t"					//使能mmu, SCTL.M
		"mcr p15, 0, r0, c1, c0, 0\n\t"
		:
		: "r" (0xffffffff), "r" (ttb)
		: "r0"
	);	
}

void mmu_disable(void)
{
	asm __volatile__(
		"mrc p15, 0, r0, c1, c0, 0\n\t"
		"bic r0, r0, #1\n\t"                    //禁止mmu, SCTL.M
		"mcr p15, 0, r0, c1, c0, 0\n\t"
		::: "r0"
	);
}

void set_ttb(u32 *ttb)
{
	asm __volatile__(
		"mcr p15, 0, %0, c2, c0, 0\n\t"
		:
		: "r" (ttb)
	);
}

void clear_tlb(void)
{
	asm __volatile__(
		"mcr p15, 0, r0, c8, c7, 0\n\t"
	);
}

void enable_cache()
{
	asm __volatile__(
		"mrc p15, 0, r0, c1, c0, 0\n\t"
		"orr r0, #(1<<2)\n\t"
		"orr r0, #(1<<12)\n\t"
		"mcr p15, 0, r0, c1, c0, 0\n\t"
		:
		:
		: "r0"
	);
}

