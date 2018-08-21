#define GDT_SIZE 9
#define ADDRESS_BASE  0x00000000
#define ADDRESS_LIMIT 0xFFFFFFFF

typedef unsigned int uint32_t;

extern void refresh_segs(uint32_t ptr);
//extern void refresh_segs(void);

struct gdt_ptr {
	uint32_t size: 16;
	uint32_t offset: 32;
}__attribute__((packed));

//#define DEFAULT
#ifdef DEFAULT
struct gdt_entry_flags {
    uint32_t c0: 2;
    uint32_t sz: 1;
    uint32_t gr: 1;
}__attribute__((packed));

struct gdt_entry_access {
    uint32_t ac: 1;
    uint32_t rw: 1;
    uint32_t dc: 1;
    uint32_t ex: 1;
    uint32_t c1: 1;
    uint32_t privl: 2;
    uint32_t pr: 1;
}__attribute__((packed));
#endif

struct gdt_entry {
    uint32_t limitLow: 16;
    uint32_t baseLow: 16;
    uint32_t baseMid: 8;
    #ifdef DEFAULT
	struct gdt_entry_access access;
	#else
	uint32_t access: 8;
	#endif
    uint32_t limitHigh: 4;
    #ifdef DEFAULT
	struct gdt_entry_flags flags;
	#else
	uint32_t flags: 4;
    #endif
	uint32_t baseHigh: 8;
}__attribute__((packed));

static struct gdt_entry GDT[GDT_SIZE];
static struct gdt_ptr gp;

#ifdef DEFAULT
void fill_gdt_entry(struct gdt_entry* entry, uint32_t limit, uint32_t base, struct gdt_entry_access access, struct gdt_entry_flags flags) {
#else
void fill_gdt_entry(struct gdt_entry* entry, uint32_t limit, uint32_t base, uint32_t access, uint32_t flags) {
#endif
	entry->limitLow = limit & 0xFFFF;
	entry->baseLow = base & 0xFFFF;
	entry->baseMid = (base >> 16) & 0xFF;
	entry->access = access;
	entry->limitHigh = (limit >> 16) & 0xF;
	entry->flags = flags;
	entry->baseHigh = (base >> 24) & 0xFF;
}

void init_GDT(void) {
	#ifdef DEFAULT
	struct gdt_entry_access access = {0, 0, 0, 0, 1, 0, 0};
	struct gdt_entry_flags flags = {0, 1, 1};
	fill_gdt_entry(GDT + 0, ADDRESS_LIMIT, ADDRESS_BASE, access, flags);
	access = (struct gdt_entry_access){0, 1, 0, 1, 1, 0, 1};
	fill_gdt_entry(GDT + 1, ADDRESS_LIMIT, ADDRESS_BASE, access, flags);
	access = (struct gdt_entry_access){0, 1, 0, 0, 1, 0, 1};
	fill_gdt_entry(GDT + 2, ADDRESS_LIMIT, ADDRESS_BASE, access, flags);
	#else
	fill_gdt_entry(GDT + 0, ADDRESS_LIMIT, ADDRESS_BASE, 0, 0);
	fill_gdt_entry(GDT + 0, ADDRESS_LIMIT, ADDRESS_BASE, 0x9A, 0xC);
	fill_gdt_entry(GDT + 0, ADDRESS_LIMIT, ADDRESS_BASE, 0x92, 0xC);
	#endif

	gp.size = (sizeof(struct gdt_entry) * GDT_SIZE - 1) & 0xFFFF;
	gp.offset = (uint32_t) &GDT;

	/*asm volatile("lgdt %0"
		:
		:
		"r" ((int)&gp));
	*/
	//refresh_segs();
	refresh_segs((uint32_t)&gp);
}
