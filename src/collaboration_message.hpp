
#ifndef __COLLABORATION_MESSAGE_HPP__
#define __COLLABORATION_MESSAGE_HPP__

#include  "message.hpp"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>

using boost::lexical_cast;



namespace kibitz {


  class collaboration_message : public message {
    string collaboration_type_;
    string job_id_;
  protected:
      
    collaboration_message( const string& collaboration_type, const string& version = "1.0"  )
      :message( "collaboration", version ),
       collaboration_type_( collaboration_type ),
       job_id_( lexical_cast<string>(boost::uuids::random_generator()() )) {
    }

    collaboration_message( const ptree& json ) 
      :message( json ),
       collaboration_type_( json.get<string>( "collaboration_type" ) ),
       job_id_( json.get<string>( "job_id" ) ) {
    }

    void populate_header( ptree& tree ) const {
      message::populate_header( tree ) ;
      tree.put( "collaboration_type", collaboration_type_ );
      tree.put( "job_id", job_id_ );
    }

  public:
    virtual ~collaboration_message() {}
    virtual string to_json() const = 0;
    virtual const string& collaboration_type() const { return collaboration_type_ ; }
    virtual const string& job_id() const { return job_id_; }
    virtual void set_job_id( const string& job_id ) { job_id_ = job_id; }
  };

  typedef shared_ptr<collaboration_message> collaboration_message_ptr_t;
}

#endif



