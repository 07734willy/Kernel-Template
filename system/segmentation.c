#include <inttypes.h>
#include <segmentation.h>

#define GDT_SIZE 9
#define ADDRESS_BASE  0x00000000
#define ADDRESS_LIMIT 0xFFFFFFFF

struct gdt_ptr {
	uint16_t size;
	uint32_t offset;
}__attribute__((packed));

extern void load_gdt(struct gdt_ptr* ptr);

struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_mid;
	uint8_t access;
    uint8_t lim_flag;
	uint8_t base_high;
}__attribute__((packed));

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

void init_GDT(void) {
	fill_gdt_entry(GDT + 0, ADDRESS_LIMIT, ADDRESS_BASE, 0, 0);
	fill_gdt_entry(GDT + 1, ADDRESS_LIMIT, ADDRESS_BASE, 0x9A, 0xC);
	fill_gdt_entry(GDT + 2, ADDRESS_LIMIT, ADDRESS_BASE, 0x92, 0xC);

	gp.size = (sizeof(struct gdt_entry) * GDT_SIZE - 1) & 0xFFFF;
	gp.offset = (uint32_t) &GDT;

	load_gdt(&gp);
}
