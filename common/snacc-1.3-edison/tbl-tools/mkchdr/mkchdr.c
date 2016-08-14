/*
 * file: .../tbl-tools/mkchdr/mkchdr.c - given a type table generated
 *	by snacc (-T option) mkchdr generates a .h file that contains the C
 *	type defs for the types in the given table.
 *
 *  NOTE: these C typedefs are as the table decoder/encoder will
 *        return/expect.  They are DIFFERENT from the ones usually
 *        generated by snacc's C backend
 *
 *
 * Mike Sample
 *
 * Copyright (C) 1993 Michael Sample
 *            and the University of British Columbia
 *
 * $Header: /data/cvs/prototype/story/src/public/tools/snacc-1.3/tbl-tools/mkchdr/mkchdr.c,v 1.1 2006/10/16 09:18:09 joywu Exp $
 * $Log: mkchdr.c,v $
 * Revision 1.1  2006/10/16 09:18:09  joywu
 * no message
 *
 * Revision 1.1  1997/02/15 19:05:30  rj
 * first check-in
 *
 */

#include "tbl-incl.h"
#include "tbl-gen-c-hdr.h"

void Usage PARAMS ((prg),
    char *prg)
{
    fprintf (stderr,"Usage: %s <tbl-file> [output-file]\n\n",prg);
    fprintf (stderr,"Reads the type table from tbl-file and writes\n");
    fprintf (stderr,"the equivalent C typedefs to output-file.\n");
    fprintf (stderr,"If no output-file is given, stdout is used.\n\n");
    fprintf (stderr,"The table files are generated with the snacc -T option.\n");
}

int
main PARAMS ((argc, argv),
    int argc _AND_
    char **argv)
{
    char *tblFileName;
    FILE *outputFile;
    TBL *tbl;

    if (argc < 2)
    {
        Usage (argv[0]);
        return 1;
    }

    tblFileName = argv[1];

    if (argc == 3)
    {
        /* open output file */
        outputFile = fopen (argv[2], "w");
        if (outputFile == NULL)
        {
            perror ("fopen: ");
            return 1;
        }
    }
    else /* use stdout */
        outputFile = stdout;

    InitNibbleMem (1024,1024);


    tbl = LoadTblFile (tblFileName);

    if (tbl == NULL)
        return 1;  /* Load routine will have printed errs */

    TblPrintCHdr (tbl, outputFile);

    return 0;
} /* main */
