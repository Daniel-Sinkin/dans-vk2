# Runs run_opt.sh via git-bash. Thin wrapper: all logic lives in the .sh.
& 'G:\Program Files\Git\bin\bash.exe' "$PSScriptRoot\run_opt.sh" @args
exit $LASTEXITCODE
