/*
 * !!!move this function in owne header file!!!
 */
static inline u16 get_ds(){
	u16 seg;
	asm volatile("movw %%ds, %0" : "=rm" (seg));
	return seg;
}


/*
 * gdt_pointer struct
 */
struct gdt_ptr {
	u16 size;
	u32 address;
}__attribute__((packed));

struct gdt_entry {
	u16 lower_limit;
	u16 lower_base;
	u8 middel_base;
	u8 access;
	u8 granualirty;
	u8 upper_base;
}__attribute__((packed));

/*
 * create the gdt entry
 */


typedef unsigned int uint32_t;

struct gdtEntryAccess {
    uint32_t c0: 2;
    uint32_t sz: 1;
    uint32_t gr: 1;
} __attribute__((packed))

struct gdtEntryFlags {
    uint32_t ac: 1;
    uint32_t rw: 1;
    uint32_t dc: 1;
    uint32_t ex: 1;
    uint32_t c1: 1;
    uint32_t privl: 2;
    uint32_t pr: 1;
} __attribute__((packed))

struct gdtEntry {
    uint32_t limit: 16;
    uint32_t baseLow: 16;
    uint32_t bowMid: 8;
    struct gdtEntryAccess access;
    uint32_t limit: 4;
    struct gdtEntryFlags flags;
    uint32_t baseHigh: 8;
} __attribute__((packed))


static void setup_gdt(){



	//variable containing all bytes
	u64 gdt_init[] __attribute__((aligned 16))={
	
		[1] = gdt


	}
	
	static struct gdt_ptr gdt;

	gdt.size = sizeof(gdt_ptr) - 1;
	gdt.address = (u64)&gdt + get_ds() * 16;

	asm volatile("lgdt %0" : : "m" (gdt));
}
