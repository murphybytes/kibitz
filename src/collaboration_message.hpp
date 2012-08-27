
#ifndef __COLLABORATION_MESSAGE_HPP__
#define __COLLABORATION_MESSAGE_HPP__

#include  "message.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>



namespace kibitz {
  class collaboration_message : public message {
    const string collaboration_type_;
    const string job_id_;    
  protected:
    
  
    collaboration_message( const string& collaboration_type, const string& version = "1.0" )
      :message( "collaboration", version ),
       collaboration_type_( collaboration_type ),
       job_id_( lexical_cast<string>(boost::uuids::random_generator()() )) {
    }

    void populate_header( ptree& tree ) const {
      message::populate_header( tree ) ;
      tree.put( "collaboration_type", collaboration_type_ );
      tree.put( "job_id", job_id_ );
    }
  public:
    virtual ~collaboration_message() {}
    virtual string to_json() const = 0;
    const string& collaboration_type() const { return collaboration_type_ ; }
    const string& job_id() const { return job_id_; }
  };

  typedef shared_ptr<collaboration_message> collaboration_message_ptr_t;
}

#endif



