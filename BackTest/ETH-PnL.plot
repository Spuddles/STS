set terminal png size 1600,1200 enhanced
set title 'ETHBTC PnL (1 minute data)'
plot 'Position-ETH.csv' using 4:5 with lines title 'Profit and Loss'