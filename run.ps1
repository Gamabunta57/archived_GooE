[CmdletBinding()]
Param (
    [ValidateSet("Debug", "Release", "Dist")]
    $config = "Debug",
    [ValidateSet("x86", "x86_64")]
    $arch = "x86_64",
    $binariesFolder = "out\bin\",
    [ValidateSet("Sandbox")]
    $project = "Sandbox"
)


$binFolder = $binariesFolder + $config + "-" + $arch + "\"

$projects = @{
    "Sandbox" = @{
        "out" = "Sandbox\";
        "outfile" = "sandbox.exe";
    }
}

$file = $binFolder + $projects[$project].out + $projects[$project].outfile
&$file
