select model as MODEL, speed as MHZ, hd as GB from pc where price <= 1200

select distinct maker as MAKER from product where type LIKE 'PRINTER'

select model as MODEL, ram as RAM, screen as SCREEN 
from laptop where price >= 1000

select * from printer where color LIKE 'y'

select 
	model MODEL, 
	speed SPEED, 
	hd HD 
from pc
where cd IN ('12x', '16x') 
AND price <= 2000;

