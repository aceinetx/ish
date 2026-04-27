function ish_split(inputstr, sep)
	if sep == nil then
		sep = "%s"
	end

	local t={}
	for str in string.gmatch(inputstr, "([^"..sep.."]+)") do
		table.insert(t, str)
	end

	return t
end

function expand_aliases(shellcode)
	args = ish_split(shellcode, " ")
	alias = ish_aliases[args[1]]
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

function sh(shellcode)
	shellcode = expand_all_aliases(shellcode)

	args = ish_split(shellcode, " ")
	if args[1] == "cd" then
		if args[2] == nil then
			return ish_chdir(os.getenv("HOME"))
		end
		args[2] = string.gsub(args[2], "~", os.getenv("HOME"))
		return ish_chdir(args[2])
	end

	if _G[args[1]] ~= nil then
		func = _G[args[1]]
		if type(func) == "function" then
			func(args)
			return
		end
	end

	return os.execute(shellcode)
end

function prompt() 
	wd = string.char(27) .. "[1;35m" .. ish_getcwd() .. string.char(27) .. "[0m\n"
	s = wd .. "$ "
	return string.gsub(s, os.getenv("HOME"), "~")
end

function pass()

end

ish_aliases["ga"] = "git add"
ish_aliases["gc"] = "git commit"
ish_aliases["gp"] = "git push"
ish_aliases["md"] = "mkdir -p"
ish_aliases["rd"] = "rmdir"
