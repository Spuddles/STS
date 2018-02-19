set terminal png size 3200,1600 enhanced
set title '*BTC PnL (1 minute data)'
set grid xtics lt 0 lw 1 lc rgb "#bbbbbb"
set grid ytics lt 0 lw 1 lc rgb "#bbbbbb"
set title 'ETHBTC price plot (1 minute data)'
plot [0:14274]  'Algo-ETH.csv' using 3:7 with lines title 'Upper BB', \
                'Algo-ETH.csv' using 3:5 with lines title 'Close Price', \
                'Algo-ETH.csv' using 3:6 with lines title 'MA Price', \
                'Algo-ETH.csv' using 3:4 with lines title 'Lower BB', \
                'Algo-ETH.csv' using 3:8 lt rgb 'red' title 'Sell Signal', \
                'Algo-ETH.csv' using 3:9 lt rgb 'black' title 'Buy Signal'