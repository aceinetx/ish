ish.plugins["zoxide"] = function()
	if not ish.hasprogram("zoxide") then
		print("zoxide not installed")
		return
	end

	function __zoxide_query(path)
		cmd = "zoxide query --exclude \"" .. ish.getcwd() .. "\" -- " .. path
		fd = io.popen(cmd)
		str = fd:read("*l")
		fd:close()
		return str
	end

	_G["z"] = function(args)
		if args[2] == nil then
			ish.chdir(os.getenv("HOME"))
			return
		end
		ish.chdir(__zoxide_query(args[2]))
	end

	_G["cd"] = _G["z"]
end
