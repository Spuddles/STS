x64\Debug\BackTest.exe -DB ..\Trading.sqlite -CONFIG ..\strategyConfigs\Crumble-ADA.json
type Crumble-ADA.log | find "POSITION, BTC" > Position-ADA.csv

x64\Debug\BackTest.exe -DB ..\Trading.sqlite -CONFIG ..\strategyConfigs\Crumble-ADX.json
type Crumble-ADX.log | find "POSITION, BTC" > Position-ADX.csv

x64\Debug\BackTest.exe -DB ..\Trading.sqlite -CONFIG ..\strategyConfigs\Crumble-AION.json
type Crumble-AION.log | find "POSITION, BTC" > Position-AION.csv

x64\Debug\BackTest.exe -DB ..\Trading.sqlite -CONFIG ..\strategyConfigs\Crumble-AMB.json
type Crumble-AMB.log | find "POSITION, BTC" > Position-AMB.csv

x64\Debug\BackTest.exe -DB ..\Trading.sqlite -CONFIG ..\strategyConfigs\Crumble-BNB.json
type Crumble-BNB.log | find "POSITION, BTC" > Position-BNB.csv

x64\Debug\BackTest.exe -DB ..\Trading.sqlite -CONFIG ..\strategyConfigs\Crumble-ETH.json
type Crumble-ETH.log | find "POSITION, BTC" > Position-ETH.csv

x64\Debug\BackTest.exe -DB ..\Trading.sqlite -CONFIG ..\strategyConfigs\Crumble-IOTA.json
type Crumble-IOTA.log | find "POSITION, BTC" > Position-IOTA.csv

x64\Debug\BackTest.exe -DB ..\Trading.sqlite -CONFIG ..\strategyConfigs\Crumble-LTC.json
type Crumble-LTC.log | find "POSITION, BTC" > Position-LTC.csv

x64\Debug\BackTest.exe -DB ..\Trading.sqlite -CONFIG ..\strategyConfigs\Crumble-NEO.json
type Crumble-NEO.log | find "POSITION, BTC" > Position-NEO.csv

x64\Debug\BackTest.exe -DB ..\Trading.sqlite -CONFIG ..\strategyConfigs\Crumble-CND.json
type Crumble-CND.log | find "POSITION, BTC" > Position-CND.csv
gnuplot PnL.plot > PnL.png

start PnL.png