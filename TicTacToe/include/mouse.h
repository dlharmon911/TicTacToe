#ifndef _MOUSE_H_
#define _MOUSE_H_

#include <cstdint>

namespace TicTacToe
{
	class Mouse
	{
	public:
		Mouse();
		~Mouse();
		void OnEventAxes(int32_t x, int32_t y);
		void OnEventButtonDown(int32_t b);
		void OnEventButtonUp(int32_t b);
		int32_t getX() const;
		int32_t getY() const;
		int32_t getStatus() const;
		void acknowledgeMove ();

		enum Status
		{
			Status_Idle = 0,
			Status_Left_Down = 1,
			Status_Right_Down = 2,
			Status_Moved = 4
		};

	private:

		int32_t m_status;
		int32_t m_x;
		int32_t m_y;
	};
}

#endif // _MOUSE_H_

