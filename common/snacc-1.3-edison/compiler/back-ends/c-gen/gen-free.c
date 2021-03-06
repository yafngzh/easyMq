/*
 * compiler/back-ends/c-gen/gen-free.c - routines for printing C hierachical free routines
 *
 * Mike Sample
 * 92/04
 * Copyright (C) 1991, 1992 Michael Sample
 *            and the University of British Columbia
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * $Header: /data/cvs/prototype/story/src/public/tools/snacc-1.3/compiler/back-ends/c-gen/gen-free.c,v 1.1 2006/10/16 09:06:30 joywu Exp $
 * $Log: gen-free.c,v $
 * Revision 1.1  2006/10/16 09:06:30  joywu
 * no message
 *
 * Revision 1.3  1995/07/25 18:42:47  rj
 * file name has been shortened for redundant part: c-gen/gen-c-free -> c-gen/gen-free.
 *
 * changed `_' to `-' in file names.
 *
 * Revision 1.2  1994/09/01  00:23:29  rj
 * snacc_config.h and other superfluous .h files removed.
 *
 * Revision 1.1  1994/08/28  09:48:26  rj
 * first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
 *
 */

#include <stdio.h>

#include "asn-incl.h"
#include "asn1module.h"
#include "mem.h"
#include "define.h"
#include "rules.h"
#include "type-info.h"
#include "str-util.h"
#include "util.h"
#include "gen-free.h"

static char *returnTypeG = "void";
static char *valueArgNameG = "v";
static CRules *genFreeCRulesG;

/* non-exported prototypes */

static void PrintCFreePrototype PROTO ((FILE *hdr, TypeDef *td));
static void PrintCFreeDeclaration PROTO ((FILE *src, TypeDef *td));
static void PrintCFreeDefine PROTO ((FILE *hdr, TypeDef *td));
static void PrintCFreeLocals PROTO ((FILE *src,TypeDef *td));
static void PrintCFreeElmts PROTO ((FILE *src, TypeDef *td, Type *parent, NamedTypeList *elmts, char *varName));
static void PrintCElmtFree PROTO ((FILE *src, TypeDef *td, Type *parent, Type *e, char *varName));
static void PrintCFreeListElmts PROTO ((FILE *src, TypeDef *td, Type *t, char *varName));
static void PrintCFreeListRoutineBody PROTO ((FILE *src, TypeDef *td, Type *t,char *varName));
static void PrintCFreeListDefine PROTO ((FILE *hdr,TypeDef *td));
static void PrintCFreeChoiceElmts PROTO ((FILE *src, TypeDef *td, Type *t, char *varName));



