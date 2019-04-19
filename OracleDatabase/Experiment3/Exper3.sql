select * from EMP;
select * from DEPT;

set serveroutput on;

declare
    en emp.empno%type;
    jb emp.job%type;
begin
    DBMS_output.put_line('������Ա�����');
    en := &sth;
    select JOB into jb from EMP where empno = en;
    if en > 7000 then 
        DBMS_output.put_line('�����ǣ�' || jb);
    end if;
end;

declare
    sum_num number := 0;
begin
    for i in 1..100 loop
        sum_num := sum_num + i;
    end loop;
    DBMS_output.put_line('1 �� 100 �����ֵĺ��ǣ�' || sum_num);
end;

select * from EMP;
select * from DEPT;

-- ʹ�ü��ϵĸ���� emp �������һ��������Ա����Ϣ
declare
    type emplyee_collection is record(
        em_no       emp.empno%type,
        em_name     emp.ename%type,
        em_job      emp.job%type,
        em_mgr      emp.mgr%type,
        em_hiredate emp.hiredate%type,
        em_sal      emp.sal%type,
        em_comm     emp.comm%type,
        em_deptno   emp.deptno%type
    );
    emplyee emplyee_collection;
begin
    emplyee.em_no := 1111;
    emplyee.em_name := 'sss';
    emplyee.em_job := 'ss';
    emplyee.em_mgr := 2222;
    emplyee.em_hiredate := to_date('2011-08-22', 'yyyy-mm-dd');
    emplyee.em_sal := 3333;
    emplyee.em_comm := 10;
    emplyee.em_deptno := 20;
    insert into emp values emplyee;
end;

declare
    type index_table is table of varchar2(20) index by pls_integer;
    demo index_table;
begin
    demo(1) := 'aaa';
    demo(2) := 'bbb';
    demo(3) := 'ccc';
    
    DBMS_output.put_line('�������ı���');
    for i in 1..demo.count Loop
        DBMS_output.put_line(demo(i));
    end Loop;
end;

declare
    TYPE nested_table IS TABLE OF VARCHAR(20);
    demo nested_table := nested_table('aaa', 'bbb', 'ccc');
begin
    DBMS_output.put_line('Ƕ�ױ��ı���');
    for i in 1..demo.count Loop
        DBMS_output.put_line(demo(i));
    end Loop;
end;


declare
    type vary_array is varray(7) of varchar2(20);
    demo vary_array := vary_array('aaa', 'bbb', 'ccc');
begin
    DBMS_output.put_line('�ɱ�����ı���');
    for i in 1..demo.count Loop
        DBMS_output.put_line(demo(i));
    end Loop;
end;
