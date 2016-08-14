/*
 * compiler/core/snacc.c---Compiles ASN.1 src files into an internal type tree.
 *        Imported type/value references are resolved if possible.
 *        Produces C or C++ encoder/decoder/print/free code and .h for
 *        data struct and prototypes.
 *        Generated C can be either ANSI or old style via macros.
 *        Produces values for OBJECT IDENTIFIERs, INTEGERs and BOOLEANs
 *
 * Mike Sample 1991/92
 *
 * NOTES
 *
 *  See the README file for compiling tips.  This should compile
 *  with ANSI or non-ANSI c compilers.
 *
 *  each ASN.1 source file must contain a complete ASN.1 module:
 *       <ModName> DEFINITIONS ::= BEGIN ... END
 *
 * 91/09/04---modified to handle new data struct (ASN.1 generated)
 *            for module info. MS.
 *
 *
 * Copyright (C) 1991, 1992 Michael Sample
 *            and the University of British Columbia
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program and the associated libraries are distributed in the hope
 * that they will be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License and GNU Library General
 * Public License for more details.
 *
 * $Header: /data/cvs/prototype/story/src/public/tools/snacc-1.3/compiler/core/snacc.c,v 1.1 2006/10/16 09:07:19 joywu Exp $
 * $Log: snacc.c,v $
 * Revision 1.1  2006/10/16 09:07:19  joywu
 * no message
 *
 * Revision 1.10  1997/05/07 15:18:35  wan
 * Added (limited) size constraints, bitstring and enumeration names to tables
 *
 * Revision 1.9  1997/02/16 15:12:21  rj
 * made ``return *this after calling abort()'' a compile time option.
 *
 * Revision 1.8  1997/01/02 08:50:55  rj
 * - use the TIME_WITH_SYS_TIME flag (checked and generated by configure)
 * - do not silently ignore the -tcl option if not compiled for tcl code generation
 *
 * Revision 1.7  1995/09/07  19:13:39  rj
 * new options -mA and -mC for snacc(1) that switch the names used between those defined in the ASN.1 files and those used in the generated C++ code.
 * uses newly introduced enum MetaNameStyle.
 *
 * Revision 1.6  1995/08/17  15:00:13  rj
 * the PDU flag belongs to the metacode, not only to the tcl interface. (type and variable named adjusted)
 *
 * Revision 1.5  1995/07/25  19:34:06  rj
 * bug report address changed to protect an innocent's mailbox :-)
 *
 * snacc -h now prints the usage to stdout.
 *
 * allow for more than one PDU per .asn1 file set:
 * - struct TclPDU becomes a linked list.
 * - it gets a flag `used' to detect invalid command line arguments.
 *
 * code for idl backend added.
 *
 * by default, snacc now derives output file names from the .asn1 input file name instead of the module name.
 *
 * changed `_' to `-' in file names.
*
* Revision 1.4  1994/12/11  20:21:06  rj
* #include string(s).h
*
* Revision 1.3  1994/10/08  03:23:27  rj
* since i was still irritated by cpp standing for c++ and not the C preprocessor, i renamed them to cxx (which is one known suffix for C++ source files). since the standard #define is __cplusplus, cplusplus would have been the more obvious choice, but it is a little too long.
*
* code for meta structures added (provides information about the generated code itself).
*
* code for Tcl interface added (makes use of the above mentioned meta code).
*
* Revision 1.2  1994/09/01  00:44:31  rj
* snacc_config.h and other superfluous .h files removed. version string moved into a separate version.h file.
*
* Revision 1.1  1994/08/28  09:49:37  rj
* first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
*
*/

#include "snacc.h"

#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif

#if STDC_HEADERS || HAVE_STRING_H
#include <string.h>
#else
#include <strings.h>
#endif
#include <stdio.h>

#include "asn-incl.h"
#include "version.h"
#include "mem.h"
#include "asn1module.h"
#include "exports.h"
#include "parser.h"          /* for parser (ech!) globals */
#include "dependency.h"
#include "link-types.h"
#include "link-values.h"
#include "err-chk.h"
#include "print.h"
#include "recursive.h"
#include "define.h"
#include "normalize.h"
#include "do-macros.h"
#include "snacc-util.h"
#if META
#include "meta.h"
#endif

#include "str-util.h"

#include "c-gen/rules.h"       /* for c file generation */
#include "c-gen/type-info.h"
#include "c-gen/gen-code.h"

#include "c++-gen/rules.h"       /* for c++ file generation */
#include "c++-gen/types.h"
#include "c++-gen/gen-code.h"

#include "gen-tbls.h"  /* for type table generation */

#if IDL
#include "idl-gen/rules.h"
#include "idl-gen/types.h"
#include "idl-gen/gen-code.h"
#endif


/* prototypes for this file's routines */

Module *ParseAsn1File PROTO ((char *fileName));

void GenCCode PROTO ((ModuleList *allMods, long int longJmpVal, int genTypes, int genEncoders, int genDecoders, int genPrinters, int genValues, int genFree));

/* edison modify it */
void GenCxxCode PROTO ((ModuleList *allMods, long int longJmpVal, int genTypes, int genEncoders, int genDecoders, int genPrinters, int genValues, int genFree, if_META (MetaNameStyle genMeta COMMA MetaPDU *meta_pdus COMMA) if_TCL (int genTcl COMMA) int novolatilefuncs, char *cxx_namespace));

void GenIDLCode PROTO ((ModuleList *allMods, long int longJmpVal, int genTypes, int genPrinters, int genValues, int genFree));

int ModNamesUnique PROTO ((ModuleList *m));

