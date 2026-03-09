; b: ix + channel.events
; ix: channel
; iy: music state
_banjo_mute_all_8253:
_banjo_mute_channel_8253:

    ; gate speaker off
    ld bc, #PCSPEAKER_GATE
    xor a, a
    out (c), a

    ret
