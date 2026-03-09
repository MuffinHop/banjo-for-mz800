; banjo sound driver
; Intel 8253 (PC speaker) support

.include "../banjo_defines_sdas.inc"

.module BANJO_8253

PCSPEAKER_DATA .equ 0xe004
PCSPEAKER_GATE .equ 0xe008

.globl _banjo_init_8253
.globl _banjo_init_sfx_channel_8253, _banjo_mute_all_8253
.globl _banjo_mute_channel_8253
.globl _banjo_update_channels_8253, _banjo_update_channel_8253

.ifdef BANJO_GBDK
    .area _CODE_1 (REL,CON)
.else
    .area _CODE (REL,CON)
.endif

    .include "fnums_8253.inc"
    .include "command_jump_table.inc"
    .include "init.inc"
    .include "mute_unmute.inc"
    .include "note_on_off.inc"
    .include "update.inc"
    .include "update_pitch_registers.inc"
    .include "volume_change.inc"
