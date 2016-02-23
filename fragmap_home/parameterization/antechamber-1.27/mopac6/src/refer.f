      SUBROUTINE REFER
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      INCLUDE 'SIZES'
      COMMON
     1       /REFS/ ALLREF(107,6)
     2       /MOLKST/ NUMAT,NAT(NUMATM),NFIRST(NUMATM),NMIDLE(NUMATM),
     3                NLAST(NUMATM), NORBS, NELECS,NALPHA,NBETA,
     4                NCLOSE,NOPEN,NDUMY,FRACT
     5       /KEYWRD/ KEYWRD
      LOGICAL ALLOK, ELEMNS(107), MIXOK, MIX
      SAVE MIX
      CHARACTER KEYWRD*241, ALLREF*80
      DATA MIX/.FALSE./
      MIXOK=(INDEX(KEYWRD,'PARASOK').NE.0)
      DO 10 I=1,102
   10 ELEMNS(I)=.FALSE.
      IF(INDEX(KEYWRD,'PM3').NE.0)THEN
         MODE=4
      ELSEIF(INDEX(KEYWRD,'AM1').NE.0)THEN
         MODE=3
      ELSEIF(INDEX(KEYWRD,'MINDO').NE.0)THEN
         MODE=2
      ELSEIF(INDEX(KEYWRD,'PDG').NE.0)THEN
         MODE=5
      ELSEIF(INDEX(KEYWRD,'MDG').NE.0)THEN
         MODE=6
      ELSE
         MODE=1
      ENDIF
      ALLREF(99,MODE)=' DUMMY ATOMS ARE USED; THESE DO NOT AFFECT '
     1//'THE CALCULATION'
      ALLREF(100,MODE)=' '
      DO 20 I=1,NUMAT
         J=NAT(I)
   20 ELEMNS(J)=.TRUE.
      ALLOK=.TRUE.
      DO 30 I=1,102
         IF(ELEMNS(I))THEN
            IF(I.LT.99.AND..NOT.MIX.AND.MODE.EQ.3)
     1MIX=(INDEX(ALLREF(I,3),'MNDO').NE.0)
            IF(ALLREF(I,MODE)(1:1).NE.' ')THEN
               WRITE(6,'(A,I3)')' DATA ARE NOT AVAILABLE FOR ELEMENT NO.
     1',I
               ALLOK=.FALSE.
            ELSE
               WRITE(6,'(A)')ALLREF(I,MODE)
            ENDIF
         ENDIF
   30 CONTINUE
      IF(MIX.AND..NOT.MIXOK)THEN
         WRITE(6,40)
     1  'SOME ELEMENTS HAVE BEEN SPECIFIED FOR WHICH ONLY MNDO',
     2  'PARAMETERS ARE AVAILABLE.  SUCH MIXTURES OF METHODS ARE',
     3  'VERY RISKY AND HAVE NOT BEEN FULLY TESTED.  IF YOU FEEL',
     4  'THE RISK IS WORTH WHILE - CHECK THE MANUAL FIRST - THEN',
     5  'SPECIFY "PARASOK" IN THE KEYWORDS'
         STOP
      ENDIF
      IF(ALLOK)RETURN
      WRITE(6,40)
     1 'SOME ELEMENTS HAVE BEEN SPECIFIED FOR WHICH',
     2 'NO PARAMETERS ARE AVAILABLE.  CALCULATION STOPPED.'
      STOP
   40 FORMAT(/////10X,A,4(/10X,A))
      END
