/*
===========================================================================

Procyon, a 2D game.

Copyright (C) 2015 Tim Ullrich.

This file is part of Procyon.

Procyon is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Procyon is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Procyon.  If not, see <http://www.gnu.org/licenses/>.

===========================================================================
*/
#ifndef _JOYSTICK_H
#define _JOYSTICK_H

#include "ProcyonCommon.h"
#include "KeyCodes.h"

namespace Procyon {

	/*
	================
	SupportedAxes

	Enumeration of required axes whose value may be queryed on a IJoystick device.
	================
	*/
	enum SupportedAxes
	{
		AXIS_LEFT_STICK_X,
		AXIS_LEFT_STICK_Y,
		AXIS_RIGHT_STICK_X,
		AXIS_RIGHT_STICK_Y,
		AXIS_LEFT_TRIGGER,
		AXIS_RIGHT_TRIGGER,

		SUPPORTED_AXES_COUNT
	};

	/*
	================
	JoystickEventType

	Enumeration of possible JoystickInputEvent types.
	================
	*/
	enum JoystickEventType
	{
		JOY_EVENT_KEY_DOWN,
		JOY_EVENT_KEY_UP,
		JOY_EVENT_AXIS
	};

	/*
	================
	JoystickInputEvent

	Events data received via IJoystick::Poll().
	================
	*/
	struct JoystickInputEvent
	{
		JoystickEventType type;

		union
		{
			struct // JOY_EVENT_KEY_DOWN and JOY_EVENT_KEY_UP
			{
				ProcyonKeyCode key;
			};
			struct // JOY_EVENT_AXIS
			{
				SupportedAxes 	axis;
				int 			value;
				double 			normalized;
			};
		};
	};
	
	/*
	================
	IJoystick

	Represents a single open joystick (gamepad) input device. Implementation is platform defined.
	================
	*/
	class IJoystick
	{
	public:
		virtual 		~IJoystick() { }

		// Updates the internal state of this joystick device and returns an event to the caller
		// je is valid only when Poll() returns true.
		virtual bool 	Poll( JoystickInputEvent& je ) = 0;

		// Return the normalized [-1.0, 1.0] deadzone-corrected value of the provided axis.
		virtual double 	GetAxisValue( SupportedAxes axis ) const = 0;
	};

	/*
	================
	Joystick_Open

	Attempts to open a device path as a joystick. Currently throws if there is in error.
	The returned implementation is platform defined.
	================
	*/
	IJoystick* Joystick_Open( const std::string& devPath );
	
} /* namespace Procyon */

#endif /* _JOYSTICK_H */
