select * from STUDIO

select ADDRESS from STUDIO
where NAME LIKE '%Disney%'

select BIRTHDATE from MOVIESTAR
where NAME LIKE '%Jack Nicholson'

select STARNAME from STARSIN
where MOVIEYEAR = 1980 OR MOVIETITLE LIKE '%Knight%'

select distinct NAME from MOVIEEXEC 
where NETWORTH > 10000000

select distinct NAME from MOVIESTAR where GENDER LIKE '%M%' OR ADDRESS LIKE '%Prefect Rd%'
