#ifndef SEGMENTATION_HH
#define SEGMENTATION_HH

#define ADDRESS_BASE  0x00000000
#define ADDRESS_LIMIT 0xFFFFFFFF

typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

struct gdt_ptr {
	uint16_t size;
	uint32_t offset;
}__attribute__((packed));

struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_mid;
	uint8_t access;
    uint8_t lim_flag;
	uint8_t base_high;
}__attribute__((packed));


uint32_t init_GDT(void);
void fill_gdt_entry(struct gdt_entry* entry, uint32_t limit, uint32_t base, uint8_t access, uint8_t flags);

#endif
