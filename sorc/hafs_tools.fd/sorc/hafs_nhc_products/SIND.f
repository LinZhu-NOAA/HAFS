C
C$$$  SUBPROGRAM DOCUMENTATION BLOCK
C                .      .    .                                       .
C SUBPROGRAM:    SIND        SINE FUNCTION FROM DEGREES INPUT
C   PRGMMR: S. J. LORD       ORG: W/NMC22    DATE: 91-06-06
C
C ABSTRACT: SINE FUNCTION FROM DEGREES INPUT.
C
C PROGRAM HISTORY LOG:
C   91-06-06  S. J. LORD
C   YY-MM-DD  MODIFIER1   DESCRIPTION OF CHANGE
C   YY-MM-DD  MODIFIER2   DESCRIPTION OF CHANGE
C
C USAGE:    CALL PGM-NAME(INARG1, INARG2, WRKARG, OUTARG1, ... )
C   INPUT ARGUMENT LIST:
C     INARG1   - GENERIC DESCRIPTION, INCLUDING CONTENT, UNITS,
C     INARG2   - TYPE.  EXPLAIN FUNCTION IF CONTROL VARIABLE.
C
C   OUTPUT ARGUMENT LIST:      (INCLUDING WORK ARRAYS)
C     WRKARG   - GENERIC DESCRIPTION, ETC., AS ABOVE.
C     OUTARG1  - EXPLAIN COMPLETELY IF ERROR RETURN
C     ERRFLAG  - EVEN IF MANY LINES ARE NEEDED
C
C   INPUT FILES:   (DELETE IF NO INPUT FILES IN SUBPROGRAM)
C     DDNAME1  - GENERIC NAME & CONTENT
C
C   OUTPUT FILES:  (DELETE IF NO OUTPUT FILES IN SUBPROGRAM)
C     DDNAME2  - GENERIC NAME & CONTENT AS ABOVE
C     FT06F001 - INCLUDE IF ANY PRINTOUT
C
C REMARKS: LIST CAVEATS, OTHER HELPFUL HINTS OR INFORMATION
C
C ATTRIBUTES:
C   LANGUAGE: INDICATE EXTENSIONS, COMPILER OPTIONS
C   MACHINE:  NAS, CYBER, WHATEVER
C
C$$$
      FUNCTION SIND(ARG)
C     DEGRAD CONVERTS DEGREES TO RADIANS
      DATA DEGRAD/0.017453/
      SIND=SIN(ARG*DEGRAD)
      RETURN
      END
