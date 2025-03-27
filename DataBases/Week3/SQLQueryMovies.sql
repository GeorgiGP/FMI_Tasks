select NAME from MOVIESTAR
where GENDER LIKE 'F' AND NAME IN 
(select NAME from MOVIEEXEC
where NETWORTH >= 10000000)

select NAME from MOVIESTAR
where NAME NOT IN 
(select NAME from MOVIEEXEC)

select TITLE from MOVIE
where LENGTH > (select LENGTH from MOVIE 
where TITLE LIKE 'Star Wars')

select TITLE, EXE.NAME 
from MOVIE M, 
	(select NAME, CERT# from MOVIEEXEC 
	where NETWORTH > 
	(select NETWORTH from MOVIEEXEC 
	where NAME LIKE 'Merv Griffin')) EXE
where M.PRODUCERC# = EXE.CERT#


