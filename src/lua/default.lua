function prompt() 
	returncode_string = ""
	if ish.returncode ~= 0 and ish.returncode then
		returncode_string = tostring(ish.returncode)
	end
	cwd = string.gsub(ish.getcwd(), os.getenv("HOME"), "~")
	spaces = string.rep(" ", columns() - #returncode_string - #cwd)

	dir = ish.BOLD_PURPLE .. cwd .. spaces .. ish.BOLD_HIGH_RED .. returncode_string .. ish.RESET .. "\n"
	s = dir .. "$ "
	return s
end

function ls(args)
	table.insert(args, 2, "--color=tty")
	ish.execute(table.concat(args, " "))
end

ish.aliases["md"] = "mkdir -p"
ish.aliases["rd"] = "rmdir"
ish.aliases["l"] = "ls -l"
