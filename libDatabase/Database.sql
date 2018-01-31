CREATE TABLE `tblCoins` ( `id` INTEGER NOT NULL UNIQUE, `name` TEXT, `description` TEXT, `token` TEXT, PRIMARY KEY(`id`) )

CREATE TABLE "tblProducts" ( `Id` INTEGER NOT NULL UNIQUE, `active` INTEGER, `baseCoin` INTEGER, `decimalPlaces` INTEGER, `matchingUnitType` INTEGER, `minQty` REAL, `minTrade` REAL, `quoteCoin` INTEGER, `status` INTEGER, `symbol` TEXT ( 16 ) NOT NULL UNIQUE, `tickSize` INTEGER, `withdrawFee` REAL, FOREIGN KEY(`baseCoin`) REFERENCES `tblCoins`(`id`), FOREIGN KEY(`quoteCoin`) REFERENCES `tblCoins`(`id`), PRIMARY KEY(`Id`) )

CREATE TABLE "tblOneMinutePrices" ( `id` INTEGER NOT NULL, `openTime` INTEGER, `open` REAL, `high` REAL, `low` REAL, `close` REAL, `volume` REAL, `closeTime` INTEGER, `trades` INTEGER, UNIQUE(`id`,`openTime`), FOREIGN KEY(`id`) REFERENCES `tblProducts`(`Id`) )