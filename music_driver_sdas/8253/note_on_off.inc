; bc : jump table address (preserve this!)
; de : instruction pointer (preserve this!)
; ix: channel
; iy: music state
mpnl_note_on_8253:

    ; tone lookup starts at A, shift incoming C-based notes to match
    add a, #3

    ; standard note-on setup
    call music_note_on

    jp mpnl_command_done

; bc : jump table address (preserve this!)
; de : instruction pointer (preserve this!)
; ix: channel
; iy: music state
mpnl_note_off_8253:

    ; clear note-on bit and force gate off
    res CHAN_FLAG_BIT_NOTE_ON, channel.flags(ix)

    push bc

    ld bc, #PCSPEAKER_GATE
    xor a, a
    out (c), a

    pop bc

    jp mpnl_command_done_one_byte_command
