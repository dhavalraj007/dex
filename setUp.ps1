$url = "https://github.com/libsdl-org/SDL/releases/download/release-2.28.5/SDL2-devel-2.28.5-mingw.zip"
$outputPath = "./dependencies"
New-Item -Path $outputPath -ItemType Directory -Force

Invoke-WebRequest -UserAgent "Wget" -Uri $url -OutFile "$outputPath/sdl.zip"
Expand-Archive "$outputPath/sdl.zip" -DestinationPath $outputPath -Force
Remove-Item "$outputPath/sdl.zip"


$url = "https://github.com/libsdl-org/SDL_ttf/releases/download/release-2.20.2/SDL2_ttf-devel-2.20.2-mingw.zip"
$outputPath = "./dependencies"
New-Item -Path $outputPath -ItemType Directory -Force

Invoke-WebRequest -UserAgent "Wget" -Uri $url -OutFile "$outputPath/sdl-ttf.zip"
Expand-Archive "$outputPath/sdl-ttf.zip" -DestinationPath $outputPath -Force
Remove-Item "$outputPath/sdl-ttf.zip"

$TobeCopied = Get-ChildItem -Path "$outputPath/SDL2_ttf-2.20.2"
Copy-Item -Path $TobeCopied.FullName -Destination "$outputPath/SDL2-2.28.5" -Recurse -Force
Remove-Item "$outputPath/SDL2_ttf-2.20.2" -Recurse

