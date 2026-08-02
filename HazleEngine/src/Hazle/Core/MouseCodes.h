#pragma once

namespace Hazle
{
	using MouseCode = uint16_t;

	namespace Mouse
	{
		enum : MouseCode
		{
			Button0 = 0, // left mouse button
			Button1 = 1, // right mouse button
			Button2 = 2, // middle mouse button
			Button3 = 3, // extra mouse button 1
			Button4 = 4, // extra mouse button 2
			Button5 = 5, // extra mouse button 3
			Button6 = 6, // extra mouse button 4
			Button7 = 7, // extra mouse button 5

			ButtonLast = Button7, // last mouse button
			ButtonLeft = Button0, // left mouse button
			ButtonRight = Button1, // right mouse button
			ButtonMiddle = Button2 // middle mouse button
		};
	}
}