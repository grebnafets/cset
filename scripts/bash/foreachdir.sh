#!/bin/bash
# Please, once you are done doing whatever with your script, put it below
# and comment it out. Try to avoid using the rm command.
# Just move; whatever you want to remove, into the waste directory
#	@ ${CSET_SEARCH_PATH}/waste
# Double check its contents with ls command and now you can safly use rm ;)

# -----------------------------------------------------------------------------
# Creating api directory under each sub under cset.
#for DIR in cset/*; do
#	mkdir "${DIR}/api"
#done
#------------------------------------------------------------------------------
#for DIR in cset/*; do
#	mkdir "${DIR}/adjustment"
#done
#------------------------------------------------------------------------------
# Moving cpp directory into api, this breaks the makefile symbolic link
#for DIR in cset/*; do
#	for SUB in ${DIR}/*; do
#		if [ "${SUB}" = "${DIR}/cpp" ]; then
#			mv ${SUB} ${DIR}/api
#		fi
#	done
#done
#------------------------------------------------------------------------------
#for DIR in cset/*; do
#	for SUB in ${DIR}/*; do
#		if [ "${SUB}" = "${DIR}/api" ]; then
#			echo "Moving ${SUB}/cpp/makefile to ${SUB}/cpp/makefile-old"
#			mv ${SUB}/cpp/makefile ${SUB}/cpp/makefile-old
#		fi
#	done
#done
#------------------------------------------------------------------------------
#for DIR in cset/*; do
#	for SUB in ${DIR}/*; do
#		if [ "${SUB}" = "${DIR}/api" ]; then
#			echo "Creating symlink ${SUB}/cpp/makefile->CSET_SEARCH_PATH/build/cpp/makefile"
#			ln -s ${CSET_SEARCH_PATH}/build/cpp/makefile ${SUB}/cpp/makefile
#		fi
#	done
#done
#------------------------------------------------------------------------------
#for DIR in cset/*; do
#	for SUB in ${DIR}/*; do
#		if [ "${SUB}" = "${DIR}/api" ]; then
#			echo "Moving ${SUB}/cpp/makefile-old into waste"
#			mv  ${SUB}/cpp/makefile-old ${CSET_SEARCH_PATH}/waste
#		fi
#	done
#done
#------------------------------------------------------------------------------
#for DIR in cset/*; do
#	for SUB in ${DIR}/*; do
#		if [ "${SUB}" = "${DIR}/api" ]; then
#			echo "Creating relative symlink path ../../../../cpp/makefile->CSET_SEARCH_PATH/build/cpp/makefile"
#			ln -s ../../../../build/cpp/makefile ${SUB}/cpp/makefile
#		fi
#	done
#done

