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
#include "UnixJoystick.h"
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

namespace Procyon {
namespace Unix {

	// evdev event code corresponding to each element of the SupportedAxes enum.
	static ushort sEvAbsCodes[ SUPPORTED_AXES_COUNT ]
		= { ABS_X, ABS_Y, ABS_RX, ABS_RY, ABS_Z, ABS_RZ };

	/*
	================
	EvAbsCodeToAxisIndex

	Convert an evdev EV_ABS code to an index into mAxes or -1 if the code indicates an unsupported
	axis. If this function does not return -1, the result may be safely cast to a SupportedAxes.
	================
	*/
	static int EvAbsCodeToAxisIndex( ushort code )
	{
		switch( code )
		{
			case ABS_X: 	return (int)AXIS_LEFT_STICK_X;
			case ABS_Y: 	return (int)AXIS_LEFT_STICK_Y;
			case ABS_RX:	return (int)AXIS_RIGHT_STICK_X;
			case ABS_RY: 	return (int)AXIS_RIGHT_STICK_Y;
			case ABS_Z: 	return (int)AXIS_LEFT_TRIGGER;
			case ABS_RZ: 	return (int)AXIS_RIGHT_TRIGGER;
			default: 		return -1;
		}
	}

	/*
	================
	EvKeyCodeToProcyonJoyButton

	Convert an evdev EV_KEY code to an ordinal in ProcyonJoyButton or -1 if the code indicates an
	unsupported key. If this function does not return -1, the result may be safely cast to a ProcyonKeyCode.
	================
	*/
	static ProcyonJoyButton EvKeyCodeToProcyonJoyButton( ushort code )
	{
		switch( code )
		{
			case BTN_NORTH: 	return JOY_BTN_NORTH;
			case BTN_EAST: 		return JOY_BTN_EAST;
			case BTN_WEST: 		return JOY_BTN_WEST;
			case BTN_SOUTH: 	return JOY_BTN_SOUTH;
			case BTN_THUMBL: 	return JOY_BTN_THUMBL;
			case BTN_THUMBR: 	return JOY_BTN_THUMBR;
			case BTN_TL: 		return JOY_BTN_TOPL;
			case BTN_TR: 		return JOY_BTN_TOPR;
			case BTN_SELECT: 	return JOY_BTN_SELECT;
			case BTN_START: 	return JOY_BTN_START;
			case BTN_MODE: 		return JOY_BTN_MODE;
			default: 			return JOY_BTN_UNKNOWN;
		}
	}

	/*
	================
	EvTypeToString

	Convert an evdev event type to a human readable string.
	================
	*/
	static const char* EvTypeToString( ushort type )
	{
		return libevdev_event_type_get_name( type ); // relying on libevdev to do the job here.
	}

	/*
	================
	EvTypeToString

	Convert an evdev event code to a human readable string.
	================
	*/
	static const char* EvCodeToString( ushort type, ushort code )
	{
		return libevdev_event_code_get_name( type, code ); // and here
	}

	UnixJoysticEffect::UnixJoysticEffect()
	{
		native.id = -1; // indicates not yet uploaded
	}

	void UnixJoysticEffect::SetRumble( ushort strongMag, ushort weakMag )
	{
		native.type = FF_RUMBLE;
		native.u.rumble.strong_magnitude = strongMag;
		native.u.rumble.weak_magnitude   = weakMag;
	}

	void UnixJoysticEffect::SetPeriodic( ushort period, ushort magnitude
		, ushort offset, ushort phase )
	{
		native.type = FF_PERIODIC;
		native.u.periodic.waveform = FF_SQUARE;
		native.u.periodic.period = 0.1*0x100;
		native.u.periodic.magnitude = 0x4000;     /* 0.5 * Maximum magnitude */
		native.u.periodic.offset = 0;
		native.u.periodic.phase = 0;
		native.u.periodic.envelope.attack_length = 0x100;
		native.u.periodic.envelope.attack_level  = 0;
		native.u.periodic.envelope.fade_length = 0x100;
		native.u.periodic.envelope.fade_level  = 0;
	}

	void UnixJoysticEffect::SetReplay( ushort length, ushort delay)
	{
		native.replay.length = length;
		native.replay.delay  = delay;
	}

	void UnixJoysticEffect::SetDirection( ushort direction )
	{
		native.direction = direction;
	}

	bool UnixJoysticEffect::IsUploaded() const
	{
		return native.id != -1;
	}

