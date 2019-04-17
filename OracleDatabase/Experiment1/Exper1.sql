create table athlete(
    athleteID NUMBER(4) primary key,
    name varchar2(20),
    sex varchar2(20),
    department varchar2(20)
);

insert into athlete(athleteID, name, sex, department) values(1001,  '??',  '?',  '????');
insert into athlete(athleteID, name, sex, department) values(1002,  '??',  '?',  '???');
insert into athlete(athleteID, name, sex, department) values(1003,  '??',  '?',  '????');
insert into athlete(athleteID, name, sex, department) values(1004,  '??',  '?',  '???');
insert into athlete(athleteID, name, sex, department) values(1005,  '??',  '?',  '???');
insert into athlete(athleteID, name, sex, department) values(1006,  '??',  '?',  '???');

create table item(
    ItemID varchar2(10) primary key,
    itemname varchar2(20),
    location varchar2(10)
);

insert into item(ItemID, itemname, location) values('X001',  '?????', '???');
insert into item(ItemID, itemname, location) values('X002',  '????', '???');
insert into item(ItemID, itemname, location) values('X003',  '????', '???');
insert into item(ItemID, itemname, location) values('X004',  '????', '???');
insert into item(ItemID, itemname, location) values('X005',  '?????', '???');

create table score(
    athleteID NUMBER(4) references athlete(athleteID),
    ItemID varchar2(10) references item(ItemID),
    score NUMBER(4) 
);

insert into score(athleteID, ItemID, score) values(1001,  'X001', 6);
insert into score(athleteID, ItemID, score) values(1002,  'X001', 4);
insert into score(athleteID, ItemID, score) values(1003,  'X001', 2);
insert into score(athleteID, ItemID, score) values(1004,  'X001', 0);
insert into score(athleteID, ItemID, score) values(1001,  'X003', 2);
insert into score(athleteID, ItemID, score) values(1002,  'X003', 6);
insert into score(athleteID, ItemID, score) values(1004,  'X003', 2);
insert into score(athleteID, ItemID, score) values(1003,  'X003', 0);
insert into score(athleteID, ItemID, score) values(1005,  'X004', 6);
insert into score(athleteID, ItemID, score) values(1006,  'X004', 4);
insert into score(athleteID, ItemID, score) values(1001,  'X002', 6);
insert into score(athleteID, ItemID, score) values(1002,  'X002', 4);
insert into score(athleteID, ItemID, score) values(1003,  'X002', 2);
insert into score(athleteID, ItemID, score) values(1006,  'X005', 6);

alter table score add note char(100); 
alter table score modify(note varchar2(100));
alter table athlete add birthday date;

insert into athlete(athleteID, name, sex,  birthday) values(1007,  '??',  '?',  to_date('1998-11-11',  'YYYY-MM-DD'));
insert into score(athleteID, ItemID, score) values(1007, 'X001', 0);
insert into score(athleteID, ItemID, score) values(1007, 'X003', 2);

update score set note='????', score=0 where ItemID='X002';

select * from athlete;
select * from item;
select * from score;