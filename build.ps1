[CmdletBinding()]
Param (
    [ValidateSet("Debug", "Release", "Dist")]
    $config = "Debug",
    [ValidateSet("x86", "x86_64")]
    $arch = "x86_64",
    #$intermediatesFolder = "out\intermediate",
    $binariesFolder = "out\bin\"
)

function GetOutputPath {
    param (
        $project
    )
    return $binFolder + $project.out
}

#$intFolder = $intermediatesFolder + $config + "-" + $arch + "\"
$binFolder = $binariesFolder + $config + "-" + $arch + "\"

$exec = "g++.exe"
$optimizationLevel = If ($config -eq "Debug") {0} else {3}
$debugSymbols = If ($config -eq "Debug") {"-g"} else {""}

$appProject = "SandBox";

$projects = @{
    "GooE" = @{
        "out" = "GooE\";
        "dir" = "GooE\src";
        "includes" = "";
        "libraries" = "";
        "outfile" = "libGooE.dll";
        "options" = "-shared";
    };
    $appProject = @{
        "out" = $appProject + "\";
        "dir" = $appProject + "\src";
        "includes" = "";
        "libraries" = "";
        "outfile" = "sandbox.exe";
        "options" = "-Lout\bin\Debug-x86_64\GooE -lGooE"
    }
}

$projectOrder = "GooE", $appProject;

foreach($projectName in $projectOrder) {
    $project = $projects[$projectName]
    $files = foreach ($file in (Get-ChildItem $project.dir -Recurse -Name -Filter "*.cpp")) { $project.dir + "\" + $file };
    $folderOut = GetOutputPath -project $project
    $outputFile = "-o " + $folderOut + $project.outfile;
    $options = $project.options;
    $optimization = "-O" + $optimizationLevel

    If (!(Test-Path $folderOut)) { New-Item $folderOut -ItemType Directory }

    Invoke-Expression "$exec $files $optimization $debugSymbols $options $outputFile"
}

$engineLib = $binFolder + $projects.GooE.out + $projects.GooE.outfile;
$destFolder = $binFolder  + $projects[$appProject].out;
Copy-Item $engineLib $destFolder