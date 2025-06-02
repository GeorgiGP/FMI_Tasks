select * from ships  
select * from CLASSES
insert into CLASSES VALUES('Nelson', 'bb', 'Gt.Britain', 9, 16, 34000)
select * from CLASSES
insert into ships VALUES('Nelson', 'Nelson', 1927), ('Rodney', 'Nelson', 1927)
select * from ships 

select * from SHIPS
DELETE FROM SHIPS WHERE SHIPS.NAME IN (SELECT SHIP FROM OUTCOMES WHERE RESULT = 'sunk')
select * from SHIPS

select * from classes
update classes SET BORE *= 2.5, DISPLACEMENT *= 1.1
select * from classes