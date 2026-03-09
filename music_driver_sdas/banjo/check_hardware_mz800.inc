
; banjo sound driver
; Sharp MZ-800 target

; check for presence of hardware and set bits in the banjo_has_chips
; variable, corresponding to what was found
_banjo_check_hardware:

    ; the MZ-800 target uses an external SN76489 and the built-in 8253 speaker
    ld a, #BANJO_HAS_SN|BANJO_HAS_8253
    ld (_banjo_has_chips), a

    ; clear system flags (no GG/Mark III/System E semantics)
    xor a, a
    ld (_banjo_system_flags), a

    ret