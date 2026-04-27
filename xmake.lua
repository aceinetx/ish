add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate", {outputdir = "build"}) -- generate compile commands

if is_plat("linux") then
	set_policy("build.sanitizer.address", true)
	set_policy("build.sanitizer.leak", true)
end

add_requires("lua", "readline")

set_warnings("all") -- warns
set_languages("c99")

target("ish")
	set_kind("binary")

	add_rules("utils.bin2c", {extensions = {".lua"}})

	add_includedirs("include")
	add_files("src/*.c", "src/lua/*.lua")

	add_packages("lua", "readline")
target_end()
