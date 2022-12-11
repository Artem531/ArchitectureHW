#ifndef ORDER_HANDLER_H
#define ORDER_HANDLER_H

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Timestamp.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/Exception.h"
#include "Poco/ThreadPool.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include <iostream>
#include <iostream>
#include <fstream>

using Poco::DateTimeFormat;
using Poco::DateTimeFormatter;
using Poco::ThreadPool;
using Poco::Timestamp;
using Poco::Net::HTMLForm;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerParams;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::NameValueCollection;
using Poco::Net::ServerSocket;
using Poco::Util::Application;
using Poco::Util::HelpFormatter;
using Poco::Util::Option;
using Poco::Util::OptionCallback;
using Poco::Util::OptionSet;
using Poco::Util::ServerApplication;

#include "../../database/order.h"
#include "../web_server/utils.h"

class OrderHandler : public HTTPRequestHandler
{
private:
public:
    OrderHandler(const std::string &format) : _format(format) {}

    void handleRequest(HTTPServerRequest &request,
                       HTTPServerResponse &response)
    {
        HTMLForm form(request, request.stream());
        std::cout << "order handler \n";
        if( startsWith( request.getURI(), "/order/add" ) ) {
            if( request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST ) {
                if ( form.has("order_id") && form.has("service_id") ) {
                    long order_id = atol( form.get( "order_id" ).c_str() );
                    long service_id = atol( form.get( "service_id" ).c_str() );
                    database::Order order;

                    try {
                        database::Order::add_to_order( order_id, service_id );
                        response.setStatus( Poco::Net::HTTPResponse::HTTP_OK );
                        response.setChunkedTransferEncoding( true );
                        response.setContentType( "application/json" );
                        std::ostream &ostr = response.send();
                        ostr << order.get_id();
                        return;
                    } catch (...) {
                        response.setStatus( Poco::Net::HTTPResponse::HTTP_NOT_FOUND );
                        std::ostream &ostr = response.send();
                        ostr << "database error";
                        response.send();
                        return;
                    }
                }
            }
        } else if( startsWith( request.getURI(), "/order/all" ) ) {
            try {
                auto results = database::Order::read_all();
                Poco::JSON::Array arr;
                for( auto s : results )
                    arr.add( s.toJSON() );
                response.setStatus( Poco::Net::HTTPResponse::HTTP_OK );
                response.setChunkedTransferEncoding( true );
                response.setContentType( "application/json" );
                std::ostream &ostr = response.send();
                Poco::JSON::Stringifier::stringify( arr, ostr );
            } catch (...) {
                response.setStatus( Poco::Net::HTTPResponse::HTTP_NOT_FOUND );
                std::ostream &ostr = response.send();
                ostr << "{ \"result\": false , \"reason\": \"not found\" }";
                response.send();
                return;
            }
            return;
        } else if( startsWith( request.getURI(), "/order" ) ) {
            if( request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST ) {
                if( form.has( "user_id" ) ) {
                    database::Order order;
                    order.user_id() = atol( form.get( "user_id" ).c_str() );
                    
                    try {
                        order.save_to_mysql();
                        response.setStatus( Poco::Net::HTTPResponse::HTTP_CREATED );
                        response.setChunkedTransferEncoding( true );
                        response.setContentType( "application/json" );
                        std::ostream &ostr = response.send();
                        ostr << order.get_id();
                        return;
                    } catch (...) {
                        response.setStatus( Poco::Net::HTTPResponse::HTTP_NOT_FOUND );
                        std::ostream &ostr = response.send();
                        ostr << "database error";
                        response.send();
                        return;
                    }
                }
            } else {
                if( form.has( "user_id" ) ) {
                    long user_id = atol( form.get( "user_id" ).c_str() );
                    try {
                        auto results = database::Order::read_by_id( user_id );
                        Poco::JSON::Array arr;
                        for ( auto s : results )
                            arr.add( s.toJSON() );
                        response.setStatus( Poco::Net::HTTPResponse::HTTP_OK );
                        response.setChunkedTransferEncoding( true );
                        response.setContentType( "application/json" );
                        std::ostream &ostr = response.send();
                        Poco::JSON::Stringifier::stringify( arr, ostr );
                        return;
                    } catch (...) {
                        response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
                        std::ostream &ostr = response.send();
                        ostr << "{ \"result\": false , \"reason\": \"not found\" }";
                        response.send();
                        return;
                    }
                }
            }
        }
        response.setStatus( Poco::Net::HTTPResponse::HTTP_NOT_FOUND );
        std::ostream &ostr = response.send();
        ostr << "request error";
        response.send();
    }

private:
    std::string _format;
};
#endif