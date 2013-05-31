@echo off
set ACTIVEMQ_HOME="/Users/rajdavies/dev/projects/camelonedemo/apache-activemq-5.8.0"
set ACTIVEMQ_BASE="/Users/rajdavies/dev/projects/activemq-anywhere-example/activemq/broker"

set PARAM=%1
:getParam
shift
if "%1"=="" goto end
set PARAM=%PARAM% %1
goto getParam
:end

%ACTIVEMQ_HOME%/bin/activemq %PARAM%