#ifndef dLibraryMemoryHeader
#define dLibraryMemoryHeader

#include <allegro5/allegro5.h>
#ifdef USE_FONT
#include <allegro5/allegro_font.h>
#endif
#include <memory>

namespace dLib
{
	template <typename T>
	void default_deleter(T* t) { if (t) delete t; }

	template <> void default_deleter(ALLEGRO_BITMAP* t);
	template <> void default_deleter(ALLEGRO_COND* t);
	template <> void default_deleter(ALLEGRO_CONFIG* t);
	template <> void default_deleter(ALLEGRO_DISPLAY* t);
	template <> void default_deleter(ALLEGRO_EVENT_QUEUE* t);
	template <> void default_deleter(ALLEGRO_FILE* t);
#ifdef USE_FONT
	template <> void default_deleter(ALLEGRO_FONT* t);
#endif
	template <> void default_deleter(ALLEGRO_FS_ENTRY* t);
	template <> void default_deleter(ALLEGRO_MOUSE_CURSOR* t);
	template <> void default_deleter(ALLEGRO_MUTEX* t);
	template <> void default_deleter(ALLEGRO_PATH* t);
	template <> void default_deleter(ALLEGRO_THREAD* t);
	template <> void default_deleter(ALLEGRO_TIMER* t);
	template <> void default_deleter(ALLEGRO_EVENT_SOURCE* t);
	template <> void default_deleter(ALLEGRO_USTR* t);

	template <typename T>
	void null_deleter(T* t) {}

	template< typename T >
	void array_deleter(T* t) { if (t) delete[] t; }
}

#endif // !dLibraryMemoryHeader