void
PrintCFree PARAMS ((src, hdr, r, mods, m, td),
    FILE *src _AND_
    FILE *hdr _AND_
    CRules *r _AND_
    ModuleList *mods _AND_
    Module *m _AND_
    TypeDef *td)
{
    NamedType *e;
    CTDI *ctdi;
    CTypeId rhsTypeId;  /* cTypeId of the type that defined this typedef */

    genFreeCRulesG = r;

    ctdi =  td->cTypeDefInfo;
    if ((ctdi == NULL) || (td->type->cTypeRefInfo == NULL))
    {
        fprintf (stderr,"PrintCFree: ERROR - no type info\n");
        return;
    }

    if (!ctdi->genFreeRoutine)
        return;

    rhsTypeId = td->type->cTypeRefInfo->cTypeId;
    switch (rhsTypeId)
    {
        case C_ANY:
        case C_ANYDEFINEDBY:
        case C_LIB:
        case C_TYPEREF:
            PrintCFreeDefine (hdr, td);
            fprintf (hdr,"\n\n");
            break;

        case C_CHOICE:
            PrintCFreePrototype (hdr, td);
            PrintCFreeDeclaration (src, td);
            fprintf (src,"{\n");
            PrintCFreeLocals (src, td);
            fprintf (src,"    if (%s == NULL)\n", valueArgNameG);
            fprintf (src,"        return;\n", valueArgNameG);
            PrintCFreeChoiceElmts (src, td, td->type, valueArgNameG);
            fprintf (src,"}  /* %s */",td->cTypeDefInfo->freeRoutineName);
            fprintf (hdr,"\n\n");
            fprintf (src,"\n\n");
            break;

        case C_STRUCT:
            PrintCFreePrototype (hdr, td);
            PrintCFreeDeclaration (src, td);
            fprintf (src,"{\n");
            PrintCFreeLocals (src, td);
            fprintf (src,"    if (%s == NULL)\n", valueArgNameG);
            fprintf (src,"        return;\n", valueArgNameG);
            PrintCFreeElmts (src, td, td->type, td->type->basicType->a.set, valueArgNameG);
            fprintf (src,"}  /* %s */", td->cTypeDefInfo->freeRoutineName);
            fprintf (hdr,"\n\n");
            fprintf (src,"\n\n");
            break;


        case C_LIST:
            PrintCFreePrototype (hdr, td);
            PrintCFreeDeclaration (src, td);
            fprintf (src,"{\n");
            PrintCFreeLocals (src, td);
            fprintf (src,"    if (%s == NULL)\n", valueArgNameG);
            fprintf (src,"        return;\n", valueArgNameG);
            PrintCFreeListRoutineBody (src, td, td->type, valueArgNameG);
            fprintf (src,"}  /* %s */", td->cTypeDefInfo->freeRoutineName);
            fprintf (hdr,"\n\n");
            fprintf (src,"\n\n");
            break;



        case C_NO_TYPE:
            break;

        default:
            fprintf (stderr,"PrintCFree: ERROR - unknown c type id\n");
            break;
    }

}  /*  PrintCFree */



/*
 * Prints prototype for encode routine in hdr file
 */
static void
PrintCFreePrototype PARAMS ((hdr, td),
    FILE *hdr _AND_
    TypeDef *td)
{
    CTDI *ctdi;

    ctdi = td->cTypeDefInfo;
    fprintf (hdr,"%s %s PROTO ((%s *v));\n", returnTypeG, ctdi->freeRoutineName, ctdi->cTypeName);

}  /*  PrintCFreePrototype */



/*
 * Prints declarations of encode routine for the given type def
 */
static void
PrintCFreeDeclaration PARAMS ((src, td),
    FILE *src _AND_
    TypeDef *td)
{
    CTDI *ctdi;

    ctdi =  td->cTypeDefInfo;
    fprintf (src,"%s\n%s PARAMS ((v),\n%s *v)\n", returnTypeG, ctdi->freeRoutineName,  ctdi->cTypeName);

}  /*  PrintCFreeDeclaration */




static void
PrintCFreeDefine PARAMS ((hdr, td),
    FILE *hdr _AND_
    TypeDef *td)
{

    fprintf(hdr, "#define %s %s ", td->cTypeDefInfo->freeRoutineName, td->type->cTypeRefInfo->freeRoutineName);

/*
    fprintf(hdr, "#define %s(v)  ", td->cTypeDefInfo->freeRoutineName);
    fprintf (hdr, "%s (v)", td->type->cTypeRefInfo->freeRoutineName);
*/
}  /*  PrintCFreeDefine */




static void
PrintCFreeLocals PARAMS ((src, td),
    FILE *src _AND_
    TypeDef *td)
{
    fprintf (src, "\n");

    if ((td->type->basicType->choiceId == BASICTYPE_SETOF) ||
        (td->type->basicType->choiceId == BASICTYPE_SEQUENCEOF))
    {
        fprintf (src,"    AsnListNode *l;\n");
        fprintf (src,"    AsnListNode *tmp;\n");
    }

}  /*  PrintCFreeLocals */



