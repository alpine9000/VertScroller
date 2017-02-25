	include "includes.i"
	if USE_GCC=1
	xdef _memset
	endif
	xdef _custom
	xdef _spriteMask
	xdef _spriteFrameBuffer
	xdef _menuFrameBuffer
	xdef _scoreBoardFrameBuffer
	
	if TRACKLOADER=1
byteMap:
	dc.l	Entry
	dc.l	endCode-byteMap
	endif

	include "wbstartup.i"		; does nothing if TRACKLOADER=1
	
Entry:
	if TRACKLOADER=0
	jmp 	StartupFromOS
	else
	lea	userstack,a7	
	endif

Main:
	jsr	_init_amiga
	jsr	_game_loop

	if TRACKLOADER=0
QuitGame:
	IntsOff
	jsr	_hw_waitVerticalBlank		
	movem.l	d0-a6,-(sp)
	jsr	P61_End
	movem.l	(sp)+,d0-a6
	jmp	LongJump
	endif

	if USE_GCC=1
	if 0
	cnop    0,4	
__memset:	
	movem.l .l387,-(a7)
	move.l  (8+.l389,a7),d1
	move.l  (12+.l389,a7),d0
	move.l  (4+.l389,a7),a1
	tst.l   d0
	beq     .l371
	move.l  a1,a0
.l370
	move.b  d1,(a0)+
	subq.l  #1,d0
	bne     .l370
.l371
	move.l  a1,d0
.l387   reg
.l389   equ     0
        rts
	else
	cnop    0,4
_memset:
	movem.l	a1,-(sp)
	movem.l	l383,-(a7)
	move.l	a0,a1
	tst.l	d1
	beq	l367
l366
	move.b	d0,(a0)+
	subq.l	#1,d1
	bne	l366
l367
	move.l	a1,d0
l383	reg
l385	equ	0
	movem.l	(sp)+,a1
	rts
	endif
	
	endif ; USE_GCC

	align 4
	include "os.i"

	if TRACKLOADER=0
	section data_c
	endif
	align 4
_scoreBoardBitplanes:
	incbin  "out/scoreboard.bin"
_spriteBitplanes:
	incbin	"out/sprite.bin"
spriteMask
	incbin	"out/sprite-mask.bin"
_spriteMask:
	dc.l	spriteMask
_menuFrameBuffer:
	dc.l	_menuBitplanes
_scoreBoardFrameBuffer:
	dc.l	_scoreBoardBitplanes	
_spriteFrameBuffer:
	dc.l	_spriteBitplanes

	include "out/fade_in.s"
	include "out/menu_fade_in.s"	

	section .noload
	cnop 0,512
_menuBitplanes
	incbin	"out/menu.bin"
	cnop 0,512
	
	section  .data
	
	align 4
	xdef _player_record
_player_record:
	incbin "recordings/default.raw"

	section data_c
	align 4
	if SFX==1
	xdef _sound_land
_sound_land:	
	incbin "out/jump.raw"
	xdef _sound_falling
_sound_falling:	
	incbin "out/falling.raw"	
	xdef _sound_coin
_sound_coin:	
	incbin "out/coin1.raw"
	xdef _sound_pop
_sound_pop:	
	incbin "out/pop.raw"
	xdef _sound_kill
_sound_kill:	
	incbin "out/kill.raw"		

	endif
	
	align 4
_custom:
	dc.l	CUSTOM

	if TRACKLOADER=0	
	section	bss
	endif
	align 4
	if TRACKLOADER=1
startUserstack:
	ds.b	1000
userstack:
	endif
	end