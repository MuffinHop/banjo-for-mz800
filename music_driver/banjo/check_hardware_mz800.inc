
; banjo sound driver
; Sharp MZ-800 target

; check for presence of hardware and set bits in the banjo_has_chips
; variable, corresponding to what was found
banjo_check_hardware:

    ; the MZ-800 target uses an external SN76489 at fixed I/O
    ld a, BANJO_HAS_SN
    ld (banjo_has_chips), a

    ; clear system flags (no GG/Mark III/System E semantics)
    xor a, a
    ld (banjo_system_flags), a

    ret