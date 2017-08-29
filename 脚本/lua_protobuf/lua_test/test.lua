
package.path = package.path ..';protobuf\\?.lua;proto\\?.lua';
package.cpath = package.cpath .. ';lib\\?.dll'

--local a = package.loadlib("pb.dll", "luaopen_pb");  
--a()

print(package.path)
require "client_pb"

print("Hello World!")















