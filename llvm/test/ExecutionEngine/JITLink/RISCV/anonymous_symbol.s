# REQUIRES: asserts
# RUN: llvm-mc -triple=riscv64 -filetype=obj -o %t %s
# RUN: llvm-jitlink -debug-only=jitlink -num-threads=0 -noexec %t 2>&1 \
# RUN:              | FileCheck %s
#
# Because of the exist of cfi directive, sections like eh_frame section will be emitted
# in llvm's object code emission phase. Anonymous symbols will also be emitted to indicate
# the section start and section end. So that by relocating these symbol, the section length
# can be calculated.
#
# CHECK: Creating defined graph symbol for ELF symbol ".L0 "
# CHECK: Creating defined graph symbol for ELF symbol "main"
        .text
        .globl main
        .p2align 2
        .type main,@function
main:
        .cfi_startproc
        ret
        .Lfunc_end0:
        .size main, .Lfunc_end0-main
        .cfi_endproc