Module		*usefulTypeModG = NULL;
static char	versionG[] = VERSION;
static char	releasedateG[] = RELDATE;
static char	bugreportaddressG[] = BUGREPADDR;
int		maxFileNameLenG = -1; /* values > 2 are considered valid */
/* this is used in back_ends/c_gen/str_util.c */

void
Usage PARAMS ((prgName, fp),
		char	*prgName _AND_
		FILE	*fp)
{
	fprintf (fp, "\nUsage: %s ", prgName);
	fprintf (fp, "[-h] [-P] [-t] [-v] [-e] [-d] [-p] [-f]\n");
#if IDL
	fprintf (fp, "            [-c | -C | -[T|O] <table output file> | -idl ]\n");
#else
	fprintf (fp, "            [-c | -C | -[T|O] <table output file>]\n");
#endif
	fprintf (fp, "            [-u <useful types ASN.1 file>]\n");
	fprintf (fp, "            [-mm] [-mf <max file name length>]\n");
	fprintf (fp, "            [-l <neg number>]\n");
#if META
	fprintf (fp, "            [-meta <type list>] [-mA | -mC]\n");
#if TCL
	fprintf (fp, "            [-tcl <type list>]\n");
#endif
#endif
	fprintf (fp, "            <ASN.1 file list>\n\n");
	fprintf (fp, "  -h   prints this msg\n");
	fprintf (fp, "  -s <namespace>   generate namespace for cxx, added by edisonpeng\n");
	fprintf (fp, "  -c   generate C encoders and decoders (default)\n");
	fprintf (fp, "  -C   generate C++ encoders and decoders\n");
	fprintf (fp, "  -novolat   for broken C++ compilers: return *this after calling abort()\n");
	fprintf (fp, "  -T <filename> write a type table file for the ASN.1 modules to file filename\n");
	fprintf (fp, "  -O <filename> writes the type table file in the original (<1.3b2) format\n");
#if IDL
	fprintf (fp, "  -idl generate CORBA IDL\n");
#endif
	fprintf (fp, "  -u <filename> specifies the ASN.1 file with definition of the useful types\n");
	fprintf (fp, "       (i.e. PrintableString). See the useful.asn1 file (in the\n");
	fprintf (fp, "       snacc/asn1specs/ directory).\n");
	fprintf (fp, "  -P   print the parsed ASN.1 modules to stdout from their parse trees\n");
	fprintf (fp, "       (helpful debugging)\n");
	fprintf (fp, "  -t   generate type definitions\n");
	fprintf (fp, "  -v   generate value definitions (limited)\n");
	fprintf (fp, "  -e   generate encode routines\n");
	fprintf (fp, "  -d   generate decode routines\n");
	fprintf (fp, "  -p   generate print routines\n");
	fprintf (fp, "  -f   generate hierarchical free routines (C only)\n");
	fprintf (fp, "      note: if none of -t -v -e -d -p -f are given, all are generated.\n");
	fprintf (fp, "      These do not affect type tables.\n");


	fprintf (fp, "  -mm  mangle output file name into module name (by default, the output file\n");
	fprintf (fp, "       inherits the input file's name, with only the suffix replaced)\n");
	fprintf (fp, "  -mf <num> num is maximum file name length for the generated source files\n");

	fprintf (fp, "  -l <neg num> where to start error longjmp values decending from (obscure).\n");

#if META
	fprintf (fp, "  -meta <type list> generate meta code that describes the generated types. Implies -C.\n");
	fprintf (fp, "  -mA  metacode: use names as defined in the ASN.1 files.\n");
	fprintf (fp, "  -mC  metacode: use names as used in the generated C++ files.\n");
#if TCL
	fprintf (fp, "  -tcl <type list> generate code for a Tcl interpreter. Implies -meta.\n");
#endif
	fprintf (fp, "    <type list> has the following syntax: <module>.<type>[,<module>.<type>[...]]\n");
	fprintf (fp, "    the types listed are the top level PDUs.\n");
#endif

	fprintf (fp, "\nUse `-' as the ASN.1 source file name to parse stdin.\n\n");


	fprintf (fp, "This ASN.1 compiler produces C or C++ BER encoders and decoders or type tables.\n");

	fprintf (fp, "\nVersion %s, %s.\n", versionG, releasedateG);
#if 0
	fprintf (fp, "Please send bug reports and comments to %s.\n\n", bugreportaddressG);
#else
	fprintf (fp, "Please see %s for new versions and where to send bug reports and comments.\n\n", bugreportaddressG);
#endif

	fprintf (fp, "Copyright (C) 1993 Michael Sample and UBC\n");
	fprintf (fp, "Copyright (C) 1994, 1995 by Robert Joop and GMD FOKUS\n\n");

	fprintf (fp, "This program is free software; you can redistribute it and/or modify\n");
	fprintf (fp, "it under the terms of the GNU General Public License as published by\n");
	fprintf (fp, "the Free Software Foundation; either version 2 of the License, or\n");
	fprintf (fp, "(at your option) any later version.\n\n");

	fprintf (fp, "This program is distributed in the hope that it will be useful,\n");
	fprintf (fp, "but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
	fprintf (fp, "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n");
	fprintf (fp, "GNU General Public License for more details.\n\n");

	/*
	   fprintf (fp, "You should have received a copy of the GNU General Public License\n");
	   fprintf (fp, "along with this program; if not, write to the Free Software\n");
	   fprintf (fp, "Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.\n");
	 */
}


typedef struct
{
	char      *fileName;
} SRC_FILE;


#if META
static MetaPDU *parse_type_list (arg)
	char *arg;
{
	MetaPDU *meta_pdus = NULL;
	char *module;
	for (module = strtok (arg, ".:"); module; module = strtok (NULL, ".:"))
	{
		MetaPDU *pdu = MT (MetaPDU);
		char *type = strtok (NULL, " /,;");
		if (!type)
		{
			fprintf (stderr, "usage: {-meta|-tcl} module.type[,module.type[...]]\n");
			exit (1);
		}
		pdu->module = module;
		pdu->type = type;
		pdu->used = FALSE;

		pdu->next = meta_pdus;
		meta_pdus = pdu;
	}
	return meta_pdus;
}
#endif

int main PARAMS ((argc, argv),
		int			argc _AND_
		char		**argv)
{
	int			i, j, semErr;
	int			retVal;
	int			fd;
	SRC_FILE		*srcList;
	int			numSrcFiles;
	ModuleList		*allMods;
	Module		*currMod;
	Module		**tmpModHndl;
	int			currArg;
	int			argsProcessed;
	char		*usefulTypeModFileName;
	FILE		*fPtr;
	int			printModuleFlag = FALSE;  /* default: Don't print */
	int			genTypeTbls = 0; /* default: Don't gen tbls */
	char		*tblFileName;
	int			genTypeCode = FALSE;
	int			genEncodeCode = FALSE;
	int			genDecodeCode = FALSE;
	int			genPrintCode = FALSE;
	int			genValueCode = FALSE;
	int			genFreeCode = FALSE;
	//edison add it
	char        *cxx_namespace = NULL;
#if META
	MetaNameStyle	genMetaCode = META_off;
	MetaPDU		*meta_pdus = NULL;
#if TCL
	int			genTclCode = FALSE;
#endif
#endif
	int			genCCode = FALSE;        /* defaults to C if neither specified */
	int			genCxxCode = FALSE;
#if IDL
	int			genIDLCode = FALSE;
#endif
	long		longJmpVal = -100;
	int			novolatilefuncs = FALSE;

#ifdef YYDEBUG
	/* prints yacc debugging info to stdout */
	yydebug = 1;
#endif


	if (argc <= 1)
	{
		Usage (argv[0], stderr);
		exit (1);
	}

	srcList = (SRC_FILE*) Malloc ((argc -1) * sizeof (SRC_FILE));


	/*
	 * parse cmd line args
	 */
	numSrcFiles = 0;
	usefulTypeModFileName = NULL;
	for (currArg = 1; (currArg < argc); )
	{
		if ((argv[currArg][0] == '-') && (argv[currArg][1] != '\0'))
			switch (argv[currArg][1])
			{
				case 's':
					cxx_namespace = argv[currArg+1];
					currArg += 2;
					break;
				case 'h':
					Usage (argv[0], stdout);
					exit (1);
					break;

				case 'P':
					printModuleFlag = TRUE;
					currArg++;
					break;

				case 'v':
					genValueCode = TRUE;
					currArg++;
					break;

#if IDL
				case 'i':
					if (!strcmp (argv[currArg]+1, "idl"))
					{
						genIDLCode = TRUE;
						currArg++;
					}
					else
						goto error;
					break;
#endif

				case 't':
					if (!strcmp (argv[currArg]+1, "tcl"))
					{
#if TCL
						meta_pdus = parse_type_list (argv[++currArg]);
						genTclCode = TRUE;
						if (!genMetaCode)
							genMetaCode = META_backend_names;
						genCxxCode = TRUE;
#else
						goto error;
#endif
					}
					else
						genTypeCode = TRUE;
					currArg++;
					break;

				case 'e':
					genEncodeCode = TRUE;
					currArg++;
					break;

				case 'd':
					genDecodeCode = TRUE;
					currArg++;
					break;

				case 'p':
					genPrintCode = TRUE;
					currArg++;
					break;

				case 'f':
					genFreeCode = TRUE;
					currArg++;
					break;

				case 'C':
					genCxxCode = TRUE;
					currArg++;
					break;

				case 'n':
					if (!strcmp (argv[currArg]+1, "novolat"))
					{
						novolatilefuncs = TRUE;
						currArg++;
					}
					else
						goto error;
					break;

				case 'c':
					genCCode = TRUE;
					currArg++;
					break;

				case 'u':
					if (argv[currArg][2] != '\0')  /* no space after -u */
					{
						usefulTypeModFileName = &argv[currArg][2];
						currArg++;
					}
					else
					{
						usefulTypeModFileName = argv[currArg+1];
						currArg += 2;
					}
					break;

				case 'l':
					if (argv[currArg][2] != '\0')  /* no space after -l */
					{
						longJmpVal = atoi (&argv[currArg][2]);
						currArg++;
					}
					else
					{
						longJmpVal = atoi (argv[currArg+1]);
						currArg += 2;
					}
					break;

				case 'T':
				case 'O':
					genTypeTbls = argv[currArg][1]=='T'?2:1;
					if (argv[currArg][2] != '\0')  /* no space after -T */
					{
						tblFileName = &argv[currArg][2];
						currArg++;
					}
					else
					{
						tblFileName = argv[currArg+1];
						currArg += 2;
					}
					break;


				case 'm':
					if (argv[currArg][2] == 'f')
					{
						if (argv[currArg][3] != '\0')  /* no space after -mf */
						{
							maxFileNameLenG = atoi (&argv[currArg][3]);
							currArg++;
						}
						else
						{
							maxFileNameLenG = atoi (argv[currArg+1]);
							currArg += 2;
						}
						break;
					}
#if META
					else if (!strcmp (argv[currArg]+1, "meta"))
					{
						meta_pdus = parse_type_list (argv[++currArg]);
						if (!genMetaCode)
							genMetaCode = META_backend_names;
						genCxxCode = TRUE;
						currArg++;
						break;
					}
					else if (!strcmp (argv[currArg]+1, "mA"))
					{
						genMetaCode = META_asn1_names;
						genCxxCode = TRUE;
						currArg++;
						break;
					}
					else if (!strcmp (argv[currArg]+1, "mC"))
					{
						genMetaCode = META_backend_names;
						genCxxCode = TRUE;
						currArg++;
						break;
					}
#endif
					else if (argv[currArg][2] == 'm')
					{
						keepbaseG = FALSE;
						currArg++;
						break;
					}

					/* else fall through to default error */
error:
				default:
					fprintf (stderr, "%s: ERROR---unknown cmd line option `%s'\n\n", argv[0], argv[currArg]);
					Usage (argv[0], stderr);
					exit (1);
			}

		else /* asn1srcFileName */
			srcList[numSrcFiles++].fileName = argv[currArg++];

	}


	if (numSrcFiles == 0)
	{
		fprintf (stderr, "%s: ERROR---no ASN.1 source files were specified\n", argv[0]);
		Usage (argv[0], stderr);
		exit (1);
	}


	/*
	 * set default options
	 */
	if (!(genTypeCode || genValueCode || genEncodeCode || genDecodeCode ||
				genFreeCode || genPrintCode))
	{
		genTypeCode = TRUE;
		genValueCode = TRUE;
		genEncodeCode = TRUE;
		genDecodeCode = TRUE;
		genFreeCode = TRUE;
		genPrintCode = TRUE;
	}

	else if (genCCode + genCxxCode + genTypeTbls
#if IDL
			+ genIDLCode
#endif
			> 1)
	{
		fprintf (stderr, "%s: ERROR---Choose only one of the -c -C or -T options\n", argv[0]);
		Usage (argv[0], stderr);
		exit (1);
	}

	if (!genCCode && !genCxxCode && !genTypeTbls
#if IDL
			&& !genIDLCode
#endif
	   )
		genCCode = TRUE;  /* default to C if neither specified */


	/*
	 * STEP 1
	 * parse 'useful' type module for linking purposes only (if given)
	 * the useful type encode, decode, print, free routines are
	 * already in the runtime library.
	 */
	if (usefulTypeModFileName != NULL)
	{
		usefulTypeModG = ParseAsn1File (usefulTypeModFileName);

		if (usefulTypeModG == NULL)
			exit (1);
	}
	else
	{
		fprintf (stderr, "  Hmmm, you didn't specify a useful types ASN.1 file with the `-u' option.\n");
		fprintf (stderr, "  I'll continue assuming your ASN.1 file(s) don't need any useful types.\n");
	}


	/*
	 * STEP 2---parse each ASN.1 src file
	 */
	allMods = (ModuleList *)AsnListNew (sizeof (void*));
	for (i = 0; i < numSrcFiles; i++)
	{
		currMod = ParseAsn1File (srcList[i].fileName);

		if (currMod == NULL)
			exit (1);

		/*
		 * insert this module at the head of the list
		 * of already parsed (if any) modules
		 */
		tmpModHndl = (Module **)AsnListAppend (allMods);
		*tmpModHndl = currMod;

	}  /* end per src file for loop */


	/*
	 * Check that the module names/oids are unique.
	 */
	if (!ModNamesUnique (allMods))
	{
		fprintf (stderr, "Conflicting module names, cannot proceed.\n");
		exit (1);
	}



	/*
	 * STEP 3
	 * Now that all files have been parsed,
	 * link local and locatable import type refs
	 */
	if (LinkTypeRefs (allMods) < 0)
	{
		fprintf (stderr, "Type linking errors---cannot proceed\n");
		exit (1);
	}



	/*
	 * STEP 4
	 * Parse constructed values now that types are all parsed
	 * and have been linked.  Need type info to be able to
	 * parse values easily (elimitate ambiguity).
	 */
	FOR_EACH_LIST_ELMT (currMod, allMods)
	{
		if (ParseValues (allMods, currMod) != 0)
			fprintf (stderr, "WARNING: Value parsing error (s), attempting to continue\n");
	}


	/*
	 * STEP 5
	 * Value parsing may have defined some new values
	 * so can link local and locatable import value refs now.
	 */
	if (LinkValueRefs (allMods) < 0)
	{
		fprintf (stderr, "Value linking errors---cannot proceed\n");
		exit (1);
	}



	/*
	 * STEP 6
	 * process macros
	 *   - adding type/value defs as nec
	 *   - mark type defs with ANY DEFINED BY id if nec
	 *     so they are put in the id to ANY type hash tbl.
	 */
	semErr = 0;
	FOR_EACH_LIST_ELMT (currMod, allMods)
	{
		ProcessMacros (currMod);
		if (currMod->status == MOD_ERROR)
			semErr = 1;
	}
	if (semErr)
		exit (1);

	/*
	 * STEP 7
	 * convert silly type constructs into
	 * a normal format, leaving behind pure type/value info
	 * eg: expand COMPONENTS OF refs, SELECTION types.
	 * boil down values into simplest rep. (eg OID -> ENC_OID)
	 */
	semErr = 0;
	FOR_EACH_LIST_ELMT (currMod, allMods)
	{
		NormalizeModule (currMod);
		if (currMod->status == MOD_ERROR)
			semErr = 1;
	}
	if (semErr)
		exit (1);


	/*
	 * STEP 8
	 * Mark recusive types.  Currently the recursive information is
	 * not used elsewhere.
	 */
	FOR_EACH_LIST_ELMT (currMod, allMods)
	{
		MarkRecursiveTypes (currMod);
	}


	/*
	 * STEP 9
	 * Check for errors in the ASN.1 modules.
	 * Check all modules and exit if errors were found
	 */
	semErr = 0;
	if (usefulTypeModG != NULL)
	{
		ErrChkModule (usefulTypeModG);
		if (usefulTypeModG->status == MOD_ERROR)
			semErr = 1;
	}

	FOR_EACH_LIST_ELMT (currMod, allMods)
	{
		ErrChkModule (currMod);
		if (currMod->status == MOD_ERROR)
			semErr = 1;
	}


	if (semErr)
		exit (1);




	/*
	 *  exit if any sundry errors occurred at any point.
	 *  smallErrG is set upon finding small errors that prevent code
	 *  production but should not affect the other processing/error
	 *  checking steps.  This allows full display of errors.
	 */
	if (smallErrG)
	{
		/*
		 * for debugging show "parsed" version of ASN.1 module if
		 * the print flag is set.
		 * Dumps each module to stdout. Printed from Module data struct
		 * print here before exiting otherwise print after sorting
		 */
		if (printModuleFlag)
		{
			FOR_EACH_LIST_ELMT (currMod, allMods)
			{
				printf ("\n\n");
				PrintModule (stdout, currMod);
			}
		}

		exit (1);
	}

	/*
	 * STEP 10
	 * Make C/C++ typenames/routine names for enc/decode.
	 * Type/Value renaming will occur if name conflicts
	 * arise between modules.
	 *
	 * NOTE: this is done before sorting the types because
	 *       the type sorting routine may use the 'isPtr'
	 *       information to help order knots of recursive types.
	 */
	if (genCCode)
		FillCTypeInfo (&cRulesG, allMods);

	else if (genCxxCode)
		FillCxxTypeInfo (&cxxRulesG, allMods);

#if IDL
	else if (genIDLCode)
		FillIDLTypeInfo (&idlRulesG, allMods);
#endif


	/*
	 * STEP 11
	 * Sort each typedef list such that independent types are
	 * before the types that depend on them
	 *
	 *  modules remain in same order as given on command line
	 *  (cmd line file order should be
	 *      least dependent module-> most dependent module
	 *      so that include file order in generated src is correct)
	 *  (useful.asn1 is always considered 'first' if given)
	 */
	SortAllDependencies (allMods);

	/*
	 * for debugging show "parsed" version of ASN.1 module.
	 * dumps each module to stdout. Printed from Module data struct
	 * Shows the results of normalization and sorting.
	 */
	if (printModuleFlag)
	{
		FOR_EACH_LIST_ELMT (currMod, allMods)
		{
			printf ("\n\n");
			PrintModule (stdout, currMod);
		}
	}

	/*
	 * Step 12
	 * Final Step: Code/Type Table generation
	 */
	if (genCCode)
		GenCCode (allMods, longJmpVal, genTypeCode, genValueCode, genEncodeCode, genDecodeCode, genPrintCode, genFreeCode);

	else if (genCxxCode) {
		printf("%s\n", cxx_namespace);
		GenCxxCode (allMods, longJmpVal, genTypeCode, genValueCode, genEncodeCode, genDecodeCode, genPrintCode, genFreeCode, if_META (genMetaCode COMMA meta_pdus COMMA) if_TCL (genTclCode COMMA) novolatilefuncs, cxx_namespace);

	} else if (genTypeTbls)
		GenTypeTbls (allMods, tblFileName, genTypeTbls);

#if IDL
	else if (genIDLCode)
		GenIDLCode (allMods, longJmpVal, genTypeCode, genValueCode, genPrintCode, genFreeCode);
#endif

	return 0; /* keep make happy */

} /* end main */



/*
 * Calls the yacc/lex parser given a the ASN.1 src file's filename.
 * Returns a Module *for the given ASN.1 module. If the filename is
 * "-" stdin is used.
 */
	Module *
ParseAsn1File PARAMS ((fileName),
		char *fileName)
{
	FILE *fPtr;
	Module *retVal;
	int parseResult;

	/*
	 *  Open input file for lexical analyzer/parser
	 *  Use stdin if the filename is "-"
	 */
	if (strcmp (fileName, "-") == 0)
		if (keepbaseG)
		{
			fprintf (stderr, "ERROR---asn1 src file `%s' cannot be processed without output filename mangling\n", fileName);
			return NULL;
		}
		else
			fPtr = stdin;
	else
		fPtr = fopen (fileName, "r");

	if (fPtr == NULL)
	{
		fprintf (stderr, "ERROR---asn1 src file `%s' cannot be opened for reading\n", fileName);
		return NULL;
	}

	retVal = (Module *)Malloc (sizeof (Module));

	/*
	 * Init Parser by giving it a ptr to the Module data struct
	 * to initialize/use, and the file name associtated with
	 * the given FILE *, fPtr (for error reporting).
	 * fPtr should be an opened FILE *to an ASN.1 source FILE
	 */
	InitAsn1Parser (retVal, fileName, fPtr);


	/*
	 * parse the current asn1 src file into the
	 * Module data struct
	 */
	parseResult = yyparse();

	if (parseResult != 0 || retVal->status == MOD_ERROR)
	{
		/* parser will print exact err msg */
		fprintf (stderr, "Parsing errors---cannot proceed\n");
		return NULL;
	}

	if (fPtr != stdin)
		fclose (fPtr);

	return retVal;

}  /* ParseAsn1File */


/*
 * Given the list of parsed, linked, normalized, error-checked and sorted
 * modules, and some code generation flags, generates C code and
 * writes it to files derived from each modules name.  Each module
 * gets 2 source files, one .h for data struct and prototypes, the other .c
 * for the enc/dec/print/free routine code.
 */
void
GenCCode PARAMS ((allMods, longJmpVal, genTypes, genValues, genEncoders, genDecoders, genPrinters, genFree),
		ModuleList *allMods _AND_
		long int longJmpVal _AND_
		int genTypes _AND_
		int genValues _AND_
		int genEncoders _AND_
		int genDecoders _AND_
		int genPrinters _AND_
		int genFree)
{
	Module *currMod;
	char *modBaseFileName;
	FILE *cHdrFilePtr;
	FILE *cSrcFilePtr;
	DefinedObj *fNames;
	int fNameConflict = FALSE;

	/*
	 * Make names for each module's encoder/decoder src and hdr files
	 * so import references can be made via include files.
	 * If file names conflict, print error msg & exit.
	 */
	fNames = NewObjList();
	FOR_EACH_LIST_ELMT (currMod, allMods)
	{
		modBaseFileName = MakeBaseFileName (keepbaseG
				? currMod->asn1SrcFileName
				: currMod->modId->name); /* shorten module name if necessary (SYSV etc) */
		currMod->cHdrFileName = MakeCHdrFileName (modBaseFileName);
		currMod->cSrcFileName = MakeCSrcFileName (modBaseFileName);

		if (ObjIsDefined (fNames, currMod->cHdrFileName, StrObjCmp) ||
				ObjIsDefined (fNames, currMod->cSrcFileName, StrObjCmp))
		{
			fprintf (stderr, "Ack! ERROR---file name conflict for generated source files with names `%s' and `%s'.\n\n", currMod->cHdrFileName, currMod->cSrcFileName);
			fprintf (stderr, "This usually means the max file name length is truncating the file names.\n");
			fprintf (stderr, "Try re-naming the modules with shorter names or increasing the argument to -mf option (if you are using it).\n");
			fprintf (stderr, "This error can also be caused by 2 modules with the same names but different OBJECT IDENTIFIERs.");
			fprintf (stderr, "  Try renaming the modules to correct this.\n");
			fNameConflict = TRUE;
		}
		else
		{
			DefineObj (&fNames, currMod->cHdrFileName);
			DefineObj (&fNames, currMod->cSrcFileName);
		}
		Free (modBaseFileName);
	}
	if (fNameConflict)
		exit (1);

	FreeDefinedObjs (&fNames);
	/*
	 * make c files
	 */
	FOR_EACH_LIST_ELMT (currMod, allMods)
	{
		cHdrFilePtr = fopen (currMod->cHdrFileName, "w");
		cSrcFilePtr = fopen (currMod->cSrcFileName, "w");
		if ((cSrcFilePtr == NULL) || (cHdrFilePtr == NULL))
			perror ("fopen");
		else
		{
			PrintCCode (cSrcFilePtr, cHdrFilePtr, allMods, currMod, &cRulesG, longJmpVal, genTypes,  genValues, genEncoders, genDecoders, genPrinters, genFree);

			fclose (cHdrFilePtr);
			fclose (cSrcFilePtr);
		}
	}

}  /* GenCCode */


/*
 * Given the list of parsed, linked, normalized, error-checked and sorted
 * modules, and some code generation flags, generates C++ code and
 * writes it to files derived from each modules name.  Each module
 * gets 2 source files, one .h for data struct and prototypes, the other .C
 * for the enc/dec/print/free routine code.
 */
void
GenCxxCode PARAMS ((allMods, longJmpVal, genTypes, genValues, genEncoders, genDecoders, genPrinters, genFree, if_META (genMeta COMMA meta_pdus COMMA) if_TCL (genTcl COMMA) novolatilefuncs, cxx_namespace),
		ModuleList *allMods _AND_
		long int longJmpVal _AND_
		int genTypes _AND_
		int genValues _AND_
		int genEncoders _AND_
		int genDecoders _AND_
		int genPrinters _AND_
		int genFree _AND_
		if_META (MetaNameStyle genMeta _AND_)
		if_META (MetaPDU *meta_pdus _AND_)
		if_TCL (int genTcl _AND_)
		int novolatilefuncs _AND_
		char *cxx_namespace)
{
	Module		*currMod;
	char		*modBaseFileName;
	FILE		*hdrFilePtr;
	FILE		*srcFilePtr;
#ifdef _IBM_ENC_
	FILE		*hdbFilePtr;    /* 19.8.93 IBM-ENC */
	FILE		*sdbFilePtr;    /* 19.8.93 IBM-ENC */
#endif /* _IBM_ENC_ */
	DefinedObj		*fNames;
	int			fNameConflict = FALSE;

#if META
	static const char	metabasefn[] = "modules";
	Meta		meta;
#if TCL
	const MetaPDU	*pdu;
#endif
#endif

	/*
	 * Make names for each module's encoder/decoder src and hdr files
	 * so import references can be made via include files
	 * check for truncation --> name conflicts & exit if nec
	 */
	fNames = NewObjList();
#if META
	if (genMeta)
		DefineObj (&fNames, meta.srcfn = MakeCxxSrcFileName (metabasefn));
#endif
	FOR_EACH_LIST_ELMT (currMod, allMods)
	{
		modBaseFileName = MakeBaseFileName (keepbaseG
				? currMod->asn1SrcFileName
				: currMod->modId->name); /* shorten module name if necessary (SYSV etc) */
		currMod->cxxHdrFileName = MakeCxxHdrFileName (modBaseFileName);
		currMod->cxxSrcFileName = MakeCxxSrcFileName (modBaseFileName);
#ifdef _IBM_ENC_
		currMod->dbHdrFileName  = MakedbHdrFileName (modBaseFileName); /* 19.8.93 IBM-ENC */
		currMod->dbSrcFileName  = MakedbSrcFileName (modBaseFileName); /* 19.8.93 IBM-ENC */
#endif /* _IBM_ENC_ */
#if META
		{
			char *in, *out;

			out = currMod->cxxname = (char *)malloc (strlen (in = currMod->modId->name)+1);
			do
				*out++ = *in == '-' ? '_' : *in;
			while (*in++);
		}
#endif

		if (ObjIsDefined (fNames, currMod->cxxHdrFileName, StrObjCmp) || ObjIsDefined (fNames, currMod->cxxSrcFileName, StrObjCmp))
		{
			fprintf (stderr, "Ack! ERROR---file name conflict for generated source files with names `%s' and `%s'.\n\n", currMod->cxxHdrFileName, currMod->cxxSrcFileName);
			fprintf (stderr, "This usually means the max file name length is truncating the file names.\n");
			fprintf (stderr, "Try re-naming the modules with shorter names or increasing the argument to -mf option (if you are using it).\n");
			fprintf (stderr, "This error can also be caused by 2 modules have the same names but different OBJECT IDENTIFIERs.");
			fprintf (stderr, "  Try renaming the modules to correct this.\n");
			fNameConflict = TRUE;
		}
		else
		{
			DefineObj (&fNames, currMod->cxxHdrFileName);
			DefineObj (&fNames, currMod->cxxSrcFileName);
		}
		Free (modBaseFileName);
	}
	if (fNameConflict)
		exit (1);

	FreeDefinedObjs (&fNames);

	/*
	 * make C++ files
	 */
#if META
	if (genMeta)
	{
		time_t now = time (NULL);

		if (!(meta.srcfp = fopen (meta.srcfn, "w")))
		{
			perror ("fopen");
			exit (1);
		}
		fprintf (meta.srcfp, "// NOTE: this is a machine generated file--editing not recommended\n");
		fprintf (meta.srcfp, "//\n");
		fprintf (meta.srcfp, "// modules.C - reference to all modules and their types\n");
		fprintf (meta.srcfp, "//\n");
		fprintf (meta.srcfp, "// This file was generated by snacc on %s", ctime (&now));
	}
#endif
	FOR_EACH_LIST_ELMT (currMod, allMods)
	{
		/*
		 * create and fill .h file for module's data structs
		 */
		hdrFilePtr = fopen (currMod->cxxHdrFileName, "w");
		srcFilePtr = fopen (currMod->cxxSrcFileName, "w");
#ifndef _IBM_ENC_
		if ((hdrFilePtr == NULL) || (srcFilePtr == NULL))
#else
			hdbFilePtr = fopen (currMod->dbHdrFileName, "w");       /* 19.8.93 IBM-ENC */
		sdbFilePtr = fopen (currMod->dbSrcFileName, "w");       /* 19.8.93 IBM-ENC */
		if ((hdrFilePtr == NULL) || (srcFilePtr == NULL) ||
				(hdbFilePtr == NULL) || (sdbFilePtr == NULL))      /* 19.8.93 IBM-ENC */
#endif /* _IBM_ENC_ */
			perror ("fopen");
		else
		{
			//edison modify it (2009-12-07)
			PrintCxxCode (srcFilePtr, hdrFilePtr,
					if_IBM_ENC (sdbFilePtr COMMA hdbFilePtr COMMA /* 19.8.93 IBM-ENC */)
					if_META (genMeta COMMA &meta COMMA meta_pdus COMMA)
					allMods, currMod, &cxxRulesG, longJmpVal,
					genTypes, genValues, genEncoders, genDecoders, genPrinters, genFree,
					if_TCL (genTcl COMMA) novolatilefuncs,
					cxx_namespace);

			fclose (hdrFilePtr);
			fclose (srcFilePtr);
#ifdef _IBM_ENC_
			fclose (hdbFilePtr);       /* 19.8.93 IBM-ENC */
			fclose (sdbFilePtr);       /* 19.8.93 IBM-ENC */
#endif /* _IBM_ENC_ */
		}
	}
#if META
	if (genMeta)
	{
		fprintf (meta.srcfp, "\n");
		fprintf (meta.srcfp, "#ifndef META\n");
		fprintf (meta.srcfp, "#define META	1\n");
		fprintf (meta.srcfp, "#endif\n");
		if (meta_pdus)
		{
			for (pdu=meta_pdus; pdu; pdu=pdu->next)
				if (!pdu->used)
					fprintf (stderr, "warning: PDU %s.%s couldn't be found\n", pdu->module, pdu->type);
		}
#if TCL
		fprintf (meta.srcfp, "#ifndef TCL\n");
		fprintf (meta.srcfp, "#define TCL	META\n");
		fprintf (meta.srcfp, "#endif\n");
#endif
		fprintf (meta.srcfp, "\n");

		fprintf (meta.srcfp, "#include \"asn-incl.h\"\n");
		FOR_EACH_LIST_ELMT (currMod, allMods)
			fprintf (meta.srcfp, "#include \"%s\"\n", currMod->cxxHdrFileName);
		fprintf (meta.srcfp, "\n");

		fprintf (meta.srcfp, "#if META\n\n");

		fprintf (meta.srcfp, "const AsnModuleDesc *asnModuleDescs[] =\n");
		fprintf (meta.srcfp, "{\n");
		FOR_EACH_LIST_ELMT (currMod, allMods)
			fprintf (meta.srcfp, "  &%sModuleDesc,\n", currMod->cxxname);
		fprintf (meta.srcfp, "  NULL\n");
		fprintf (meta.srcfp, "};\n\n");

		if (genTcl)
		{
			fprintf (meta.srcfp, "#if TCL\n\n");

			fprintf (meta.srcfp, "// hack to avoid the neccessity to list -ltk -ltcl both before and after -lasn1tcl:\n");
			fprintf (meta.srcfp, "static int (*dummy)(Tcl_Interp *) = Tcl_AppInit;\n\n");

			fprintf (meta.srcfp, "#endif // TCL\n\n");
		}

		fprintf (meta.srcfp, "#endif // META\n");

		fclose (meta.srcfp);
	}
#endif
}  /* GenCxxCode */


#if IDL
/*
 * Given the list of parsed, linked, normalized, error-checked and sorted
 * modules, and some code generation flags, generates C++ code and
 * writes it to files derived from each modules name.  Each module
 * gets 2 source files, one .h for data struct and prototypes, the other .C
 * for the enc/dec/print/free routine code.
 */
void
GenIDLCode PARAMS ((allMods, longJmpVal, genTypes, genValues, genPrinters, genFree),
		ModuleList *allMods _AND_
		long int longJmpVal _AND_
		int genTypes _AND_
		int genValues _AND_
		int genPrinters _AND_
		int genFree)
{
	Module		*currMod;
	char		*modBaseFileName;
	FILE		*idlFilePtr;
	DefinedObj		*fNames;
	int			fNameConflict = FALSE;

	/*
	 * Make names for each module's encoder/decoder src and hdr files
	 * so import references can be made via include files
	 * check for truncation --> name conflicts & exit if nec
	 */
	fNames = NewObjList();
	FOR_EACH_LIST_ELMT (currMod, allMods)
	{
		modBaseFileName = MakeBaseFileName (keepbaseG
				? currMod->asn1SrcFileName
				: currMod->modId->name); /* shorten module name if necessary (SYSV etc) */
		currMod->idlFileName = MakeIDLFileName (modBaseFileName);
		{
			char *in, *out;

			out = currMod->idlname = (char *)malloc (strlen (in = currMod->modId->name)+1);
			do
				*out++ = *in == '-' ? '_' : *in;
			while (*in++);
		}

		if (ObjIsDefined (fNames, currMod->idlFileName, StrObjCmp))
		{
			fprintf (stderr, "Ack! ERROR---file name conflict for generated source file with name `%s'.\n\n", currMod->idlFileName);
			fprintf (stderr, "This usually means the max file name length is truncating the file names.\n");
			fprintf (stderr, "Try re-naming the modules with shorter names or increasing the argument to -mf option (if you are using it).\n");
			fprintf (stderr, "This error can also be caused by 2 modules have the same names but different OBJECT IDENTIFIERs.");
			fprintf (stderr, "  Try renaming the modules to correct this.\n");
			fNameConflict = TRUE;
		}
		else
		{
			DefineObj (&fNames, currMod->idlFileName);
		}
		Free (modBaseFileName);
	}
	if (fNameConflict)
		exit (1);

	FreeDefinedObjs (&fNames);

	/*
	 * make C++ files
	 */
	FOR_EACH_LIST_ELMT (currMod, allMods)
	{
		/*
		 * create and fill .h file for module's data structs
		 */
		idlFilePtr = fopen (currMod->idlFileName, "w");
		if (idlFilePtr == NULL)
			perror ("fopen");
		else
		{
			PrintIDLCode (idlFilePtr, allMods, currMod, &idlRulesG, longJmpVal, genValues);

			fclose (idlFilePtr);
		}
	}
}  /* GenIDLCode */
#endif /* IDL */


/*
 * returns 1 if the module names and oid's are unique.
 * otherwise returns 0
 */
int ModNamesUnique PARAMS ((mods),
		ModuleList *mods)
{
	DefinedObj *names;
	DefinedObj *oids;
	Module *m;
	int retVal = 1;

	names = NewObjList();
	oids = NewObjList();

	FOR_EACH_LIST_ELMT (m, mods)
	{
		if (((m->modId->oid != NULL) &&
					ObjIsDefined (oids, m->modId->oid, OidObjCmp)))
		{
			/* oops, 2 modules have the same oid */
			PrintErrLoc (m->asn1SrcFileName, 1);
			fprintf (stderr, "ERROR---2 modules have the OBJECT IDENTIFIER `");
			PrintOid (stderr, m->modId->oid);
			fprintf (stderr, "'.\n");
			retVal = 0;
		}
		/* name is only signficant if oid is empty */
		else if ((m->modId->oid == NULL) &&
				(ObjIsDefined (names, m->modId->name, StrObjCmp)))
		{
			/* oops, 2 modules have the same name */
			PrintErrLoc (m->asn1SrcFileName, 1);
			fprintf (stderr, "ERROR---2 modules have the name `%s'\n", m->modId->name);
			retVal = 0;
		}
		else
		{
			DefineObj (&names, m->modId->name);
			if (m->modId->oid != NULL)
				DefineObj (&oids, m->modId->oid);
		}
	}
	FreeDefinedObjs (&names);
	FreeDefinedObjs (&oids);
	return retVal;
} /* ModNamesUnique */
