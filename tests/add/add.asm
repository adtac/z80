SECTION "start", ROM0
start::
  nop
  jp main

main::
  ld c, 5
  ld b, 5
  ld a, b
  add a, c
  halt
