#include "memory.h"

template <> void dLib::default_deleter(ALLEGRO_BITMAP* t)		{ if (t) al_destroy_bitmap(t); }
template <> void dLib::default_deleter(ALLEGRO_COND* t)			{ if (t) al_destroy_cond(t); }
template <> void dLib::default_deleter(ALLEGRO_CONFIG* t)		{ if (t) al_destroy_config(t); }
template <> void dLib::default_deleter(ALLEGRO_DISPLAY* t)		{ if (t) al_destroy_display(t); }
template <> void dLib::default_deleter(ALLEGRO_EVENT_QUEUE* t)	{ if (t) al_destroy_event_queue(t); }
template <> void dLib::default_deleter(ALLEGRO_FILE* t)			{ if (t) al_fclose(t); }
#ifdef USE_FONT
template <> void dLib::default_deleter(ALLEGRO_FONT* t)			{ if (t) al_destroy_font(t); }
#endif
template <> void dLib::default_deleter(ALLEGRO_FS_ENTRY* t)		{ if (t) al_destroy_fs_entry(t); }
template <> void dLib::default_deleter(ALLEGRO_MOUSE_CURSOR* t) { if (t) al_destroy_mouse_cursor(t); }
template <> void dLib::default_deleter(ALLEGRO_MUTEX* t)		{ if (t) al_destroy_mutex(t); }
template <> void dLib::default_deleter(ALLEGRO_PATH* t)			{ if (t) al_destroy_path(t); }
template <> void dLib::default_deleter(ALLEGRO_THREAD* t)		{ if (t) al_destroy_thread(t); }
template <> void dLib::default_deleter(ALLEGRO_TIMER* t)		{ if (t) al_destroy_timer(t); }
template <> void dLib::default_deleter(ALLEGRO_EVENT_SOURCE* t) { if (t) al_destroy_user_event_source(t); }
template <> void dLib::default_deleter(ALLEGRO_USTR* t)			{ if (t) al_ustr_free(t); }
