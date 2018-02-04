# set terminal svg size 10240,7680
# set output 'data.svg'
set title 'BNBBTC price plot (1 minute data)'
plot [0:11395]  'data.csv' using 1:5 with lines title 'Upper BB', \
                'data.csv' using 1:3 with lines title 'Close Price', \
                'data.csv' using 1:4 with lines title 'MA Price', \
                'data.csv' using 1:2 with lines title 'Lower BB', \
                'data.csv' using 1:6 lt rgb 'red' title 'Sell Signal', \
                'data.csv' using 1:7 lt rgb 'black' title 'Buy Signal'