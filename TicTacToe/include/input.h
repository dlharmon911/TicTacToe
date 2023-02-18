#ifndef _INPUT_H_
#define _INPUT_H_

#include <allegro5/allegro5.h>
#include <cstdint>

namespace TicTacToe
{
	class Input
	{
	public:
		enum class MouseButton
		{
			Left,
			Right,
			Count
		};

		Input();
		~Input();
		void clear();
		void OnEventAxes(int32_t x, int32_t y);
		void OnEventButtonDown(MouseButton button);
		void OnEventButtonUp(MouseButton button);
		void OnEventKeyDown(int32_t key);
		void OnEventKeyUp(int32_t key);
		int32_t getX() const;
		int32_t getY() const;
		int32_t getButton(MouseButton button) const;
		int32_t getKey(int32_t key) const;

	private:

		bool m_keys[ALLEGRO_KEY_MAX];
		bool m_b[int32_t(MouseButton::Count)];
		int32_t m_x;
		int32_t m_y;
	};
}

#endif // _MOUSE_H_

