      function ran2(idum)
!---------------------------------------------------------------------
!  See NUMERICAL RECIPES for detail of the function under the same
!  name
!  PARAMETERS:
!       idum       : random seed
!---------------------------------------------------------------------
      implicit none

      real :: ran2
      integer, intent(inout) :: idum
      integer, parameter :: im1=2147483563, im2=2147483399, imm1=im1-1
      integer, parameter :: ia1=40014, ia2=40692, iq1=53668, iq2=52774
      integer, parameter :: ir1=12211, ir2=3791, ntab=32, ndiv=1+imm1/ntab
      real, parameter :: am=1./im1, eps=1.2e-7, rnmx=1.-eps
      integer :: idum2=123456789, iy=0, j, k
      integer, dimension(ntab) :: iv
      
      save iv,iy,idum2
      
      do j=1,ntab
        iv(j)=0
      enddo

      if(idum.le.0) then
        idum=max(-idum,1)
        idum2=idum
        do j=ntab+8,1,-1
          k=idum/iq1
          idum=ia1*(idum-k*iq1)-k*ir1
          if(idum.lt.0) idum=idum+im1
          if(j.le.ntab) iv(j)=idum
        enddo
        iy=iv(1)
      endif
      k=idum/iq1
      idum=ia1*(idum-k*iq1)-k*ir1
      if(idum.lt.0) idum=idum+im1
      k=idum2/iq2
      idum2=ia2*(idum2-k*iq2)-k*ir2
      if(idum2.lt.0) idum2=idum2+im2
      j=1+iy/ndiv
      iy=iv(j)-idum2
      iv(j)=idum
      if(iy.lt.1) iy=iy+imm1
      ran2=min(am*iy,rnmx)
      
      end function ran2

!**********************************************************************


      FUNCTION GASDEV(IDUM)
!----------------------------------------------------------------------
!  Gaussian white noise generator using uniformly distributed white
!  noise generator
!---------------------------------------------------------------------- 

      implicit none
      real:: gasdev, ran2
      integer, intent(inout):: idum
      real :: v1, v2, gv1, gv2
      real, parameter :: pi=3.1415927

      V1=RAN2(IDUM)
      V2=RAN2(IDUM)

      gv1=sqrt(-2.0*log(v1))*cos(2.0*pi*v2)
      gv2=sqrt(-2.0*log(v2))*cos(2.0*pi*v1)

      gasdev=gv1

      END FUNCTION GASDEV



