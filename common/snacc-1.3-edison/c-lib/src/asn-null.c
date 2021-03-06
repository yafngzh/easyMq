/*
 * asn_null.c - BER encode, decode, print and free routines for the
 *              ASN.1 NULL type.
 *
 * MS 92
 * Copyright (C) 1992 Michael Sample and the University of British Columbia
 *
 * This library is free software; you can redistribute it and/or
 * modify it provided that this copyright/license information is retained
 * in original form.
 *
 * If you modify this file, you must clearly indicate your changes.
 *
 * This source code is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * $Header: /data/cvs/prototype/story/src/public/tools/snacc-1.3/c-lib/src/asn-null.c,v 1.1 2006/10/16 09:05:00 joywu Exp $
 * $Log: asn-null.c,v $
 * Revision 1.1  2006/10/16 09:05:00  joywu
 * no message
 *
 * Revision 1.3  1995/07/24 21:04:52  rj
 * changed `_' to `-' in file names.
 *
 * Revision 1.2  1994/09/01  00:06:08  rj
 * reduce the risk of unwanted surprises with macro expansion by properly separating the C tokens.
 *
 * Revision 1.1  1994/08/28  09:45:57  rj
 * first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
 *
 */

#include "asn-config.h"
#include "asn-len.h"
#include "asn-tag.h"
#include "asn-null.h"


/*
 * encodes universal TAG LENGTH and Contents of and ASN.1 NULL
 */
AsnLen
BEncAsnNull PARAMS ((b, data),
    BUF_TYPE     b _AND_
    AsnNull *data)
{
    AsnLen len;

    len =  BEncAsnNullContent (b, data);
    len += BEncDefLen (b, len);
    len += BEncTag1 (b, UNIV, PRIM, NULLTYPE_TAG_CODE);
    return len;
}  /* BEncAsnNull */


/*
 * decodes universal TAG LENGTH and Contents of and ASN.1 NULL
 */
void
BDecAsnNull PARAMS ((b, result, bytesDecoded, env),
    BUF_TYPE b _AND_
    AsnNull *result _AND_
    AsnLen *bytesDecoded _AND_
    jmp_buf env)
{
    AsnTag tag;
    AsnLen elmtLen;

    if ((tag = BDecTag (b, bytesDecoded, env)) != MAKE_TAG_ID (UNIV, PRIM, NULLTYPE_TAG_CODE))
    {
         Asn1Error ("BDecAsnNull: ERROR wrong tag on NULL.\n");
         longjmp (env, -40);
    }

    elmtLen = BDecLen (b, bytesDecoded, env);
    BDecAsnNullContent (b, tag, elmtLen, result, bytesDecoded, env);

}  /* BDecAsnNull */


void
BDecAsnNullContent PARAMS ((b, tagId, len, result, bytesDecoded, env),
    BUF_TYPE b _AND_
    AsnTag tagId _AND_
    AsnLen len _AND_
    AsnNull *result _AND_
    AsnLen *bytesDecoded _AND_
    jmp_buf env)
{
    if (len != 0)
    {
        Asn1Error ("BDecAsnNullContent: ERROR - NULL type's len must be 0\n");
        longjmp (env, -17);
    }
}  /* BDecAsnNullContent */

/*
 * Prints the NULL value to the given FILE * in Value Notation.
 * ignores the indent.
 */
void
PrintAsnNull PARAMS ((f,v, indent),
    FILE *f _AND_
    AsnNull *v _AND_
    unsigned short int indent)
{
    fprintf (f, "NULL");
}
