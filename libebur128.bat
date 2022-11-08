@echo off
setlocal EnableExtensions EnableDelayedExpansion

set ProjectDir=%~dp0

set OutDir=%~1

attrib -R %OutDir%..\include\ebur128.h >nul
copy /y %ProjectDir%ebur128\ebur128.h %OutDir%..\include\ebur128.h
