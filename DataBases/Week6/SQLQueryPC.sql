SELECT RES1.model, RES1.code AS 'CODE-1', RES1.screen AS screen1,
RES2.code AS 'CODE-2', RES2.screen AS screen2
FROM
(SELECT MODEL, CODE, screen FROM laptop
WHERE screen = 11) RES1 JOIN 
(SELECT MODEL, CODE, screen FROM laptop
WHERE screen = 15) RES2 ON RES1.model=RES2.model

SELECT DISTINCT PR.MODEL FROM product PR JOIN pc ON PR.model=pc.model
WHERE type LIKE 'PC' AND price < ALL(
	SELECT DISTINCT PRICE 
	FROM laptop JOIN product LAMBDA 
	ON laptop.model LIKE LAMBDA.model 
	WHERE LAMBDA.maker LIKE PR.maker
) AND (
	SELECT COUNT(*) 
	FROM laptop JOIN product LAMBDA2
	ON laptop.model LIKE LAMBDA2.model
	WHERE LAMBDA2.maker LIKE PR.maker
) > 0

SELECT DISTINCT PR.MODEL, AVGP AS avg_price
FROM product PR JOIN pc ON PR.model=pc.model JOIN
	(SELECT model, AVG(price) AS AVGP FROM PC PCLAMBDA GROUP BY model) PRICES ON pc.model=PRICES.model
WHERE type LIKE 'PC' AND PRICES.AVGP < ALL(
	SELECT DISTINCT PRICE 
	FROM laptop JOIN product LAMBDA 
	ON laptop.model LIKE LAMBDA.model 
	WHERE LAMBDA.maker LIKE PR.maker
) AND (
	SELECT COUNT(*) 
	FROM laptop JOIN product LAMBDA2
	ON laptop.model LIKE LAMBDA2.model
	WHERE LAMBDA2.maker LIKE PR.maker
) > 0

