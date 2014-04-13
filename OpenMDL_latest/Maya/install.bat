@ECHO OFF

SET BITS=x86
SET VERSION=2008
SET TAG64=WOW6432Node\

SET REG_QUERY=HKLM\Hardware\Description\System\CentralProcessor\0
SET REG_KEY=Identifier

FOR /F "TOKENS=2,3,*" %%A IN ('REG QUERY "%REG_QUERY%" /v %REG_KEY%') DO (SET OS_TYPE=%%B)
ECHO %OS_TYPE%
IF %OS_TYPE% == x86 GOTO L_32_BIT ELSE GOTO L_64_BIT

REM ============================================================================
REM 64 bit version
REM ============================================================================
:L_64_BIT
SET TAG64=WOW6432Node\

:L_MAYA_CHECK_64
REG QUERY "HKEY_LOCAL_MACHINE\SOFTWARE\%TAG64%Autodesk\Maya\%VERSION%\Setup\InstallPath" /v MAYA_INSTALL_LOCATION || GOTO L_VERSION_CHECK_64
FOR /F "TOKENS=2,*" %%A IN ('REG QUERY "HKEY_LOCAL_MACHINE\SOFTWARE\Autodesk\Maya\%VERSION%\Setup\InstallPath" /v MAYA_INSTALL_LOCATION') DO (SET MAYA_PATH=%%B)
COPY "OpenMDLMaya%VERSION%_%BITS%.mll" "%MAYA_PATH%bin\plug-ins\"
COPY "OpenMDLMaya.mel" "%MAYA_PATH%scripts\others\"

:L_VERSION_CHECK_64
IF '%VERSION%' == '2011' GOTO L_BIT_CHECK_64 ELSE GOTO L_NEXT_VERSION_64

:L_NEXT_VERSION_64
IF '%VERSION%' == '2010' SET VERSION=2011
IF '%VERSION%' == '2009' SET VERSION=2010
IF '%VERSION%' == '2008' SET VERSION=2009
GOTO L_MAYA_CHECK_64

:L_BIT_CHECK_64
IF '%BITS%' == 'x64' GOTO L_END
SET BITS=x64
SET VERSION=2008
SET TAG64=
GOTO L_MAYA_CHECK_64

REM ============================================================================
REM 32 bit version
REM ============================================================================

:L_32_BIT

:L_MAYA_CHECK_32
REG QUERY "HKEY_LOCAL_MACHINE\SOFTWARE\Autodesk\Maya\%VERSION%\Setup\InstallPath" /v MAYA_INSTALL_LOCATION || GOTO L_VERSION_CHECK_32
FOR /F "TOKENS=2,*" %%A IN ('REG QUERY "HKEY_LOCAL_MACHINE\SOFTWARE\Autodesk\Maya\%VERSION%\Setup\InstallPath" /v MAYA_INSTALL_LOCATION') DO (SET MAYA_PATH=%%B)
COPY "OpenMDLMaya%VERSION%_%BITS%.mll" "%MAYA_PATH%bin\plug-ins\"
COPY "OpenMDLMaya.mel" "%MAYA_PATH%scripts\others\"

:L_VERSION_CHECK_32
IF '%VERSION%' == '2011' GOTO L_END ELSE GOTO L_NEXT_VERSION_32

:L_NEXT_VERSION_32
IF '%VERSION%' == '2010' SET VERSION=2011
IF '%VERSION%' == '2009' SET VERSION=2010
IF '%VERSION%' == '2008' SET VERSION=2009
GOTO L_MAYA_CHECK_32

:L_END
PAUSE