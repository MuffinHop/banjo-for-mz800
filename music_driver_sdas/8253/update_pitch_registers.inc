; iy: music_state
; ix: channel we're updating
music_update_pitch_registers_8253:

    ; convert note/effects state into an fnum-style value
    call music_calc_fnum

    ; map note to 8253 counter divisor and apply octave/fraction
    ld hl, #pcspeaker_tone_lookup
    call music_fnum_lookup
    call music_fnum_shift_octave

    ; divisor must not be zero
    ld a, l
    or a, h
    jr nz, mupr8253_write

        ld hl, #1

    mupr8253_write:

    ; write 16-bit divisor: low byte then high byte
    push bc

    ld bc, #PCSPEAKER_DATA

    ld a, l
    out (c), a

    ld a, h
    out (c), a

    pop bc

    ret
