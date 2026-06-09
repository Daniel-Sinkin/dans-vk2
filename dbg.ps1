# Runs dbg.sh via git-bash. Thin wrapper: all logic lives in the .sh.
& 'G:\Program Files\Git\bin\bash.exe' "$PSScriptRoot\dbg.sh" @args
exit $LASTEXITCODE
