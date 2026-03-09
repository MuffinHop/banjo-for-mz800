
; banjo sound driver
; Sharp MZ-800 target

PCSPEAKER_GATE .equ 0xe008

; a: number of channels
; l: chips to use
_banjo_init_system_call:

    ; default speaker gate state is muted until a channel requests output
    ld bc, #PCSPEAKER_GATE
    xor a, a
    out (c), a

    ; no other platform-level audio control register is required on MZ-800
    ret
