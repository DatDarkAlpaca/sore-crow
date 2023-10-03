@echo off

set SolutionDir=..\..\
set Configuration=Debug
set Platform=x64
set ProjectName=sore-crow

set OutputPath=%SolutionDir%bin\%Configuration%-%Platform%\%ProjectName%

py copy_resources.py %OutputPath% sore-crow