# set terminal svg size 10240,7680
# set output 'data.svg'
set title 'LTCBTC zzz price plot (1 minute data)'
plot [0:14274]  'ALGO-LTC.csv' using 3:7 with lines title 'Upper BB', \
                'ALGO-LTC.csv' using 3:5 with lines title 'Close Price', \
                'ALGO-LTC.csv' using 3:6 with lines title 'MA Price', \
                'ALGO-LTC.csv' using 3:4 with lines title 'Lower BB', \
                'ALGO-LTC.csv' using 3:8 lt rgb 'red' title 'Sell Signal', \
                'ALGO-LTC.csv' using 3:9 lt rgb 'black' title 'Buy Signal', \
                'ALGO-LTC.csv' using 3:10 with lines title 'Gradient'
