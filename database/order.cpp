#include "order.h"
#include "database.h"
#include "../config/config.h"

#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

#include <sstream>
#include <exception>

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

namespace database
{

    void Order::init() {
        try {
            std::cout << "\n init order \n";
            Poco::Data::Session session = database::Database::get().create_session_write();
        
            Statement create_stmt( session );
            create_stmt << "CREATE TABLE IF NOT EXISTS `UserOrder` ("
                        << "`id` INT NOT NULL AUTO_INCREMENT,"
                        << "`user_id` INT NOT NULL,"
                        << "PRIMARY KEY (`id`),"
                        << "FOREIGN KEY (`user_id`) REFERENCES `User`(`id`) ON UPDATE CASCADE ON DELETE CASCADE);",
                now;
            
            std::cout << "\n init service order \n";
            Statement create_service_order_table( session );
            create_service_order_table << "CREATE TABLE IF NOT EXISTS `Service_Order` ("
                        << "`id` INT NOT NULL AUTO_INCREMENT,"
                        << "`service_id` INT NOT NULL,"
                        << "`order_id` INT NOT NULL,"
                        << "PRIMARY KEY (`id`),"
                        << "FOREIGN KEY (`service_id`) REFERENCES `Service`(`id`) ON UPDATE CASCADE ON DELETE CASCADE,"
                        << "FOREIGN KEY (`order_id`) REFERENCES `UserOrder`(`id`) ON UPDATE CASCADE ON DELETE CASCADE);",
                now;
            
        } catch( Poco::Data::MySQL::ConnectionException& e ) {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        } catch( Poco::Data::MySQL::StatementException& e ) {
            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    Poco::JSON::Object::Ptr Order::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
        root->set( "id", _id );
        root->set( "user_id", _user_id );
    
        return root;
    }

    Order Order::fromJSON( const std::string& str )
    {
        Order order;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse( str );
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        order.id() = object->getValue<long>( "id" );
        order.user_id() = object->getValue<long>( "user_id" );

        return order;
    }

    std::vector<Order> Order::read_by_id( long id )
    {
        try {
            Poco::Data::Session session = database::Database::get().create_session_read();
            Poco::Data::Statement select( session );
            std::vector<Order> result;
            Order a;
          
            select << "SELECT id, user_id FROM UserOrder WHERE user_id=?",
                into( a._id ),
                into( a._user_id ),
                use( id ),
                range( 0, 1 ); //  iterate over result set one row at a time

            while ( !select.done() ) {
                if( select.execute() ) {
                    result.push_back( a );
                }
            }
            return result;
        } catch( Poco::Data::MySQL::ConnectionException& e ) {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        } catch( Poco::Data::MySQL::StatementException& e ) {
            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    std::vector<Order> Order::read_all()
    {
        try {
            Poco::Data::Session session = database::Database::get().create_session_read();
            Statement select( session );
            std::vector<Order> result;
            Order a;
            select << "SELECT id, user_id FROM UserOrder",
                into( a._id ),
                into( a._user_id ),
                range( 0, 1 ); //  iterate over result set one row at a time
         
            while( !select.done() ) {
                if( select.execute() ) {
                    result.push_back( a );
                }
            }
            return result;
        } catch ( Poco::Data::MySQL::ConnectionException& e ) {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        } catch ( Poco::Data::MySQL::StatementException& e ) {
            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    void Order::add_to_order( long _id, long _service_id )
    {
        try {
            Poco::Data::Session session = database::Database::get().create_session_write();
            Poco::Data::Statement insert( session );

            insert << "INSERT INTO Service_Order (service_id, order_id) VALUES(?, ?)",
                use( _service_id ),
                use( _id );

            insert.execute();

            std::cout << "inserted in order:" << _id << ", service:" << _service_id << std::endl;
        } catch( Poco::Data::MySQL::ConnectionException &e ) {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        } catch ( Poco::Data::MySQL::StatementException &e ) {
            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    void Order::save_to_mysql()
    {
        try {
            Poco::Data::Session session = database::Database::get().create_session_write();
            Poco::Data::Statement insert( session );

            insert << "INSERT INTO UserOrder (user_id) VALUES (?)",
                 use( _user_id );

            insert.execute();

            Poco::Data::Statement select( session );
            select << "SELECT LAST_INSERT_ID()",
                into( _id ),
                range( 0, 1 ); //  iterate over result set one row at a time

            if( !select.done() ) {
                select.execute();
            }
            std::cout << "inserted:" << _id << std::endl;

        } catch( Poco::Data::MySQL::ConnectionException &e ) {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
            
        } catch( Poco::Data::MySQL::StatementException &e ) {
            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    long Order::get_id() const
    {
        return _id;
    }

    long Order::get_user_id() const
    {
        return _user_id;
    }

    long &Order::id()
    {
        return _id;
    }

    long &Order::user_id()
    {
        return _user_id;
    }

}
