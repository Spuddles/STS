# set terminal svg size 10240,7680
# set output 'data.svg'
set title 'IOTABTC price plot (1 minute data)'
plot [0:14274]  'Crumble-IOTA.log' using 3:7 with lines title 'Upper BB', \
                'Crumble-IOTA.log' using 3:5 with lines title 'Close Price', \
                'Crumble-IOTA.log' using 3:6 with lines title 'MA Price', \
                'Crumble-IOTA.log' using 3:4 with lines title 'Lower BB', \
                'Crumble-IOTA.log' using 3:8 lt rgb 'red' title 'Sell Signal', \
                'Crumble-IOTA.log' using 3:9 lt rgb 'black' title 'Buy Signal'