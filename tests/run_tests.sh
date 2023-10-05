#!/bin/bash

run_tests () {
  local DIR_NAME=$(pwd | grep -oP "/\w+$" | grep -oP "\w+")
  local FILENAMES=$(ls tests-unit-* 2> /dev/null)
  for FILENAME in ${FILENAMES}
  do
    echo "Running test: ${FILENAME}"
    ./${FILENAME}
  done
}

cd unit
TARGETS="core Agents Worlds Interfaces gp_agents agent"
for TARGET in ${TARGETS}
do
if [ -d ${TARGET} ]
  then
    echo "Entering ${TARGET}"
    cd ${TARGET}
    run_tests
    echo "Exiting ${TARGET}"
    cd ..
  else
    echo "Directory not found: ${TARGET}. Skipping."
  fi
done
cd ..
