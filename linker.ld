ENTRY(_start)

SECTIONS
{
    /* Starts at LOADER_ADDR. */
    . = 0x80000;
    /* For AArch64, use . = 0x80000; */
    __start = .;
    __text_start = .;
    .text :
    {
        KEEP(*(.text.boot))
        *(.text)
    }
    . = ALIGN(4096); /* align to page size */
    __text_end = .;

    __rodata_start = .;
    .rodata :
    {
        *(.rodata)
    }
    . = ALIGN(4096); /* align to page size */
    __rodata_end = .;

    __data_start = .;
    .data :
    {
        *(.data)
    }
    . = ALIGN(4096); /* align to page size */
    __data_end = .;

    __init_array_start = .;
    .init_array :
    {
        *(.init_array)
    }
    __init_array_end = .;
    . = ALIGN(4096); /* align to page size */

    __bss_start = .;
    .bss :
    {
        bss = .;
        *(.bss*)
    }
    . = ALIGN(4096); /* align to page size */
    __bss_end = .;
    __bss_size = __bss_end - __bss_start;
    __end = .;
    _ZN4Heap16g_main_heap_dataE = .;
    /*
   /DISCARD/ : { *(.comment) *(.gnu*) *(.note*) *(.eh_frame*) }
   */
}
