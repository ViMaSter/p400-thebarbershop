@echo off

:BEGIN
:: Check if git exists
git >nul 2>&1

:: Redirect to tutorial if not
if %errorlevel%==9009 goto GIT

	:: Remove content
	rd /s /q 0_unreal\wrapper\Content\TheBarberShop\Assets\Audio\Music\
	:: Remove folder
	rmdir /s /q 0_unreal\wrapper\Content\TheBarberShop\Assets\Audio\Music\

	:: Clear git cache of folder
	git rm --cached 0_unreal\wrapper\Content\TheBarberShop\Assets\Audio\Music -r

	mkdir 0_unreal/wrapper/Content/TheBarberShop/Assets/Audio/Music

	git add 0_unreal/wrapper/Content/TheBarberShop/Assets/Audio/Music -f

	git submodule add https://github.com/ViMaSter/p400-thebarbershop-music.git 0_unreal/wrapper/Content/TheBarberShop/Assets/Audio/Music

	goto END

:GIT
echo So... git is not available. I'll provide a guide on what to fix. After that, run this file again!
pause
start "" "http://stackoverflow.com/a/26620861"

:END