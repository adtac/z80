SECTION "start", ROM0
start::
  nop
  jp main

main::
  ld hl, 49152
  ld [hl], 1
  inc hl

  ld [hl], 2

  ld hl, 49152

  ld c, 0

  .loop

    ld a, [hl]
    inc hl
    ld b, a

    ld a, [hl]
    inc hl

    add a, b
    ld [hl], a

    dec hl

    ld a, c
    inc c

    cp 100
    jr .loop

  halt
