

add_rules("mode.debug")
add_requires("cpr", "sfml", "zlib")

target("gdrender")
	add_packages("sfml")
	add_packages("cpr", "sfml", "zlib")
	add_includedirs(
		"GD/include/boost",
		"GD/include/imgui",
		"GD/include/imgui-sfml",
		"GD/include/portable-file-dialogs",
		"GD/include/raudio",
		"GD/include/tsl",
		"GD/include/zlib",
		"GD/include"
	)
	add_defines("RAUDIO_STANDALONE", "SUPPORT_MODULE_RAUDIO")
    set_kind("binary")
    add_files(
		"GD/*.c",
		"GD/include/imgui/*.cpp",
		"GD/include/imgui-sfml/*.cpp",
		"GD/*.cpp"
	)
    set_languages("c++23")
	set_warnings("more")