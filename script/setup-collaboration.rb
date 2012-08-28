require 'ffi-rzmq'
require 'json'

if ARGV.length != 1 
  puts ruby setup-collaboration.rb <locatorbinding>
  exit 
end

begin
  ctx = ZMQ::Context.new
  socket = ctx.socket ZMQ::PUSH
  rc = socket.connect ARGV[0]

  #notification = "create_bindings"
  notification = "initialize_job"


  message = { 
    "message_type" => "notification",
    "version" => "1.0",
    "notification_type" => "worker_broadcast",
    "notification" => notification 
  }

  
  socket.send_string message.to_json

  socket.close
  ctx.terminate
rescue => e
  puts "script failed"
  puts e.backtrace.join("\n")
  
end





