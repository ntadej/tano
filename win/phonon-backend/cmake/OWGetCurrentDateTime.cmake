# - ow_get_current_date_time(time)
# Gets the current date time
#
# Example:
# ow_get_current_date_time(time)
# message(${time})
#
# Output generated:
# 20070323121316
#
# which means: 2007-03-23 12:13:16 (YYYY-MM-DD HH:MM:SS)
#
# Copyright (C) 2006-2007  Wengo
# Copyright (C) 2007-2008  Tanguy Krotoff <tkrotoff@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING file.


macro (ow_get_current_date_time time)

	if (UNIX AND NOT WIN32)
		set(flags "-DLINK_LIBRARIES=stdc++")
	endif (UNIX AND NOT WIN32)

	try_run(
		runResult
		compileResult
		${CMAKE_BINARY_DIR}
		${CMAKE_SOURCE_DIR}/cmake/get_current_datetime/get_current_datetime.cpp
		CMAKE_FLAGS ${flags}
		COMPILE_OUTPUT_VARIABLE compileOutput
		RUN_OUTPUT_VARIABLE ${time}
	)
	string(REGEX MATCH "[0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]" ${time} ${${time}})

	#message("compileOutput="${compileOutput})
	#message("compileResult="${compileResult})
	#message("runResult="${runResult})
	#message("runOutput="${${time}})

endmacro (ow_get_current_date_time)
