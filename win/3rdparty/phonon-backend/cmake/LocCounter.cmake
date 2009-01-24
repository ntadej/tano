# - loc_counter(path loc recursive)
# Gets the LOC of a given path.
#
# Example:
# loc_counter(path loc)
# message(${loc})
#
# Output generated:
# 4506
#
# Copyright (C) 2008  Tanguy Krotoff <tkrotoff@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING file.


macro (loc_counter path loc recursive)

	message(STATUS "LOC ${path}...")

	if (UNIX AND NOT WIN32)
		set(flags "-DLINK_LIBRARIES=stdc++")
	endif (UNIX AND NOT WIN32)

	if (WIN32)
		set(definitions "-DUNICODE")
	endif (WIN32)

	try_run(
		runResult
		compileResult
		${CMAKE_BINARY_DIR}
		${CMAKE_SOURCE_DIR}/cmake/loc_counter/loc_counter.cpp
		COMPILE_DEFINITIONS ${definitions}
		CMAKE_FLAGS ${flags}
		COMPILE_OUTPUT_VARIABLE compileOutput
		RUN_OUTPUT_VARIABLE ${loc}
		ARGS
			"\"${path}\""
			${recursive}
	)

	#message("compileOutput="${compileOutput})
	#message("compileResult="${compileResult})
	#message("runResult="${runResult})
	#message("runOutput="${${loc}})

endmacro (loc_counter)