	int UnixJoysticEffect::GetEffectId() const
	{
		return native.id;
	}

	UnixJoystick::UnixJoystick( const std::string& devPath )
		: mDev( NULL )
		, mSyncDrop( false )
		, mSupportedEffectsCount( 0 )
	{
		int fd = open( devPath.c_str(), O_RDWR | O_NONBLOCK );

		if ( fd == -1 || libevdev_new_from_fd( fd, &mDev ) )
		{
			PROCYON_ERROR( "Joystick", "Unable to open joystick device '%s'", devPath.c_str() );
			throw std::runtime_error( "UnixJoystick" );
		}

		if( !DeviceIsJoystick() )
		{
			PROCYON_ERROR( "Joystick", "Device '%s' is not a joystick!", devPath.c_str() );
			throw std::runtime_error( "UnixJoystick" );
		}

		GatherJoystickInfo();
	}

	UnixJoystick::~UnixJoystick()
	{
		close( libevdev_get_fd( mDev ) );
		if( mDev )
			libevdev_free( mDev );
	}

	bool UnixJoystick::DeviceIsJoystick()
	{
		// see https://www.kernel.org/doc/Documentation/input/gamepad.txt
		return libevdev_has_event_type( mDev, EV_KEY ) 	// Must support key presses
			&& libevdev_has_event_type( mDev, EV_ABS )	// Must support absolute axes.

			&& libevdev_has_event_type( mDev, EV_FF )
			&& libevdev_has_event_code( mDev, EV_FF, FF_RUMBLE )
			&& libevdev_has_event_code( mDev, EV_FF, FF_PERIODIC )
			&& libevdev_has_event_code( mDev, EV_FF, FF_SQUARE )
			&& libevdev_has_event_code( mDev, EV_FF, FF_TRIANGLE )
			&& libevdev_has_event_code( mDev, EV_FF, FF_SINE )

			// Game pad buttons (must support all 4 for now)
			&& libevdev_has_event_code( mDev, EV_KEY, BTN_NORTH )
			&& libevdev_has_event_code( mDev, EV_KEY, BTN_WEST )
			&& libevdev_has_event_code( mDev, EV_KEY, BTN_SOUTH )
			&& libevdev_has_event_code( mDev, EV_KEY, BTN_EAST )

			// Thumb clicks
			&& libevdev_has_event_code( mDev, EV_KEY, BTN_THUMBL )
			&& libevdev_has_event_code( mDev, EV_KEY, BTN_THUMBR )

			// Shoulder buttons
			&& libevdev_has_event_code( mDev, EV_KEY, BTN_TL )
			&& libevdev_has_event_code( mDev, EV_KEY, BTN_TR )

			// Must support both Left and Right sticks
			&& libevdev_has_event_code( mDev, EV_ABS, ABS_X ) 	// Left stick x-axis
			&& libevdev_has_event_code( mDev, EV_ABS, ABS_Y ) 	// Left stick y-axis
			&& libevdev_has_event_code( mDev, EV_ABS, ABS_RX ) 	// Right stick x-axis
			&& libevdev_has_event_code( mDev, EV_ABS, ABS_RY ) 	// Right stick y-axis

			// Must support two triggers.
			&& libevdev_has_event_code( mDev, EV_ABS, ABS_Z  ) 	// Left trigger
			&& libevdev_has_event_code( mDev, EV_ABS, ABS_RZ  )  // Right triger

			// Analog dpad (vs. digital reported as BTN_DPAD_*, NYI)
			&& libevdev_has_event_code( mDev, EV_ABS, ABS_HAT0X )	// Dpad neg is left pos is right
			&& libevdev_has_event_code( mDev, EV_ABS, ABS_HAT0Y )	// Dpad neg is up pos is down.

			// Center digital buttons
			&& libevdev_has_event_code( mDev, EV_KEY, BTN_SELECT ) 	// Left center digital btn
			&& libevdev_has_event_code( mDev, EV_KEY, BTN_START )  	// Right center digital btn
			&& libevdev_has_event_code( mDev, EV_KEY, BTN_MODE );	// 'Branded' center digital btn
	}

