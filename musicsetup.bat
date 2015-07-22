:: Remove content
rd /s /q 0_unreal\wrapper\Content\TheBarberShop\Assets\Audio\Music\
:: Remove folder
rmdir /s /q 0_unreal\wrapper\Content\TheBarberShop\Assets\Audio\Music\

:: Clear git cache of folder
git rm --cached 0_unreal\wrapper\Content\TheBarberShop\Assets\Audio\Music -r

mkdir 0_unreal/wrapper/Content/TheBarberShop/Assets/Audio/Music

git add 0_unreal/wrapper/Content/TheBarberShop/Assets/Audio/Music -f

git submodule add https://github.com/ViMaSter/p400-thebarbershop-music.git 0_unreal/wrapper/Content/TheBarberShop/Assets/Audio/Music