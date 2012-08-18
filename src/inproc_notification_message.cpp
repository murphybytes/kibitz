#include "inproc_notification_message.hpp"


namespace kibitz {
  inproc_notification_message::inproc_notification_message( int notification )
    :notification_message( "inproc" ),
     notification_(notification) {
  }
  inproc_notification_message::inproc_notification_message( const ptree& json ) 
    :notification_message("inproc"),
     notification_( json.get<int>("notification") ) {
  }

  string inproc_notification_message::to_json() const {
    stringstream stm;
    ptree tree;
    tree.put( "message_type", "notification" );
    tree.put( "notification_type", "inproc" );
    tree.put( "notification", notification_ );
    boost::property_tree::json_parser::write_json( stm, tree );
    return stm.str();
  }
  
}
