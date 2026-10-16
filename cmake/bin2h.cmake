include(CMakeParseArguments)

# Function to embed contents of a file as byte array in C/C++ header file(.h). The header file
# will contain a byte array
# Parameters
#   SOURCE_FILE	 - The path of source file whose contents will be embedded in the header file.
#   HEADER_FILE	 - The path of header file.
# Usage:
#   bin2h(SOURCE_FILE "Logo.png" HEADER_FILE "Logo.h")
function(BIN2H)
	set(oneValueArgs SOURCE_FILE HEADER_FILE)
	cmake_parse_arguments(BIN2H "${options}" "${oneValueArgs}" "" ${ARGN})

	# reads source file contents as hex string
	file(READ ${BIN2H_SOURCE_FILE} hexString HEX)
	string(LENGTH ${hexString} hexStringLength)

	# adds '0x' prefix and comma suffix before and after every byte respectively
	string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1," arrayValues ${hexString})
	# removes trailing comma
	string(REGEX REPLACE ",$" "" arrayValues ${arrayValues})

	file(WRITE ${BIN2H_HEADER_FILE} "${arrayValues}")
endfunction()