	void UnixJoystick::GatherAxisInfo( UnixJoystick::AxisInfo& axis, ushort axisCode )
	{
		const struct input_absinfo* native = libevdev_get_abs_info( mDev, axisCode );
		axis.max 		= native->maximum;
		axis.min 		= native->minimum;
		axis.fuzz 		= native->fuzz;
		axis.flat 		= 4000;//native->flat;
		axis.resolution = native->resolution;

		// precompute values for deadzone correction
		axis.deadzone.flatmin = (axis.max + axis.min) - 2 * axis.flat;
		axis.deadzone.flatmax = (axis.max + axis.min) + 2 * axis.flat;
		axis.deadzone.realign =
			(double)( axis.max - axis.min ) / (double)( (axis.max - axis.min) - 4 * axis.flat );

		PROCYON_DEBUG( "Joystick", "AxisInfo: %s, Range: [%i, %i] Fuzz: %i Flat: %i Res: %i"
			, EvCodeToString( EV_ABS, axisCode ), axis.min, axis.max
			, axis.fuzz, axis.flat, axis.resolution );
	}

	void UnixJoystick::GatherJoystickInfo()
	{
		// gather joystick device info
		mInfo.name 				= libevdev_get_name( mDev );
		mInfo.vendorId 			= libevdev_get_id_vendor( mDev );
		mInfo.firmwareVersion 	= libevdev_get_id_version( mDev );
		mInfo.productId 		= libevdev_get_id_product( mDev );
		mInfo.busType 			= libevdev_get_id_bustype( mDev );
		mInfo.version 			= libevdev_get_driver_version( mDev );

		PROCYON_DEBUG( "Joystick", "Name '%s' Vendor %d Product %d BusType %d Firmware %d Version %d"
			, mInfo.name.c_str(), mInfo.vendorId, mInfo.productId, mInfo.busType
			, mInfo.firmwareVersion, mInfo.version );

		// gather info about each joystick axis we care about.
		for ( int i = 0; i < SUPPORTED_AXES_COUNT; i++ )
		{
			GatherAxisInfo( mAxes[ i ], sEvAbsCodes[ i ] );
		}

		// Get the max number of supported haptic effects.
		// see https://www.kernel.org/doc/Documentation/input/ff.txt
		int numEffects;
		if ( ioctl( libevdev_get_fd( mDev ), EVIOCGEFFECTS, &numEffects ) == -1 )
		{
			PROCYON_ERROR( "Joystick", "Unable to query joystick max effect count.");
			throw std::runtime_error( "UnixJoystick" );
		}
		mSupportedEffectsCount = glm::clamp( numEffects, 0, MAX_EFFECTS );
		PROCYON_DEBUG( "Joystick", "Number supported effects: %i", numEffects );
	}

	void UnixJoystick::AddOrUpdateEffect( UnixJoysticEffect& effect )
	{
		if ( ioctl( libevdev_get_fd( mDev ), EVIOCSFF, &effect.native ) == -1 )
			PROCYON_ERROR( "Joystick", "Error uploading effect." );
	}

	void UnixJoystick::TriggerEffect( UnixJoysticEffect& effect )
	{
		if ( !effect.IsUploaded() )
			return;

		struct input_event play;
		play.type = EV_FF;
		play.code =  effect.GetEffectId();
		play.value = 1; // play: 1, stop: 0
		write( libevdev_get_fd( mDev ), (const void*) &play, sizeof(play) );
	}

	void UnixJoystick::StopEffect( UnixJoysticEffect& effect )
	{
		if ( !effect.IsUploaded() )
			return;

		struct input_event play;
		play.type = EV_FF;
		play.code =  effect.GetEffectId();
		play.value = 0; // play: 1, stop: 0
		write( libevdev_get_fd( mDev ), (const void*) &play, sizeof(play) );
	}

	// Gain 0 - 100
	void UnixJoystick::SetGain( int gain )
	{
		struct input_event ie;
		ie.type = EV_FF;
		ie.code = FF_GAIN;
		ie.value = 0xFFFFUL * glm::clamp( gain, 0, 100 ) / 100;

		if ( write( libevdev_get_fd( mDev ), &ie, sizeof( ie ) ) == -1 )
			PROCYON_ERROR( "Joystick", "Error setting gain." );
	}

	// Autocenter 0 - 100, 0 means no autocenter.
	void UnixJoystick::SetAutocenter( int autocenter )
	{
		struct input_event ie;
		ie.type = EV_FF;
		ie.code = FF_AUTOCENTER;
		ie.value = 0xFFFFUL * autocenter / 100;

		if ( write( libevdev_get_fd( mDev ), &ie, sizeof( ie ) ) == -1 )
			PROCYON_ERROR( "Joystick", "Error setting autocenter." );
	}

