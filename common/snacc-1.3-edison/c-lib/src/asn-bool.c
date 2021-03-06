/*
 * asn_bool.c
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
 * $Header: /data/cvs/prototype/story/src/public/tools/snacc-1.3/c-lib/src/asn-bool.c,v 1.1 2006/10/16 09:05:00 joywu Exp $
 * $Log: asn-bool.c,v $
 * Revision 1.1  2006/10/16 09:05:00  joywu
 * no message
 *
 * Revision 1.3  1995/07/24 21:04:49  rj
 * changed `_' to `-' in file names.
 *
 * Revision 1.2  1994/09/01  00:05:57  rj
 * reduce the risk of unwanted surprises with macro expansion by properly separating the C tokens.
 *
 * Revision 1.1  1994/08/28  09:45:51  rj
 * first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
 *
 */

#include "asn-config.h"
#include "asn-len.h"
#include "asn-tag.h"
#include "asn-bool.h"

/*
 * encodes universal TAG LENGTH and Contents of and ASN.1 BOOLEAN
 */
AsnLen
BEncAsnBool PARAMS ((b, data),
    BUF_TYPE b _AND_
    AsnBool *data)
{
    AsnLen len;

    len =  BEncAsnBoolContent (b, data);
    len += BEncDefLen (b, len);
    len += BEncTag1 (b, UNIV, PRIM, BOOLEAN_TAG_CODE);
    return len;
}  /* BEncAsnBool */

/*
 * decodes universal TAG LENGTH and Contents of and ASN.1 BOOLEAN
 */
void
BDecAsnBool PARAMS ((b, result, bytesDecoded, env),
    BUF_TYPE   b _AND_
    AsnBool    *result _AND_
    AsnLen *bytesDecoded _AND_
    jmp_buf env)
{
    AsnTag tag;
    AsnLen elmtLen;

    if ((tag = BDecTag (b, bytesDecoded, env)) != MAKE_TAG_ID (UNIV, PRIM, BOOLEAN_TAG_CODE))
    {
         Asn1Error ("BDecAsnBool: ERROR - wrong tag on BOOLEAN.\n");
         longjmp (env, -40);
    }

    elmtLen = BDecLen (b, bytesDecoded, env);
    BDecAsnBoolContent (b, tag, elmtLen, result, bytesDecoded, env);

}  /* BDecAsnBool */

/*
 * Encodes just the content of the given BOOLEAN value to the given buffer.
 */
AsnLen
BEncAsnBoolContent PARAMS ((b, data),
    BUF_TYPE  b _AND_
    AsnBool  *data)
{
    BufPutByteRvs (b, *data ? 0xFF : 0);
    return 1;
}  /* BEncAsnBoolContent */

/*
 * Decodes just the content of an ASN.1 BOOLEAN from the given buffer.
 * longjmps if there is a buffer reading problem
 */
void
BDecAsnBoolContent PARAMS ((b, tagId, len, result, bytesDecoded, env),
    BUF_TYPE b _AND_
    AsnTag tagId _AND_
    AsnLen len _AND_
    AsnBool  *result _AND_
    AsnLen  *bytesDecoded _AND_
    jmp_buf env)
{
    if (len != 1)
    {
        Asn1Error ("BDecAsnBoolContent: ERROR - BOOLEAN length must be 1\n");
        longjmp (env,-5);
    }

    (*bytesDecoded)++;
    *result = (BufGetByte (b) != 0);

    if (BufReadError (b))
    {
         Asn1Error ("BDecAsnBoolContent: ERROR - decoded past end of data\n");
         longjmp (env, -6);
    }
}  /* BDecAsnBoolContent */

/*
 * Prints the given BOOLEAN to the given FILE * in ASN.1 Value notation.
 * Does not use the indent.
 */
void
PrintAsnBool PARAMS ((f, v, indent),
    FILE *f _AND_
    AsnBool *v _AND_
    unsigned short int indent)
{
    if (*v)
        fprintf (f, "TRUE");
    else
        fprintf (f, "FALSE");
}
