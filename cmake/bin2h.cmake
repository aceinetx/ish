include(CMakeParseArguments)

# Function to wrap a given string into multiple lines at the given column position.
# Parameters:
#   VARIABLE	- The name of the CMake variable holding the string.
#   AT_COLUMN   - The column position at which string will be wrapped.
function(WRAP_STRING)
	set(oneValueArgs VARIABLE AT_COLUMN)
	cmake_parse_arguments(WRAP_STRING "${options}" "${oneValueArgs}" "" ${ARGN})

	string(LENGTH ${${WRAP_STRING_VARIABLE}} stringLength)
	math(EXPR offset "0")

	while(stringLength GREATER 0)

		if(stringLength GREATER ${WRAP_STRING_AT_COLUMN})
			math(EXPR length "${WRAP_STRING_AT_COLUMN}")
		else()
			math(EXPR length "${stringLength}")
		endif()

		string(SUBSTRING ${${WRAP_STRING_VARIABLE}} ${offset} ${length} line)
		set(lines "${lines}\n${line}")

		math(EXPR stringLength "${stringLength} - ${length}")
		math(EXPR offset "${offset} + ${length}")
	endwhile()

	set(${WRAP_STRING_VARIABLE} "${lines}" PARENT_SCOPE)
endfunction()

# Function to embed contents of a file as byte array in C/C++ header file(.h). The header file
# will contain a byte array and integer variable holding the size of the array.
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

	# wraps the hex string into multiple lines at column 32(i.e. 16 bytes per line)
	wrap_string(VARIABLE hexString AT_COLUMN 32)
	math(EXPR arraySize "${hexStringLength} / 2")

	# adds '0x' prefix and comma suffix before and after every byte respectively
	string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1, " arrayValues ${hexString})
	# removes trailing comma
	string(REGEX REPLACE ", $" "" arrayValues ${arrayValues})

	file(WRITE ${BIN2H_HEADER_FILE} "${arrayValues}")

	file(APPEND "${CMAKE_CURRENT_BINARY_DIR}/${FILE_BASE}.h" "\n")
endfunction()
