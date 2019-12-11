#include <stdio.h>
#include <string.h>
#include <stdlib.h>


EXEC SQL BEGIN DECLARE SECTION;


//exeUpdate variables//
char upmanuID[11];
long int upCustCareNo;

//exedelete variables//
char delmanuID[11];

//exeINSERT variables//
char manuID[11];
char mName[100];
char mLicenseNo[6];
char mAdd[100];
long int CustCareNo;

//query1 variables//
char q1PID[6];
char q1Pname[50];
int q1Stock;
char q1ManuID[11];
char  q1Category[30];
char q1Med_Category[30];

//query2 variable//
char q2PID[6];
char q2Pname[50];
int q2Stock;
char q2ManuID[11];

//query3 variables//
char q3DID[6];
char q3DEmail[40];
char q3DName[20];
char q3D_GSTIN[100];
char q3D_City[100];

//query4 variables//
char q4DateOfPurchase[10];
double q4CostPrice;
int q4Qty;
char q4PID[6];
char q4DID[6];
char q4WID[5];
char q4BatchNo[5];  


EXEC SQL END DECLARE SECTION;

void query4()
{
    printf("Enter distributor ID : ");
    scanf("%6s", q4DID);
    EXEC SQL DECLARE curs4 CURSOR FOR select b.* from "mypharmacystore"."bought_from" as b natural join "mypharmacystore"."product" where b."d_id" = :q4DID;
    EXEC SQL OPEN curs4;
    EXEC SQL FETCH curs4 INTO :q4DateOfPurchase,:q4CostPrice,:q4Qty,:q4PID,:q4DID,:q4WID,:q4BatchNo;        

    while(sqlca.sqlcode == 0)
    {
        printf("Date_of_Purchase = %s, Cost_Price = %f, Quantity_Bought = %d, P_ID = %s, D_ID = %s, Warehouse_ID = %s, BatchNo = %s\n", q4DateOfPurchase, q4CostPrice, q4Qty, q4PID, q4DID, q4WID, q4BatchNo);
	EXEC SQL FETCH curs4 INTO :q4DateOfPurchase,:q4CostPrice,:q4Qty,:q4PID,:q4DID,:q4WID,:q4BatchNo;        
    }
    EXEC SQL CLOSE curs4;
}

void query3()
{
    printf("Enter the name of a city : ");
    scanf("%s", q3D_City);
    EXEC SQL DECLARE curs3 CURSOR FOR select * from "mypharmacystore"."distributor" as d where d."d_city" = :q3D_City ;
    EXEC SQL OPEN curs3;
    EXEC SQL FETCH curs3 INTO :q3DID,:q3DEmail,:q3DName,:q3D_GSTIN,:q3D_City;
    
    while(sqlca.sqlcode == 0)
    {
        printf("D_ID = %s , D_Email = %s, D_Name = %s, D_GSTIN = %s, D_City = %s, \n",q3DID,q3DEmail,q3DName,q3D_GSTIN,q3D_City);
        EXEC SQL FETCH curs3 INTO :q3DID,:q3DEmail,:q3DName,:q3D_GSTIN,:q3D_City;
    }
    EXEC SQL CLOSE curs3;
}

void query1()
{
  
    EXEC SQL DECLARE curs1 CURSOR FOR SELECT * FROM "mypharmacystore"."product" AS p natural JOIN "mypharmacystore"."medicine" AS m where m."med_category" = 'Antibiotic';
    
    EXEC SQL OPEN curs1;
    EXEC SQL FETCH curs1 INTO :q1PID,:q1Pname,:q1Stock,:q1ManuID,:q1Category,:q1Med_Category;
    
    while(sqlca.sqlcode == 0)
    {
        printf("P_ID = %s, P_Name = %s, Stock = %d, Manu_ID = %s, Category = %s, Med_Category = %s\n", q1PID, q1Pname, q1Stock, q1ManuID, q1Category, q1Med_Category);
           EXEC SQL FETCH curs1 INTO :q1PID, :q1Pname, :q1Stock, :q1ManuID, :q1Category, :q1Med_Category;
  }
    EXEC SQL CLOSE curs1;        
}

void exeUpdate()
{
    printf("Enter Manufacturer's ID whose Customer Care No is to be updated : ");
    scanf("%11s", upmanuID);
    printf("Enter New Number : ");
    scanf("%ld", &upCustCareNo);
    
    EXEC SQL UPDATE "mypharmacystore"."manufacturer" as m set m."customer_care_no" = :upCustCareNo where m."manu_ID" = :upmanuID;
    EXEC SQL COMMIT;
    if(sqlca.sqlcode == 0)
    {
        printf("Update successful\n");
    }    
    else
    {
        EXEC SQL WHENEVER SQLERROR SQLPRINT;
    }
}

void exeDelete()
{
    printf("Enter Manufacturer ID to be removed : ");
    scanf("%11s", delmanuID);
    EXEC SQL DELETE FROM "mypharmacystore"."manufacturer" as m where m."manu_ID" = :delmanuID;
    EXEC SQL COMMIT;
    if(sqlca.sqlcode == 0)
        printf("Delete successful\n");
    else
    {
        EXEC SQL WHENEVER SQLERROR SQLPRINT;
    }
}

void exeInsert()
{
    printf("Enter Manufacturer's ID(11chars): ");
    scanf("%11s", manuID);
    printf("Enter Manufacturer's Name : ");
    scanf("%s", mName);
    printf("Enter License Number(6chars): ");
    scanf("%6s", mLicenseNo);
    printf("Enter Address : ");
    scanf("%s", mAdd);
    printf("Enter Customer Care Number : ");
    scanf("%ld", &CustCareNo);
    EXEC SQL INSERT INTO "mypharmacystore"."manufacturer" VALUES(:manuID, :mName, :mLicenseNo, :mAdd, :CustCareNo);
    EXEC SQL COMMIT;
    if(sqlca.sqlcode == 0)
        printf("Insert successful\n");
    else
    {
        EXEC SQL WHENEVER SQLERROR SQLPRINT;
    }
}

int main ()
{    
    EXEC SQL CONNECT TO "201701069@10.100.71.21" USER "201701069" USING "201701069";
    EXEC SQL WHENEVER SQLERROR SQLPRINT;
    EXEC SQL WHENEVER SQLWARNING SQLPRINT;    
    EXEC SQL set search_path to Online_streaming_site;
   
    int close=0;
    while(close == 0)
    {
        printf("1) Add a new Manufacturer\n");
        printf("2) Update Customer Care Number\n");
        printf("3) Remove Manufacturer\n");
        printf("4) To run query\n"); 
        int x;
        scanf("%d", &x);
        if(x == 1)
        {
            exeInsert();
        }
        else if(x == 2)
        {
            exeUpdate();
        }
        else if(x == 3)
        {
            exeDelete();
        }
        else if(x == 4)
        {
            printf("1) List details of all antibiotics available at the store\n");
           printf("2) List the details of all the medicines sold to the shop by a given distributor\n");
            printf("3) Information about distributor(s) in a given city\n");
            
            int p;
            scanf("%d", &p);
            if(p == 1)
            {
                query1();
            }
           
            else if(p == 3)
            {
                query3();
            }
            else if(p == 2)
            {
                query4();
            }
        }
        else
            exit(0);
        printf("Enter 1 to exit the program and 0 to continue\n");
        scanf("%d", &close);
    }    return 0;
}
