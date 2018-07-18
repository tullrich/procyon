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
#ifndef _UNIX_JOYSTICK_H
#define _UNIX_JOYSTICK_H

#include "ProcyonCommon.h"
#include "Platform/Joystick.h"
#include <libevdev.h>

// max number of uploadedable effects supported by this implementation
#define MAX_EFFECTS 16

namespace Procyon {
namespace Unix {

	/*
	================
	UnixJoysticEffect

	Wraps an ff_effect struct providing various helper routines to ease manipulation.
	Intended for internal use by UnixJoystick only.
	================
	*/
	class UnixJoysticEffect
	{
	public:
		UnixJoysticEffect();

		// make this a 'rumble' type effect.
		void SetRumble( ushort strongMag, ushort weakMag );

		// make this a 'periodic' type effect defined over some function.
		void SetPeriodic( ushort period, ushort magnitude, ushort offset, ushort phase );

		// set the direction of the effect.
		void SetDirection( ushort direction );

		// set the repeat properties of this effect.
		void SetReplay( ushort length, ushort delay );

		// returns true if this effect has been uploaded.
		bool IsUploaded() const;

		// get the native id of this effect
		int GetEffectId() const;

		struct ff_effect native;
	};

	/*
	================
	UnixJoystick

	Wraps interaction with a unix evdev joystick device.
	================
	*/
	class UnixJoystick : public IJoystick
	{
	public:
				UnixJoystick( const std::string& devPath );
		virtual	~UnixJoystick();

		// Updates the internal state of this joystick device and returns an event to the caller
		// je is valid only when Poll() returns true.
		virtual bool 	Poll( JoystickInputEvent& je );

		// Return the normalized [-1.0, 1.0] deadzone-corrected value of the provided axis.
		virtual double 	GetAxisValue( SupportedAxes axis ) const;

	protected:
		/*
		================
		JoystickDeviceInfo

		Aggregated joystick info cached from the kmd.
		================
		*/
		struct JoystickDeviceInfo
		{
			JoystickDeviceInfo()
				: vendorId( -1 )
				, firmwareVersion( -1 )
				, productId( -1 )
				, version( -1 )
				, busType( -1 )
			{
			}

			std::string name;
			int 		vendorId;
			int 		firmwareVersion;
			int 		productId;
			int 		version;
			int 		busType;
		};

		/*
		================
		DeadZone

		Precomputed values necessary for axis deadzone correction.
		================
		*/
		struct DeadZone
		{
			DeadZone()
				: flatmin( 0 )
				, flatmax( 0 )
				, realign( 0.0 )
			{
			}

			int 	flatmin;
			int 	flatmax;
			double 	realign;
		};

		/*
		================
		AxisInfo

		Aggregated axis info cached from the kmd.
		================
		*/
		struct AxisInfo
		{
			AxisInfo()
				: max( 0 )
				, min( 0 )
				, fuzz( 0 )
				, flat( 0 )
				, resolution( 0 )
				, value( 0 )
				, normalized( 0.0 )
			{
			}

			int 	max;
			int 	min;
			int 	fuzz;
			int 	flat;
			int 	resolution;

			// axis value is reported in units per millimeter (units/mm)
			int 	value;		// the value of this axis since the last call to Poll()
			double 	normalized; // values normalized to the range [-1.0, 1.0]

			DeadZone deadzone;
		};

		// returns true if this devices meets the hardcoded criteria of a joystick.
		bool 	DeviceIsJoystick();

		// fills a provided AxisInfo with information about an axis indicated by axisCode.
		void 	GatherAxisInfo( AxisInfo& axis, ushort axisCode );

		// fills mInfo and gathers info about all supported mAxes.
		void 	GatherJoystickInfo();

		// uploads or modifies an effect with the kmd.
		void 	AddOrUpdateEffect( UnixJoysticEffect& effect );

		// trigger an uploaded effect.
		void 	TriggerEffect( UnixJoysticEffect& effect );

		// stop an uploaded effect.
		void 	StopEffect( UnixJoysticEffect& effect );

		// set the 'gain'(global magnitude modifier) of this device; must be 0-100.
		void 	SetGain( int gain );

		// set the 'autocenter' (?) of this device; must be 0-100, 0 indicates no-autocenter.
		void 	SetAutocenter( int autocenter );

		// internal event handlers.
		bool 	HandleAxisChanged( const struct input_event& ev, JoystickInputEvent& out );
		bool 	HandleKeyEvent( const struct input_event& ev, JoystickInputEvent& out );
		bool 	HandleEvent( const struct input_event& ev, JoystickInputEvent& out );

		// the evdev handle this device valid throughout UnixJoystick's lifetime.
		struct libevdev* 	mDev;

		// cached info about this device.
		JoystickDeviceInfo 	mInfo;

		// used by Poll() and libevdev to resync data after a SYN_DROPPED event.
		bool 				mSyncDrop;

		// number of effects the device can play at the same time
		int 				mSupportedEffectsCount;

		// effect slots. Only the first mSupportedEffectsCount effects are usable.
		UnixJoysticEffect 	mEffects[ MAX_EFFECTS ];

		// information about each axis (a device must support all of these)
		AxisInfo 			mAxes[ SUPPORTED_AXES_COUNT ];
	};

} /* namespace Unix */
} /* namespace Procyon */

#endif /* _UNIX_JOYSTICK_H */
