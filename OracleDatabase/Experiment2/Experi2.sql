

select * from EMP where SUBSTR(ENAME, 0, 3) = 'JAM';

select EMPNO, ENAME, JOB,  MGR, HIREDATE, SAL, COMM, DEPTNO from EMP where DEPTNO = 10;
select EMPNO, SUBSTR(ENAME, 4), JOB,  MGR, HIREDATE, SAL, COMM, DEPTNO from EMP where DEPTNO = 10;

SELECT * FROM EMP WHERE LAST_DAY(HIREDATE) - 2 = HIREDATE;

SELECT ENAME, SAL, DECODE(JOB, 'CLERK', '??' ,'SALESMAN', '??', 'MANAGER', '??', 'ANALYST', '???', 'PRESIDENT', '??') JOB FROM EMP;
SELECT EMPNO, ENAME, HIREDATE, JOB, nvl(SLA)+nvl(COMM) FROM EMP WHERE HIREDATE between '1-1? -1981' and '31-12? -1981';

select * from SALGRADE;
select * from EMP;
select * from DEPT;

--  SUM(SAL + NVL(COMM, 0)) > 5000 and  

select a ????, b ????
from (
    select JOB a, SUM(SAL + NVL(COMM, 0))  b
    from EMP
    where JOB != 'SALESMAN'
    group by JOB
)
where b > 5000
order by b;


  select JOB a, SUM(SAL + NVL(COMM, 0))  b
    from EMP
    where JOB != 'SALESMAN'
    group by JOB;


select to_char(sysdate-hiredate, 'YYYY') from EMP;
select sysdate-hiredate from EMP;
select to_char(sysdate, 'yyyy') from EMP;
select avg(to_char(sysdate, 'yyyy')-to_char(hiredate, 'yyyy')) from EMP;





select NVL2(COMM, '???', '???') ????, 
            round(AVG( SAL + NVL(COMM, 0)), 2) ????, 
            count(*) ??, 
            avg(to_char(sysdate, 'yyyy')-to_char(hiredate, 'yyyy')) ??????
from EMP
group by NVL2(COMM, '???', '???');




select s.GRADE, count(*), round(AVG( e.SAL + NVL(e.COMM, 0)), 2)
from EMP e, SALGRADE s
where e.SAL + NVL(e.COMM, 0) between s.LOSAL and s.HISAL
group by s.GRADE;


select e.EMPNO ??, e.ENAME ??, e.JOB ??, 
            to_char(e.HIREDATE, 'YYYY-MM-DD') ????,
            e1.ENAME ????, e.SAL + NVL(e.COMM, 0) ?????, 
            s.grade ????, d.deptno ????, 
            d.dname ????, d.loc ????
from EMP e, EMP e1, DEPT d, salgrade s
where e.DEPTNO=d.DEPTNO and e.MGR=e1.EMPNO(+) and
            e.SAL + NVL(e.COMM, 0) between s.LOSAL and s.HISAL and
            e.SAL between 1500 and 3500
order by e.SAL + NVL(e.COMM, 0) desc,  e.hiredate; 










