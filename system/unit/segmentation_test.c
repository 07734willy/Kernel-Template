#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <segmentation.h>

int main() {
	struct gdt_entry GDT[1];
	struct gdt_entry2 GDT2[1];
	struct gdt_entry_access access;
	struct gdt_entry_flags flags = {0, 1, 1};
	access = (struct gdt_entry_access){0, 1, 0, 1, 1, 0, 1};
	fill_gdt_entry(GDT, ADDRESS_LIMIT, ADDRESS_BASE, access, flags);
	fill_gdt_entry2(GDT2, ADDRESS_LIMIT, ADDRESS_BASE, 0x9A, 0xC);
	printf("Size2: %d %d\n", sizeof(struct gdt_entry_access), sizeof(struct gdt_entry_flags));
	printf("Sizes: %d %d\n", sizeof(struct gdt_entry), sizeof(struct gdt_entry2));
	printf("Equal: %d\n", memcmp(GDT, GDT+1, sizeof(struct gdt_entry)));
}
