
ENTRY(_start);
OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm");
OUTPUT_ARCH(arm);

SECTIONS
{
	. = 0x50000000;		/*设置当前段起始地址从0x50000000开始*/
	.text : AT(0x0) {
		__start_obj_start = .;
		start.o(.text)
		__start_obj_end = .;
		*(.text);		/*所有输入文件的.text放在.text中*/
	}

	. = ALIGN(4);
	.rodata : {
		*(.rodata);
	}

	. = ALIGN(4);
	.data : {
		*(.data);
	}

	. = ALIGN(4);
	__bss_start = .;	/*在当前地址设置标签__bss_start*/
	.bss : {
		*(.bss);
	}
	__bss_end = .;		/*在当前地址设置标签__bss_end*/
}
