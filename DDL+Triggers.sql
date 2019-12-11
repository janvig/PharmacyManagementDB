create schema MyPharmacyStore;

set  search_path to MyPharmacyStore;

create table Manufacturer (
	Manu_ID char(11) PRIMARY KEY, 
	M_Name varchar(100), 
	M_LicenseNo char(6), 
	M_Address varchar(100), 
	Customer_Care_No bigint 
);

create table Product ( 
	P_ID char(6) PRIMARY KEY, 
	P_Name varchar(100), 
	Stock int, 
	Manu_ID char(11) references Manufacturer(Manu_ID), 
	Category varchar(100)
);

create table HealthCare ( 
	P_ID char(6) references Product(P_ID), 
	Type varchar(100) 
);

create table Vaccine ( 
	P_ID char(6) references Product(P_ID), 
	Disease varchar(100) 
);

create table Medicine ( 
	P_ID char(6) references Product(P_ID), 
	Med_Category varchar(100) 
);

create table Distributor ( 
	D_ID char(6) PRIMARY KEY, 
	D_email varchar(40), 
	D_Name varchar(20), 
	D_GSTIN char(100), 
	D_City varchar(100) 
);

create table Warehouse ( 
	W_ID char(5) PRIMARY KEY, 
	W_Location varchar(100), 
	D_ID char(6) references Distributor(D_ID), 
	Size int 
);

create table Attendant ( 
	Att_ID char(3) PRIMARY KEY, 
	Att_Name varchar(100), 
	Att_Shift char, 
	W_ID char(5) references Warehouse(W_ID), 
	Att_Contact_No bigint 
);

create table Batch ( 
	DateOfExp date, 
	StockPresent int, 
	BatchNo char(5) PRIMARY KEY, 
	P_ID char(6) references Product(P_ID), 
	Date_Of_Manu date 
);

create table Bought_From ( 
	Date_of_Purchase date, 
	Cost_Price real, 
	Quantity_Bought int, 
	P_ID char(6) references Product(P_ID), 
	D_ID char(6) references Distributor(D_ID), 
	W_ID char(5) references Warehouse(W_ID), 
	BatchNo char(5) references Batch(BatchNo) 
);

create table Membership ( 
	Mem_ID char(8) PRIMARY KEY, 
	Mem_Time_Period int, 
	Discount_Rate real, 
	Mem_Name varchar(100) 
);

create table Customer ( 
	C_ID char(4) PRIMARY KEY, 
	C_Name varchar(100),
	C_PhoneNo bigint, 
	Gender char, 
	Age int, 
	Mem_ID char(8) references Membership(Mem_ID) 
);

create table Employee ( 
	ESSN char(6) PRIMARY KEY, 
	E_Name varchar(100), 
	Salary real, 
	Date_Join date 
);

create table Sold_To ( 
	P_ID char(6) references Product(P_ID), 
	TimeStamp time, 
	Date_of_Sale date, 
	Selling_Price real, 
	Invoice_No int, 
	Quantity_Sold int, 
	C_ID char(4) references Customer(C_ID), 
	BatchNo char(5) references Batch(BatchNo), 
	ESSN char(6) references Employee(ESSN) 
);

create table Doctor (
	Doc_ID char(6) PRIMARY KEY, 
	Doc_Phone_No bigint, 
	Doc_Address varchar(100), 
	Doc_Name varchar(100) 
);

create table Referred_By ( 
	Doc_ID char(6) references Doctor(Doc_ID), 
	C_ID char(4) references Customer(C_ID), 
	Prescription_ID varchar(100) 
);

create table Qualification ( 
	Doc_ID char(6) references Doctor(Doc_ID), 
	Doc_Qualification varchar(100) 
);


CREATE FUNCTION soldtobatch()
RETURNS TRIGGER AS $body$
DECLARE
    Quantity_Bought NUMERIC(10);
BEGIN
    IF (TG_OP = 'INSERT') THEN
            SELECT StockPresent INTO Quantity_Bought FROM Batch WHERE BatchNo = NEW.BatchNo;  
            Quantity_Bought := Quantity_Bought - NEW.Quantity_Sold;
            UPDATE Batch SET StockPresent = Quantity_Bought WHERE BatchNo = NEW.BatchNo;           
            RETURN NEW;            
    END IF;
    IF (TG_OP = 'UPDATE') THEN
                    SELECT StockPresent INTO Quantity_Bought FROM Batch WHERE BatchNo = NEW.BatchNo;  
                    Quantity_Bought := Quantity_Bought + OLD.Quantity_Sold - NEW.Quantity_Sold;
                    UPDATE Batch SET StockPresent = Quantity_Bought WHERE BatchNo = NEW.BatchNo;                        
            RETURN NEW;            
    END IF;
    IF (TG_OP = 'DELETE') THEN
            SELECT Quantity_Sold INTO Quantity_Bought FROM Batch WHERE BatchNo = OLD.BatchNo;  
            Quantity_Bought := Quantity_Bought + OLD.Quantity_Sold;
            UPDATE Batch SET Quantity_Sold = Quantity_Bought WHERE BatchNo = OLD.BatchNo;            
            RETURN OLD;            
    END IF;
    RETURN NULL;
END;
$body$ LANGUAGE 'plpgsql';


CREATE TRIGGER batch_stock_change
AFTER INSERT OR UPDATE OR DELETE ON Sold_To
            FOR EACH ROW EXECUTE PROCEDURE soldtobatch();
