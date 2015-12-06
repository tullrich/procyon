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

namespace Procyon {

	/*
	================
	Singleton::Singleton
	================
	*/
	template< typename T1, typename T2 >
	Singleton< T1, T2 >::Singleton( InstancePtr existing )
		: WiredService( T1::Class() )
		, mSingleInstance( existing )
	{
	}

	/*
	================
	Singleton::Singleton
	================
	*/
	template< typename T1, typename T2 >
	Singleton< T1, T2 >::Singleton()
		: WiredService( T1::Class() )
	{
	}

	/*
	================
	Singleton::PreAllocate
	================
	*/
	template< typename T1, typename T2 >
	void Singleton< T1, T2 >::PreAllocate() 
	{
		if ( !mSingleInstance )
		{
			mSingleInstance = std::make_shared< T1 >();
		} 
	};

	/*
	================
	Singleton::GetOrAllocate
	================
	*/
	template< typename T1, typename T2 >
	void* Singleton< T1, T2 >::GetOrAllocate()
	{
		if( !mSingleInstance )
			mSingleInstance = std::make_shared< T1 >();

		return static_cast< void* >( mSingleInstance.get() );
	} 

	/*
	================
	Factory::Factory
	================
	*/
	template< typename T1, typename T2 >
	Factory< T1, T2 >::Factory()
		: WiredService( T1::Class() )
	{
	} 

	/*
	================
	Factory::GetOrAllocate
	================
	*/
	template< typename T1, typename T2 >
	void* Factory< T1, T2 >::GetOrAllocate()
	{
		return static_cast< void* >( new T1() );
	}

	/*
	================
	TypeCatalog::TypeCatalog
	================
	*/
	template< typename... Wires >
	TypeCatalog::TypeCatalog( Wires... wires )
	{
    	AddServices_R( wires... );
	}

	/*
	================
	TypeCatalog::Resolve
	================
	*/
	template< typename T >
	T* TypeCatalog::Resolve()
	{
		WireIterator search;
		if( ( search = mWires.find( T::Class() ) ) == mWires.end() )
		{
			PROCYON_WARN( "IOC", "Unable to find concrete implementation of %s"
				, T::Class()->GetName() );
			return NULL;
		}

		IWiredServicePtr wire = search->second;
		return static_cast< T* >( wire->GetOrAllocate() );
	}

	/*
	================
	TypeCatalog::AllocateSingletons
	================
	*/
	void TypeCatalog::AllocateSingletons()
	{
		for( auto service : mServices )
		{
			service.second->PreAllocate();
		}
	}

	/*
	================
	TypeCatalog::ConnectWireToService
	================
	*/
	template< typename T1, typename T2 >
	void TypeCatalog::ConnectWireToService( IWiredServicePtr service )
	{
		// See if this type has already been implemented
		if( mServices.find(  T2::Class() ) != mServices.end() )
		{
			PROCYON_ERROR( "IOC", "Attempting to wire class %s to %s when it is a concrete service."
				,  T2::Class()->GetName(), T1::Class()->GetName() );
			throw std::invalid_argument( T1::Class()->GetName() );
		}

		WireIterator search;
		if( ( search = mWires.find( T2::Class() ) ) != mWires.end() )
		{
			IWiredServicePtr factory = search->second;

			if( factory->GetClass() != T1::Class() )
			{
				PROCYON_WARN( "IOC", "Attempting to wire class %s to %s when it is already wired"
					" to %s."
					, T2::Class()->GetName()
					, T1::Class()->GetName()
					, factory->GetClass()->GetName() );
			}
			throw std::invalid_argument( T1::Class()->GetName() );
		}

		mWires.emplace_hint( search, T2::Class(), service );

		PROCYON_DEBUG( "IOC", "Class %s implemented by %s."
			, T2::Class()->GetName(), T1::Class()->GetName() );
	}

	/*
	================
	TypeCatalog::ConnectWiresToService_R
	================
	*/
	template< typename T1, typename T2, typename... Interfaces >
	void TypeCatalog::ConnectWiresToService_R( IWiredServicePtr service )
	{
		ConnectWireToService< T1, T2 >( service );
		ConnectWiresToService_R<T1, Interfaces... >( service );
	}

	/*
	================
	TypeCatalog::AddService
	================
	*/
	template< template<typename, typename> class ServiceType, typename T1, typename... Interfaces >
	void TypeCatalog::AddService( ServiceType< T1, Implements< Interfaces... > > s )
	{
		typedef ServiceType< T1, Implements< Interfaces... > > ServiceClass;

		WireIterator search;
		if( ( search = mWires.find( T1::Class() ) ) != mWires.end() )
		{
			IWiredServicePtr& wire = search->second;

			PROCYON_ERROR( "IOC", "Attempting to add service %s that has already been wired to"
				" another (%s).", T1::Class()->GetName(), wire->GetClass()->GetName() );
			throw std::invalid_argument( T1::Class()->GetName() );
		}

		IWiredServicePtr service 
			= std::make_shared< ServiceClass >( s );

		mWires[ T1::Class() ] = service;
		mServices[ T1::Class() ] = service;

		ConnectWiresToService_R< T1, Interfaces... >( service );
	}

	/*
	================
	TypeCatalog::AddServices_R
	================
	*/
    template< template<typename, typename> class ServiceType
 		, typename T1, typename... Interfaces, typename... Services >
    void TypeCatalog::AddServices_R( ServiceType< T1, Implements< Interfaces... > > s
    	, Services... restServices )
    {
    	AddService( s );
    	AddServices_R( restServices... );
    }

} /* namespace Procyon */