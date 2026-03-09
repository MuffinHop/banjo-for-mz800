
; banjo sound driver
; Sharp MZ-800 target

; a: number of channels
; l: chips to use
banjo_init_system_call:

    ; no platform-level audio control register is required on MZ-800
    ret