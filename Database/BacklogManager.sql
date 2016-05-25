/*Strong entities*/
CREATE TABLE Activity(Name VARCHAR(255) PRIMARY KEY);
CREATE TABLE Listing(LID INT PRIMARY KEY,Title VARCHAR(255), ActivityName VARCHAR(255), FOREIGN KEY(ActivityName) REFERENCES Activity(Name) );
CREATE TABLE UserDefinedField(Name VARCHAR(255) PRIMARY KEY,DataType VARCHAR(255),Description VARCHAR(255));

/*Weak entities*/
CREATE TABLE UDFentry(Data BLOB, UDFname VARCHAR(255), FOREIGN KEY(UDFname) REFERENCES UserDefinedField(Name));

/*many to many relationships*/
CREATE TABLE Listing_UDF(LID INT, UDFname VARCHAR(255), FOREIGN KEY(LID) REFERENCES Listing(LID), FOREIGN KEY(UDFname) REFERENCES UserDefinedField(Name));
CREATE TABLE Listing_UDFentry(LID INT, UDFname VARCHAR(255), EntryData BLOB, FOREIGN KEY(LID) REFERENCES Listing(LID), FOREIGN KEY(UDFname) REFERENCES UserDefinedField(Name), FOREIGN KEY(EntryData) REFERENCES UDFentry(Data) );

