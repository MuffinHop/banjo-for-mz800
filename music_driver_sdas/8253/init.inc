; ix: channel
; iy: music state
_banjo_init_8253:

    xor a, a
    ld channel.subchannel(ix), a
    ld channel.port(ix), a

    ; default channel volume is full
    ld channel.volume(ix), #0x0f

    ; ensure speaker is muted until a note is started
    ld bc, #PCSPEAKER_GATE
    xor a, a
    out (c), a

    ; move to next channel slot
    ld de, #_sizeof_channel
    add ix, de

    ret

; ix: channel
; iy: music state
_banjo_init_sfx_channel_8253:

    xor a, a
    ld channel.subchannel(ix), a
    ld channel.port(ix), a

    ; default channel volume is full
    ld channel.volume(ix), #0x0f

    ; ensure speaker is muted until the SFX starts a note
    ld bc, #PCSPEAKER_GATE
    xor a, a
    out (c), a

    ret
