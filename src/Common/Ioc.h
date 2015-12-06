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

#ifndef _IOC_H
#define _IOC_H

#include "ProcyonCommon.h"

namespace Procyon {

/*
================
IWiredService

An endpoint that a wire may be receiving service from.
================
*/
class IWiredService
{
public:
	// returns a pointer to the wired service.
	virtual void* 						GetOrAllocate() = 0;

	// returns the concrete class implementing this service.
	virtual const ClassReflectionTable*	GetClass() = 0;

	// preallocate the single instance of a singleton.
	virtual void						PreAllocate() = 0;
};

typedef std::shared_ptr< IWiredService > IWiredServicePtr;

/*
================
WiredService

Common implementation of both wired factories and singletons.
================
*/
class WiredService : public IWiredService
{
public:
	WiredService( const ClassReflectionTable* cls )
		: mClass( cls )
	{
	}

	~WiredService() { }

	virtual void* 						GetOrAllocate() = 0;
	virtual const ClassReflectionTable* GetClass();
	virtual void						PreAllocate() { };

protected:
	// the concrete class constructed by this factory.
	const ClassReflectionTable* mClass;
};


/*
================
Implements

helper for readability.
================
*/
template< typename... interfaces >
class Implements { };

/*
================
Singleton

Wired endpoint that creates and reuses only a single instance for the lifetime of the TypeCatalog.
================
*/
template< typename T1, typename T2 = Implements< > >
class Singleton : public WiredService
{
public:
	typedef std::shared_ptr< T1 >
		InstancePtr;

	Singleton( InstancePtr existing );
	Singleton();

	virtual void 	PreAllocate();
	virtual void* 	GetOrAllocate();

	IWiredServicePtr CreateService()
 	{
 		return std::make_shared< Singleton< T1, T2 > >( mSingleInstance );
 	}

protected:
	InstancePtr mSingleInstance;
};

/*
================
Factory

Wired endpoint that creates an instance each time it is resolved.
================
*/
template< typename T1, typename T2 = Implements< > >
class Factory : public WiredService
{
public:
	Factory( );

	virtual void* GetOrAllocate();


 	IWiredServicePtr CreateService3()
 	{
 		return std::make_shared< Factory< T1, T2 > >();
 	}
};

/*
================
TypeCatalog
================
*/
class TypeCatalog
{
public:
	template< typename... Wires >
    TypeCatalog( Wires... wires );

	template< typename T >
	T* 			Resolve();

	inline void AllocateSingletons();

protected:
	template< typename T1, typename T2 >
	void ConnectWireToService( IWiredServicePtr service );

	template< typename T1 >
	void ConnectWiresToService_R( IWiredServicePtr service ) { }

 	template< typename T1, typename T2, typename... Interfaces >
	void ConnectWiresToService_R( IWiredServicePtr service );

 	template< template<typename, typename> class ServiceType, typename T1, typename... Interfaces >
	void AddService( ServiceType< T1, Implements< Interfaces... > > s );

    template< typename... Params >
    void AddServices_R() { }

 	template< template<typename, typename> class ServiceType
 		, typename T1, typename... Interfaces, typename... Services >
    void AddServices_R( ServiceType< T1, Implements< Interfaces... > > s
    	, Services... restServices );

    typedef std::map< const ClassReflectionTable*, IWiredServicePtr > ServiceMap;
    typedef ServiceMap::iterator ConcreteServiceIterator;
    ServiceMap mServices;

	typedef std::map< const ClassReflectionTable*, IWiredServicePtr > WireMap;
	typedef WireMap::iterator WireIterator;
	WireMap mWires;
};

} /* namespace Procyon */


#include "Ioc.inl"


#endif /* _IOC_H */