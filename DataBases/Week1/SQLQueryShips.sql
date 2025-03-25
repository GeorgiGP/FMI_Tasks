select 
	CLASS, 
	COUNTRY 
from CLASSES 
where NUMGUNS <= 10

select 
	NAME SHIPNAME
from SHIPS 
where LAUNCHED <= 1918

select 
	SHIP,
	BATTLE
from OUTCOMES
where RESULT LIKE 'sunk'

select
	NAME
from SHIPS where NAME LIKE CLASS

select distinct NAME
from SHIPS where NAME LIKE 'R%'

select * 
from SHIPS
where NAME LIKE '% %'