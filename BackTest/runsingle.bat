@ECHO OFF
REM %1 is the coin code for this run
x64\Debug\BackTest.exe -DB ..\Trading.sqlite -CONFIG ..\strategyConfigs\Crumble-%1.json
type Crumble-%1.log | find "POSITION, BTC" > Position-%1.csv
type Crumble-%1.log | find "ALGO" > Algo-%1.log
type Crumble-%1.log | find "SIGNAL" > Signal-%1.log
type Crumble-%1.log | find "Sells:"
type Crumble-%1.log | find "Total BTC value"
gnuplot %1-PnL.plot > %1-PnL.png
