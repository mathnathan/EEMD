      subroutine min_max(LEX,ximf,spmax,spmin,nmax,nmin)
!------------------------------------------------------------------
!  This is a routine to define maxima and minima from series ximf.
!  All the extrema are defined as the corresponding values of
!  ximf in spmax and spmin. All non-extrema values in spmax and
!  spmin are defined as 1.0e31.
!------------------------------------------------------------------
      implicit none

      integer, intent(in):: LEX
      real, dimension(LEX), intent(in):: ximf
      real, dimension(LEX), intent(out):: spmax, spmin
      integer, intent(out):: nmax, nmin
      integer:: i
      
      nmax=0
      nmin=0

      spmax(1)=ximf(1)
      spmax(LEX)=ximf(LEX)
      spmin(1)=spmax(1)
      spmin(LEX)=spmax(LEX)

      nmax=2
      nmin=2

      do i=2,LEX-1
        if(ximf(i) > ximf(i-1) .and. ximf(i) >= ximf(i+1)) then
          spmax(i) = ximf(i)
          nmax = nmax+1
        else
          spmax(i)=1.0e31
        endif
        if(ximf(i) < ximf(i-1) .and. ximf(i) <= ximf(i+1)) then
          spmin(i)=ximf(i)
          nmin=nmin+1
        else
          spmin(i)=1.0e31
        endif
      enddo

      call endmax(LEX, spmax, nmax)
      call endmin(LEX, spmin, nmin)

      end subroutine min_max

!****************************************************************

      subroutine endmax(LEX, temp, nmax)
!--------------------------------------------------------------------
! This is a subroutine to determine end values of the upper envolope
! using the method described in Appendix B of Wu and Huang (2009, 
! AADA, Vol. 1, pp1).
!--------------------------------------------------------------------
      implicit none

      integer, intent(in) :: nmax, LEX
      real, dimension(LEX), intent(inout):: temp 
      real, dimension(nmax) :: exmax, X
      integer :: I, J, lend
      real :: slope1, slope2, tmp1, tmp2
     
      lend=nmax

      J=1
      DO I=1, LEX
        IF( temp(I).LT.1.0E30 ) THEN
          X(J)=float(I)
          exmax(J)=temp(I)
          J=J+1
        ENDIF
      ENDDO

      if (nmax >= 4) then
        slope1=(exmax(2)-exmax(3))/(X(2)-X(3))
        tmp1=slope1*(X(1)-X(2))+exmax(2)
        if(tmp1 > exmax(1)) then
          temp(1)=tmp1
        endif
        
        slope2=(exmax(lend-1)-exmax(lend-2))/(X(lend-1)-X(lend-2))
        tmp2=slope2*(X(lend)-X(lend-1))+exmax(lend-1)
        if(tmp2 > exmax(lend)) then
          temp(LEX)=tmp2
        endif
      endif
      
      end subroutine endmax


!****************************************************************

      subroutine endmin(LEX, temp, nmax)
!--------------------------------------------------------------------
! This is a subroutine to determine end values of the lower envolope
! using the method described in Appendix B of Wu and Huang (2009, 
! AADA, Vol. 1, pp1).
!--------------------------------------------------------------------
      implicit none

      integer, intent(in) :: nmax, LEX
      real, dimension(LEX), intent(inout):: temp 
      real, dimension(nmax) :: exmax, X
      integer :: I, J, lend
      real :: slope1, slope2, tmp1, tmp2
     
      lend=nmax

      J=1
      DO I=1, LEX
        IF( temp(I).LT.1.0E30 ) THEN
          X(J)=float(I)
          exmax(J)=temp(I)
          J=J+1
        ENDIF
      ENDDO

      if (nmax >= 4) then
        slope1=(exmax(2)-exmax(3))/(X(2)-X(3))
        tmp1=slope1*(X(1)-X(2))+exmax(2)
        if(tmp1 < exmax(1)) then
          temp(1)=tmp1
        endif
        
        slope2=(exmax(lend-1)-exmax(lend-2))/(X(lend-1)-X(lend-2))
        tmp2=slope2*(X(lend)-X(lend-1))+exmax(lend-1)
        if(tmp2 < exmax(lend)) then
          temp(LEX)=tmp2
        endif
      endif
      
      end subroutine endmin



