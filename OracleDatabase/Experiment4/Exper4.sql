
set serveroutput on;

-- ??????5100?????5100???????

DECLARE 
        erow emp%ROWTYPE;
        CURSOR mycursor IS SELECT * FROM emp;
BEGIN 
      FOR erow IN mycursor LOOP
        IF erow.deptno=10 THEN 
          IF erow.sal*1.1>5100 THEN 
            UPDATE emp SET sal=5100 WHERE empno=erow.empno;
          ELSE
            UPDATE emp SET sal=sal*1.1 WHERE empno=erow.empno;
          END IF;
        ELSIF erow.deptno=20 THEN 
          IF erow.sal*1.2>5100 THEN 
            UPDATE emp SET sal=5100 WHERE empno=erow.empno;
          ELSE
            UPDATE emp SET sal=sal*1.2 WHERE empno=erow.empno;
          END IF;
        ELSIF erow.deptno=30 THEN 
          IF erow.sal*1.3>5100 THEN 
            UPDATE emp SET sal=5100 WHERE empno=erow.empno;
          ELSE
            UPDATE emp SET sal=sal*1.3 WHERE empno=erow.empno;
          END IF;
        ELSE
          NULL;
        END IF;
      END LOOP;
END;

select * from EMP;

-- ?????????????????????


CREATE OR REPLACE PROCEDURE get_emp_info_proc(p_eno emp.empno%TYPE)
AS
	v_ename	emp.ename%TYPE ;
	v_sal		emp.sal%TYPE ;
	v_count 	NUMBER ;
BEGIN
	SELECT COUNT(empno) INTO v_count FROM emp WHERE empno=p_eno ;
	IF v_count = 0 THEN
		RETURN ;
	END IF ;
	SELECT ename,sal INTO v_ename,v_sal FROM emp WHERE empno=p_eno ;
	DBMS_OUTPUT.put_line('ID is ' || p_eno  || ' staff name is: ' || v_ename || ', salary is: ' || v_sal) ;
END;

--SET SERVEROUTPUT ON;
begin
  get_emp_info_proc(7698);
end;

-- ?????????????????

CREATE OR REPLACE FUNCTION get_salary_fun(p_eno emp.empno%TYPE) 
RETURN NUMBER
AS
	v_salary emp.sal%TYPE ;
BEGIN
	SELECT sal + nvl(comm,0) INTO v_salary FROM emp WHERE empno=p_eno ;
	RETURN v_salary ;
END;

SET SERVEROUTPUT ON;
DECLARE
	v_salary	NUMBER;
BEGIN
	v_salary := get_salary_fun(7902);
	DBMS_OUTPUT.put_line('The salary of staff 7902 is: ' || v_salary);
END;



