REM %1 is the coin code for this run
x64\Release\BackTest.exe -DB ..\Trading.sqlite -CONFIG ..\strategyConfigs\Crumble-%1.json
type Crumble-%1.log | find "POSITION, BTC" > Position-%1.csv
type Crumble-%1.log | find "ALGO" > Algo-%1.csv
