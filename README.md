# Spud's Trading System

This repo consists of a set of apps for the various parts of the trading system

# PriceCollector
A tool to grab the 1 minute interval price data from the binance server and use it to populate a list of coins, products and historic price data in a SQLite database. Binance allow you to grab the last 500 data points which works out about 8.5 hours of prices. Therefore you need to run this app 3 or 4 times a day to make sure you have continous set of data.

# BackTest
Once you have written a strategy, use this tool to run it against the hisitoric price data to see how it performs. It uses a json configuration file to change the settings at runtime. Data that is output from the strategy can be pushed into an application like gnuplot to visualise the data points.

# LiveTrader
Take the strategy you wrote earlier and use it to trade live on th binance exchange. You will need to get your private API key from the binance user page as well as have some real crypto coins to trade with.


# THINGS TO DO
1. Update the database code to create all the tables when run for the first time.
2. Create a basic database with one product, 2 coins and some price data for testing
3. Add the code for live trading
4. Build the LiveTrader app

