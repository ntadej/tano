# - ow_get_gcc_version(version)
# Internal function, gets GNU GCC version number
#
# Example:
# ow_get_gcc_version(version)
# message(${version})
#
# Output generated:
# 4.0.1
#
# Copyright (C) 2007  Wengo
# Copyright (C) 2007  Andreas Schneider <mail@cynapses.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING file.


macro (ow_get_gcc_version version)

	if (CMAKE_COMPILER_IS_GNUCC)
		exec_program(
			${CMAKE_CXX_COMPILER}
			ARGS
				--version
			OUTPUT_VARIABLE
				output
		)
		string(REGEX REPLACE ".* ([0-9]\\.[0-9]\\.[0-9]) .*" "\\1" version ${output})
	endif(CMAKE_COMPILER_IS_GNUCC)

endmacro (ow_get_gcc_version)
