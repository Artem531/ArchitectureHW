#include "service.h"
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

    void Service::init()
    {
    }

    Poco::JSON::Object::Ptr Service::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root->set("id", _id);
        root->set("service_special_id", _service_special_id);
        root->set("name", _name);
        root->set("price", _price);

        return root;
    }

    Service Service::fromJSON(const std::string &str)
    {
        Service service;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(str);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        service.id() = object->getValue<long>("id");
        service.service_special_id() = object->getValue<std::string>("service_special_id");
        service.name() = object->getValue<std::string>("name");
        service.price() = object->getValue<double>("price");

        return service;
    }

    Service Service::read_by_id(long id)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            Service a;
            select << "SELECT id, service_special_id, name, price FROM Service where id=?",
                into(a._id),
                into(a._service_special_id),
                into(a._name),
                into(a._price),
                use(id),
                range(0, 1); //  iterate over result set one row at a time
  
            select.execute();
            Poco::Data::RecordSet rs(select);
            if (!rs.moveFirst()) throw std::logic_error("not found");

            return a;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    Service Service::read_by_special_id(std::string service_special_id)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            Service a;
            select << "SELECT id, service_special_id, name, price FROM Service where service_special_id=?",
                into(a._id),
                into(a._service_special_id),
                into(a._name),
                into(a._price),
                use(service_special_id),
                range(0, 1); //  iterate over result set one row at a time
  
            select.execute();
            Poco::Data::RecordSet rs(select);
            if (!rs.moveFirst()) throw std::logic_error("not found");

            return a;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    std::vector<Service> Service::read_all()
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Statement select(session);
            std::vector<Service> result;
            Service a;
            select << "SELECT id, service_special_id, name, price FROM Service",
                into(a._id),
                into(a._service_special_id),
                into(a._name),
                into(a._price),
                range(0, 1); //  iterate over result set one row at a time

            while (!select.done())
            {
                if(select.execute()){
                    result.push_back(a);
                }
            }
            return result;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    std::vector<Service> Service::search(std::string name)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Statement select(session);
            std::vector<Service> result;
            Service a;
            name+="%";
            select << "SELECT id, service_special_id, name, price FROM Service where name LIKE ?",
                into(a._id),
                into(a._service_special_id),
                into(a._name),
                into(a._price),
                use(name),
                range(0, 1); //  iterate over result set one row at a time

            while (!select.done())
            {
                if(select.execute())  result.push_back(a);
            }
            return result;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

   
    void Service::save_to_mysql()
    {

        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement insert(session);

            insert << "INSERT INTO Service (service_special_id, name, price) VALUES(?, ?, ?)",
                use(_service_special_id),
                use(_name),
                use(_price);

            insert.execute();

            Poco::Data::Statement select(session);
            select << "SELECT LAST_INSERT_ID()",
                into(_id),
                range(0, 1); //  iterate over result set one row at a time

            if (!select.done())
            {
                select.execute();
            }
            std::cout << "inserted:" << _id << std::endl;
        }
        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    long Service::get_id() const
    {
        return _id;
    }

    const std::string &Service::get_service_special_id() const
    {
        return _service_special_id;
    }

    const std::string &Service::get_name() const
    {
        return _name;
    }

    double Service::get_price() const
    {
        return _price;
    }

    long &Service::id()
    {
        return _id;
    }

    std::string &Service::service_special_id()
    {
        return _service_special_id;
    }

    std::string &Service::name()
    {
        return _name;
    }

    double &Service::price()
    {
        return _price;
    }

}