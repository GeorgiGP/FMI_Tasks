SELECT ROUND(AVG(speed), 2) AS AvgSpeed 
FROM pc

SELECT maker, AVG(laptop.screen) AS AvgScreen
FROM product JOIN laptop ON PRODUCT.model=laptop.model
WHERE TYPE LIKE 'Laptop'
GROUP BY maker

SELECT AVG(speed) AS AvgSpeed
FROM laptop 
WHERE price >= 1000

SELECT maker, AVG(PRICE) AS AvgPrice
FROM product JOIN pc ON PRODUCT.model=pc.model
WHERE MAKER LIKE 'A' AND PRODUCT.type LIKE 'PC'
GROUP BY MAKER

SELECT maker, AVG(PRICE) AS AvgPrice
FROM (
	SELECT MAKER, code, product.model, PRICE
	FROM product JOIN pc ON PRODUCT.model=pc.model
	WHERE MAKER LIKE 'B' AND PRODUCT.type LIKE 'PC'
	UNION
	SELECT MAKER, code, product.model, PRICE
	FROM product JOIN laptop ON PRODUCT.model=laptop.model
	WHERE MAKER LIKE 'B' AND PRODUCT.type LIKE 'Laptop'
) LAMBDA_RES
GROUP BY LAMBDA_RES.maker

SELECT speed, AVG(price) AS AvgPrive
from pc
GROUP BY speed

SELECT maker, COUNT(code) as number_of_pc
FROM product JOIN PC ON PRODUCT.model=PC.model
WHERE PRODUCT.type LIKE 'PC'
GROUP BY maker
HAVING COUNT(code) >= 3

SELECT maker, price
FROM product JOIN PC ON PRODUCT.model=PC.model
WHERE PRODUCT.type LIKE 'PC' AND price >= ALL(
	SELECT PRICE FROM PC JOIN product ON PC.model=PRODUCT.model
)

SELECT speed, AVG(PRICE) AS AvgPrice
FROM PC
GROUP BY speed
HAVING SPEED >= 800

SELECT maker, AVG(hd) AS AvgHDD
FROM product JOIN PC ON PRODUCT.model=PC.model
WHERE PRODUCT.type LIKE 'PC'
GROUP BY MAKER
HAVING MAKER IN(SELECT DISTINCT MAKER FROM product WHERE TYPE LIKE 'Printer')