static void
PrintCFreeElmts PARAMS ((src, td, parent, elmts, varName),
    FILE *src _AND_
    TypeDef *td _AND_
    Type *parent _AND_
    NamedTypeList *elmts _AND_
    char *varName)
{
    NamedType *e;

    if (elmts == NULL)
    {
        fprintf (src,"/* ERROR? - expected elmts for this type*/\n");
        return;
    }

    FOR_EACH_LIST_ELMT (e, elmts)
        PrintCElmtFree (src, td, parent, e->type, varName);

}  /* PrintCBerElmtsEncodeCode */



/*
 * Prints code for encoding the elmts of a SEQ or SET
 */
static void
PrintCElmtFree PARAMS ((src, td, parent, e, varName),
    FILE *src _AND_
    TypeDef *td _AND_
    Type *parent _AND_
    Type *e _AND_
    char *varName)
{
    CTRI *ctri;
    char elmtVarRef[MAX_VAR_REF];
    Type *tmpType;

    if ((e == NULL) || (e->cTypeRefInfo == NULL))
        return;

    ctri =  e->cTypeRefInfo;

    /* build ref to the elmt */
    MakeVarPtrRef (genFreeCRulesG, td, parent, e, varName, elmtVarRef);

    /* if optional then put in NULL check */
    if (e->optional || (e->defaultVal != NULL))
        fprintf (src, "    if (%s (%s))\n    {\n", ctri->optTestRoutineName, elmtVarRef);

    /* free contents of elmt first */
    switch (ctri->cTypeId)
    {
        case C_ANY:
        case C_ANYDEFINEDBY:
        case C_LIB:
        case C_TYPEREF:
            fprintf (src,"    %s (%s);\n", ctri->freeRoutineName, elmtVarRef);
            break;

        case C_LIST:
            PrintCFreeListElmts (src, td, e, elmtVarRef);
            break;

        /*
         * this  follwing shouldn't happen since embedded
         * choices/struct are moved to separate typedefs
         * in normalize.c.
         */
        case C_CHOICE:
            PrintCFreeChoiceElmts (src, td, e, elmtVarRef);
            break;

        case C_STRUCT:
            PrintCFreeElmts (src, td, e, e->basicType->a.set, elmtVarRef);
            break;


        case C_NO_TYPE:
            break;

        default:
            fprintf (stderr,"PrintCElmtFree: ERROR - unknown c type id\n");
            break;
    }

    /* free elmt itself if it is ref'd by ptr */
    if (ctri->isPtr)
        fprintf (src,"    Asn1Free (%s);\n",elmtVarRef);

    /* write closing brkt for NULL check for optional elmts */
    if (e->optional || (e->defaultVal != NULL))
        fprintf (src, "    }\n");

    fprintf (src,"\n");

}  /*  PrintCElmtFree */


static void
PrintCFreeListDefine PARAMS ((hdr, td),
    FILE *hdr _AND_
    TypeDef *td)
{
    fprintf(hdr, "#define %s(v)  ", td->cTypeDefInfo->freeRoutineName);
    fprintf (hdr, "ASN1_FREE_LIST (v, %s)", td->type->cTypeRefInfo->freeRoutineName);
}


