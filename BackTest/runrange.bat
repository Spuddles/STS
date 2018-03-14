@ECHO OFF
REM %1 is the coin code for this run
x64\Release\BackTest.exe -DB ..\Trading.sqlite -CONFIG ..\strategyConfigs\Crumble-%1.json
rem type CrumbleStrat-%1BTC.log | find "PNL" > CrumbleStrat-%1BTC-PNL.csv
