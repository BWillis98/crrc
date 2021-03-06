#pragma once

#include <QtCore/QJsonArray>
#include <Cutelyst/Controller>

namespace crrc
{
  class Programs : public Cutelyst::Controller
  {
    Q_OBJECT
  public:
    explicit Programs( QObject* parent = nullptr ) : Controller( parent ) {}
    ~Programs() = default;

    C_ATTR(index, :Path("/programs") :AutoArgs)
    void index( Cutelyst::Context* c ) const;

    C_ATTR( base, :Chained( "/" ) : PathPart( "programs" ) : CaptureArgs( 0 ) )
    void base( Cutelyst::Context* ) const;

    C_ATTR( object, :Chained( "base" ) : PathPart( "id" ) : CaptureArgs( 1 ) )
    void object( Cutelyst::Context* c, const QString& id ) const;

    C_ATTR( create, :Chained( "base" ) : PathPart( "create" ) : Args( 0 ) )
    void create( Cutelyst::Context* c ) const;

    C_ATTR( view, :Chained( "object" ) : PathPart( "view" ) : Args( 0 ) )
    void view( Cutelyst::Context* c ) const;

    C_ATTR( data, :Chained( "object" ) : PathPart( "data" ) : Args( 0 ) )
    void data( Cutelyst::Context* c ) const;

    C_ATTR( update, :Chained( "object" ) : PathPart( "update" ) : Args( 0 ) )
    void update( Cutelyst::Context* c ) const { create( c ); }

    C_ATTR( edit, :Chained( "base" ) : PathPart( "edit" ) : Args( 0 ) )
    void edit( Cutelyst::Context* c ) const;

    C_ATTR( search, :Chained( "base" ) : PathPart( "search" ) : Args( 0 ) )
    void search( Cutelyst::Context* c ) const;

    C_ATTR( remove, :Chained( "base" ) : PathPart( "remove" ) : Args( 0 ) )
    void remove( Cutelyst::Context* c );

  private:
    bool checkInstitution( Cutelyst::Context* context ) const;
    QJsonArray toArray( const QVariantList& list ) const;
    void editAndRespond( Cutelyst::Context* c ) const;
  };
}
