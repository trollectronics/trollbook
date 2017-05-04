#ifndef _BIOS_H_
#define _BIOS_H_

typedef struct BiosInfo BiosInfo;
struct BiosInfo {
	unsigned char		*font;
	int			term_x;
	int			term_y;
	int			def_fg;
	int			def_bg;

	int			vsync_clock;
};

#define	BIOS_INFO_ADDR		((volatile BiosInfo *) 0xDDC00)

#endif