	bool UnixJoystick::HandleAxisChanged( const struct input_event& ev, JoystickInputEvent& out )
	{
		// ignore unless this is a supported axis.
		int ret = EvAbsCodeToAxisIndex( ev.code );
		if ( ret == -1 )
			return false; // no event

		AxisInfo& axis 	= mAxes[ ret ];

		// axis deadzone correct.
		int corrected = ev.value;
		if ( corrected > axis.deadzone.flatmin )
		{
			if (  corrected < axis.deadzone.flatmax )
			{
				corrected = 0;
			}
			else
			{
				corrected -= axis.deadzone.flatmax;
			}
		}
		else
		{
			corrected -= axis.deadzone.flatmin;
		}
		corrected *= axis.deadzone.realign;

		//
		axis.value 		= glm::clamp( corrected, axis.min, axis.max );
		axis.normalized =
			glm::clamp( 2.0 * (double)(axis.value-axis.min) / (axis.max-axis.min) - 1.0
				, -1.0, 1.0 );

		if( ev.code == ABS_X )
		{
			PROCYON_DEBUG( "Joystick", "%i, [%i, %i] %f"
				, axis.value, axis.min, axis.max, axis.normalized );
		}

		out.type 		= JOY_EVENT_AXIS;
		out.axis 		= (SupportedAxes)ret;
		out.value 		= axis.value;
		out.normalized 	= axis.normalized;
		return true;
	}

	bool UnixJoystick::HandleKeyEvent( const struct input_event& ev, JoystickInputEvent& out )
	{
		PROCYON_DEBUG( "Joystick", "HandleEvent Type: %s Code: %s Value: %i"
			, EvTypeToString( ev.type ), EvCodeToString( ev.type, ev.code ), ev.value );

		int ret = EvKeyCodeToProcyonJoyButton( ev.code );
		out.type = ( ev.value == 0 ) ? JOY_EVENT_BTN_UP : JOY_EVENT_BTN_DOWN;
		out.btn = (ProcyonJoyButton)ret;
		return true;
	}

	bool UnixJoystick::HandleEvent( const struct input_event& ev, JoystickInputEvent& out )
	{
		// syn packets hold no data but are protocol required.
		if ( ev.type == EV_SYN )
			return false;

		// handle changes on the axes.
		if ( ev.type == EV_ABS )
		{
			return HandleAxisChanged( ev, out );
		}
		else if ( ev.type == EV_KEY )
		{
			return HandleKeyEvent( ev, out );
		}
		else
		{
			//  https://www.kernel.org/doc/Documentation/input/event-codes.txt
			PROCYON_DEBUG( "Joystick", "HandleEvent Type: %s Code: %s Value: %i"
				, EvTypeToString( ev.type ), EvCodeToString( ev.type, ev.code ), ev.value );
		}

		return false;
	}

	bool UnixJoystick::Poll( JoystickInputEvent& je )
	{
		struct pollfd fds =
		{
			.fd 		= libevdev_get_fd( mDev ),
			.events 	= POLLIN,
			.revents 	= 0
		};

		if ( poll( &fds, 1, 0 ) > 0 || libevdev_has_event_pending( mDev ) )
		{
			unsigned int flags
				= ( !mSyncDrop ) ? LIBEVDEV_READ_FLAG_NORMAL : LIBEVDEV_READ_FLAG_SYNC;

			struct input_event ev;
			int res = 0;
 			while ( ( res = libevdev_next_event( mDev, flags, &ev ) ) >= 0 || mSyncDrop )
 			{
				switch( res )
				{
				case -EAGAIN:
					flags = LIBEVDEV_READ_FLAG_NORMAL;
					mSyncDrop = false;
					break;
				case LIBEVDEV_READ_STATUS_SYNC:
					PROCYON_DEBUG( "Joystick", "LIBEVDEV_READ_STATUS_SYNC" );
					flags = LIBEVDEV_READ_FLAG_SYNC;
					mSyncDrop = true;

				case LIBEVDEV_READ_STATUS_SUCCESS: // intentional fall-through
					if ( HandleEvent( ev, je ) )
					{
						return true;
					}
					break;
				}
 			}
		}

		return false;
	}

	double UnixJoystick::GetAxisValue( SupportedAxes axis ) const
	{
		return mAxes[ axis ].normalized;
	}

} /* namespace Unix */


IJoystick* Joystick_Open( const std::string& devPath )
{
	//TODO try catch here?
	return new Unix::UnixJoystick( devPath );
}

} /* namespace Procyon */
