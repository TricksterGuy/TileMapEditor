FIND_PROGRAM( VALGRIND_PATH valgrind )
FIND_PROGRAM( KCACHEGRIND_PATH kcachegrind )

IF(NOT VALGRIND_PATH)
	MESSAGE(FATAL_ERROR "valgrind not found! Aborting...")
ENDIF()

IF(NOT KCACHEGRIND_PATH)
	MESSAGE(FATAL_ERROR "kcachegrind not found! Aborting...")
ENDIF()

IF(NOT CMAKE_COMPILER_IS_GNUCXX)
	MESSAGE(FATAL_ERROR "Compiler is not GNU gcc! Aborting...")
ENDIF()


IF ( NOT (CMAKE_BUILD_TYPE STREQUAL "Debug"))
  MESSAGE( WARNING "Code coverage results with an optimized (non-Debug) build may be misleading" )
ENDIF()


# Param _targetname     The name of new the custom make target
# Param _testrunner     The name of the target which runs the tests.
#						MUST return ZERO always, even on errors.
# Optional third parameter is passed as arguments to _testrunner
#   Pass them in list form, e.g.: "-j;2" for -j 2
FUNCTION(SETUP_TARGET_FOR_PROFILING _targetname _testrunner)
	SET(profile_callgrind "${CMAKE_BINARY_DIR}/profile.callgrind")

	# Setup target
	ADD_CUSTOM_TARGET(${_targetname}
		# Run tests
		COMMAND ${VALGRIND_PATH} --tool=callgrind --callgrind-out-file=${profile_callgrind} ${CMAKE_BINARY_DIR}/${_testrunner}
		COMMAND ${KCACHEGRIND_PATH} ${profile_callgrind}
		COMMAND ${CMAKE_COMMAND} -E remove ${profile_callgrind}

		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	)

ENDFUNCTION()
