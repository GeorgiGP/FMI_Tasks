INSERT INTO PC VALUES(12, 1100, 2400, 2048, 500, '52x', 299)
SELECT * FROM PC
INSERT INTO product VALUES('C', 1100, 'PC')
SELECT * FROM product

SELECT * FROM PC
DELETE FROM PC WHERE PC.model = 1100
SELECT * FROM PC

SELECT * FROM laptop
DELETE FROM laptop 
WHERE laptop.model IN 
	(SELECT PRODUCT.model 
	FROM product 
	WHERE type = 'laptop' AND product.maker NOT IN 
		(SELECT DISTINCT MAKER 
		FROM printer JOIN product ON printer.model = PRODUCT.model)
	)
SELECT * FROM laptop

SELECT * FROM product
UPDATE product SET maker='A' WHERE maker = 'B'
SELECT * FROM product

SELECT * FROM PC
UPDATE PC SET price /= 2 , hd += 20
SELECT * FROM PC

select * from product
select * from laptop
UPDATE laptop SET screen += 1 WHERE MODEL IN (SELECT model FROM product WHERE maker = 'B' AND type = 'laptop')
select * from laptop