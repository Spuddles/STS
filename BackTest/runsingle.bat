@ECHO OFF
REM %1 is the coin code for this run
x64\Release\BackTest.exe -DB ..\Trading.sqlite -CONFIG ..\strategyConfigs\Crumble-%1-Live.json
type CrumbleStrat-%1BTC.log | find "POSITION, BTC" > Position-%1.csv
type CrumbleStrat-%1BTC.log | find "ALGO" > Algo-%1.log
rem type CrumbleStrat-%1BTC.log | find "SIGNAL" > Signal-%1.log
rem type CrumbleStrat-%1BTC.log | find "Sells:"
rem type CrumbleStrat-%1BTC.log | find "Total BTC value"
gnuplot %1-PnL.plot > %1-PnL.png
