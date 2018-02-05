set terminal png size 1600,1200 enhanced
set title 'CNDBTC PnL (1 minute data)'
plot 'Position-CND.csv' using 4:5 with lines title 'Profit and Loss'