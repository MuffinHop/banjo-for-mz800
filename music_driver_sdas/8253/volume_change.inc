music_volume_change_8253:

    ; calculate effective 0..15 volume including master + macros
    ld a, music_state.master_volume(iy)
    ld c, #0x0f
    call music_calculate_volume_0x0f

    ; gate speaker on for any audible level, off at zero
    or a, a
    jr z, mvc8253_off

        push bc

        ld bc, #PCSPEAKER_GATE
        ld a, #0x01
        out (c), a

        pop bc

        jp music_volume_change_8253_done

    mvc8253_off:

        push bc

        ld bc, #PCSPEAKER_GATE
        xor a, a
        out (c), a

        pop bc

        jp music_volume_change_8253_done
