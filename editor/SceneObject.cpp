#include "SceneObject.h"
#include "Renderer.h"

SceneObject::SceneObject( QString name, QObject* owner )
	: QObject( owner )
	, mParent( nullptr )
	, mName( name )
	, mDimensions( 100.0f, 100.0f )
    , mRotation( 0.0f )
{
}

SceneObject* SceneObject::GetChild( int idx )
{
	return const_cast< SceneObject* >( const_cast< const SceneObject* >( this )->GetChild( idx ) );
}

const SceneObject* SceneObject::GetChild( int idx ) const
{
	if ( idx < GetChildCount() )
	{
		return mChildren[ idx ];
	}
	return nullptr;
}

void SceneObject::AddChild( SceneObject* child )
{
	for ( auto c : mChildren )
	{
		if ( child == c )
			return;
	}
	child->SetParent( this );
	mChildren.push_back( child );
	emit ChildAdded( child, mChildren.size() - 1 );
}

void SceneObject::RemoveChild( SceneObject* child )
{
	int idx = GetChildIndex( child );
	if ( idx != -1 )
	{

		PROCYON_DEBUG( "Editor", "RemoveChild 1" );
		emit ChildRemovedStart( child, idx );
		PROCYON_DEBUG( "Editor", "RemoveChild 2" );
		mChildren.remove( idx );
		child->SetParent( nullptr );
		emit ChildRemovedEnd( child, idx );
	}
}

// returns -1 on failure
int SceneObject::GetChildIndex( const SceneObject* child ) const
{
	for ( int i = 0; i < mChildren.size(); ++i )
	{
		if ( child == mChildren[ i ] )
		{
			return i;
		}
	}
	return -1;
}

// returns -1 on failure
int SceneObject::GetIndexInParent() const
{
	if ( !mParent )
	{
		return -1;
	}
	return mParent->GetChildIndex( this );
}


void SceneObject::Render( Procyon::Renderer* renderer )
{
	OnRender( renderer );

	for ( int i = 0; i < mChildren.size(); ++i )
	{
		mChildren[ i ]->Render( renderer );
	}
}


void SceneObject::OnRender( Procyon::Renderer* renderer )
{
	glm::vec2 start( mPosition.x(), mPosition.y() );
	glm::vec2 end1( mPosition.x(), mPosition.y() + mDimensions.y() );
	glm::vec2 end2( mPosition.x() + mDimensions.x(), mPosition.y() );
    renderer->DrawWorldLine( start, end1, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) );
    renderer->DrawWorldLine( start, end2, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) );
}
