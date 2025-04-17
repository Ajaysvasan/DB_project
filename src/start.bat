@echo off
set fileOne=main
set fileTwo=parser
set fileThree=dbms

echo Compiling %fileOne%.cpp , %fileTwo%.cpp and %fileThree%.cpp...

g++ %fileOne%.cpp %fileTwo%.cpp %fileThree%.cpp -o %fileOne%.exe

if %errorlevel% neq 0 (
    echo Compilation failed!
    pause
    exit /b %errorlevel%
)

echo Running %fileOne%.exe...
%fileOne%.exe
pause
