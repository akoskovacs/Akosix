#ifndef MULTIBOOT_H
#define MULTIBOOT_H

struct multiboot_elf_section {
    uint32_t num;
    uint32_t size;
    uint32_t addr;
    uint32_t shndx;
} __packed;

struct multiboot_info {
	uint32_t flags;
	uint32_t mem_lower;
	uint32_t mem_upper;
	uint32_t boot_device;
    uint32_t cmdline;
    uint32_t mods_count;
    uint32_t mods_addr;
    struct multiboot_elf_section elf_sec;
	uint32_t mmap_length;
	uint32_t mmap_addr;
    uint32_t drives_length;
    uint32_t drivers_addr;
    uint32_t config_table;
	uint32_t boot_loader_name;
	uint32_t apm_table;
	uint32_t vbe_control_info;
	uint32_t vbe_mode_info;
	uint32_t vbe_mode;
    uint16_t vbe_interface_seg;
    uint16_t vbe_interface_off;
    uint16_t vbe_interface_len;
} __packed;


struct multiboot_mmap_entry {
    uint32_t size;
    uint64_t addr;
    uint64_t length;
#define MB_MEMORY_AVAILABLE 1
#define MB_MEMORY_RESERVED 2
    uint32_t type;
} __packed;

#endif // MULTIBOOT_H
