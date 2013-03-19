      SUBROUTINE NATURAL_SPLINE(YA,LEX,N)
!----------------------------------------------------------------------
!  This is a program of cubic spline interpolation. The imported
!  series, YA have a length of LEX, with N numbers of value
!  not equal to 1.0E31. The program is to use the cubic line to
!  interpolate the values for the points other thatn these N
!  numbers.        
!-----------------------------------------------------------------------

      implicit none

      integer, intent(in):: N, LEX      
      real, dimension(LEX), intent(inout):: YA
      real, dimension(N):: Y, Y2
      integer, dimension(N):: LX
      integer:: J, I, KLO, KHI
      real:: YP1, YPN, H, A, B 


!  The following code is to realocate the series of X(N), Y(N)

      J=1
      DO I=1, LEX
        IF( YA(I).LT.1.0E30 ) THEN
          LX(J)=I
          Y(J)=YA(I)
          J=J+1
        ENDIF
      ENDDO

!  The following code is used to calculate the second order derivative, 
!  set the derivatives at both ends for a natural cubic spline.
      YP1=1.0E31
      YPN=1.0E31

      CALL SPLINE(LX,Y,N,YP1,YPN,Y2)

!  calculate the cubic spline
      DO I=2,N
        KLO=LX(I-1)
        KHI=LX(I)
        H=real(KHI-KLO)
        DO J=KLO+1,KHI-1
          A=REAL(KHI-J)/H
          B=REAL(J-KLO)/H
          YA(J)=A*Y(I-1)+B*Y(I)   &
                +((A*A*A-A)*Y2(I-1)+(B*B*B-B)*Y2(I))*(H*H)/6.0
        ENDDO
      ENDDO

      END SUBROUTINE NATURAL_SPLINE


!**************************************************************************


      SUBROUTINE SPLINE(LX,Y,N,YP1,YPN,Y2)
!-----------------------------------------------------------------
!  see NUMEIRCAL RECIPES to find out meaning of each variables
!-----------------------------------------------------------------
      implicit none
    
      integer, intent(in) :: N
      real, intent(in) :: YP1, YPN
      integer, dimension(N), intent(in):: LX
      real, dimension(N) :: X, U
      real, dimension(N), intent(inout):: Y, Y2 
      integer:: I, K
      real:: SIG, P, QN, UN

      DO I=1,N
        X(I)=real(LX(I))
      ENDDO

      Y2(1)=0.
      U(1)=0.
      
      DO I=2,N-1
        SIG=(X(I)-X(I-1))/(X(I+1)-X(I-1))
        P=SIG*Y2(I-1)+2.
        Y2(I)=(SIG-1.)/P
        U(I)=(6.*((Y(I+1)-Y(I))/(X(I+1)-X(I))-(Y(I)-Y(I-1)) &
             /(X(I)-X(I-1)))/(X(I+1)-X(I-1))-SIG*U(I-1))/P
      ENDDO

      QN=0.
      UN=0.

      Y2(N)=(UN-QN*U(N-1))/(QN*Y2(N-1)+1.)

      DO K=N-1,1,-1
        Y2(K)=Y2(K)*Y2(K+1)+U(K)
      ENDDO

      END SUBROUTINE SPLINE


