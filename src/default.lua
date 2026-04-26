function prompt() 
	s = ish_getcwd() .. "\n$ "
	return string.gsub(s, os.getenv("HOME"), "~")
end
