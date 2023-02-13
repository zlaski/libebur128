@echo off
setlocal EnableExtensions EnableDelayedExpansion

set "ProjectDir=%~dp0"
set "SolutionPath=%~1"
set "IntDir=%~2"
set "OutDir=%~3"

attrib -R %OutDir%..\include\ebur128.h >nul
copy /y %ProjectDir%ebur128\ebur128.h %OutDir%..\include\ebur128.h
attrib +R %OutDir%..\include\ebur128.h >nul
