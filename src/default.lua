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

function sh(shellcode)
	args = ish_split(shellcode, " ")
	if args[1] == "cd" then
		if args[2] == nil then
			return ish_chdir(os.getenv("HOME"))
		end
		args[2] = string.gsub(args[2], "~", os.getenv("HOME"))
		return ish_chdir(args[2])
	end

	return os.execute(shellcode)
end

function prompt() 
	wd = string.char(27) .. "[1;35m" .. ish_getcwd() .. string.char(27) .. "[0m\n"
	s = wd .. "$ "
	return string.gsub(s, os.getenv("HOME"), "~")
end
