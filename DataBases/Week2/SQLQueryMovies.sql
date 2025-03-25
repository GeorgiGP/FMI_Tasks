(select STARSIN.STARNAME from STARSIN 
where MOVIETITLE LIKE 'The Usual Suspects')
INTERSECT
(select MOVIESTAR.NAME from MOVIESTAR where GENDER LIKE 'm')

select STARNAME from STARSIN where STARSIN.MOVIETITLE IN 
((select MOVIETITLE AS TITLE from STARSIN where MOVIEYEAR = 1995)
INTERSECT
(select TITLE from MOVIE where MOVIE.STUDIONAME LIKE 'MGM'))

select NAME from MOVIEEXEC where CERT# IN
(select PRODUCERC# from MOVIE where STUDIONAME LIKE 'MGM')

select TITLE from MOVIE 
where LENGTH > (select LENGTH from MOVIE where TITLE = 'Star Wars')

select NAME from MOVIEEXEC 
where MOVIEEXEC.NETWORTH > (select NETWORTH from MOVIEEXEC where NAME = 'Stephen Spielberg')





