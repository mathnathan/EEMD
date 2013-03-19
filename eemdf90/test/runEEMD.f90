      PROGRAM runEEMD

      IMPLICIT NONE
      INCLUDE 'eemd.common'
      REAL,    PARAMETER :: STDEV=0.2
      INTEGER, PARAMETER :: EMEMBERS=100
      INTEGER, PARAMETER :: IMFS=12  
      
      INTEGER :: i,j,TimeSeriesLength
      REAL, DIMENSION(MAXSIZE) :: indata
      INTEGER:: idum  
      REAL :: eemd_result(MAXSIZE,IMFS+2)
      
      idum=3571
      open (4, file='tempTS.dat',FORM='UNFORMATTED',ACCESS='DIRECT', RECL=1)
      OPEN (15,FILE='tempEEMD.dat',FORM='UNFORMATTED',ACCESS='DIRECT', RECL=1)
!      OPEN(UNIT = 15, FILE = 'newfort.txt', STATUS = 'UNKNOWN')
      open (19, file='tempEEMD.ctl')

      do i = 1, MAXSIZE
         read(4, rec=i, ERR=10) indata(i)
      enddo
  10  continue 
      TimeSeriesLength = i-1

      CALL eemd(TimeSeriesLength,indata,STDEV,EMEMBERS,IMFS,idum,eemd_result)
      DO i=1,TimeSeriesLength
        DO j=1,IMFS+2
          WRITE(15,REC=i+(j-1)*TimeSeriesLength) (eemd_result(i,j))
        ENDDO
      ENDDO

 500  FORMAT(14F12.8)
 
      WRITE (19,*) 'DSET      ^tempEEMD.dat'
      WRITE (19,*) 'TITLE     Sample Model Data'
      WRITE (19,*) 'UNDEF    -9.99E+30 '
      WRITE (19,*) 'XDEF     1 linear   0 1'
      WRITE (19,*) 'YDEF     1 linear   0 1'
      WRITE (19,*) 'EDEF       16	NAMES	TS 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15'
      WRITE (19,*) 'ZDEF       1  linear 1 1'
      WRITE (19,*) 'TDEF       ',TimeSeriesLength,' LINEAR 1JAN2000 1MO'
      WRITE (19,*) 'VARS      1 '
      WRITE (19,*) '   imf  0  99  100 Data Points'
      WRITE (19,*) 'ENDVARS'
      WRITE (19,*) ' '
 
      END PROGRAM runEEMD
