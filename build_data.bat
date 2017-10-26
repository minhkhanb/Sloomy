@echo off
call config.bat

echo *** Creating directories ***
mkdir %TMPRES%
mkdir %BIN%
mkdir %RES_DES%
cd %DATA_EXPORT_TOOL_DIR%

echo *** Build Data ***

cd %DESIGN_EXPORT_DIR%
del /q %DESIGN_EXPORT_DIR%\*.h >nul 2>nul
del /q %DESIGN_EXPORT_DIR%\*.cpp >nul 2>nul

pause
cd %DESIGN_EXPORT_DIR%\release
echo %DESIGN_EXPORT_DIR%\release
copy *.h %OBJECT_RELEASE_DIR%
copy *.cpp %OBJECT_RELEASE_DIR%

::echo *** Create New FileVersion ***
::mkdir %BIN%\FileVersion
::copy Ver_* %BIN%\FileVersion
::echo *** FileVersion create completed ***

cd %TOOLS_PATH%

call copy_files.bat %RES% %TMPRES%
pause
echo copy sound....
call copy_files.bat %SOUND_DIR% %TMPRES%
echo end  copy sound
pause
cd %TMPRES%

rem pack animation
%ANIM_EXPORT% 	-i 	main_character_boy_200.pine		-o main_character_boy_200.bpine  QUALITY_TRUE_COLOR	EXPORT_AS_3D_TEXTURE	EXPORT_FMODULE_OPACITY USE_MULTI_MODULE_TYPE EXPORT_PALETTE_COLOR
for %%i in (*.pine) do (
	java -jar PineSpriteExport.jar 	%%i	%%~ni.bpine  QUALITY_TRUE_COLOR	EXPORT_AS_3D_TEXTURE	EXPORT_FMODULE_OPACITY USE_MULTI_MODULE_TYPE EXPORT_PALETTE_COLOR
)

	
copy *.h %SRC_DIR_H%


cd %PRJ_DIR%
rem rd %TMPRES%

copy %DESIGN_BDSG_EXPORT% %DESIGN_BDSG_EXPORT_TO_IOS%

echo *** Building Data Complete ***

pause