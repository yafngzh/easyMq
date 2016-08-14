/*
 *    p-rec.h
 *
 *    "P-REC" ASN.1 module C type definitions and prototypes
 *
 *    This .h file was generated by snacc on Wed Sep  5 14:26:27 2007
 *
 *    UBC snacc written compiler by Mike Sample
 *
 *    NOTE: This is a machine generated file--editing not recommended
 */


#ifndef _p_rec_h_
#define _p_rec_h_




typedef AsnInt EmployeeNumber; /* [APPLICATION 128] IMPLICIT INTEGER */

#define BEncEmployeeNumberContent BEncAsnIntContent

#define BDecEmployeeNumberContent BDecAsnIntContent

#define PrintEmployeeNumber PrintAsnInt

#define FreeEmployeeNumber FreeAsnInt 




typedef struct Name /* [APPLICATION 1] IMPLICIT SEQUENCE */
{
    IA5String givenName; /* IA5String */
    IA5String initial; /* IA5String */
    IA5String familyName; /* IA5String */
} Name;

AsnLen BEncNameContent PROTO ((BUF_TYPE b, Name *v));

void BDecNameContent PROTO ((BUF_TYPE b, AsnTag tagId0, AsnLen elmtLen0, Name *v, AsnLen *bytesDecoded, ENV_TYPE env));


void PrintName PROTO ((FILE* f, Name *v, unsigned short int indent));
void FreeName PROTO ((Name *v));





typedef IA5String Date; /* [APPLICATION 3] IMPLICIT IA5String */

AsnLen BEncDate PROTO ((BUF_TYPE b, Date *v));

void BDecDate PROTO ((BUF_TYPE b, Date *result, AsnLen *bytesDecoded, ENV_TYPE env));
#define BEncDateContent BEncIA5StringContent

#define BDecDateContent BDecIA5StringContent

#define PrintDate PrintIA5String

#define FreeDate FreeIA5String 




typedef struct ChildInformation /* SET */
{
    struct Name* name; /* Name */
    Date dateOfBirth; /* [0] Date */
} ChildInformation;

AsnLen BEncChildInformationContent PROTO ((BUF_TYPE b, ChildInformation *v));

void BDecChildInformationContent PROTO ((BUF_TYPE b, AsnTag tagId0, AsnLen elmtLen0, ChildInformation *v, AsnLen *bytesDecoded, ENV_TYPE env));


void PrintChildInformation PROTO ((FILE* f, ChildInformation *v, unsigned short int indent));
void FreeChildInformation PROTO ((ChildInformation *v));





typedef AsnList PersonnelRecordSeqOf; /* SEQUENCE OF ChildInformation */

AsnLen BEncPersonnelRecordSeqOfContent PROTO ((BUF_TYPE b, PersonnelRecordSeqOf *v));

void BDecPersonnelRecordSeqOfContent PROTO ((BUF_TYPE b, AsnTag tagId0, AsnLen elmtLen0, PersonnelRecordSeqOf *v, AsnLen *bytesDecoded, ENV_TYPE env));


void PrintPersonnelRecordSeqOf PROTO ((FILE* f, PersonnelRecordSeqOf *v, unsigned short int indent));
void FreePersonnelRecordSeqOf PROTO ((PersonnelRecordSeqOf *v));





typedef struct PersonnelRecord /* [APPLICATION 0] IMPLICIT SET */
{
    struct Name* name; /* Name */
    IA5String title; /* [0] IA5String */
    EmployeeNumber employeeNumber; /* EmployeeNumber */
    Date dateOfHire; /* [1] Date */
    struct Name* nameOfSpouse; /* [2] Name */
    PersonnelRecordSeqOf* children; /* [3] IMPLICIT PersonnelRecordSeqOf DEFAULT -- snacc warning: can't parse this value yet --{ } */
} PersonnelRecord;

AsnLen BEncPersonnelRecord PROTO ((BUF_TYPE b, PersonnelRecord *v));

void BDecPersonnelRecord PROTO ((BUF_TYPE b, PersonnelRecord *result, AsnLen *bytesDecoded, ENV_TYPE env));
AsnLen BEncPersonnelRecordContent PROTO ((BUF_TYPE b, PersonnelRecord *v));

void BDecPersonnelRecordContent PROTO ((BUF_TYPE b, AsnTag tagId0, AsnLen elmtLen0, PersonnelRecord *v, AsnLen *bytesDecoded, ENV_TYPE env));


void PrintPersonnelRecord PROTO ((FILE* f, PersonnelRecord *v, unsigned short int indent));
void FreePersonnelRecord PROTO ((PersonnelRecord *v));






#endif /* conditional include of p-rec.h */