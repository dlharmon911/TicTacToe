#ifndef dLibraryCommonHeader
#define dLibraryCommonHeader

#define USE_FONT
//#define USE_NATIVE_DIALOG

#include <allegro5/allegro5.h>
#ifdef USE_FONT
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#endif
#ifdef USE_NATIVE_DIALOG
#include <allegro5/allegro_native_dialog.h>
#endif
#include "memory.h"

// this is a C++ workaround to the C Allegro objects

namespace dLib
{
	template <typename T>
	class ptype_object
	{
	public:
		ptype_object() : m_data() {}

		ptype_object(T* data, bool manage = true) : m_data(data, (manage ? dLib::default_deleter<T> : null_deleter<T>)) {}

		ptype_object(const ptype_object& object) : m_data(object.m_data) {}

		virtual ~ptype_object() { this->reset(); }

		explicit operator bool() const
		{
			return static_cast<bool>(this->m_data);
		}

		bool is_valid() const
		{
			return static_cast<bool>(this->m_data);
		}

		ptype_object& operator = (const ptype_object& object)
		{
			this->m_data = object.m_data;
			return *this;
		}

		void reset()
		{
			this->m_data.reset();
		}

		void reset(T* data, bool manage = true)
		{
			this->m_data.reset(data, (manage ? default_deleter<T> : null_deleter<T>));
		}

		T* get() const
		{
			return this->m_data.get();
		}

		operator T* () const
		{
			return this->m_data.get();
		}

	protected:
		std::shared_ptr<T> m_data;
	};

	template <typename T>
	class rtype_object
	{
	public:
		rtype_object() : m_data() {}

		rtype_object(const T& data) : m_data(data) {}

		rtype_object(const rtype_object& object) : m_data(object.m_data) {}

		virtual ~rtype_object() {}

		rtype_object& operator = (const rtype_object& object)
		{
			this->m_data = object.m_data;
			return *this;
		}

		void reset(const T& object) { this->m_data = object; }

		const T& get() const
		{
			return this->m_data;
		}

		T& get()
		{
			return this->m_data;
		}

		operator T& ()
		{
			return this->m_data;
		}

		operator const T& () const
		{
			return this->m_data;
		}

	protected:
		T m_data;
	};
}

namespace Allegro
{
	typedef typename dLib::ptype_object<ALLEGRO_BITMAP> Bitmap;
	typedef typename dLib::ptype_object<ALLEGRO_COND> Condition;
	typedef typename dLib::ptype_object<ALLEGRO_CONFIG> Config;
	typedef typename dLib::ptype_object<ALLEGRO_DISPLAY> Display;
	typedef typename dLib::ptype_object<ALLEGRO_EVENT_QUEUE> EventQueue;
	typedef typename dLib::ptype_object<ALLEGRO_FILE> File;

#ifdef ALLEGRO_FONT
	typedef typename dLib::ptype_object<ALLEGRO_FONT> Font;
#endif

	typedef typename dLib::ptype_object<ALLEGRO_FS_ENTRY> FileSystemEntry;
	typedef typename dLib::ptype_object<ALLEGRO_MOUSE_CURSOR> MouseCursor;
	typedef typename dLib::ptype_object<ALLEGRO_MUTEX> Mutex;
	typedef typename dLib::ptype_object<ALLEGRO_PATH> Path;
	typedef typename dLib::ptype_object<ALLEGRO_THREAD> Thread;
	typedef typename dLib::ptype_object<ALLEGRO_TIMER> Timer;
	typedef typename dLib::ptype_object<ALLEGRO_EVENT_SOURCE> UserEventSource;
#ifdef USE_NATIVE_DIALOG
	typedef typename dLib::ptype_object <ALLEGRO_MENU> Menu;
#endif
	typedef typename dLib::ptype_object<ALLEGRO_USTR> UString;

	typedef typename dLib::rtype_object<ALLEGRO_COLOR> Color;
	typedef typename dLib::rtype_object<ALLEGRO_DISPLAY_MODE> DisplayMode;
	typedef typename dLib::rtype_object<ALLEGRO_EVENT> Event;
	typedef typename dLib::rtype_object<ALLEGRO_FILE_INTERFACE> FileInterface;
	typedef typename dLib::rtype_object<ALLEGRO_FS_INTERFACE> FileSystemInterface;
	typedef typename dLib::rtype_object<ALLEGRO_JOYSTICK> Joystick;
	typedef typename dLib::rtype_object<ALLEGRO_JOYSTICK_STATE> JoystickState;
	typedef typename dLib::rtype_object<ALLEGRO_KEYBOARD_STATE> KeyboardState;
	typedef typename dLib::rtype_object<ALLEGRO_LOCKED_REGION> LockedRegion;
	typedef typename dLib::rtype_object<ALLEGRO_MEMORY_INTERFACE> MemoryInterface;
	typedef typename dLib::rtype_object<ALLEGRO_MOUSE_STATE> MouseState;
	typedef typename dLib::rtype_object<ALLEGRO_STATE> State;
	typedef typename dLib::rtype_object<ALLEGRO_TIMEOUT> Timeout;
	typedef typename dLib::rtype_object<ALLEGRO_TRANSFORM> Transform;
	typedef typename dLib::rtype_object<ALLEGRO_USER_EVENT> UserEvent;
	typedef typename dLib::rtype_object<ALLEGRO_USTR_INFO> StringInfo;
}

#endif // !dLibraryCommonHeader
