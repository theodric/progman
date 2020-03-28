/*
 * Copyright 1998-2007 Decklin Foster <decklin@red-bean.com>.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef AEWM_H
#define AEWM_H

#define VERSION "1.3.12"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xft/Xft.h>
#include <X11/xpm.h>
#include "common.h"
#include "atom.h"

/* Default options */

/* Title bars */
#define DEF_FG "white"
#define DEF_BG "#0000a8"
#define DEF_FG_UNFOCUSED "black"
#define DEF_BG_UNFOCUSED "white"
#define DEF_BUTTON_BG "#c0c7c8"
#define DEF_BEVEL_DARK "#87888f"
#define DEF_BEVEL_LIGHT "white"

/* Borders */
#define DEF_BD "black"

#ifdef HIDPI
#define DEF_XFTFONT "Microsoft Sans Serif:bold:size=14"
#define DEF_BEVEL 2
#define DEF_PAD 6
#define DEF_BW 6
#else
#define DEF_XFTFONT "system:size=13"
#define DEF_BEVEL 2
#define DEF_PAD 3
#define DEF_BW 3
#endif

#define DEF_NEW1 "aemenu --switch"
#define DEF_NEW2 "xterm"
#define DEF_NEW3 "aemenu --launch"
#define DEF_NEW4 "aedesk -1"
#define DEF_NEW5 "aedesk +1"

#define DOUBLE_CLICK_MSEC 250

/* End of options */

#define SubMask (SubstructureRedirectMask|SubstructureNotifyMask)
#define ButtonMask (ButtonPressMask|ButtonReleaseMask)
#define MouseMask (ButtonMask|PointerMotionMask)

#define BW(c) ((c)->decor ? (opt_bw + 2) : 0)
#define GRAV(c) ((c->size.flags & PWinGravity) ? c->size.win_gravity : \
    NorthWestGravity)
#define CAN_PLACE_SELF(t) ((t) == net_wm_type_dock || \
    (t) == net_wm_type_menu || (t) == net_wm_type_splash || \
    (t) == net_wm_type_desk)
#define HAS_DECOR(t) (!CAN_PLACE_SELF(t))
#define IS_ON_CUR_DESK(c) IS_ON_DESK((c)->desk, cur_desk)
#define IS_RESIZE_WIN(c, w) (w == c->resize_nw || w == c->resize_w || \
	w == c->resize_sw || w == c->resize_s || w == c->resize_se || \
	w == c->resize_e || w == c->resize_ne || w == c->resize_n)

#ifdef DEBUG
#define SHOW_EV(name, memb) \
    case name: ev_type = #name; w = e.memb.window; break;
#define SHOW(name) \
    case name: return #name;
#endif

typedef struct geom geom_t;
struct geom {
	long x;
	long y;
	long w;
	long h;
};

typedef struct client client_t;
struct client {
	client_t *next;
	char *name;
	Window win, trans;
	geom_t geom, save;
	Window frame;
	geom_t frame_geom;
	Window close;
	geom_t close_geom;
	Window titlebar;
	geom_t titlebar_geom;
	Window shade;
	geom_t shade_geom;
	Window zoom;
	geom_t zoom_geom;
	Window resize_nw;
	geom_t resize_nw_geom;
	Window resize_n;
	geom_t resize_n_geom;
	Window resize_ne;
	geom_t resize_ne_geom;
	Window resize_e;
	geom_t resize_e_geom;
	Window resize_se;
	geom_t resize_se_geom;
	Window resize_s;
	geom_t resize_s_geom;
	Window resize_sw;
	geom_t resize_sw_geom;
	Window resize_w;
	geom_t resize_w_geom;
	XftDraw *xftdraw;
	XSizeHints size;
	Colormap cmap;
	int ignore_unmap;
	unsigned int focus_order;
	unsigned long desk;
	Bool placed;
#ifdef SHAPE
	Bool shaped;
#endif
	Bool shaded;
	Bool zoomed;
	Bool fullscreen;
	int state;
	Bool decor;
	Bool dock;
	int old_bw;
};

typedef void sweep_func(client_t *, geom_t, int, int, int, int, strut_t *,
    void *);

enum {
	MATCH_WINDOW,
	MATCH_FRAME,
	MATCH_ANY,
};	/* find_client */
enum {
	DEL_WITHDRAW,
	DEL_REMAP,
};	/* del_client */

