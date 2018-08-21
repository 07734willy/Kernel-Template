#include <segmentation.h>
#define GDT_SIZE 9

extern void refresh_segs(uint32_t ptr);
//extern void refresh_segs(void);

#define DEFAULT

static struct gdt_entry GDT[GDT_SIZE];
static struct gdt_ptr gp;

void fill_gdt_entry(struct gdt_entry* entry, uint32_t limit, uint32_t base, uint8_t access, uint8_t flags) {
	entry->limit_low = limit & 0xFFFF;
	entry->base_low = base & 0xFFFF;
	entry->base_mid = (base >> 16) & 0xFF;
	entry->access = access;
	entry->lim_flag = ((limit >> 16) & 0xF) | ((flags << 4) & 0xF0);
	entry->base_high = (base >> 24) & 0xFF;
}

uint32_t init_GDT(void) {
	fill_gdt_entry(GDT + 0, ADDRESS_LIMIT, ADDRESS_BASE, 0, 0);
	fill_gdt_entry(GDT + 1, ADDRESS_LIMIT, ADDRESS_BASE, 0x9A, 0xC);
	fill_gdt_entry(GDT + 2, ADDRESS_LIMIT, ADDRESS_BASE, 0x92, 0xC);

	gp.size = (sizeof(struct gdt_entry) * GDT_SIZE - 1) & 0xFFFF;
	gp.offset = (uint32_t) &GDT;

	/*asm volatile("lgdt %0"
		:
		:
		"r" ((int)&gp));
	*/
	//refresh_segs();
	return (uint32_t)&gp;
	//refresh_segs((uint32_t)&gp);
}
