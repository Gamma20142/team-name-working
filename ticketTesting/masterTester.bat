
PUSHD GenericSpecificTest
call test.bat
POPD


PUSHD GenericSpecificTest2
call test.bat
POPD


type reportDirectories.txt | masterReportWriter.exe

