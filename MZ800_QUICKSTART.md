# Sharp MZ-800 Quickstart
Yo! Do note the 8253/PC Speaker on MZ-800 and 700, uses frequency 1 100 000 Hz, not what IBM PC uses. This is important info for your musician, remember to set the custom clock rate ;) 
## 1. Build the MZ-800 driver libs

```sh
cd music_driver_sdas
./build_music_driver_sdas.sh
```

- `lib/mz800/banjo.rel`
- `lib/mz800/banjo_sn.rel`
- `lib/mz800/banjo_8253.rel`
- `lib/mz800/banjo_queue.rel` (optional, queue API)

## 2. Convert your song

```sh
python3 furnace2json.py -o my_song.json my_song.fur
python3 json2sms.py -i my_song --mz800 -a MUSIC -o my_song.asm my_song.json
```

`--mz800` means:

- SDAS-compatible output
- Non-banked metadata (`bank = 0`)
- Song symbol exported as `_my_song`

`-a MUSIC` puts the converted data in `.area _MUSIC` so you can place song data independently from player code.

## 3. Assemble song data into an object

```sh
sdasz80 -g -o my_song.rel music_driver_sdas/banjo_defines_sdas.inc my_song.asm
```

## 4. Link required objects in your project

Minimum:

- `lib/mz800/banjo.rel`
- `lib/mz800/banjo_sn.rel`
- `lib/mz800/banjo_8253.rel`
- `my_song.rel`

If you want queued song/sfx playback:

- `lib/banjo_sfx.rel`
- `lib/mz800/banjo_queue.rel`

## 5. Place player and song at fixed addresses

For demoscene builds, treat linker area bases as your `.org` control:

- `_MAIN`: your own program code
- `_CODE`: Banjo player code (`banjo*.rel`)
- `_MUSIC`: converted song data (`my_song.rel`, from `-a MUSIC`)
- `_DATA`: driver RAM state

Example with `sdldz80` command file (`link.lk`):

```txt
-b _MAIN=0x7000
-b _CODE=0x9000
-b _MUSIC=0xB000
-b _DATA=0xC000

main.rel
lib/mz800/banjo.rel
lib/mz800/banjo_sn.rel
lib/mz800/banjo_8253.rel
my_song.rel
```

```sh
sdldz80 -f link.lk -m -i -o demo
```

If you link through `sdcc` instead of calling `sdldz80` directly, pass the same bases as linker flags:

```sh
sdcc ... -Wl-b_MAIN=0x7000 -Wl-b_CODE=0x9000 -Wl-b_MUSIC=0xB000 -Wl-b_DATA=0xC000 ...
```

## 6. Minimal Z80 integration 

```asm
.include "music_driver_sdas/banjo_defines_sdas.inc"

.globl _banjo_check_hardware
.globl _banjo_init
.globl _banjo_play_song
.globl _banjo_update_song
.globl _my_song

.area _DATA
_song_channels: .ds _sizeof_channel * (CHAN_COUNT_SN + CHAN_COUNT_8253)

.area _MAIN
start:
    call _banjo_check_hardware

    ; A = max channels available to driver
    ; L = chips to initialize
    ld a, #(CHAN_COUNT_SN + CHAN_COUNT_8253)
    ld l, #BANJO_HAS_SN|BANJO_HAS_8253
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

## 7. Where

- Song data (`my_song.asm`): `_MUSIC` if `-a MUSIC` is used, otherwise default code area
- Driver state (`_song_state`, etc.): RAM inside Banjo libs
- `_song_channels`: RAM to allocate in your own demo/game