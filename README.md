# Spud's Trading System

This repo consists of a set of apps for the various parts of the trading system

## PriceCollector
A tool to grab the 1 minute interval price data from the binance server and use it to populate a list of coins, products and historic price data in an SQLite database. Binance allow you to grab the last 500 data points which works out about 8.5 hours of prices. Therefore you need to run this app 3 or 4 times a day to make sure you have a continous set of data.

## BackTest
Once you have written a strategy, use this tool to run it against the historic price data to see how it performs. It uses a json configuration file to change the settings at runtime. Data that is output from the strategy can be pushed into an application like gnuplot to visualise the data points.

## LiveTrader
Take the strategy you wrote earlier and use it to trade live on the binance exchange. You will need to get your private API key from the binance user page as well as have some real crypto coins to trade with.


## THINGS TO DO
* Update the database code to create all the tables when run for the first time.
* Add the SHA256 and HMAC code to the repo
* Fix json code to handle strings or doubles as the API keeps changing!
* Create a WebSocket WinHTTP based class to handle user and public data streams
* Add list of 3rd party apps to this file
* Add the code for live trading
    - Send a simple test trade
    - Set up a socket endpoint to receive trade information 
    - Get account status from the exchange
* Build the LiveTrader app

