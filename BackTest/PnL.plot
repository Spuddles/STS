set terminal png size 3200,1600 enhanced
set title '*BTC PnL (1 minute data)'
set grid xtics lt 0 lw 1 lc rgb "#bbbbbb"
set grid ytics lt 0 lw 1 lc rgb "#bbbbbb"
plot \
    'Position-ETH.csv' using 4:5 with lines lw 2 title 'ETH', \
    'Position-BNB.csv' using 4:5 with lines lw 2 title 'BNB', \
    'Position-LTC.csv' using 4:5 with lines lw 2 title 'LTC', \
    'Position-NEO.csv' using 4:5 with lines lw 2 title 'NEO'