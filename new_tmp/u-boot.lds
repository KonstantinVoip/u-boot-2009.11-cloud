OUTPUT_ARCH(powerpc)
PHDRS
{
  text PT_LOAD;
  bss PT_LOAD;
}
SECTIONS
{
  . = + SIZEOF_HEADERS;
  .interp : { *(.interp) }
  .hash : { *(.hash) }
  .dynsym : { *(.dynsym) }
  .dynstr : { *(.dynstr) }
  .rel.text : { *(.rel.text) }
  .rela.text : { *(.rela.text) }
  .rel.data : { *(.rel.data) }
  .rela.data : { *(.rela.data) }
  .rel.rodata : { *(.rel.rodata) }
  .rela.rodata : { *(.rela.rodata) }
  .rel.got : { *(.rel.got) }
  .rela.got : { *(.rela.got) }
  .rel.ctors : { *(.rel.ctors) }
  .rela.ctors : { *(.rela.ctors) }
  .rel.dtors : { *(.rel.dtors) }
  .rela.dtors : { *(.rela.dtors) }
  .rel.bss : { *(.rel.bss) }
  .rela.bss : { *(.rela.bss) }
  .rel.plt : { *(.rel.plt) }
  .rela.plt : { *(.rela.plt) }
  .init : { *(.init) }
  .plt : { *(.plt) }
  .text :
  {
    *(.text)
    *(.got1)
   } :text
    _etext = .;
    PROVIDE (etext = .);
    .rodata :
   {
    *(.eh_frame)
    *(SORT_BY_ALIGNMENT(SORT_BY_NAME(.rodata*)))
  } :text
  .fini : { *(.fini) } =0
  .ctors : { *(.ctors) }
  .dtors : { *(.dtors) }
  . = (. + 0x00FF) & 0xFFFFFF00;
  _erotext = .;
  PROVIDE (erotext = .);
  .reloc :
  {
    *(.got)
    _GOT2_TABLE_ = .;
    *(.got2)
    _FIXUP_TABLE_ = .;
    *(.fixup)
  }
  __got2_entries = (_FIXUP_TABLE_ - _GOT2_TABLE_) >> 2;
  __fixup_entries = (. - _FIXUP_TABLE_) >> 2;
  .data :
  {
    *(.data)
    *(.data1)
    *(.sdata)
    *(.sdata2)
    *(.dynamic)
    CONSTRUCTORS
  }
  _edata = .;
  PROVIDE (edata = .);
  . = .;
  __u_boot_cmd_start = .;
  .u_boot_cmd : { *(.u_boot_cmd) }
  __u_boot_cmd_end = .;
  . = .;
  __start___ex_table = .;
  __ex_table : { *(__ex_table) }
  __stop___ex_table = .;
  . = ALIGN(256);
  __init_begin = .;
  .text.init : { *(.text.init) }
  .data.init : { *(.data.init) }
  . = ALIGN(256);
  __init_end = .;
  .bootpg 0x1107fffc - 0xffc :
  {
  start.o (.bootpg)
  } :text = 0xffff
  .resetvec 0x1107fffc :
  {
    *(.resetvec)
  } :text = 0xffff
  . = 0x1107fffc + 0x4;
  __bss_start = .;
  .bss (NOLOAD) :
  {
   *(.sbss) *(.scommon)
   *(.dynbss)
   *(.bss)
   *(COMMON)
  } :bss
  . = ALIGN(4);
  _end = . ;
  PROVIDE (end = .);
}
