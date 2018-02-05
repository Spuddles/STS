set terminal png size 3200,1600 enhanced
set title 'BNBBTC PnL (1 minute data)'
plot \
    'Position-ADA.csv' using 4:5 with lines title 'ADA', \
    'Position-ADX.csv' using 4:5 with lines title 'ADX', \
    'Position-AION.csv' using 4:5 with lines title 'AION', \
    'Position-AMB.csv' using 4:5 with lines title 'AMB', \
    'Position-BNB.csv' using 4:5 with lines title 'BNB', \
    'Position-CND.csv' using 4:5 with lines title 'CND', \
    'Position-IOTA.csv' using 4:5 with lines title 'IOTA', \
    'Position-LTC.csv' using 4:5 with lines title 'LTC', \
    'Position-NEO.csv' using 4:5 with lines title 'NEO'