local str_v = redis.pcall("get" , KEYS[1])
local n_opt_v = tonumber(ARGV[1])
local str_ret = "err"

if n_opt_v >= 0 then
	if tonumber(str_v) > n_opt_v then
		str_ret = redis.pcall("decrby" , KEYS[1] , n_opt_v)
	end
end


return str_ret

--py 代码
--r = redis.Redis(host='192.168.0.x', port=6379,db=0)
--print r.eval(str_ , "1",'hbb_test_lua', "30")