	include "includes.i"

	xdef _custom
	xdef _spriteMask
	xdef _spriteFrameBuffer
	
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
	jsr	_game_init
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

	include "os.i"

	if TRACKLOADER=0
	section data_c
	endif
	align 4
_spriteBitplanes:
	incbin	"out/sprite.bin"

spriteMask
	incbin	"out/sprite-mask.bin"
_spriteMask:
	dc.l	spriteMask
_spriteFrameBuffer:
	dc.l	_spriteBitplanes

	include "out/fade_in.s"

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