/* aewm.c */
extern client_t *head, *focused;
extern unsigned int focus_order;
extern int screen;
extern unsigned long cur_desk;
extern unsigned long ndesks;
#ifdef SHAPE
extern Bool shape;
extern int shape_event;
#endif
extern XftFont *xftfont;
extern XftColor xft_fg;
extern XftColor xft_fg_unfocused;
extern Colormap cmap;
extern XColor fg;
extern XColor bg;
extern XColor fg_unfocused;
extern XColor bg_unfocused;
extern XColor button_bg;
extern XColor bevel_dark;
extern XColor bevel_light;
extern XColor bd;
extern GC pixmap_gc;
extern GC invert_gc;
extern Pixmap close_pm;
extern Pixmap close_pm_mask;
extern XpmAttributes close_pm_attrs;
extern Pixmap shade_pm;
extern Pixmap shade_pm_mask;
extern XpmAttributes shade_pm_attrs;
extern Pixmap zoom_pm;
extern Pixmap zoom_pm_mask;
extern XpmAttributes zoom_pm_attrs;
extern Pixmap unzoom_pm;
extern Pixmap unzoom_pm_mask;
extern XpmAttributes unzoom_pm_attrs;
extern Cursor map_curs;
extern Cursor move_curs;
extern Cursor resize_n_curs;
extern Cursor resize_s_curs;
extern Cursor resize_e_curs;
extern Cursor resize_w_curs;
extern Cursor resize_nw_curs;
extern Cursor resize_sw_curs;
extern Cursor resize_ne_curs;
extern Cursor resize_se_curs;
extern char *opt_xftfont;
extern char *opt_fg;
extern char *opt_bg;
extern char *opt_fg_unfocused;
extern char *opt_bg_unfocused;
extern char *opt_button_bg;
extern int opt_bevel;
extern char *opt_bevel_dark;
extern char *opt_bevel_light;
extern char *opt_bd;
extern int opt_bw;
extern int opt_pad;
extern char *opt_new[];
extern void sig_handler(int signum);
extern int exitmsg[2];

/* event.c */
extern void event_loop(void);
extern int handle_xerror(Display *dpy, XErrorEvent *e);
extern int ignore_xerror(Display *dpy, XErrorEvent *e);
#ifdef DEBUG
extern void show_event(XEvent e);
#endif

/* client.c */
extern client_t *new_client(Window w);
extern client_t *find_client(Window w, int mode);
extern client_t *top_client(void);
extern client_t *prev_focused(void);
extern void map_client(client_t *);
extern int titlebar_height(client_t *c);
extern void recalc_frame(client_t *c);
extern int set_wm_state(client_t *c, unsigned long state);
extern void check_states(client_t *c);
extern void parse_state_atom(client_t *, Atom);
extern void send_config(client_t *c);
extern void redraw_frame(client_t *c);
extern void collect_struts(client_t *, strut_t *);
#ifdef SHAPE
extern void set_shape(client_t *c);
#endif
extern void del_client(client_t *c, int mode);

/* ui.c */
extern void user_action(client_t *c, Window win, int x, int y, int button,
    int down);
extern int pos_in_frame(client_t *c, int x, int y);
extern Cursor cursor_for_resize_win(client_t *c, Window win);
extern void focus_client(client_t *c);
extern void move_client(client_t *c);
extern void resize_client(client_t *c, Window resize_pos);
extern void iconify_client(client_t *c);
extern void uniconify_client(client_t *c);
extern void shade_client(client_t *c);
extern void unshade_client(client_t *c);
extern void fullscreen_client(client_t *c);
extern void unfullscreen_client(client_t *c);
extern void zoom_client(client_t *c);
extern void unzoom_client(client_t *c);
extern void send_wm_delete(client_t *c);
extern void goto_desk(int new_desk);
extern void map_if_desk(client_t *c);
extern int sweep(client_t *c, Cursor curs, sweep_func cb, void *cb_arg,
    strut_t *s);
extern void recalc_map(client_t *c, geom_t orig, int x0, int y0, int x1,
    int y1, strut_t *s, void *arg);
extern void recalc_move(client_t *c, geom_t orig, int x0, int y0, int x1,
    int y1, strut_t *s, void *arg);
extern void recalc_resize(client_t *c, geom_t orig, int x0, int y0, int x1,
    int y1, strut_t *s, void *arg);
#ifdef DEBUG
extern void dump_name(client_t *c, const char *label, char flag);
extern void dump_win(Window w, const char *label, char flag);
extern void dump_info(client_t *c);
extern void dump_geom(client_t *c, const char *label);
extern void dump_removal(client_t *c, int mode);
extern void dump_clients(void);
#endif
#endif	/* AEWM_H */
