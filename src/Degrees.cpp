#include "Degrees.h"
#include "dao/DegreeDAO.h"

#include <QtCore/QDebug>
#include <QtCore/QJsonDocument>

using crrc::Degrees;

void Degrees::index( Cutelyst::Context* c ) const
{
  dao::DegreeDAO dao;
  c->setStash( "degrees", dao.retrieveAll() );
  c->setStash( "template", "degrees/index.html" );
}

void Degrees::base( Cutelyst::Context* c ) const
{
  c->response()->setHeader( "Cache-Control", "no-cache, no-store, must-revalidate" );
}


void Degrees::object( Cutelyst::Context* c, const QString& id ) const
{
  c->setStash( "object", dao::DegreeDAO().retrieve( id ) );
}

void Degrees::edit( Cutelyst::Context* c ) const
{
  auto id = c->request()->param( "degree_id", "" );
  const auto title = c->request()->param( "title", "" );
  qDebug() << "Processing POST with id: " << id << ", title: " << title;
  QJsonObject obj;

  if ( title.isEmpty() )
  {
    obj.insert( "status", false );
    obj.insert( "id", id );
    sendJson( c, obj );
    return;
  }

  dao::DegreeDAO dao;
  if ( id.isEmpty() )
  {
    qDebug() << "Inserting new degree";
    const auto cid = dao.insert( c );
    obj.insert( "id", static_cast<int>( cid ) );
    obj.insert( "status", true );
  }
  else
  {
    qDebug() << "Updating degree";
    dao.update( c );
    obj.insert( "status", true );
    obj.insert( "id", id );
  }

  sendJson( c, obj );
}

void Degrees::view( Cutelyst::Context* c ) const
{
  c->setStash( "template", "degrees/view.html" );
}

void Degrees::search( Cutelyst::Context* c ) const
{
  const auto text = c->request()->param( "text", "" );

  if ( text.isEmpty() )
  {
    c->response()->redirect( "/degrees" );
    return;
  }

  dao::DegreeDAO dao;
  c->stash( {
    { "degrees", dao.search( c ) },
    { "searchText", text },
    { "template", "degrees/index.html" }
  } );
}

void Degrees::remove( Cutelyst::Context* c ) const
{
  auto id = c->request()->param( "degree_id", "" );
  QJsonObject obj;

  if ( ! id.isEmpty() )
  {
    auto const result = dao::DegreeDAO().remove( id.toUInt() );
    obj.insert( "count", static_cast<int>( result ) );
    obj.insert( "id", id.toInt() );
    obj.insert( "status", true );
  }
  else
  {
    obj.insert( "status", false );
  }

  sendJson( c, obj );
}

void Degrees::sendJson( Cutelyst::Context* c, const QJsonObject& obj ) const
{
  const auto bytes = QJsonDocument( obj ).toJson();
  c->response()->setContentType( "application/json" );
  c->response()->setContentLength( bytes.size() );
  c->response()->setBody( bytes );
}