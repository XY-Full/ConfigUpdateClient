# 若无法执行，且提示“无法加载文件 make.ps1，因为在此系统上禁止运行脚本。”
# 请执行如下命令
# Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass
$ErrorActionPreference = "Stop"

$path = Get-Location
$libPath = "$path\lib"
$env:Path = "$libPath;$env:Path"

$protocPath = "$path\bin"
$srcDir = "$path\proto"
$outDir = "$path\proto_files"

# 创建输出目录
if (!(Test-Path -Path $outDir)) {
    New-Item -ItemType Directory -Path $outDir | Out-Null
}

# 获取 .proto 文件列表
$protoFiles = Get-ChildItem -Path $srcDir -Filter *.proto | ForEach-Object { $_.Name }

function Compile-ProtoIfNeeded {
    param (
        [string]$SrcDir,
        [string]$OutDir,
        [string[]]$Files
    )
    foreach ($protoFile in $Files) {
        $protoPath = Join-Path $SrcDir $protoFile
        $baseName = [System.IO.Path]::GetFileNameWithoutExtension($protoFile)
        $outCc = Join-Path $OutDir "$baseName.pb.cc"
        $outH = Join-Path $OutDir "$baseName.pb.h"

        if (!(Test-Path $outCc) -or (Get-Item $protoPath).LastWriteTime -gt (Get-Item $outCc).LastWriteTime) {
            Write-Host "Recompiling $protoFile ..." -ForegroundColor Cyan
            Push-Location $SrcDir
            & "$protocPath\protoc.exe" --proto_path=. --proto_path=$path\proto --cpp_out=$OutDir $protoFile
            Pop-Location
        }
    }
}

Compile-ProtoIfNeeded -SrcDir $srcDir -OutDir $outDir -Files $protoFiles
