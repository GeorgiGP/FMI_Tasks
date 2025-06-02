CREATE DATABASE Creating
USE Creating
DROP TABLE PRODUCT
CREATE TABLE PRODUCT (
	model CHAR(4),
	maker DATE,
	type VARCHAR(7)
);
DROP TABLE Printer
CREATE TABLE Printer (
	code INTEGER,
	model CHAR(4),
	price DECIMAL(5,2)
);
select * from PRODUCT
INSERT INTO PRODUCT VALUES('cat', '2016-04-22', 'Neggggg') 
select * from PRODUCT

select * from Printer
ALTER TABLE Printer ADD type CHAR(6) DEFAULT 'jet' CHECK (type IN ('laser', 'matrix', 'jet'))
ALTER TABLE Printer ADD color CHAR CHECK (color IN ('y', 'n')) DEFAULT 'n'
INSERT INTO Printer VALUES(2, '-22', 128.23, DEFAULT, DEFAULT) 
ALTER TABLE Printer DROP COLUMN price

select * from Printer