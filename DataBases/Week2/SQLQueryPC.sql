select maker, speed
from laptop, product
where laptop.model = product.model AND hd > 9

(select product.model, price 
from product, laptop
where product.model = laptop.model AND maker LIKE 'B')
UNION
(select product.model, price
from product, pc
where product.model = pc.model AND maker LIKE 'B')

select distinct maker from product where type LIKE 'Laptop'
EXCEPT
(select distinct maker from product where type LIKE 'PC')


select * from product
select * from laptop
select * from pc

select hd
from pc 
GROUP BY hd
HAVING COUNT(code) >= 2;

select pc1.model, pc2.model
from pc AS pc1, pc AS pc2
where pc1.hd = pc2.hd AND 
pc1.speed = pc2.speed AND 
pc1.model < pc2.model

select maker
from product, pc
where product.model = pc.model AND speed >= 400
GROUP BY maker
HAVING COUNT(code) >= 2






