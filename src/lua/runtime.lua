ish.BOLD_PURPLE = string.char(27) .. "[1;35m"
ish.BOLD_HIGH_RED = string.char(27) .. "[1;91m"
ish.RESET = string.char(27) .. "[0m"
ish.plugins = {}

ish.split = function(inputstr, sep)
	if sep == nil then
		sep = "%s"
	end

	local t={}
	for str in string.gmatch(inputstr, "([^"..sep.."]+)") do
		table.insert(t, str)
	end

	return t
end

ish.execute = function(cmd)
	local success, reason, code = os.execute(cmd)
	ish.returncode = code
	return success, reason, code
end

ish.addpath = function(path)
	ish.setenv("PATH", os.getenv("PATH") .. ":" .. path)
end

ish.enableplugin = function(plugin)
	func = ish.plugins[plugin]
	if func == nil then
		print("config error: no such plugin " .. plugin)
		return
	end
	func()
end

ish.hasprogram = function(name)
	for _, dir_from_path in ipairs(ish.split(os.getenv("PATH"), ":")) do
		path = dir_from_path
		if string.sub(path, -1) == "/" then
			path = path:sub(1, -2)
		end
		path = path .. "/" .. name

		if ish.exists(path) then
			return true
		end
	end
	return false
end

function expand_aliases(shellcode)
	args = ish.split(shellcode, " ")
	alias = ish.aliases[args[1]]
	if alias ~= nil then
		args[1] = alias
		new = table.concat(args, " ")
		return new
	end
	return nil
end

function expand_all_aliases(shellcode)
	while true do
		new_shellcode = expand_aliases(shellcode)
		if new_shellcode == nil then
			break
		end
		shellcode = new_shellcode
	end
	return shellcode
end

function columns()
	cmd = io.popen("tput cols")
	n = cmd:read("*n")
	cmd:close()
	return n
end

function lines()
	cmd = io.popen("tput lines")
	n = cmd:read("*n")
	cmd:close()
	return n
end

function sh(shellcode)
	shellcode = expand_all_aliases(shellcode)

	local args = ish.split(shellcode, " ")
	if _G[args[1]] ~= nil then
		local func = _G[args[1]]
		if type(func) == "function" then
			local ret = func(args)
			ish.returncode = 0

			if ret == nil then
				return
			end

			if type(ret) == "number" then
				ish.returncode = ret
			else
				print(ret)
			end

			return
		end
	end

	if args[1] == "cd" then
		if args[2] == nil then
			return ish.chdir(os.getenv("HOME"))
		end
		args[2] = string.gsub(args[2], "~", os.getenv("HOME"))
		return ish.chdir(args[2])
	end

	ish.execute(shellcode)
end

function pass() end

if ish.istermux() and not ish.exists("/data/data/com.termux/files/home/.config/termux/termux.properties") and not ish.exists("/data/data/com.termux/files/home/.termux/termux.properties") then
	sh "mkdir -p /data/data/com.termux/files/home/.termux"
	sh "cp /data/data/com.termux/files/usr/share/examples/termux/termux.properties /data/data/com.termux/files/home/.termux/"
end
