# Sharp MZ-800 Quickstart

## 1. Build the MZ-800 driver libs

```sh
cd music_driver_sdas
./build_music_driver_sdas.sh
```

- `lib/mz800/banjo.rel`
- `lib/mz800/banjo_sn.rel`
- `lib/mz800/banjo_8255.rel`
- `lib/mz800/banjo_queue.rel` (optional, queue API)

## 2. Convert your song

```sh
python3 furnace2json.py -o my_song.json my_song.fur
python3 json2sms.py -i my_song --mz800 -o my_song.asm my_song.json
```

`--mz800` means:

- SDAS-compatible output
- Non-banked metadata (`bank = 0`)
- Song symbol exported as `_my_song`

## 3. Assemble song data into an object

```sh
sdasz80 -g -o my_song.rel music_driver_sdas/banjo_defines_sdas.inc my_song.asm
```

## 4. Link required objects in your project

Minimum:

- `lib/mz800/banjo.rel`
- `lib/mz800/banjo_sn.rel`
- `lib/mz800/banjo_8255.rel`
- `my_song.rel`

If you want queued song/sfx playback:

- `lib/banjo_sfx.rel`
- `lib/mz800/banjo_queue.rel`

## 5. Minimal Z80 integration 

```asm
.include "music_driver_sdas/banjo_defines_sdas.inc"

.globl _banjo_check_hardware
.globl _banjo_init
.globl _banjo_play_song
.globl _banjo_update_song
.globl _my_song

.area _DATA
_song_channels: .ds _sizeof_channel * CHAN_COUNT_SN

.area _CODE
start:
    call _banjo_check_hardware

    ; A = max channels available to driver
    ; L = chips to initialize
    ld a, #CHAN_COUNT_SN
    ld l, #BANJO_HAS_SN
    call _banjo_init

    ; HL = pointer to converted song data
    ld hl, #_my_song
    ; D controls initial loop mode (0 off, non-zero on)
    ld d, #BANJO_LOOP_ON
    call _banjo_play_song

main_loop:
    ; call once per frame / vblank
    call _banjo_update_song
    jp main_loop
```

## 6. Where

- Song data (`my_song.asm`): program area
- Driver state (`_song_state`, etc.): RAM inside Banjo libs
- `_song_channels`: RAM to allocate in your own demo/game