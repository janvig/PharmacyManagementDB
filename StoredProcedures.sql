--1) To list the details of a bill using Invoice number


CREATE or replace FUNCTION mypharmacystore.details_by_invoice(v_invo bigint)
    RETURNS table(
        q  character varying
        )
    LANGUAGE 'plpgsql'
    
AS $BODY$
declare
        r record;
        query text;
begin
        query:='select *  from mypharmacystore.sold_to';
        for r in execute query
        loop
        if(r.invoice_no=v_invo)then
        q:=r.p_id;
        return next;
        end if;
        end loop;
        
        
end
$BODY$;



--2) To find the total count of a particular product sold to a customer


CREATE or replace FUNCTION mypharmacystore.total_count_by_c_id_and_p_name(cus_id character varying,med_name character varying)
    RETURNS int


    LANGUAGE 'plpgsql'
    
AS $BODY$
declare
	co int;


begin
        select c_id,p_name,count(*) into co from product natural join sold_to where c_id = cus_id and p_name = med_name order by c_id,p_name;
        return co;
end
$BODY$;


--3) Prescribed by a doctor


CREATE or replace FUNCTION mypharmacystore.ref_by_doctor_count(d_id character varying)
    RETURNS table(
                c_id character varying,
                count int,
                sum int
        )
    LANGUAGE 'plpgsql'
    
AS $BODY$
declare
        r record;
        cc_id character varying;
        co int;
        su int;
        query text;
begin
        query:='select doc_id, c_id,count(*)as co,sum(quantity_sold)as su from mypharmacystore.sold_to natural join mypharmacystore.referred_by group by doc_id, c_id';
        for r in execute query
        loop
                if(r.doc_id=d_id)then
		        cc_id:=r.c_id;
		        co:=r.co;
		        su:=r.su;
		        return next;
                end if;
        end loop;
end
$BODY$;

