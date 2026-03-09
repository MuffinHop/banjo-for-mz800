; ix: channel
; iy: music state
_banjo_update_channels_8253:

    call music_update_channel_8253

    ret

; ix: channel
; iy: music state
_banjo_update_channel_8253:

    call music_update_channel_8253

    ret

; ix: channel
; iy: music state
music_update_channel_8253:

    ld bc, #music_command_jump_table_8253
    call music_process_new_line

    ; get flags in b for later
    ld b, channel.flags(ix)

    ; only update active (unmuted) notes
    bit CHAN_FLAG_BIT_NOTE_ON, b
    jr z, muc8253_done

        ; process volume macro
        bit CHAN_FLAG_BIT_VOLUME_MACRO, b
        call nz, music_update_volume_macro

        ; process arp macro
        bit CHAN_FLAG_BIT_ARP_MACRO, b
        call nz, music_update_arp_macro

        ; process ex macro
        bit CHAN_FLAG_BIT_EX_MACRO, b
        call nz, music_update_ex_macro

        ; b now has events
        ld b, channel.events(ix)

        ; update pitch when requested
        ld a, #CHAN_EVENT_PITCH_CHANGED|CHAN_EVENT_PITCH_CHANGE_ALWAYS
        and a, b
        call nz, music_update_pitch_registers_8253

        ; update gate state on volume change or master-volume changes
        ld a, b
        or a, music_state.flags(iy)
        and a, #CHAN_EVENT_VOLUME_CHANGE
        jp nz, music_volume_change_8253

        music_volume_change_8253_done:

        ; clear events which only last one frame
        ld a, b
        and a, #CHAN_EVENT_PITCH_CHANGE_ALWAYS|CHAN_EVENT_MACRO_RELEASE|CHAN_EVENT_LEGATO|CHAN_EVENT_TIC_WAIT
        ld channel.events(ix), a

    muc8253_done:

        ; move on to next channel
        ld de, #_sizeof_channel
        add ix, de

        ret
