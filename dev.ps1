# Runs dev.sh via git-bash. Thin wrapper: all logic lives in the .sh.
& 'G:\Program Files\Git\bin\bash.exe' "$PSScriptRoot\dev.sh" @args
exit $LASTEXITCODE
