@echo off
call runsingle ETH
call runsingle LTC
call runsingle BNB
call runsingle NEO

gnuplot PnL.plot > PnL.png

start PnL.png