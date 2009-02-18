# - ow_ensure_out_of_source_build()
# Ensures build directory is different from source directory
#
# If the build directory is the source directory then it will bump
# an error message and stop the compilation processus
#
# Copyright (C) 2006  Alexander Neundorf <neundorf@kde.org>
# Copyright (C) 2006-2007  Wengo
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING file.


macro (ow_ensure_out_of_source_build)

	string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}" inSource)
	if (inSource)
		message(FATAL_ERROR "A separate build directory is required, please create one and run cmake from this directory")
	endif (inSource)

endmacro (ow_ensure_out_of_source_build)