static void
PrintCFreeListRoutineBody PARAMS ((src, td, t, varName),
    FILE *src _AND_
    TypeDef *td _AND_
    Type *t _AND_
    char *varName)
{
    Type *e;
    CTRI *ctri;
    char *elmtVarRef;

    fprintf (src,"    for (l = FIRST_LIST_NODE (%s); l != NULL; )\n", varName);
    fprintf (src,"    {\n");

    e = t->basicType->a.setOf;
    ctri = e->cTypeRefInfo;
    elmtVarRef = "(l->data)";
    switch (ctri->cTypeId)
    {
        case C_LIB:
        case C_TYPEREF:
            fprintf (src,"        %s (%s);\n", ctri->freeRoutineName, elmtVarRef);
            break;

        case C_LIST:
            PrintCFreeListElmts (src, td, e, elmtVarRef);
            break;

        /*
         * this  follwing shouldn't happen since embedded
         * choices/struct are moved to separate typedefs
         * in normalize.c.
         */
        case C_CHOICE:
            PrintCFreeChoiceElmts (src, td, e, elmtVarRef);
            break;

        case C_STRUCT:
            PrintCFreeElmts (src, td, e, e->basicType->a.set, elmtVarRef);
            break;


        case C_NO_TYPE:
            break;

        default:
            fprintf (stderr,"PrintCElmtFree: ERROR - unknown c type id\n");
            break;
    }

    fprintf (src,"        tmp = l->next;\n");
    fprintf (src,"        Asn1Free (l->data);\n");
    fprintf (src,"        Asn1Free (l);\n");
    fprintf (src,"        l = tmp;\n");
    fprintf (src,"    }\n");
}

static void
PrintCFreeListElmts PARAMS ((src, td, t, varName),
    FILE *src _AND_
    TypeDef *td _AND_
    Type *t _AND_
    char *varName)
{
    Type *e;
    CTRI *ctri;
    char *elmtVarRef;

    fprintf (src,"    {\n");
    fprintf (src,"        AsnListNode *l;\n");
    fprintf (src,"        AsnListNode *tmp;\n");
    fprintf (src,"        for (l = FIRST_LIST_NODE (%s); l != NULL; )\n", varName);
    fprintf (src,"        {\n");


    e = t->basicType->a.setOf;
    ctri = e->cTypeRefInfo;
    elmtVarRef = "(l->data)";
    switch (ctri->cTypeId)
    {
        case C_LIB:
        case C_TYPEREF:
            fprintf (src,"        %s (%s);\n", ctri->freeRoutineName, elmtVarRef);
            break;

        case C_LIST:
            PrintCFreeListElmts (src, td, e, elmtVarRef);
            break;

        /*
         * this  follwing shouldn't happen since embedded
         * choices/struct are moved to separate typedefs
         * in normalize.c.
         */
        case C_CHOICE:
            PrintCFreeChoiceElmts (src, td, e, elmtVarRef);
            break;

        case C_STRUCT:
            PrintCFreeElmts (src, td, e, e->basicType->a.set, elmtVarRef);
            break;


        case C_NO_TYPE:
            break;

        default:
            fprintf (stderr,"PrintCElmtFree: ERROR - unknown c type id\n");
            break;
    }

    fprintf (src,"           tmp = l->next;\n");
    fprintf (src,"           Asn1Free (l->data);\n");
    fprintf (src,"           Asn1Free (l);\n");
    fprintf (src,"           l = tmp;\n");
    fprintf (src,"        }\n");
    fprintf (src,"    }\n");
} /* PrintCFreeListELmts */



static void
PrintCFreeChoiceElmts PARAMS ((src, td, t, varName),
    FILE *src _AND_
    TypeDef *td _AND_
    Type *t _AND_
    char *varName)
{
    NamedType *e;
    CTRI *ctri;
    void *tmp;

    ctri =  t->cTypeRefInfo;

    fprintf (src,"    switch (%s->%s)\n    {\n", varName, ctri->choiceIdEnumFieldName);

    FOR_EACH_LIST_ELMT (e, t->basicType->a.choice)
    {
        tmp = (void*)CURR_LIST_NODE (t->basicType->a.choice);

        if (e->type == NULL)
            continue;

        ctri =  e->type->cTypeRefInfo;

        if (e->type->cTypeRefInfo == NULL)
            fprintf (src, "       case ????:\n");
        else if (ctri->isPtr)
        {
            fprintf (src, "       case %s:\n", ctri->choiceIdSymbol);
            PrintCElmtFree (src, td, t, e->type, varName);
            fprintf (src,"    break;\n\n");
        }

        SET_CURR_LIST_NODE (t->basicType->a.choice, tmp);
    }

    fprintf (src, "    }\n");
}
