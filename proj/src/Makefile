# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/lib
.PATH: ${.CURDIR}/lib/devices/mouse
.PATH: ${.CURDIR}/lib/devices/keyboard
.PATH: ${.CURDIR}/lib/devices/kbc
.PATH: ${.CURDIR}/lib/devices/timer
.PATH: ${.CURDIR}/lib/devices/RTC
.PATH: ${.CURDIR}/lib/devices/sp
.PATH: ${.CURDIR}/lib/devices/video_card
.PATH: ${.CURDIR}/graphics
.PATH: ${.CURDIR}/assets
.PATH: ${.CURDIR}/menu
.PATH: ${.CURDIR}/game
.PATH: ${.CURDIR}/game/moving_entities

# source code files to be compiled
SRCS = proj.c video.c sprite.c utils.c  mouse.c keyboard.c kbc.c timer.c submarine.c rtc.c queue.c sp.c handler.c animatedEntity.c mainPlayer.c entity.c game.c spriteCache.c cannon.c alphabet.c enemy.c hook.c bullet.c fish.c trash.c leaderboard.c round_manager.c shop.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CPPFLAGS += -pedantic -O2 
# CPPFLAGS += -pedantic -O2 -D __LCOM_OPTIMIZED__

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF} ${LIBLM} ${LIBXPM}
LDADD += -llcf -llm -lxpm

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
