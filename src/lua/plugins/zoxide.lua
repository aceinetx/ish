ish.plugins["zoxide"] = function()
	if not ish.hasprogram("zoxide") then
		print("zoxide not installed")
		return
	end

	local function __zoxide_query(path)
		cmd = "zoxide query --exclude \"" .. ish.getcwd() .. "\" -- " .. path
		fd = io.popen(cmd)
		str = fd:read("*l")
		fd:close()
		return str
	end

	local function __zoxide_add(path)
		ish.execute("zoxide add -- \"" .. path .. "\"")
	end

	_G["z"] = function(args)
		if args[2] == nil then
			ish.chdir(os.getenv("HOME"))
			return
		end
		table.remove(args, 1)
		path = table.concat(args, " ")

		if ish.exists(path) then
			__zoxide_add(path)
			ish.chdir(path)
			return
		end
		ish.chdir(__zoxide_query(path))
	end

	_G["cd"] = _G["z"]
end
