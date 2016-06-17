cmake_minimum_required(VERSION 2.8)
include(cmake/init_project.txt)

# settings
project(railnet)
set(DESCRIPTION "a railroad network exporter for OpenTTD savegames")

SET(VERSION_MAJOR "0")
SET(VERSION_MINOR "1")
SET(VERSION_PATCH "0")

# processing
include(cmake/process_project.txt)
print_summary_base()

