CREATE DATABASE Facebook
USE Facebook

CREATE TABLE Users (
	id INTEGER,
	email VARCHAR(100),
	password VARCHAR(30),
	registration DATE
);

CREATE TABLE Friends (
	idPerson INTEGER,
	idFriend INTEGER
);

CREATE TABLE Walls (
	idPerson INTEGER,
	idWriter INTEGER,
	text VARCHAR(1000),
	date DATE
);

CREATE TABLE Groups (
	id INTEGER,
	name VARCHAR(100),
	description VARCHAR(2000) DEFAULT ''
);

CREATE TABLE GroupMembers (
	idGroup INTEGER,
	idPerson INTEGER
);

INSERT INTO Friends VALUES(4, 2)
INSERT INTO Users VALUES(2, 'ok.com', 'taina', GETDATE()) 
INSERT INTO Walls VALUES(2, 4, 'waawggagagw', '4-3-2')
INSERT INTO Groups VALUES(1, 'waawggagagw', '4-3-2')
INSERT INTO GroupMembers VALUES(1, 4)

select * from Friends
select * from Users
select * from Walls
SELECT * FROM Groups
SELECT * FROM GroupMembers