select * from PC
SELECT * FROM laptop
SELECT * FROM printer
SELECT * FROM product

--We should validate that the model corespont to the type
SELECT maker, model, type FROM product
EXCEPT (
	SELECT MAKER, PC.model, TYPE 
	FROM PC JOIN product ON PC.model=PRODUCT.model
	WHERE PRODUCT.type LIKE 'PC'
	UNION
	SELECT MAKER, laptop.model, TYPE 
	FROM laptop JOIN product ON laptop.model=PRODUCT.model
	WHERE PRODUCT.type LIKE 'Laptop'
	UNION
	SELECT MAKER, printer.model, PRODUCT.TYPE
	FROM printer JOIN product ON printer.model=PRODUCT.model
	WHERE PRODUCT.type LIKE 'Printer'
)

SELECT MAKER FROM product WHERE TYPE LIKE 'Laptop'
INTERSECT
SELECT MAKER FROM product WHERE TYPE LIKE 'Printer'

SELECT HD FROM laptop
GROUP BY HD
HAVING COUNT(*) >= 2

--2nd way
SELECT DISTINCT l1.hd FROM laptop as l1 JOIN laptop as l2 ON l1.hd = l2.hd 
WHERE l1.code < l2.code

SELECT MODEL 
FROM PC 
WHERE MODEL NOT IN (
	SELECT MODEL 
	FROM product 
	WHERE TYPE LIKE 'PC'
)
